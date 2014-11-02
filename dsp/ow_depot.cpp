/*
    ow_depot.cpp
*/

#include <QTableView>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDate>

#include "ow_depot.h"

// Размеры полей по умолчанию
const int widthNAME = 200,
  widthMASS = 60,
  widthED = 55,
  widthKOD_POTR = 200;

OW_Depot::OW_Depot(QWidget *parent)
    : OW_AbstractAccumulate(parent)
{
  captionViewLabel->setText( tr("Остатки продукции"));

  //  owDate = QDate::currentDate();

  selectMainSQL = "SELECT KOD,NAME FROM S_PROD_GR ORDER BY KOD";

  mainModel->setQuery( selectMainSQL , db );
  mainModel->setHeaderData(0, Qt::Horizontal, tr("Код"));
  mainModel->setHeaderData(1, Qt::Horizontal, tr("Наименование"));
  mainView->setColumnWidth( 0, 40 );
  mainView->setColumnWidth( 1, 130 );

  selectSQL = "SELECT S_PROD_PGR.NAME,OSTAT_PROD_PGR.MASS,S_PROD_GR.ED "
    "FROM S_PROD_PGR,OSTAT_PROD_PGR,S_PROD_GR "
    "WHERE OSTAT_PROD_PGR.PDATE='" + owDate.toString("dd.MM.yyyy") + "' "
    "AND   OSTAT_PROD_PGR.KOD=S_PROD_PGR.KOD "
    "AND   S_PROD_PGR.KOD=OSTAT_PROD_PGR.KOD "
    "AND   S_PROD_GR.KOD=S_PROD_PGR.KODM "
    "ORDER BY S_PROD_PGR.NAME";

  model->setQuery( selectSQL , db );
  model->setHeaderData( 0, Qt::Horizontal, tr("Наименование"));
  model->setHeaderData( 1, Qt::Horizontal, tr("Масса"));
  model->setHeaderData( 2, Qt::Horizontal, tr("Ед.изм."));

  view->setColumnWidth( 0, widthKOD_POTR );
  view->setColumnWidth( 1, widthMASS );
  view->setColumnWidth( 2, widthED );

  tabloLayout->addWidget( tabloCapLabel , 0 , 0 , 1 , 3 );
  tabloLayout->addWidget( tabloDateLabel , 2 , 0 , 1 , 1 );

  QFrame *line = new QFrame;	// --------------------------
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow ( QFrame::Sunken );
  tabloLayout->addWidget ( line , 1, 0, 1, 3 );

  QLabel *label = new QLabel( tr("За сутки:"));
  label->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
  label->setAutoFillBackground(true); // >>+<<
  label->setScaledContents(false); 
  label->setMargin(2);
  tabloLayout->addWidget ( label, 3, 0, 1, 1 );

  label = new QLabel( tr("Масса"));
  label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  label->setFrameStyle( QFrame::Panel | QFrame::Raised );
  label->setAutoFillBackground(true); // >>+<<
  label->setPalette( *capPalette );
  label->setFont( *boldFont );
  label->setScaledContents(false); 
  label->setMargin(2);
  tabloLayout->addWidget ( label, 2, 1, 1, 1 );

  label = new QLabel( tr("Ед.массы"));
  label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  label->setFrameStyle( QFrame::Panel | QFrame::Raised );
  label->setAutoFillBackground(true); // >>+<<
  label->setPalette( *capPalette );
  label->setFont( *boldFont );
  label->setScaledContents(false); 
  label->setMargin(2);
  tabloLayout->addWidget ( label, 2, 2, 1, 1 );
 
  tabloLabelMASSDay = new QLabel;
  tabloLabelMASSDay->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  tabloLabelMASSDay->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  tabloLabelMASSDay->setAutoFillBackground(true); // >>+<<
  tabloLabelMASSDay->setScaledContents(false); 
  tabloLabelMASSDay->setMargin(2);
  tabloLayout->addWidget ( tabloLabelMASSDay, 3, 1, 1, 1 );

  tabloLabelED_MASSDay = new QLabel;
  tabloLabelED_MASSDay->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  tabloLabelED_MASSDay->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  tabloLabelED_MASSDay->setAutoFillBackground(true); // >>+<<
  tabloLabelED_MASSDay->setScaledContents(false); 
  tabloLabelED_MASSDay->setMargin(2);
  tabloLayout->addWidget ( tabloLabelED_MASSDay, 3, 2, 1, 1 );

  // Описание диалога ---
  POne_Field* ptrPRD;
  // -- 1
  ptrPRD = precdialog->addInputObject( tr("Наименование:") , 
				       POne_Field::Type(POne_Field::ComboBox) , 200 );
  iobj_KOD = (QComboBox*)ptrPRD->inputObject();
  iobj_KOD->setEditable( false );
  //iobj_KOD->lineEdit()->setMaxLength( 30 );

  model_in_ComboBox_KOD = new QSqlQueryModel;
  iobj_KOD->setModel( model_in_ComboBox_KOD );
  // -- 2
  ptrPRD = precdialog->addInputObject( tr("Масса:") , 
				       POne_Field::Type(POne_Field::DoubleSpinBox) , 140 );
  iobj_MASS = (QDoubleSpinBox*)ptrPRD->inputObject();
  iobj_MASS->setRange(0.0, 1000000000.0);
  iobj_MASS->setSingleStep(1);
  iobj_MASS->setValue(0.0);
  iobj_MASS->setAlignment(Qt::AlignRight);

  connect( mainView->selectionModel()  , 
	   SIGNAL( currentChanged ( const QModelIndex & , const QModelIndex & )), 
	   this , 
	   SLOT(update_PGR( const QModelIndex & )));
  
  mainView->selectRow ( 0 );
}

// Обновить данные по подгруппе согласно выбранной группе --------------------------------
void OW_Depot::update_PGR( const QModelIndex & index )
{
  if (index.isValid())
    {
      QSqlRecord record = ((QSqlQueryModel *)index.model())->record( index.row () );

      strMainKOD =  record.value("KOD").toString();

      selectSQL = "SELECT S_PROD_PGR.NAME,OSTAT_PROD_PGR.MASS,S_PROD_GR.ED "
	"FROM S_PROD_PGR,OSTAT_PROD_PGR,S_PROD_GR "
	"WHERE OSTAT_PROD_PGR.PDATE='" + owDate.toString("dd.MM.yyyy") + "' "
	"AND   OSTAT_PROD_PGR.KOD=S_PROD_PGR.KOD "
	"AND   S_PROD_PGR.KOD=OSTAT_PROD_PGR.KOD "
	"AND   S_PROD_GR.KOD=S_PROD_PGR.KODM "
	"AND   S_PROD_GR.KOD=" + strMainKOD +
	"ORDER BY S_PROD_PGR.NAME";

      model->setQuery( selectSQL , db );
      view->selectRow ( 0 );

      // Обновляем данные в табло
      tabloCapLabel->setText( record.value("NAME").toString() );
      tabloDateLabel->setText( owDate.toString("dd.MM.yyyy") );
      updateTablo();
    }
}

// Обновить табло
void OW_Depot::updateTablo( void )
{
      QSqlQuery query( db );
      
      query.exec("SELECT SUM( OSTAT_PROD_PGR.MASS ) FROM OSTAT_PROD_PGR,S_PROD_PGR "
		 "WHERE OSTAT_PROD_PGR.KOD=S_PROD_PGR.KOD "
		 "AND S_PROD_PGR.KODM=" + strMainKOD + 
		 " AND PDATE='"+ owDate.toString("dd.MM.yyyy") + "'");
      query.first();
      tabloLabelMASSDay->setText( locale->toString(query.value(0).toDouble()));
				  
				  
      query.exec("SELECT ED FROM S_PROD_GR"
		 " WHERE KOD='" + strMainKOD + "'");
      query.first();
      tabloLabelED_MASSDay->setText( query.value(0).toString());

}


bool OW_Depot::virtualAddRec( void )
{
  iobj_KOD->setFocus();
  model_in_ComboBox_KOD->setQuery( "SELECT NAME FROM S_PROD_PGR "
					"WHERE KODM=" + strMainKOD , db );

  iobj_MASS->setValue(0);
  iobj_MASS->setSuffix(tr(" ")+tabloLabelED_MASSDay->text());

  // Выполняем диалог
  if( !precdialog->exec() ) return true;
  QSqlQuery query( db );

  // Формируем запрос
  query.exec("SELECT KOD FROM S_PROD_PGR WHERE NAME='" + iobj_KOD->currentText() + "'"
	     " AND KODM=" + strMainKOD );
  query.first();
  QString str_KOD = query.value(0).toString();

  strKOD = iobj_KOD->currentText();

  bool rezult =  query.exec("INSERT INTO OSTAT_PROD_PGR ( KOD, PDATE , MASS ) VALUES ( '"
			    + str_KOD + "','"
			    + owDate.toString("dd.MM.yyyy") + "','"
			    + QString::number(iobj_MASS->value(),'f',2) + "')");
  
  // Обновляем данные в табло
  updateTablo();
  return rezult;
}

bool OW_Depot::virtualEditRec( int & row )
{
  QSqlRecord record = model->record( row );
  QSqlQuery query( db );

  // Запоминаем предварительные данные
  query.exec("SELECT KOD FROM S_PROD_PGR "
	     "WHERE NAME='" + record.value("NAME").toString() + "' "
	     "AND KODM=" + strMainKOD );
  query.first();
  QString lastStrKOD = query.value(0).toString();

  // Заполняем диалог текущими данными
  model_in_ComboBox_KOD->setQuery( "SELECT NAME FROM S_PROD_PGR "
					"WHERE KODM=" + strMainKOD , db );

  iobj_KOD->setEditText( record.value("NAME").toString() );
  iobj_KOD->setFocus();

  iobj_MASS->setValue( record.value("MASS").toDouble());
  iobj_MASS->setSuffix(tr(" ")+tabloLabelED_MASSDay->text());

  // выполнить диалог - *
  if( !precdialog->exec() ) return true;

  // Формируем запрос
  query.exec("SELECT KOD FROM S_PROD_PGR WHERE NAME='" + iobj_KOD->currentText() + "'"
	     " AND KODM=" + strMainKOD );
  query.first();
  QString newStrKOD = query.value(0).toString();

  strKOD = iobj_KOD->currentText();
  
  bool rezult =  query.exec( "UPDATE OSTAT_PROD_PGR SET "
			     "KOD = " + newStrKOD + ","
			     "MASS = '" + QString::number(iobj_MASS->value(),'f',2) + "'"
			     "WHERE KOD = " + lastStrKOD + " "
			     "AND PDATE='" + owDate.toString("dd.MM.yyyy")+ "'");

  // Обновляем данные в табло
  updateTablo();
  return rezult;
}

bool OW_Depot::virtualDelRec( int & row )
{
  QSqlRecord record = model->record( row );
  QSqlQuery query( db );

  // Запоминаем предварительные данные
  query.exec("SELECT KOD FROM S_PROD_PGR "
	     "WHERE NAME='" + record.value("NAME").toString() + "' "
	     "AND KODM=" + strMainKOD );
  query.first();
  QString StrKOD = query.value(0).toString();

  
  switch( QMessageBox::information( this, tr( "Удаление записи"),
				    tr("Вы действительно хотите удалить следующую запись ?: \n")
				    + tr(" Наименование - ") + record.value("NAME").toString() + "\n"
				    + tr(" вес - ") + record.value("MASS").toString() + " " 
				    + tabloLabelED_MASSDay->text() ,
 				    tr("Да"), tr("Нет"), 0 , 1 ) ) { 
   case 0:{
     bool rezult =  query.exec("DELETE FROM OSTAT_PROD_PGR WHERE "
			       "KOD = '" +  StrKOD + "' "
			       "AND PDATE ='" +  owDate.toString("dd.MM.yyyy") + "' ");
     // Обновляем данные в табло
     updateTablo();
     return rezult;
   }
   case 1: 
     return true;
   }
   return true;
}
