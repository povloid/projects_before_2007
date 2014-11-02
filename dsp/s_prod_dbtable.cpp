/*
    ps_prod_dbtable.cpp
*/

#include <QTableView>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSplitter>

#include "s_prod_dbtable.h"

// Размеры полей по умолчанию
const int widthKOD = 40,
  widthNAME = 250,
  widthK = 60;

// Группа -----------------------------------------------------------------------------------
S_PROD_GR_DBTable::S_PROD_GR_DBTable(QWidget *parent)
    : PAbstractDBTable(parent)
{
  captionViewLabel->setText( tr("Справочник продукции - главная группа"));

  ptrView = view;

  selectSQL = "SELECT KOD,NAME,K,ED FROM S_PROD_GR ORDER BY KOD";

  model->setQuery( selectSQL , db );

  model->setHeaderData(0, Qt::Horizontal, tr("Код"));
  model->setHeaderData(1, Qt::Horizontal, tr("Наименование"));
  model->setHeaderData(2, Qt::Horizontal, tr("Коэфф.:"));
  model->setHeaderData(3, Qt::Horizontal, tr("Ед.изм.:"));

  view->setColumnWidth( 0, widthKOD );
  view->setColumnWidth( 1, widthNAME );
  view->setColumnWidth( 2, widthK );
  view->setColumnWidth( 3, widthK );
  
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

  ptrPRD = precdialog->addInputObject( tr("Коэфф.:") , 
				       POne_Field::Type(POne_Field::DoubleSpinBox) , 70 );
  iobj_K = (QDoubleSpinBox*)ptrPRD->inputObject();
  iobj_K->setRange(0.0, 100000.0);
  iobj_K->setSingleStep(0.01);
  iobj_K->setValue(0.0);
  iobj_K->setAlignment(Qt::AlignRight);
  
  ptrPRD = precdialog->addInputObject( tr("Ед.изм.:") , 
				       POne_Field::Type(POne_Field::ComboBox) , 50 );
  iobj_ED_MASS = (QComboBox*)ptrPRD->inputObject();
  iobj_ED_MASS->setEditable( false );
  //  iobj_ED_MASS->lineEdit()->setMaxLength( 3 );
  
  model_in_ComboBox = new QSqlQueryModel;
  iobj_ED_MASS->setModel( model_in_ComboBox );

  setupLayouts();
}

bool S_PROD_GR_DBTable::virtualAddRec( void )
{
  iobj_KOD->setValue(0);
  iobj_KOD->setFocus();

  iobj_NAME->setText( tr(""));
  iobj_K->setValue(0);

  model_in_ComboBox->setQuery( "SELECT ED FROM S_ED_MASS" , db );
  
  if( !precdialog->exec() ) return true;
  QSqlQuery query( db );

  strKOD = iobj_KOD->text();

  return query.exec("INSERT INTO S_PROD_GR (KOD, NAME, K, ED) VALUES ( '"
		    + QString::number(iobj_KOD->value())+"','" 
		    + iobj_NAME->text() + "','" 
		    + QString::number(iobj_K->value())+"','"
		    + iobj_ED_MASS->currentText() + "')");
}

bool S_PROD_GR_DBTable::virtualEditRec( int & row )
{
  QSqlRecord record = model->record( row );

  
  QString lastStrKOD =  record.value("KOD").toString();

  iobj_KOD->setValue( record.value("KOD").toInt() );
  iobj_KOD->setFocus();

  iobj_NAME->setText( record.value("NAME").toString());
  iobj_K->setValue( record.value("K").toInt() );
 
  model_in_ComboBox->setQuery( "SELECT ED FROM S_ED_MASS" , db );
  iobj_ED_MASS->setEditText( record.value("ED").toString() );

  if( !precdialog->exec() ) return true;

  strKOD = iobj_KOD->text();
  
  QSqlQuery query( db );
  return query.exec( "UPDATE S_PROD_GR SET KOD = '" + QString::number(iobj_KOD->value())
		     + "', NAME = '" + iobj_NAME->text() 
		     + "', K = '" + QString::number(iobj_K->value())
		     + "', ED = '" + iobj_ED_MASS->currentText()
		     + "' WHERE KOD = '" + lastStrKOD + "'" );
}

bool S_PROD_GR_DBTable::virtualDelRec( int & row )
{
  QSqlRecord record = model->record( row );

  switch( QMessageBox::information( this, tr( "Удаление запись"),
				    tr("Вы действительно хотите удалить запись по коду: ") +
				    record.value("KOD").toString() + " ?",
				    tr("Да"), tr("Нет"), 0 , 1 ) ) { 
  case 0:{
    QSqlQuery query( db );
    return query.exec("DELETE FROM S_PROD_GR WHERE KOD = '" 
		      +  record.value("KOD").toString() + "'");
  }
  case 1: 
    return true;
  }
  return true;
}

// Группа ...

// Подгруппа ---------------------------------------------------------------------------------
void S_PROD_PGR_DBTable::update_PGR( const QModelIndex & index )
{
  if (index.isValid())
    {
      QSqlRecord record = ((QSqlQueryModel *)index.model())->record( index.row () );

      strKODM =  record.value("KOD").toString();

      selectSQL = "SELECT NAME,K,KOD_DEP FROM S_PROD_PGR WHERE KODM="
	+ strKODM
	+" ORDER BY KOD";

      model->setQuery( selectSQL , db );
     
      view->selectRow ( 0 );
    }
}

S_PROD_PGR_DBTable::S_PROD_PGR_DBTable(QWidget *parent)
    : PAbstractDBTable(parent)
{
  captionViewLabel->setText( tr("Справочник продукции - подгруппа"));
  
  model->setQuery( "SELECT NAME,K,KOD_DEP FROM S_PROD_PGR WHERE KOD = -1" , db );

  model->setHeaderData(0, Qt::Horizontal, tr("Наименование"));
  model->setHeaderData(1, Qt::Horizontal, tr("Коэфф.:"));
  model->setHeaderData(2, Qt::Horizontal, tr("Цех:"));
  
  view->setColumnWidth( 0, widthNAME );  
  view->setColumnWidth( 1, widthK );
  view->setColumnWidth( 2, widthK );
  
  POne_Field* ptrPRD;

  ptrPRD = precdialog->addInputObject( tr("Наименование") , 
				       POne_Field::Type(POne_Field::LineEdit) , 200 );
  iobj_NAME = (QLineEdit*)ptrPRD->inputObject();
  iobj_NAME->setMaxLength( 30 );

  ptrPRD = precdialog->addInputObject( tr("Коэфф.:") , 
				       POne_Field::Type(POne_Field::DoubleSpinBox) , 70 );
  iobj_K = (QDoubleSpinBox*)ptrPRD->inputObject();
  iobj_K->setRange(0.0, 100000.0);
  iobj_K->setSingleStep(0.01);
  iobj_K->setValue(0.0);
  iobj_K->setAlignment(Qt::AlignRight);
  
  ptrPRD = precdialog->addInputObject( tr("Цех:") , 
				       POne_Field::Type(POne_Field::ComboBox) , 50 );
  iobj_KOD_DEP = (QComboBox*)ptrPRD->inputObject();
  iobj_KOD_DEP->setEditable( false );
  //iobj_KOD_DEP->lineEdit()->setMaxLength( 3 );

  model_in_ComboBox_KOD_DEP = new QSqlQueryModel;
  iobj_KOD_DEP->setModel( model_in_ComboBox_KOD_DEP );  

  setupLayouts();
}

#include<iostream>

bool S_PROD_PGR_DBTable::virtualAddRec( void )
{
  iobj_NAME->setText( tr(""));
  iobj_NAME->setFocus();

  iobj_K->setValue(0);
  model_in_ComboBox_KOD_DEP->setQuery( "SELECT KOD FROM S_DEP" , db );
  
  if( !precdialog->exec() ) return true;
  QSqlQuery query( db );

  strKOD = iobj_NAME->text();

  return query.exec("INSERT INTO S_PROD_PGR (NAME, K, KODM , KOD_DEP ) VALUES ( '"
		    + iobj_NAME->text() + "','" 
		    + QString::number(iobj_K->value())+"','"
		    + strKODM + "','" 
		    + iobj_KOD_DEP->currentText() + "')");

  std::cout<<QString::number(iobj_K->value()).toStdString()<<std::endl;
}

bool S_PROD_PGR_DBTable::virtualEditRec( int & row )
{
  QSqlRecord record = model->record( row );
  
  QString lastStrKOD =  record.value("NAME").toString();

  iobj_NAME->setText( lastStrKOD );
  iobj_NAME->setFocus();

  iobj_NAME->setText( record.value("NAME").toString());
  iobj_K->setValue( record.value("K").toDouble());
  
  model_in_ComboBox_KOD_DEP->setQuery( "SELECT KOD FROM S_DEP" , db );
  iobj_KOD_DEP->setEditText( record.value("KOD_DEP").toString() );
 
  if( !precdialog->exec() ) return true;

  strKOD = iobj_NAME->text();
  
  QSqlQuery query( db );
  return query.exec( "UPDATE S_PROD_PGR SET NAME = '" + strKOD 
		     + "', K = '" + QString::number(iobj_K->value())
		     + "', KOD_DEP = '" + iobj_KOD_DEP->currentText()
		     + "' WHERE NAME = '" + lastStrKOD + "'" );
}

bool S_PROD_PGR_DBTable::virtualDelRec( int & row )
{
  QSqlRecord record = model->record( row );

  switch( QMessageBox::information( this, tr( "Удаление запись"),
				    tr("Вы действительно хотите удалить запись : ") +
				    record.value("NAME").toString() + " ?",
				    tr("Да"), tr("Нет"), 0 , 1 ) ) { 
  case 0:{
    QSqlQuery query( db );
    return query.exec("DELETE FROM S_PROD_PGR WHERE NAME = '" 
		      +  record.value("NAME").toString() + "'");
  }
  case 1: 
    return true;
  }
  return true;
}
// Подгруппа ...

// Общий виджет из двух таблиц -------------------------------------------------------------

S_PROD_DBTable::S_PROD_DBTable(QWidget *parent)
    : QWidget(parent)
{
  vert_splitter = new QSplitter( Qt::Vertical ); 
  
  s_prod_gr_dbtable  = new S_PROD_GR_DBTable(vert_splitter);
  s_prod_pgr_dbtable = new S_PROD_PGR_DBTable(vert_splitter);

  connect( s_prod_gr_dbtable->ptrView->selectionModel()  , 
	   SIGNAL( currentChanged ( const QModelIndex & , const QModelIndex & )), 
	   s_prod_pgr_dbtable , 
	   SLOT(update_PGR( const QModelIndex & )));

  QVBoxLayout* formLayout = new QVBoxLayout( this );
  formLayout->addWidget( vert_splitter );
  formLayout->setMargin ( 0 );
}
// Общий виджет из двух таблиц ...
