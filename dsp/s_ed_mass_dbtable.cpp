/*
    ps_ed_mass_dbtable.cpp
*/

#include <QTableView>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>
#include <QComboBox>
#include <QSpinBox>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>

#include "s_ed_mass_dbtable.h"

S_ED_MASS_DBTable::S_ED_MASS_DBTable(QWidget *parent)
    : PAbstractDBTable(parent)
{
  captionViewLabel->setText( tr("Единици измерения массы"));


  selectSQL = "SELECT ED,FULL_NAME FROM S_ED_MASS ORDER BY ED";

  model->setQuery( selectSQL , db );
  model->setHeaderData(0, Qt::Horizontal, tr("Ед.Изм.:"));
  model->setHeaderData(1, Qt::Horizontal, tr("Полное наименование:"));

  view->setColumnWidth( 0, 60 );
  view->setColumnWidth( 1, 300 );
   
  POne_Field* ptrPRD;

  ptrPRD = precdialog->addInputObject( tr("Ед.Изм.:") , 
				       POne_Field::Type(POne_Field::LineEdit) , 40 );
  iobj_ED = (QLineEdit*)ptrPRD->inputObject();
  iobj_ED->setMaxLength( 3 );

  ptrPRD = precdialog->addInputObject( tr("Полное наименование:") , 
				       POne_Field::Type(POne_Field::LineEdit) , 300 );
  iobj_FULL_NAME = (QLineEdit*)ptrPRD->inputObject();
  iobj_FULL_NAME->setMaxLength( 30 );

  setupLayouts();
}


bool S_ED_MASS_DBTable::virtualAddRec( void )
{
  iobj_ED->setText( tr(""));
  iobj_ED->setFocus();

  iobj_FULL_NAME->clear();
  
  if( !precdialog->exec() ) return true;
  QSqlQuery query( db );

  strKOD = iobj_ED->text();

  return query.exec("INSERT INTO S_ED_MASS ( ED , FULL_NAME ) VALUES ( '"
		    + iobj_ED->text() + "','"
		    + iobj_FULL_NAME->text() +
		    "')");
}

bool S_ED_MASS_DBTable::virtualEditRec( int & row )
{
  QSqlRecord record = model->record( row );

  
  QString lastStrKOD = record.value("ED").toString();

  iobj_ED->setText( record.value("ED").toString());
  iobj_ED->setFocus();

  iobj_FULL_NAME->setText( record.value("FULL_NAME").toString());

  if( !precdialog->exec() ) return true;

  strKOD = iobj_ED->text();

  QSqlQuery query( db );
  return query.exec( "UPDATE S_ED_MASS SET ED = '" 
		     + strKOD + "', FULL_NAME = '"
		     + iobj_FULL_NAME->text()
		     + "' WHERE ED = '" + lastStrKOD + "'" );
}

bool S_ED_MASS_DBTable::virtualDelRec( int & row )
{
  QSqlRecord record = model->record( row );

  switch( QMessageBox::information( this, tr( "Удаление запись"),
				    tr("Вы действительно хотите удалить запись : ") +
				    record.value("ED").toString() + " ?",
				    tr("Да"), tr("Нет"), 0 , 1 ) ) { 
  case 0:{
    QSqlQuery query( db );
    return query.exec("DELETE FROM S_ED_MASS WHERE ED = '" 
		      +  record.value("ED").toString() + "'");
  }
  case 1: 
    return true;
  }
  return true;
}
