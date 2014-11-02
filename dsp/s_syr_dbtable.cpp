/*
    ps_syr_dbtable.cpp
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
#include <QSplitter>

#include "s_syr_dbtable.h"

// Размеры полей по умолчанию
const int widthKOD = 40,
  widthNAME = 250,
  widthK = 60;

// Группа -----------------------------------------------------------------------------------
S_SYR_GR_DBTable::S_SYR_GR_DBTable(QWidget *parent)
    : PAbstractDBTable(parent)
{
  captionViewLabel->setText( tr("Справочник сырья - главная группа"));

  ptrView = view;

  selectSQL = "SELECT KOD,NAME,K FROM S_SYR_GR ORDER BY KOD";

  model->setQuery( selectSQL , db );

  model->setHeaderData(0, Qt::Horizontal, tr("Код"));
  model->setHeaderData(1, Qt::Horizontal, tr("Наименование"));
  model->setHeaderData(2, Qt::Horizontal, tr("Коэфф.:"));

  view->setColumnWidth( 0, widthKOD );
  view->setColumnWidth( 1, widthNAME );
  view->setColumnWidth( 2, widthK );
  
  POne_Field* ptrPRD;

  ptrPRD = precdialog->addInputObject( tr("Код:") , 
				       POne_Field::Type(POne_Field::LineEdit) , 50 );
  iobj_KOD = (QLineEdit*)ptrPRD->inputObject();
  iobj_KOD->setValidator( new QIntValidator( iobj_KOD ));
  iobj_KOD->setFocus();

  ptrPRD = precdialog->addInputObject( tr("Наименование") , 
				       POne_Field::Type(POne_Field::LineEdit) , 200 );
  iobj_NAME = (QLineEdit*)ptrPRD->inputObject();
  iobj_NAME->setMaxLength( 30 );

  ptrPRD = precdialog->addInputObject( tr("Коэфф.:") , 
				       POne_Field::Type(POne_Field::LineEdit) , 50 );
  iobj_K = (QLineEdit*)ptrPRD->inputObject();
  iobj_K->setValidator( new QDoubleValidator( iobj_K ));

  setupLayouts();
}

bool S_SYR_GR_DBTable::virtualAddRec( void )
{
  iobj_KOD->setText( tr(""));
  iobj_KOD->setFocus();

  iobj_NAME->setText( tr(""));
  iobj_K->setText( tr("0"));
  
  if( !precdialog->exec() ) return true;
  QSqlQuery query( db );

  strKOD = iobj_KOD->text();

  return query.exec("INSERT INTO S_SYR_GR (KOD, NAME, K) VALUES ( '"
		    + iobj_KOD->text() + "','" 
		    + iobj_NAME->text() +"','" 
		    + iobj_K->text() + "')");
}

bool S_SYR_GR_DBTable::virtualEditRec( int & row )
{
  QSqlRecord record = model->record( row );

  
  QString lastStrKOD =  record.value("KOD").toString();

  iobj_KOD->setText( lastStrKOD );
  iobj_KOD->setFocus();

  iobj_NAME->setText( record.value("NAME").toString());
  iobj_K->setText( record.value("K").toString());
 
  if( !precdialog->exec() ) return true;

  strKOD = iobj_KOD->text();
  
  QSqlQuery query( db );
  return query.exec( "UPDATE S_SYR_GR SET KOD = '" + strKOD 
		     + "', NAME = '" + iobj_NAME->text() 
		     + "', K = '" + iobj_K->text() 
		     + "' WHERE KOD = '" + lastStrKOD + "'" );
}

bool S_SYR_GR_DBTable::virtualDelRec( int & row )
{
  QSqlRecord record = model->record( row );

  switch( QMessageBox::information( this, tr( "Удаление запись"),
				    tr("Вы действительно хотите удалить запись по коду: ") +
				    record.value("KOD").toString() + " ?",
				    tr("Да"), tr("Нет"), 0 , 1 ) ) { 
  case 0:{
    QSqlQuery query( db );
    return query.exec("DELETE FROM S_SYR_GR WHERE KOD = '" 
		      +  record.value("KOD").toString() + "'");
  }
  case 1: 
    return true;
  }
  return true;
}

// Группа ...

// Подгруппа ---------------------------------------------------------------------------------
void S_SYR_PGR_DBTable::update_PGR( const QModelIndex & index )
{
  if (index.isValid())
    {
      QSqlRecord record = ((QSqlQueryModel *)index.model())->record( index.row () );

      strKODM =  record.value("KOD").toString();

      selectSQL = "SELECT KOD,NAME,K FROM S_SYR_PGR WHERE KODM="
	+ strKODM
	+" ORDER BY KOD";

      model->setQuery( selectSQL , db );
     
      view->selectRow ( 0 );
    }
}

S_SYR_PGR_DBTable::S_SYR_PGR_DBTable(QWidget *parent)
    : PAbstractDBTable(parent)
{
  captionViewLabel->setText( tr("Справочник сырья - подгруппа"));
  
  model->setQuery( "SELECT KOD,NAME,K FROM S_SYR_PGR WHERE KOD = -1" , db );

  model->setHeaderData(0, Qt::Horizontal, tr("Код"));
  model->setHeaderData(1, Qt::Horizontal, tr("Наименование"));
  model->setHeaderData(2, Qt::Horizontal, tr("Коэфф.:"));
  
  view->setColumnWidth( 0, widthKOD );  
  view->setColumnWidth( 1, widthNAME );  
  view->setColumnWidth( 2, widthK );
  
  POne_Field* ptrPRD;

  ptrPRD = precdialog->addInputObject( tr("Код:") , 
				       POne_Field::Type(POne_Field::LineEdit) , 50 );
  iobj_KOD = (QLineEdit*)ptrPRD->inputObject();
  iobj_KOD->setValidator( new QIntValidator( iobj_KOD ));
  iobj_KOD->setFocus();


  ptrPRD = precdialog->addInputObject( tr("Наименование") , 
				       POne_Field::Type(POne_Field::LineEdit) , 200 );
  iobj_NAME = (QLineEdit*)ptrPRD->inputObject();
  iobj_NAME->setMaxLength( 30 );

  ptrPRD = precdialog->addInputObject( tr("Коэфф.:") , 
				       POne_Field::Type(POne_Field::LineEdit) , 50 );
  iobj_K = (QLineEdit*)ptrPRD->inputObject();
  iobj_K->setValidator( new QDoubleValidator( iobj_K ));

  setupLayouts();
}


bool S_SYR_PGR_DBTable::virtualAddRec( void )
{
  iobj_KOD->setText( tr(""));
  iobj_KOD->setFocus();

  iobj_NAME->setText( tr(""));
  iobj_K->setText( tr("0"));
  
  if( !precdialog->exec() ) return true;
  QSqlQuery query( db );

  strKOD = iobj_KOD->text();

  return query.exec("INSERT INTO S_SYR_PGR (KOD, NAME, K, KODM) VALUES ( '"
		    + iobj_KOD->text() + "','" 
		    + iobj_NAME->text() + "','" 
		    + iobj_K->text() + "','" 
		    + strKODM + "')");
}

bool S_SYR_PGR_DBTable::virtualEditRec( int & row )
{
  QSqlRecord record = model->record( row );

  
  QString lastStrKOD =  record.value("KOD").toString();

  iobj_KOD->setText( lastStrKOD );
  iobj_KOD->setFocus();

  iobj_NAME->setText( record.value("NAME").toString());
  iobj_K->setText( record.value("K").toString());
 
  if( !precdialog->exec() ) return true;

  strKOD = iobj_KOD->text();
  
  QSqlQuery query( db );
  return query.exec( "UPDATE S_SYR_PGR SET KOD = '" + strKOD 
		     + "', NAME = '" + iobj_NAME->text() 
		     + "', K = '" + iobj_K->text() 
		     + "' WHERE KOD = '" + lastStrKOD + "'" );
}

bool S_SYR_PGR_DBTable::virtualDelRec( int & row )
{
  QSqlRecord record = model->record( row );

  switch( QMessageBox::information( this, tr( "Удаление запись"),
				    tr("Вы действительно хотите удалить запись по коду: ") +
				    record.value("KOD").toString() + " ?",
				    tr("Да"), tr("Нет"), 0 , 1 ) ) { 
  case 0:{
    QSqlQuery query( db );
    return query.exec("DELETE FROM S_SYR_PGR WHERE KOD = '" 
		      +  record.value("KOD").toString() + "'");
  }
  case 1: 
    return true;
  }
  return true;
}
// Подгруппа ...

// Общий виджет из двух таблиц -------------------------------------------------------------

S_SYR_DBTable::S_SYR_DBTable(QWidget *parent)
    : QWidget(parent)
{
  vert_splitter = new QSplitter( Qt::Vertical ); 
  
  s_syr_gr_dbtable  = new S_SYR_GR_DBTable(vert_splitter);
  s_syr_pgr_dbtable = new S_SYR_PGR_DBTable(vert_splitter);

  connect( s_syr_gr_dbtable->ptrView->selectionModel()  , 
	   SIGNAL( currentChanged ( const QModelIndex & , const QModelIndex & )), 
	   s_syr_pgr_dbtable , 
	   SLOT(update_PGR( const QModelIndex & )));

  QVBoxLayout* formLayout = new QVBoxLayout( this );
  formLayout->addWidget( vert_splitter );
  formLayout->setMargin ( 0 );
}
// Общий виджет из двух таблиц ...
