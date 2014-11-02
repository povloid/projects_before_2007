/*
    ps_ob_dbtable.cpp
*/

#include <QTableView>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>

#include "s_ob_dbtable.h"

// Размеры полей по умолчанию
const int widthKOD = 40,
  widthNAME = 250,
  widthDEP = 60;


S_OB_DBTable::S_OB_DBTable(QWidget *parent)
    : PAbstractDBTable(parent)
{
  captionViewLabel->setText( tr("Оборудование"));

  selectSQL = "SELECT KOD,NAME,DEP FROM S_OB ORDER BY KOD";

  model->setQuery( selectSQL , db );
  model->setHeaderData(0, Qt::Horizontal, tr("Код"));
  model->setHeaderData(1, Qt::Horizontal, tr("Наименование"));
  model->setHeaderData(2, Qt::Horizontal, tr("Цех:"));

  view->setColumnWidth( 0, widthKOD );
  view->setColumnWidth( 1, widthNAME );
  view->setColumnWidth( 2, widthDEP );
  
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

  ptrPRD = precdialog->addInputObject( tr("Цех.:") , 
				       POne_Field::Type(POne_Field::ComboBox) , 50 );
  iobj_DEP = (QComboBox*)ptrPRD->inputObject();
  iobj_DEP->setEditable( false );
  //  iobj_DEP->lineEdit()->setMaxLength( 3 );
  
  model_in_ComboBox = new QSqlQueryModel;
  iobj_DEP->setModel( model_in_ComboBox );

  setupLayouts();
}

bool S_OB_DBTable::virtualAddRec( void )
{
  iobj_KOD->setValue(0);
  iobj_KOD->setFocus();

  iobj_NAME->setText( tr(""));

  model_in_ComboBox->setQuery( "SELECT KOD FROM S_DEP" , db );
  
  if( !precdialog->exec() ) return true;
  QSqlQuery query( db );

  strKOD = iobj_KOD->text();

  return query.exec("INSERT INTO S_OB ( KOD, NAME, DEP ) VALUES ( '"
		    + QString::number(iobj_KOD->value())+"','" 
		    + iobj_NAME->text() + "','"
		    + iobj_DEP->currentText()
		    + "')");
}

bool S_OB_DBTable::virtualEditRec( int & row )
{
  QSqlRecord record = model->record( row );
  
  QString lastStrKOD =  record.value("KOD").toString();

  iobj_KOD->setValue( record.value("KOD").toInt() );
  iobj_KOD->setFocus();

  iobj_NAME->setText( record.value("NAME").toString() );

  model_in_ComboBox->setQuery( "SELECT KOD FROM S_DEP" , db );
  iobj_DEP->setEditText( record.value("DEP").toString() );
 
  if( !precdialog->exec() ) return true;

  strKOD = iobj_KOD->text();
  
  QSqlQuery query( db );
  return query.exec( "UPDATE S_OB SET KOD = '" + strKOD 
		     + "', NAME = '" + QString::number(iobj_KOD->value()) 
		     + "', DEP = '" + iobj_DEP->currentText()
		     + "' WHERE KOD = '" + lastStrKOD + "'" );
}

bool S_OB_DBTable::virtualDelRec( int & row )
{
  QSqlRecord record = model->record( row );

  switch( QMessageBox::information( this, tr( "Удаление запись"),
				    tr("Вы действительно хотите удалить запись по коду: ") +
				    record.value("KOD").toString() + " ?",
				    tr("Да"), tr("Нет"), 0 , 1 ) ) { 
  case 0:{
    QSqlQuery query( db );
    return query.exec("DELETE FROM S_OB WHERE KOD = '" 
		      +  record.value("KOD").toString() + "'");
  }
  case 1: 
    return true;
  }
  return true;
}
