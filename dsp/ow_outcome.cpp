/*
    ow_outcome.cpp
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
#include <QDate>

#include "ow_outcome.h"

// Размеры полей по умолчанию
const int widthNAME = 200,
  widthVAG = 60,
  widthMASS = 60,
  widthED = 55,
  widthKOD_POTR = 200;

OW_Outcome::OW_Outcome(QWidget *parent)
    : OW_AbstractAccumulate(parent)
{
  captionViewLabel->setText( tr("Отгрузка продукции"));

  //  owDate = QDate::currentDate();

  selectMainSQL = "SELECT KOD,NAME FROM S_PROD_GR ORDER BY KOD";

  mainModel->setQuery( selectMainSQL , db );
  mainModel->setHeaderData(0, Qt::Horizontal, tr("Код"));
  mainModel->setHeaderData(1, Qt::Horizontal, tr("Наименование"));
  mainView->setColumnWidth( 0, 40 );
  mainView->setColumnWidth( 1, 130 );

  selectSQL = "SELECT S_POTR.NAME AS NAME_POTR,OTGRUZKA.VAG,OTGRUZKA.MASS,S_PROD_GR.ED "
    "FROM S_PROD_GR,OTGRUZKA,S_POTR "
    "WHERE OTGRUZKA.PDATE='" + owDate.toString("dd.MM.yyyy") + "' "
    "AND S_PROD_GR.KOD=OTGRUZKA.KOD AND S_POTR.KOD=OTGRUZKA.KOD_POTR "
    "ORDER BY S_PROD_GR.KOD";

  model->setQuery( selectSQL , db );
  model->setHeaderData( 0, Qt::Horizontal, tr("Потребитель"));
  model->setHeaderData( 1, Qt::Horizontal, tr("Вагонов"));
  model->setHeaderData( 2, Qt::Horizontal, tr("Масса"));
  model->setHeaderData( 3, Qt::Horizontal, tr("Ед.изм."));

  view->setColumnWidth( 0, widthKOD_POTR );
  view->setColumnWidth( 1, widthVAG );
  view->setColumnWidth( 2, widthMASS );
  view->setColumnWidth( 3, widthED );

  tabloLayout->addWidget( tabloCapLabel , 0 , 0 , 1 , 4 );
  tabloLayout->addWidget( tabloDateLabel , 2 , 0 , 1 , 1 );

  QFrame *line = new QFrame;	// --------------------------
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow ( QFrame::Sunken );
  tabloLayout->addWidget ( line , 1, 0, 1, 4 );

  QLabel *label = new QLabel( tr("За сутки:"));
  label->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
  label->setAutoFillBackground(true); // >>+<<
  label->setScaledContents(false); 
  label->setMargin(2);
  tabloLayout->addWidget ( label, 3, 0, 1, 1 );

  label = new QLabel( tr("За месяц:"));
  label->setAutoFillBackground(true); // >>+<<
  label->setScaledContents(false); 
  label->setMargin(2);
  tabloLayout->addWidget ( label, 4, 0, 1, 1 );

  label = new QLabel( tr("Вагонов"));
  label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  label->setFrameStyle( QFrame::Panel | QFrame::Raised );
  label->setAutoFillBackground(true); // >>+<<
  label->setPalette( *capPalette );
  label->setFont( *boldFont );
  label->setScaledContents(false); 
  label->setMargin(2);
  tabloLayout->addWidget ( label, 2, 1, 1, 1 );

  label = new QLabel( tr("Масса"));
  label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  label->setFrameStyle( QFrame::Panel | QFrame::Raised );
  label->setAutoFillBackground(true); // >>+<<
  label->setPalette( *capPalette );
  label->setFont( *boldFont );
  label->setScaledContents(false); 
  label->setMargin(2);
  tabloLayout->addWidget ( label, 2, 2, 1, 1 );

  label = new QLabel( tr("Ед.массы"));
  label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  label->setFrameStyle( QFrame::Panel | QFrame::Raised );
  label->setAutoFillBackground(true); // >>+<<
  label->setPalette( *capPalette );
  label->setFont( *boldFont );
  label->setScaledContents(false); 
  label->setMargin(2);
  tabloLayout->addWidget ( label, 2, 3, 1, 1 );

  tabloLabelVAGDay = new QLabel;
  tabloLabelVAGDay->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  tabloLabelVAGDay->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  tabloLabelVAGDay->setAutoFillBackground(true); // >>+<<
  tabloLabelVAGDay->setScaledContents(false); 
  tabloLabelVAGDay->setMargin(2);
  tabloLayout->addWidget ( tabloLabelVAGDay, 3, 1, 1, 1 );

  tabloLabelVAGMonth = new QLabel;
  tabloLabelVAGMonth->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  tabloLabelVAGMonth->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  tabloLabelVAGMonth->setAutoFillBackground(true); // >>+<<
  tabloLabelVAGMonth->setScaledContents(false); 
  tabloLabelVAGMonth->setMargin(2);
  tabloLayout->addWidget ( tabloLabelVAGMonth, 4, 1, 1, 1 );

  tabloLabelMASSDay = new QLabel;
  tabloLabelMASSDay->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  tabloLabelMASSDay->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  tabloLabelMASSDay->setAutoFillBackground(true); // >>+<<
  tabloLabelMASSDay->setScaledContents(false); 
  tabloLabelMASSDay->setMargin(2);
  tabloLayout->addWidget ( tabloLabelMASSDay, 3, 2, 1, 1 );

  tabloLabelMASSMonth = new QLabel;
  tabloLabelMASSMonth->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  tabloLabelMASSMonth->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  tabloLabelMASSMonth->setAutoFillBackground(true); // >>+<<
  tabloLabelMASSMonth->setScaledContents(false); 
  tabloLabelMASSMonth->setMargin(2);
  tabloLayout->addWidget ( tabloLabelMASSMonth, 4, 2, 1, 1 );

  tabloLabelED_MASSDay = new QLabel;
  tabloLabelED_MASSDay->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  tabloLabelED_MASSDay->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  tabloLabelED_MASSDay->setAutoFillBackground(true); // >>+<<
  tabloLabelED_MASSDay->setScaledContents(false); 
  tabloLabelED_MASSDay->setMargin(2);
  tabloLayout->addWidget ( tabloLabelED_MASSDay, 3, 3, 1, 1 );

  tabloLabelED_MASSMonth = new QLabel;
  tabloLabelED_MASSMonth->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  tabloLabelED_MASSMonth->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  tabloLabelED_MASSMonth->setAutoFillBackground(true); // >>+<<
  tabloLabelED_MASSMonth->setScaledContents(false); 
  tabloLabelED_MASSMonth->setMargin(2);
  tabloLayout->addWidget ( tabloLabelED_MASSMonth, 4, 3, 1, 1 );

  // Описание диалога ---
  POne_Field* ptrPRD;

  ptrPRD = precdialog->addInputObject( tr("Потребитель: (кл. F4 - список)") , 
				       POne_Field::Type(POne_Field::ComboBox) , 200 );
  iobj_KOD_POTR = (QComboBox*)ptrPRD->inputObject();
  iobj_KOD_POTR->setEditable( true );
  iobj_KOD_POTR->lineEdit()->setMaxLength( 30 );

  connect( iobj_KOD_POTR, SIGNAL( editTextChanged ( const QString & )), this, SLOT(setTextKOD_POTR( const QString & ))); 
  connect( iobj_KOD_POTR, SIGNAL( currentIndexChanged ( const QString & )), this, SLOT(setTextKOD_POTR( const QString & ))); 
  
  model_in_ComboBox_KOD_POTR = new QSqlQueryModel;
  iobj_KOD_POTR->setModel( model_in_ComboBox_KOD_POTR );

  ptrPRD = precdialog->addInputObject( tr("Вагонов:") , 
				       POne_Field::Type(POne_Field::SpinBox) , 80 );
  iobj_VAG = (QSpinBox*)ptrPRD->inputObject();
  iobj_VAG->setRange(0, 200);
  iobj_VAG->setValue(0);
  iobj_VAG->setSuffix(tr(" шт."));
  iobj_VAG->setAlignment(Qt::AlignRight);
  
  ptrPRD = precdialog->addInputObject( tr("Масса:") , 
				       POne_Field::Type(POne_Field::DoubleSpinBox) , 130 );
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
void OW_Outcome::update_PGR( const QModelIndex & index )
{
  if (index.isValid())
    {
      QSqlRecord record = ((QSqlQueryModel *)index.model())->record( index.row () );

      strMainKOD =  record.value("KOD").toString();

      selectSQL = "SELECT S_POTR.NAME AS NAME_POTR,OTGRUZKA.VAG,OTGRUZKA.MASS,S_PROD_GR.ED "
	"FROM S_PROD_GR,OTGRUZKA,S_POTR "
	"WHERE OTGRUZKA.PDATE='" + owDate.toString("dd.MM.yyyy") + "' "
	"AND OTGRUZKA.KOD='" + strMainKOD + "' AND S_PROD_GR.KOD='" + strMainKOD + "' AND S_POTR.KOD=OTGRUZKA.KOD_POTR "
	"ORDER BY S_POTR.NAME";

      model->setQuery( selectSQL , db );
      view->selectRow ( 0 );

      // Обновляем данные в табло
      tabloCapLabel->setText( record.value("NAME").toString() );
      tabloDateLabel->setText( owDate.toString("dd.MM.yyyy") );
      updateTablo();
    }
}

// Обновить табло
void OW_Outcome::updateTablo( void )
{
      QSqlQuery query( db );
      
      query.exec("SELECT SUM( VAG ), SUM( MASS )  FROM OTGRUZKA WHERE KOD='" 
		 + strMainKOD + "'"
		 " AND PDATE='"+ owDate.toString("dd.MM.yyyy") + "'");
      query.first();
      tabloLabelVAGDay->setText( locale->toString( query.value(0).toInt()));
      tabloLabelMASSDay->setText( locale->toString( query.value(1).toDouble()));

      query.exec("SELECT SUM( VAG ), SUM( MASS ) FROM OTGRUZKA"
		 " WHERE KOD='" + strMainKOD + "'"
		 " AND PDATE BETWEEN '" + "01" + owDate.toString(".MM.yyyy") + "'"
		 " AND '"+ owDate.toString("dd.MM.yyyy") + "'");
      query.first();
      tabloLabelVAGMonth->setText( locale->toString( query.value(0).toInt()));
      tabloLabelMASSMonth->setText( locale->toString( query.value(1).toDouble()));

      query.exec("SELECT ED FROM S_PROD_GR"
		 " WHERE KOD='" + strMainKOD + "'");
      query.first();
      tabloLabelED_MASSDay->setText( query.value(0).toString());
      tabloLabelED_MASSMonth->setText( query.value(0).toString());
}

void OW_Outcome::setTextKOD_POTR( const QString & text )
{
  if ( text != "" )
      KOD_POTRtext = text;
}

QString OW_Outcome::textKOD_POTR() const
{
  return KOD_POTRtext;
}

bool OW_Outcome::virtualAddRec( void )
{
  iobj_KOD_POTR->setFocus();
  model_in_ComboBox_KOD_POTR->setQuery( "SELECT NAME FROM S_POTR ORDER BY NAME" , db );

  iobj_VAG->setValue(0);
  iobj_MASS->setValue(0);
  iobj_MASS->setSuffix(tr(" ")+tabloLabelED_MASSDay->text());

  // Выполняем диалог
  if( !precdialog->exec() ) return true;

  QSqlQuery query( db );

  // Формируем запрос
  query.exec("SELECT KOD FROM S_POTR WHERE NAME='" + textKOD_POTR() + "'");
  if(!query.first())		// Если таковых записей не имеется
    {
      query.exec("SELECT KOD FROM S_POTR ORDER BY KOD");
      query.last();
      QString kod = QString::number( query.value(0).toInt() + 1 );

      query.exec("INSERT INTO S_POTR (KOD, NAME) VALUES ( "
		 + kod + ",'"+
		 textKOD_POTR()+
		 "')");

      // Повторяем запрос
      query.exec("SELECT KOD FROM S_POTR WHERE NAME='" + textKOD_POTR() + "'");
      query.first();
      
      strKOD = textKOD_POTR();
    }
  else 
    strKOD = iobj_KOD_POTR->currentText();

  QString str_KOD_POTR = query.value(0).toString();

  bool rezult =  query.exec("INSERT INTO OTGRUZKA ( KOD, PDATE , VAG, MASS, KOD_POTR ) VALUES ( '"
			    + strMainKOD + "','"
			    + owDate.toString("dd.MM.yyyy") + "','"
			    + QString::number(iobj_VAG->value()) + "','"
			    + QString::number(iobj_MASS->value()) + "','"
			    + str_KOD_POTR
			    + "')");

  // Обновляем данные в табло
  updateTablo();
  return rezult;
}

bool OW_Outcome::virtualEditRec( int & row )
{
  QSqlRecord record = model->record( row );
  QSqlQuery query( db );

  // Запоминаем предварительные данные
  query.exec("SELECT KOD FROM S_POTR WHERE NAME='" + record.value("NAME_POTR").toString() + "'");
  query.first();
  QString lastStrKOD_POTR = query.value(0).toString();

  // Заполняем диалог текущими данными
  model_in_ComboBox_KOD_POTR->setQuery( "SELECT NAME FROM S_POTR ORDER BY NAME" , db );
  iobj_KOD_POTR->setEditText( record.value("NAME_POTR").toString() );
  iobj_KOD_POTR->setFocus();

  iobj_VAG->setValue( record.value("VAG").toInt() );
  iobj_MASS->setValue( record.value("MASS").toDouble());
  iobj_MASS->setSuffix(tr(" ")+tabloLabelED_MASSDay->text());


  // выполнить диалог
  iobj_KOD_POTR->setEditable( false );
  bool rez = !precdialog->exec(); 
  iobj_KOD_POTR->setEditable( true );

  if( rez ) return true; // Если произошла отмена то выйти


  // Формируем запрос
  query.exec("SELECT KOD FROM S_POTR WHERE NAME='" + iobj_KOD_POTR->currentText() + "'");
  query.first();
  QString newStrKOD_POTR = query.value(0).toString();

  strKOD = iobj_KOD_POTR->currentText();
  
  bool rezult =  query.exec( "UPDATE OTGRUZKA SET "
			     "KOD_POTR = '" + newStrKOD_POTR
			     + "',VAG = '" + QString::number(iobj_VAG->value())
			     + "', MASS = '" + QString::number(iobj_MASS->value())
			     + "' WHERE KOD = '" + strMainKOD + "'"
			     " AND PDATE='" + owDate.toString("dd.MM.yyyy")+ "'"
			     " AND KOD_POTR='" + lastStrKOD_POTR + "'");

  // Обновляем данные в табло
  updateTablo();
  return rezult;
}

bool OW_Outcome::virtualDelRec( int & row )
{
  QSqlRecord record = model->record( row );
  QSqlQuery query( db );

  // Запоминаем предварительные данные
  query.exec("SELECT KOD FROM S_POTR WHERE NAME='" + record.value("NAME_POTR").toString() + "'");
  query.first();
  QString StrKOD_POTR = query.value(0).toString();

  
  switch( QMessageBox::information( this, tr( "Удаление запись"),
				    tr("Вы действительно хотите удалить следующую запись ?: \n")
				    + tr(" потребитель - ") + record.value("NAME_POTR").toString() + "\n"
				    + tr(" вагонов - ") + record.value("VAG").toString() + " шт. \n"
				    + tr(" вес - ") + record.value("MASS").toString() + " " 
				    + record.value("ED").toString() + ".\n",
 				    tr("Да"), tr("Нет"), 0 , 1 ) ) { 
   case 0:{
     bool rezult =  query.exec("DELETE FROM OTGRUZKA WHERE "
			       "KOD = '" +  strMainKOD + "' "
			       "AND PDATE ='" +  owDate.toString("dd.MM.yyyy") + "' "
			       "AND KOD_POTR ='" +  StrKOD_POTR + "' ");
     // Обновляем данные в табло
     updateTablo();
     return rezult;
   }
   case 1: 
     return true;
   }
   return true;
}
