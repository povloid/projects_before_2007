/*
    ow_prod_phos.cpp
*/


#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QTableView>
#include <QPushButton>
#include <QMessageBox>

#include <QLocale>

#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QHeaderView>

#include "ow_prod_phos.h"
#include "ow_prod_phos_tablo.h"
#include "ow_prod_phos_addedit_dialog.h"
#include "ow_prod_phos_pump_dialog.h"

#include "dspconst.h"
#include "p_ru_itemdelegate.h"


OW_Prod_Phos::OW_Prod_Phos(QWidget *parent)
  : QWidget(parent)
{
  QVBoxLayout *mainLayout = new QVBoxLayout;
  setLayout( mainLayout );
 
  QPalette palette( Qt::blue );
  palette.setColor( QPalette::Background , QColor(Qt::white));

  QFont font;
  font.setBold(true);

  // Проверка на наличие записи на сегодняшнее число 
  QString currDate = QDate::currentDate().toString("dd.MM.yyyy");
 
  owCap = new QLabel( tr("Производство фосфора на "));
  owCap->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  owCap->setFrameStyle( QFrame::Box | QFrame::Raised );
  owCap->setAutoFillBackground(true); // >>+<<
  owCap->setScaledContents(false); 
  owCap->setPalette( palette );
  owCap->setFont(font);
  owCap->setMargin(2);

  mainLayout->addWidget( owCap );

  QPalette datePalette( Qt::white );
  datePalette.setColor( QPalette::Background , QColor( 0, 85, 255 ));
  datePalette.setColor( QPalette::Foreground , Qt::white );

  dateLabel = new QLabel( currDate );
  dateLabel->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  dateLabel->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  dateLabel->setAutoFillBackground(true); // >>+<<
  dateLabel->setScaledContents(false); 
  //dateLabel->setMargin(5);
  dateLabel->setPalette( datePalette );
  dateLabel->setFont( QFont("Lucida", 13 ) );
  dateLabel->setMargin( 4 );
  //  tabloDateLabel->setSizePolicy( QSizePolicy::Expanding );
  dateLabel->setFrameStyle( QFrame::Box | QFrame::Raised );


  // --------------------------------------------------------------------------
  // Создание модели для работы с базой данных и виджета отображения оной -----

  // Подключение к базе данных ---------------------------------
  db = QSqlDatabase::database("DSP"); 
  db.transaction();		// Начинаем транзакцию

  // Создаем SQL модель
  model = new QSqlQueryModel;

  model->setHeaderData(0, Qt::Horizontal, tr("Дата:"));
  model->setHeaderData(1, Qt::Horizontal, tr("5-Печь"));
  model->setHeaderData(2, Qt::Horizontal, tr("6-Печь"));
  model->setHeaderData(3, Qt::Horizontal, tr("7-Печь"));
  model->setHeaderData(4, Qt::Horizontal, tr("8-Печь"));
  model->setHeaderData(5, Qt::Horizontal, tr("И того МВт"));
  model->setHeaderData(6, Qt::Horizontal, tr("И того тн"));


//   model->setHeaderData(7, Qt::Horizontal, tr("Козфф."));
//   model->setHeaderData(8, Qt::Horizontal, tr("5-Печь за ночь"));
//   model->setHeaderData(9, Qt::Horizontal, tr("6-Печь за ночь"));
//   model->setHeaderData(10, Qt::Horizontal, tr("7-Печь за ночь"));
//   model->setHeaderData(11, Qt::Horizontal, tr("8-Печь за ночь"));
//   model->setHeaderData(12, Qt::Horizontal, tr("И того"));
//   model->setHeaderData(13, Qt::Horizontal, tr("P4 - 5-Печь"));
//   model->setHeaderData(14, Qt::Horizontal, tr("P4 - 6-Печь"));
//   model->setHeaderData(15, Qt::Horizontal, tr("P4 - 7-Печь"));
//   model->setHeaderData(16, Qt::Horizontal, tr("P4 - 8-Печь"));
//   model->setHeaderData(17, Qt::Horizontal, tr("Пер. P4 - 5-Печь"));
//   model->setHeaderData(18, Qt::Horizontal, tr("Пер. P4 - 6-Печь"));
//   model->setHeaderData(19, Qt::Horizontal, tr("Пер. P4 - 7-Печь"));
//   model->setHeaderData(20, Qt::Horizontal, tr("Пер. P4 - 8-Печь"));
//   model->setHeaderData(21, Qt::Horizontal, tr("Пер. Шлама - 5-Печь"));
//   model->setHeaderData(22, Qt::Horizontal, tr("Пер. Шлама - 6-Печь"));
//   model->setHeaderData(23, Qt::Horizontal, tr("Пер. Шлама - 7-Печь"));
//   model->setHeaderData(24, Qt::Horizontal, tr("Пер. Шлама - 8-Печь"));

  // Создаем таблицу  
  view = new QTableView;
  view->setModel(model);
  view->setSelectionBehavior ( QAbstractItemView::SelectRows );
  view->setSelectionMode ( QAbstractItemView::SingleSelection ); 
  view->setAlternatingRowColors ( true ); // отображать с чередующимися цветами строк
  view->setTabKeyNavigation ( false ); // отключить навигацию по кнопке TAB
  ((QHeaderView *)view->verticalHeader())->setDefaultSectionSize( 20 );
  ((QHeaderView *)view->verticalHeader())->hide();
  view->horizontalHeader()->setClickable(false);

  setOWDate( QDate::currentDate() );

  // Установка размеров колонок ( полей ) при отображении
  view->setColumnWidth( 0, 80 );
  view->setColumnWidth( 1, 60 );
  view->setColumnWidth( 2, 60 );
  view->setColumnWidth( 3, 60 );
  view->setColumnWidth( 4, 60 );
  view->setColumnWidth( 5, 80 );
  view->setColumnWidth( 6, 80 );

  P_RU_ItemDelegate *p_ru_ItemDelegate = new P_RU_ItemDelegate;
  view->setItemDelegate( p_ru_ItemDelegate );


  for ( int i = 7 ; i < 25 ; i++ )
    view->setColumnHidden ( i, true );

//   view->setColumnWidth( 7, 80 );
//   view->setColumnWidth( 8, 80 );
//   view->setColumnWidth( 9, 80 );
//   view->setColumnWidth( 10, 80 );
//   view->setColumnWidth( 11, 80 );
//   view->setColumnWidth( 12, 80 );
//   view->setColumnWidth( 13, 80 );
//   view->setColumnWidth( 14, 80 );
//   view->setColumnWidth( 15, 80 );
//   view->setColumnWidth( 16, 80 );
//   view->setColumnWidth( 17, 80 );
//   view->setColumnWidth( 18, 80 );
//   view->setColumnWidth( 18, 80 );
//   view->setColumnWidth( 19, 80 );
//   view->setColumnWidth( 20, 80 );
//   view->setColumnWidth( 21, 80 );
//   view->setColumnWidth( 22, 80 );
//   view->setColumnWidth( 23, 80 );
//   view->setColumnWidth( 24, 80 );

  mainLayout->addWidget( view );

  // ..

  // Табло O.O ------------------------------------------------
  QLabel *label;
  QVBoxLayout *ppVBoxLayout;
  QHBoxLayout *ppCapLayout;

  QGroupBox *groupBox_vs_ow_prod_phos_tablo = new QGroupBox(tr("Общие показатели:"));
  ppVBoxLayout = new QVBoxLayout;
  ppVBoxLayout->setMargin ( 2 );
  groupBox_vs_ow_prod_phos_tablo->setLayout(ppVBoxLayout);

  // Верхняя часть табло -----
  ppCapLayout = new QHBoxLayout;
  ppCapLayout->setMargin ( 2 );
  ppVBoxLayout->addLayout( ppCapLayout );

  label = new QLabel( tr("Показания на:"));
  ppCapLayout->addWidget( label );

  ppCapLayout->addWidget( dateLabel );

  ppCapLayout->addStretch();

  pBtGotoCurrentDate = new QPushButton( tr ("Перейти на сегодня..."));
  pBtGotoCurrentDate->setIconSize ( QSize( 24, 24 ) );
  pBtGotoCurrentDate->setIcon( QIcon( radiantPath + "/cal_24.png") );
  pBtGotoCurrentDate->setMaximumHeight ( buttonsMaximumHeight );

  ppCapLayout->addWidget( pBtGotoCurrentDate );
  // ..

  QFrame *line;
  // Разделитель >-------<
  line = new QFrame;	// --------------------------
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow ( QFrame::Sunken );
  ppVBoxLayout->addWidget ( line );

  ow_prod_phos_tablo = new OW_Prod_Phos_Tablo;
  ppVBoxLayout->addWidget( ow_prod_phos_tablo );

  mainLayout->addWidget(groupBox_vs_ow_prod_phos_tablo);
  // ..

  // Нижняя панель с кнопками -------------------------------------
  QHBoxLayout *downLayout = new QHBoxLayout;

  pBtK = new QPushButton( tr( "Коэффициент" ) );
  pBtK->setIconSize ( QSize( 24, 24 ) );
  pBtK->setIcon( QIcon( radiantPath + "/games_24.png") );
  pBtK->setMaximumHeight ( buttonsMaximumHeight );

  downLayout->addWidget( pBtK );
  downLayout->addStretch();

  pBtAddEndEdit = new QPushButton( tr( "F4 Доб./Исп. Выработка" ) );
  pBtAddEndEdit->setIconSize ( QSize( 24, 24 ) );
  pBtAddEndEdit->setIcon( QIcon( radiantPath + "/add_24.png") );
  pBtAddEndEdit->setMaximumHeight ( buttonsMaximumHeight );
  pBtAddEndEdit->setShortcut( Qt::Key_F4 );
  downLayout->addWidget( pBtAddEndEdit );

  connect( pBtAddEndEdit, SIGNAL(clicked()), this, SLOT( addEdit()));

  pBtAddEndEditPumping = new QPushButton( tr( "F5 Доб./Исп. Перекачка" ) );
  pBtAddEndEditPumping->setIconSize ( QSize( 24, 24 ) );
  pBtAddEndEditPumping->setIcon( QIcon( radiantPath + "/add_24.png") );
  pBtAddEndEditPumping->setShortcut( Qt::Key_F5 );
  pBtAddEndEditPumping->setMaximumHeight ( buttonsMaximumHeight );
  downLayout->addWidget( pBtAddEndEditPumping );

  connect( pBtAddEndEditPumping, SIGNAL(clicked()), this, SLOT( addEditPumping()));

  pBtEditMonth = new QPushButton( tr( "F5 Ред. месяц" ) );
  pBtEditMonth->setIconSize ( QSize( 24, 24 ) );
  pBtEditMonth->setIcon( QIcon( radiantPath + "/edit_24.png") );
  pBtEditMonth->setMaximumHeight ( buttonsMaximumHeight );

  downLayout->addWidget( pBtEditMonth );

  mainLayout->addLayout( downLayout );
  // ..

  // Сигналы и слоты *-> >>--
  connect( view->selectionModel()  , 
	   SIGNAL( currentChanged ( const QModelIndex & , const QModelIndex & )), 
	   this , 
	   SLOT( update_ow_prod_phos_tablo ( const QModelIndex & )));


  view->setFocus();
  view->selectRow ( model->rowCount() - 1 );
}

// Обновить до даты
void OW_Prod_Phos::setOWDate( const QDate & date )
{
  int row = view->currentIndex().row();
    
  QSqlQuery query( db );
  
  QDate tmpDate( date.year() , date.month() , 1 );
  
  do
    {
     query.exec("INSERT INTO FOSFOR ( PDATE ) VALUES ( '" 
		+ tmpDate.toString("dd.MM.yyyy")
		+ "' ) ");

     tmpDate = tmpDate.addDays( 1 );
    }  
  while ( tmpDate < date.addDays( 1 ) );

  selectSQL = "SELECT PDATE, P5_MW, P6_MW, P7_MW, P8_MW, P_MW_SUM, P_P4_SUM, KBALK,"
    " P5_NIGHT_MW, P6_NIGHT_MW, P7_NIGHT_MW, P8_NIGHT_MW, P_NIGHT_MW_SUM,"
    " P5_P4, P6_P4, P7_P4, P8_P4,"
    " P5_PUMP_P4, P6_PUMP_P4, P7_PUMP_P4, P8_PUMP_P4,"
    " P5_PUMP_SLUDGE, P6_PUMP_SLUDGE, P7_PUMP_SLUDGE, P8_PUMP_SLUDGE"
    " FROM FOSFOR "
    " WHERE PDATE BETWEEN '01" + date.toString(".MM.yyyy") + "'" 
    " AND '" + date.toString("dd.MM.yyyy") + "'"
    " ORDER BY PDATE";

  model->setQuery( selectSQL , db ); // Выполнить запрос

  if( row > model->rowCount() - 1)
    row =  model->rowCount() - 1;
  
  view->selectRow ( row );

  dateLabel->setText( date.toString("dd.MM.yyyy"));
}

// Добавить или редактировать запись по ВЫРАБОТКЕ --------
void OW_Prod_Phos::addEdit( void )
{
  int row = view->currentIndex().row();

  if( row == -1 ) // Если не выбрана запись
    {
      QMessageBox::information( this, tr("Внимание"),
				tr("Нет выбранной записи.") );
      return;
    }
  
  QSqlRecord record = model->record( row );

  QString selectDate = record.value("PDATE").toString();

  OW_Prod_Phos_AddEdit_Dialog addEditDialog;  

  addEditDialog.setCapText( tr("Доб./Ред. записи по выработке на: [") + selectDate + tr("]") );
  addEditDialog.setCapPixmap( radiantPath + "/add_32.png" );

  OW_Prod_Phos_Tablo *tablo = addEditDialog.ow_prod_phos_tablo;

  calc_manth_SUM( tablo ); // Задаем суммы за месяц без значений текущей записи

  tablo->setText_for_day_in_P5( record.value("P5_MW").toDouble());
  tablo->setText_for_day_in_P6( record.value("P6_MW").toDouble());
  tablo->setText_for_day_in_P7( record.value("P7_MW").toDouble());
  tablo->setText_for_day_in_P8( record.value("P8_MW").toDouble());

  tablo->setText_for_night_in_P5( record.value("P5_NIGHT_MW").toDouble());
  tablo->setText_for_night_in_P6( record.value("P6_NIGHT_MW").toDouble());
  tablo->setText_for_night_in_P7( record.value("P7_NIGHT_MW").toDouble());
  tablo->setText_for_night_in_P8( record.value("P8_NIGHT_MW").toDouble());

  tablo->setText_P4_For_Day_in_P5( record.value("P5_P4").toDouble());
  tablo->setText_P4_For_Day_in_P6( record.value("P6_P4").toDouble());
  tablo->setText_P4_For_Day_in_P7( record.value("P7_P4").toDouble());
  tablo->setText_P4_For_Day_in_P8( record.value("P8_P4").toDouble());

  if (!addEditDialog.exec()) return;

  QSqlQuery query( db );

  if (!query.exec( "UPDATE FOSFOR SET "
 		   "P5_MW = '" + QString::number( tablo->value_for_day_in_P5()) + "',"
 		   "P6_MW = '" + QString::number( tablo->value_for_day_in_P6()) + "',"
 		   "P7_MW = '" + QString::number( tablo->value_for_day_in_P7()) + "',"
 		   "P8_MW = '" + QString::number( tablo->value_for_day_in_P8()) + "',"
 		   "P5_NIGHT_MW = '" + QString::number( tablo->value_for_night_in_P5()) + "',"
 		   "P6_NIGHT_MW = '" + QString::number( tablo->value_for_night_in_P6()) + "',"
 		   "P7_NIGHT_MW = '" + QString::number( tablo->value_for_night_in_P7()) + "',"
 		   "P8_NIGHT_MW = '" + QString::number( tablo->value_for_night_in_P8()) + "'"
		   " WHERE PDATE = '" + selectDate + "'" ))
    QMessageBox::warning( this, tr("Ошибка"),   
			  tr("SQL запрос не прошел!")); 
 
  model->setQuery( selectSQL , db ); // Выполнить запрос

  view->selectRow ( row );
  
  return;
}

// Обновить табло при перемещении курсора по таблице
void OW_Prod_Phos::update_ow_prod_phos_tablo( const QModelIndex & index ) // >>-- slot
{
  if (index.isValid())
    {
      QSqlRecord record = ((QSqlQueryModel *)index.model())->record( index.row () );

      OW_Prod_Phos_Tablo *tablo = ow_prod_phos_tablo;

      QString selectDate = record.value("PDATE").toString();

      calc_manth_SUM( tablo ); // Задаем суммы за месяц без значений текущей записи

      tablo->setText_for_day_in_P5( record.value("P5_MW").toDouble());
      tablo->setText_for_day_in_P6( record.value("P6_MW").toDouble());
      tablo->setText_for_day_in_P7( record.value("P7_MW").toDouble());
      tablo->setText_for_day_in_P8( record.value("P8_MW").toDouble());

      tablo->setText_for_night_in_P5( record.value("P5_NIGHT_MW").toDouble());
      tablo->setText_for_night_in_P6( record.value("P6_NIGHT_MW").toDouble());
      tablo->setText_for_night_in_P7( record.value("P7_NIGHT_MW").toDouble());
      tablo->setText_for_night_in_P8( record.value("P8_NIGHT_MW").toDouble());

      tablo->setText_P4_For_Day_in_P5( record.value("P5_P4").toDouble());
      tablo->setText_P4_For_Day_in_P6( record.value("P6_P4").toDouble());
      tablo->setText_P4_For_Day_in_P7( record.value("P7_P4").toDouble());
      tablo->setText_P4_For_Day_in_P8( record.value("P8_P4").toDouble());
    }
}

// Задаем суммы за месяц без значений текущей записи
void OW_Prod_Phos::calc_manth_SUM( OW_Prod_Phos_Tablo * const tablo )
{
  double power_for_month_in_P5 = 0, 
    power_for_month_in_P6 = 0, 
    power_for_month_in_P7 = 0, 
    power_for_month_in_P8 = 0,

    p4_for_month_in_P5 = 0, 
    p4_for_month_in_P6 = 0, 
    p4_for_month_in_P7 = 0, 
    p4_for_month_in_P8 = 0;

  int row = view->currentIndex().row();
  
  for (int index = 0 ; index < row ; ++index )
    {
	  power_for_month_in_P5 += model->record( index ).value("P5_MW").toDouble();
	  power_for_month_in_P6 += model->record( index ).value("P6_MW").toDouble();
	  power_for_month_in_P7 += model->record( index ).value("P7_MW").toDouble();
	  power_for_month_in_P8 += model->record( index ).value("P8_MW").toDouble();
      
	  p4_for_month_in_P5 += model->record( index ).value("P5_P4").toDouble();
	  p4_for_month_in_P6 += model->record( index ).value("P6_P4").toDouble();
	  p4_for_month_in_P7 += model->record( index ).value("P7_P4").toDouble();
	  p4_for_month_in_P8 += model->record( index ).value("P8_P4").toDouble();
    }
  
  tablo->setPower_For_Month_in_P5( power_for_month_in_P5 );
  tablo->setPower_For_Month_in_P6( power_for_month_in_P6 );
  tablo->setPower_For_Month_in_P7( power_for_month_in_P7 );
  tablo->setPower_For_Month_in_P8( power_for_month_in_P8 );

  tablo->setP4_For_Month_in_P5( p4_for_month_in_P5 );
  tablo->setP4_For_Month_in_P6( p4_for_month_in_P6 );
  tablo->setP4_For_Month_in_P7( p4_for_month_in_P7 );
  tablo->setP4_For_Month_in_P8( p4_for_month_in_P8 );
}

// Добавить или удалить запись по ПЕРЕКАЧКЕ --------
void OW_Prod_Phos::addEditPumping( void )
{
  int row = view->currentIndex().row();
 
  if( row == -1 ) // Если не выбрана запись
    {
      QMessageBox::information( this, tr("Внимание"),
				tr("Нет выбранной записи.") );
      return;
    }

  QSqlRecord record = model->record( row );
  
  QString selectDate = record.value("PDATE").toString();

  OW_Prod_Phos_Pump_Dialog addEditPumpingDialog;
  addEditPumpingDialog.setCapText(tr("Доб./Ред. записи по перекачке на: [") + selectDate + tr("]") );
  addEditPumpingDialog.setCapPixmap( radiantPath + "/add_32.png" );

  addEditPumpingDialog.setValuePhos( record.value("P5_PUMP_P4").toDouble() , 0);
  addEditPumpingDialog.setValuePhos( record.value("P6_PUMP_P4").toDouble() , 1);
  addEditPumpingDialog.setValuePhos( record.value("P7_PUMP_P4").toDouble() , 2);
  addEditPumpingDialog.setValuePhos( record.value("P8_PUMP_P4").toDouble() , 3);

  addEditPumpingDialog.setValueSludge( record.value("P5_PUMP_SLUDGE").toDouble() , 0);
  addEditPumpingDialog.setValueSludge( record.value("P6_PUMP_SLUDGE").toDouble() , 1);
  addEditPumpingDialog.setValueSludge( record.value("P7_PUMP_SLUDGE").toDouble() , 2);
  addEditPumpingDialog.setValueSludge( record.value("P8_PUMP_SLUDGE").toDouble() , 3);

  if (!addEditPumpingDialog.exec()) return;
  
  QSqlQuery query( db );

  if (!query.exec( "UPDATE FOSFOR SET "
 		   "P5_PUMP_P4 = '" + QString::number( addEditPumpingDialog.valuePhos( 0 )) + "',"
 		   "P6_PUMP_P4 = '" + QString::number( addEditPumpingDialog.valuePhos( 1 )) + "',"
 		   "P7_PUMP_P4 = '" + QString::number( addEditPumpingDialog.valuePhos( 2 )) + "',"
 		   "P8_PUMP_P4 = '" + QString::number( addEditPumpingDialog.valuePhos( 3 )) + "',"
 		   "P5_PUMP_SLUDGE = '" + QString::number( addEditPumpingDialog.valueSludge( 0 )) + "',"
 		   "P6_PUMP_SLUDGE = '" + QString::number( addEditPumpingDialog.valueSludge( 1 )) + "',"
 		   "P7_PUMP_SLUDGE = '" + QString::number( addEditPumpingDialog.valueSludge( 2 )) + "',"
 		   "P8_PUMP_SLUDGE = '" + QString::number( addEditPumpingDialog.valueSludge( 3 )) + "'"
		   " WHERE PDATE = '" + selectDate + "'" ))
    QMessageBox::warning( this, tr("Ошибка"),   
			  tr("SQL запрос не прошел!")); 
 
  model->setQuery( selectSQL , db ); // Выполнить запрос

  view->selectRow ( row );
  return;
}
