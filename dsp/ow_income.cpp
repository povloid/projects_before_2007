/*
    ow_income.cpp
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

#include "ow_income.h"

// Размеры полей по умолчанию
const int widthNAME = 200,
  widthVAG = 60,
  widthMASS = 60,
  widthED = 55,
  widthKOD_POST = 200;

OW_Income::OW_Income(QWidget *parent)
    : OW_AbstractAccumulate(parent)
{
  captionViewLabel->setText( tr("Приход со стороны"));

  //  owDate = QDate::currentDate();

  selectMainSQL = "SELECT KOD,NAME FROM S_PROD_GR ORDER BY KOD";

  mainModel->setQuery( selectMainSQL , db );
  mainModel->setHeaderData(0, Qt::Horizontal, tr("Код"));
  mainModel->setHeaderData(1, Qt::Horizontal, tr("Наименование"));
  mainView->setColumnWidth( 0, 40 );
  mainView->setColumnWidth( 1, 130 );

  selectSQL = "SELECT S_POST.NAME AS NAME_POST,PRIHOD.VAG,PRIHOD.MASS,S_PROD_GR.ED "
    "FROM S_PROD_GR,PRIHOD,S_POST "
    "WHERE PRIHOD.PDATE='" + owDate.toString("dd.MM.yyyy") + "' "
    "AND S_PROD_GR.KOD=PRIHOD.KOD AND S_POST.KOD=PRIHOD.KOD_POST "
    "ORDER BY S_PROD_GR.KOD";

  model->setQuery( selectSQL , db );
  model->setHeaderData( 0, Qt::Horizontal, tr("Поставщик"));
  model->setHeaderData( 1, Qt::Horizontal, tr("Вагонов"));
  model->setHeaderData( 2, Qt::Horizontal, tr("Масса"));
  model->setHeaderData( 3, Qt::Horizontal, tr("Ед.изм."));

  view->setColumnWidth( 0, widthKOD_POST );
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

  ptrPRD = precdialog->addInputObject( tr("Поставщик: (кл. F4 - список)") , 
				       POne_Field::Type(POne_Field::ComboBox) , 200 );
  iobj_KOD_POST = (QComboBox*)ptrPRD->inputObject();
  iobj_KOD_POST->setEditable( true );
  iobj_KOD_POST->lineEdit()->setMaxLength( 30 );

  connect( iobj_KOD_POST, SIGNAL( editTextChanged ( const QString & )), this, SLOT(setTextKOD_POST( const QString & ))); 
  connect( iobj_KOD_POST, SIGNAL( currentIndexChanged ( const QString & )), this, SLOT(setTextKOD_POST( const QString & ))); 
  
  model_in_ComboBox_KOD_POST = new QSqlQueryModel;
  iobj_KOD_POST->setModel( model_in_ComboBox_KOD_POST );

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
void OW_Income::update_PGR( const QModelIndex & index )
{
  if (index.isValid())
    {
      QSqlRecord record = ((QSqlQueryModel *)index.model())->record( index.row () );

      strMainKOD =  record.value("KOD").toString();

      selectSQL = "SELECT S_POST.NAME AS NAME_POST,PRIHOD.VAG,PRIHOD.MASS,S_PROD_GR.ED "
	"FROM S_PROD_GR,PRIHOD,S_POST "
	"WHERE PRIHOD.PDATE='" + owDate.toString("dd.MM.yyyy") + "' "
	"AND PRIHOD.KOD='" + strMainKOD + "' AND S_PROD_GR.KOD='" + strMainKOD + "' AND S_POST.KOD=PRIHOD.KOD_POST "
	"ORDER BY S_POST.NAME";

      model->setQuery( selectSQL , db );
      view->selectRow ( 0 );

      // Обновляем данные в табло
      tabloCapLabel->setText( record.value("NAME").toString() );
      tabloDateLabel->setText( owDate.toString("dd.MM.yyyy") );
      updateTablo();
    }
}

// Обновить табло
void OW_Income::updateTablo( void )
{
      QSqlQuery query( db );
      
      query.exec("SELECT SUM( VAG ), SUM( MASS )  FROM PRIHOD WHERE KOD='" 
		 + strMainKOD + "'"
		 " AND PDATE='"+ owDate.toString("dd.MM.yyyy") + "'");
      query.first();
      tabloLabelVAGDay->setText( locale->toString(query.value(0).toInt()));
      tabloLabelMASSDay->setText( locale->toString(query.value(1).toDouble()));

      query.exec("SELECT SUM( VAG ), SUM( MASS ) FROM PRIHOD"
		 " WHERE KOD='" + strMainKOD + "'"
		 " AND PDATE BETWEEN '" + "01" + owDate.toString(".MM.yyyy") + "'"
		 " AND '"+ owDate.toString("dd.MM.yyyy") + "'");
      query.first();
      tabloLabelVAGMonth->setText( locale->toString(query.value(0).toInt()));
      tabloLabelMASSMonth->setText( locale->toString(query.value(1).toDouble()));

      query.exec("SELECT ED FROM S_PROD_GR"
		 " WHERE KOD='" + strMainKOD + "'");
      query.first();
      tabloLabelED_MASSDay->setText( query.value(0).toString());
      tabloLabelED_MASSMonth->setText( query.value(0).toString());
}


void OW_Income::setTextKOD_POST( const QString & text )
{
  if ( text != "" )
    KOD_POSTtext = text;
}

QString OW_Income::textKOD_POST() const
{
  return KOD_POSTtext;
}



bool OW_Income::virtualAddRec( void )
{
  iobj_KOD_POST->setFocus();
  model_in_ComboBox_KOD_POST->setQuery( "SELECT NAME FROM S_POST ORDER BY NAME" , db );

  iobj_VAG->setValue(0);
  iobj_MASS->setValue(0);
  iobj_MASS->setSuffix(tr(" ")+tabloLabelED_MASSDay->text());

  // Выполняем диалог
  if( !precdialog->exec() ) return true;

  QSqlQuery query( db );


  // Формируем запрос
  query.exec("SELECT KOD FROM S_POST WHERE NAME='" + textKOD_POST() + "'");
  if(!query.first())		// Если таковых записей не имеется
    {
      query.exec("SELECT KOD FROM S_POST ORDER BY KOD");
      query.last();
      QString kod = QString::number( query.value(0).toInt() + 1 );

      query.exec("INSERT INTO S_POST (KOD, NAME) VALUES ( "
		 + kod + ",'"+
		 textKOD_POST()+
		 "')");

      // Повторяем запрос
      query.exec("SELECT KOD FROM S_POST WHERE NAME='" + textKOD_POST() + "'");
      query.first();
      
      strKOD = textKOD_POST();
    }
  else 
    strKOD = iobj_KOD_POST->currentText();

  QString str_KOD_POST = query.value(0).toString();

  bool rezult =  query.exec("INSERT INTO PRIHOD ( KOD, PDATE , VAG, MASS, KOD_POST ) VALUES ( '"
			    + strMainKOD + "','"
			    + owDate.toString("dd.MM.yyyy") + "','"
			    + QString::number(iobj_VAG->value()) + "','"
			    + QString::number(iobj_MASS->value()) + "','"
			    + str_KOD_POST
			    + "')");

  // Обновляем данные в табло
  updateTablo();
  return rezult;
}

bool OW_Income::virtualEditRec( int & row )
{
  QSqlRecord record = model->record( row );
  QSqlQuery query( db );

  // Запоминаем предварительные данные
  query.exec("SELECT KOD FROM S_POST WHERE NAME='" + record.value("NAME_POST").toString() + "'");
  query.first();
  QString lastStrKOD_POST = query.value(0).toString();

  // Заполняем диалог текущими данными
  model_in_ComboBox_KOD_POST->setQuery( "SELECT NAME FROM S_POST ORDER BY NAME" , db );
  iobj_KOD_POST->setEditText( record.value("NAME_POST").toString() );
  iobj_KOD_POST->setFocus();

  iobj_VAG->setValue( record.value("VAG").toInt() );
  iobj_MASS->setValue( record.value("MASS").toDouble());
  iobj_MASS->setSuffix(tr(" ")+tabloLabelED_MASSDay->text());

  // выполнить диалог
  iobj_KOD_POST->setEditable( false );
  bool rez = !precdialog->exec(); 
  iobj_KOD_POST->setEditable( true );

  if( rez ) return true; // Если произошла отмена то выйти

  // Формируем запрос
  query.exec("SELECT KOD FROM S_POST WHERE NAME='" + iobj_KOD_POST->currentText() + "'");
  query.first();
  QString newStrKOD_POST = query.value(0).toString();

  strKOD = iobj_KOD_POST->currentText();
  
  bool rezult =  query.exec( "UPDATE PRIHOD SET "
			     "KOD_POST = '" + newStrKOD_POST
			     + "',VAG = '" + QString::number(iobj_VAG->value())
			     + "', MASS = '" + QString::number(iobj_MASS->value())
			     + "' WHERE KOD = '" + strMainKOD + "'"
			     " AND PDATE='" + owDate.toString("dd.MM.yyyy")+ "'"
			     " AND KOD_POST='" + lastStrKOD_POST + "'");

  // Обновляем данные в табло
  updateTablo();
  return rezult;
}

bool OW_Income::virtualDelRec( int & row )
{
  QSqlRecord record = model->record( row );
  QSqlQuery query( db );

  // Запоминаем предварительные данные
  query.exec("SELECT KOD FROM S_POST WHERE NAME='" + record.value("NAME_POST").toString() + "'");
  query.first();
  QString StrKOD_POST = query.value(0).toString();

  
  switch( QMessageBox::information( this, tr( "Удаление запись"),
				    tr("Вы действительно хотите удалить следующую запись ?: \n")
				    + tr(" потребитель - ") + record.value("NAME_POST").toString() + "\n"
				    + tr(" вагонов - ") + record.value("VAG").toString() + " шт. \n"
				    + tr(" вес - ") + record.value("MASS").toString() + " " 
				    + record.value("ED").toString() + ".\n",
 				    tr("Да"), tr("Нет"), 0 , 1 ) ) { 
   case 0:{
     bool rezult =  query.exec("DELETE FROM PRIHOD WHERE "
			       "KOD = '" +  strMainKOD + "' "
			       "AND PDATE ='" +  owDate.toString("dd.MM.yyyy") + "' "
			       "AND KOD_POST ='" +  StrKOD_POST + "' ");
     // Обновляем данные в табло
     updateTablo();
     return rezult;
   }
   case 1: 
     return true;
   }
   return true;
}
