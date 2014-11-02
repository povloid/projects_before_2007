/*
    ps_vyr_dbtable.cpp
*/

#include <QTableView>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>

#include "s_vyr_dbtable.h"

// Размеры полей по умолчанию
const int widthKOD = 40,
  widthNAME = 250,
  widthED = 60;

S_VYR_DBTable::S_VYR_DBTable(QWidget *parent)
    : PAbstractDBTable(parent)
{
  captionViewLabel->setText( tr("Выработка"));

  selectSQL = "SELECT KOD,NAME,KOD_DEP,ED FROM S_VYR ORDER BY KOD";

  model->setQuery( selectSQL , db );
  model->setHeaderData(0, Qt::Horizontal, tr("Код"));
  model->setHeaderData(1, Qt::Horizontal, tr("Наименование"));
  model->setHeaderData(2, Qt::Horizontal, tr("Цех:"));
  model->setHeaderData(3, Qt::Horizontal, tr("Ед.изм.:"));

  view->setColumnWidth( 0, widthKOD );
  view->setColumnWidth( 1, widthNAME );
  view->setColumnWidth( 2, widthED );
  view->setColumnWidth( 3, widthED );
  
  POne_Field* ptrPRD;

  ptrPRD = precdialog->addInputObject( tr("Код:") , 
				       POne_Field::Type(POne_Field::SpinBox) , 70 );
  iobj_KOD = (QSpinBox*)ptrPRD->inputObject();
  iobj_KOD->setRange(0, 1000000);
  iobj_KOD->setValue(0);
  iobj_KOD->setAlignment(Qt::AlignRight);

  ptrPRD = precdialog->addInputObject( tr("Наименование") , 
				       POne_Field::Type(POne_Field::LineEdit) , 200 );
  iobj_NAME = (QLineEdit*)ptrPRD->inputObject();
  iobj_NAME->setMaxLength( 30 );

  ptrPRD = precdialog->addInputObject( tr("Цех:") , 
				       POne_Field::Type(POne_Field::ComboBox) , 50 );
  iobj_KOD_DEP = (QComboBox*)ptrPRD->inputObject();
  iobj_KOD_DEP->setEditable( true );
  iobj_KOD_DEP->lineEdit()->setMaxLength( 3 );

  model_in_ComboBox_KOD_DEP = new QSqlQueryModel;
  iobj_KOD_DEP->setModel( model_in_ComboBox_KOD_DEP );  


  ptrPRD = precdialog->addInputObject( tr("Ед.изм.:") , 
				       POne_Field::Type(POne_Field::ComboBox) , 50 );
  iobj_ED_MASS = (QComboBox*)ptrPRD->inputObject();
  iobj_ED_MASS->setEditable( false );
  //iobj_ED_MASS->lineEdit()->setMaxLength( 3 );
  
  model_in_ComboBox_ED_MASS = new QSqlQueryModel;
  iobj_ED_MASS->setModel( model_in_ComboBox_ED_MASS );

  setupLayouts();
}

bool S_VYR_DBTable::virtualAddRec( void )
{
  iobj_KOD->setValue(0);
  iobj_KOD->setFocus();

  iobj_NAME->setText( tr(""));

  model_in_ComboBox_KOD_DEP->setQuery( "SELECT KOD FROM S_DEP" , db );
  model_in_ComboBox_ED_MASS->setQuery( "SELECT ED FROM S_ED_MASS" , db );
  
  if( !precdialog->exec() ) return true;
  QSqlQuery query( db );

  strKOD = iobj_KOD->text();

  return query.exec("INSERT INTO S_VYR (KOD, NAME, ED, KOD_DEP ) VALUES ( '"
		    + QString::number(iobj_KOD->value())+"','"
		    + iobj_NAME->text() + "','"
		    + iobj_ED_MASS->currentText() + "','"
		    + iobj_KOD_DEP->currentText()
		    + "')");
}

bool S_VYR_DBTable::virtualEditRec( int & row )
{
  QSqlRecord record = model->record( row );
  
  QString lastStrKOD =  record.value("KOD").toString();

  iobj_KOD->setValue( record.value("KOD").toInt());
  iobj_KOD->setFocus();

  iobj_NAME->setText( record.value("NAME").toString() );

  model_in_ComboBox_KOD_DEP->setQuery( "SELECT KOD FROM S_DEP" , db );
  iobj_KOD_DEP->setEditText( record.value("KOD_DEP").toString() );

  model_in_ComboBox_ED_MASS->setQuery( "SELECT ED FROM S_ED_MASS" , db );
  iobj_ED_MASS->setEditText( record.value("ED").toString() );
 
  if( !precdialog->exec() ) return true;

  strKOD = iobj_KOD->text();
  
  QSqlQuery query( db );
  return query.exec( "UPDATE S_VYR SET KOD = '" + QString::number(iobj_KOD->value())
		     + "', NAME = '" + iobj_NAME->text() 
		     + "', KOD_DEP = '" + iobj_KOD_DEP->currentText()
		     + "', ED = '" + iobj_ED_MASS->currentText()
		     + "' WHERE KOD = '" + lastStrKOD + "'" );
}

bool S_VYR_DBTable::virtualDelRec( int & row )
{
  QSqlRecord record = model->record( row );

  switch( QMessageBox::information( this, tr( "Удаление запись"),
				    tr("Вы действительно хотите удалить запись по коду: ") +
				    record.value("KOD").toString() + " ?",
				    tr("Да"), tr("Нет"), 0 , 1 ) ) { 
  case 0:{
    QSqlQuery query( db );
    return query.exec("DELETE FROM S_VYR WHERE KOD = '" 
		      +  record.value("KOD").toString() + "'");
  }
  case 1: 
    return true;
  }
  return true;
}
