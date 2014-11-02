/*
    ps_potr_dbtable.cpp
*/

#include <QTableView>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>

#include "s_potr_dbtable.h"

// Размеры полей по умолчанию
const int widthKOD = 40,
  widthNAME = 250;

S_POTR_DBTable::S_POTR_DBTable(QWidget *parent)
    : PAbstractDBTable(parent)
{
  captionViewLabel->setText( tr("Потребители"));

  selectSQL = "SELECT KOD,NAME FROM S_POTR ORDER BY KOD";

  model->setQuery( selectSQL , db );
  model->setHeaderData(0, Qt::Horizontal, tr("Код"));
  model->setHeaderData(1, Qt::Horizontal, tr("Наименование"));

  view->setColumnWidth( 0, widthKOD );
  view->setColumnWidth( 1, widthNAME );
  
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

  setupLayouts();
}

bool S_POTR_DBTable::virtualAddRec( void )
{
  iobj_KOD->setValue(0);
  iobj_KOD->setFocus();

  iobj_NAME->setText( tr(""));

  if( !precdialog->exec() ) return true;
  QSqlQuery query( db );

  strKOD = iobj_KOD->text();

  return query.exec("INSERT INTO S_POTR (KOD, NAME) VALUES ( '"+
		    QString::number(iobj_KOD->value())+"','"+
		    iobj_NAME->text()+
		    "')");
}

bool S_POTR_DBTable::virtualEditRec( int & row )
{
  QSqlRecord record = model->record( row );

  
  QString lastStrKOD =  record.value("KOD").toString();

  iobj_KOD->setValue( record.value("KOD").toInt());
  iobj_KOD->setFocus();

  iobj_NAME->setText( record.value("NAME").toString());
 
  if( !precdialog->exec() ) return true;

  strKOD = iobj_KOD->text();
  
  QSqlQuery query( db );
  return query.exec( "UPDATE S_POTR SET KOD = '" + QString::number(iobj_KOD->value()) 
		     + "', NAME = '" + iobj_NAME->text() 
		     + "' WHERE KOD = '" + lastStrKOD + "'" );
}

bool S_POTR_DBTable::virtualDelRec( int & row )
{
  QSqlRecord record = model->record( row );

  switch( QMessageBox::information( this, tr( "Удаление запись"),
				    tr("Вы действительно хотите удалить запись по коду: ") +
				    record.value("KOD").toString() + " ?",
				    tr("Да"), tr("Нет"), 0 , 1 ) ) { 
  case 0:{
    QSqlQuery query( db );
    return query.exec("DELETE FROM S_POTR WHERE KOD = '" 
		      +  record.value("KOD").toString() + "'");
  }
  case 1: 
    return true;
  }
  return true;
}
