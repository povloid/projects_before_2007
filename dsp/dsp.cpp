/****************************************************************************
**
*****************************************************************************/

#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QCloseEvent>
#include <QDate>
#include <QDockWidget>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlDatabase>
#include <QStackedLayout>
#include <QToolBar>
#include <QToolBox>

#include "dsp.h"

#include "pdatepicker.h"

#include "ow_prod_phos.h"
#include "ow_producing.h"
#include "ow_outcome.h"
#include "ow_income.h"
#include "ow_depot.h"

#include "r_infoforday.h"
#include "r_allproductionbalance.h"

#include "s_bal_dbtable.h"
#include "s_ed_mass_dbtable.h"
#include "s_vyr_dbtable.h"
#include "s_prod_dbtable.h"
#include "s_syr_dbtable.h"
#include "s_pprost_dbtable.h"
#include "s_dep_dbtable.h"
#include "s_ob_dbtable.h"
#include "s_post_dbtable.h"
#include "s_potr_dbtable.h"


#include "dspconst.h"

// Ширина кнопок в навигаторе
const int NButtonWidth = 200;
// Моя надпись для навигатора
NavigatorLabel::NavigatorLabel( const QString & text, QWidget *parent )
  : QLabel( text , parent ) 
{
  QFont font;
  font.setBold(true);
  setFont(font);
  
  setMinimumSize(QSize( NButtonWidth , 0));
}
// Моя кнопка для навигатора 
NavigatorButton::NavigatorButton( const QString & text, const QIcon & icon , QWidget *parent )
    : QToolButton( parent )
{
  setText( text );
  setIcon( icon );
  setFocusPolicy( Qt::NoFocus );
  setCheckable( true );
  setAutoRaise( true );
  setToolButtonStyle ( Qt::ToolButtonTextBesideIcon );
  setMinimumSize(QSize( NButtonWidth , 0));
}


DSPWindow::DSPWindow(QWidget *parent)
  : QMainWindow(parent)
{
  owDate = new QDate;
  *owDate = QDate::currentDate();

  // Меню - Файл ---------------------------------------------------------------
  QToolBar *tb = new QToolBar(this);
  tb->setWindowTitle(tr("File Actions"));
  addToolBar(tb);

  QMenu *menu = new QMenu(tr("&File"), this);
  menuBar()->addMenu(menu);

  QAction *a;
    
  a = new QAction(QIcon(rsrcPath + "/filenew.png"), tr("&New"), this);
  a->setShortcut(Qt::CTRL + Qt::Key_N);
  //connect(a, SIGNAL(triggered()), this, SLOT(fileNew()));
  tb->addAction(a);
  menu->addAction(a);

  a = new QAction(QIcon(rsrcPath + "/fileopen.png"), tr("&Open..."), this);
  a->setShortcut(Qt::CTRL + Qt::Key_O);
  //connect(a, SIGNAL(triggered()), this, SLOT(fileOpen()));
  tb->addAction(a);
  menu->addAction(a);

  menu->addSeparator();

  actionSave = a = new QAction(QIcon(rsrcPath + "/filesave.png"), tr("&Save"), this);
  a->setShortcut(Qt::CTRL + Qt::Key_S);
  //connect(a, SIGNAL(triggered()), this, SLOT(fileSave()));
  a->setEnabled(false);
  tb->addAction(a);
  menu->addAction(a);

  a = new QAction(tr("Save &As..."), this);
  //connect(a, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
  menu->addAction(a);
  menu->addSeparator();

  a = new QAction(QIcon(rsrcPath + "/fileprint.png"), tr("&Print..."), this);
  a->setShortcut(Qt::CTRL + Qt::Key_P);
  //connect(a, SIGNAL(triggered()), this, SLOT(filePrint()));
  tb->addAction(a);
  menu->addAction(a);

  a = new QAction(QIcon(rsrcPath + "/exportpdf.png"), tr("&Export PDF..."), this);
  a->setShortcut(Qt::CTRL + Qt::Key_D);
  //connect(a, SIGNAL(triggered()), this, SLOT(filePrintPdf()));
  tb->addAction(a);
  menu->addAction(a);

  menu->addSeparator();

  a = new QAction(tr("&Quit"), this);
  a->setShortcut(Qt::CTRL + Qt::Key_Q);
  connect(a, SIGNAL(triggered()), this, SLOT(close()));
  menu->addAction(a);
  // Меню - Файл ..............................................................


  // Навигатор ----------------------------------------------------------------
  //QPalette palette( Qt::gray );
  //palette.setColor( QPalette::Button , QColor(Qt::white));

  navigator = new QDockWidget(tr("Навигатор"), this);
  navigator->setFeatures( navigator->features() ^ QDockWidget::DockWidgetClosable );
  navigator->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  navigator->setMaximumSize(QSize( NButtonWidth + 40 , 16777215)); // <- Задаем фиксированный размер
  navigator->setMinimumSize(QSize( NButtonWidth + 40 , 0)); // <----------|

  addDockWidget(Qt::LeftDockWidgetArea, navigator );
    
  navigatorBox = new QToolBox( navigator );
  navigatorBox->setFrameShape( QFrame::Panel );
  navigatorBox->setFrameShadow( QFrame::Plain );
  navigatorBox->setLineWidth(1);
  //navigatorBox->setPalette( palette );
  navigator->setWidget( navigatorBox );

  navigatorButtonGroup = new QButtonGroup;
  navigatorButtonGroup->setExclusive ( true );
  connect( navigatorButtonGroup , SIGNAL(buttonClicked ( int )), 
	   this, SLOT(selectPageInMainLayout( int )));

  NavigatorButton *navigatorButton;
  NavigatorLabel *navigatorLabel;

  // Оперативная работа ---
  QWidget *pageOpWork = new QWidget();

  QVBoxLayout *pageOpWorkVboxLayout = new QVBoxLayout( pageOpWork );
  pageOpWorkVboxLayout->setSpacing(6);
  pageOpWorkVboxLayout->setMargin(9);

  navigatorBox->addItem( pageOpWork , tr( "Оперативная работа" ));
  navigatorBox->setItemIcon ( 0 , QIcon( piconPath + "/32/ow.png") );
  // ее кнопки 

  navigatorLabel = new NavigatorLabel( tr( "Производство:"));
  pageOpWorkVboxLayout->addWidget( navigatorLabel );

  navigatorButton = new NavigatorButton( tr( "Производство фосфора") , 
				     QIcon( piconPath + "/32/Factor.png") );
  pageOpWorkVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 21 );

  navigatorButton = new NavigatorButton( tr( "Выработка") ,
				     QIcon( piconPath + "/32/vyr.png") );
  pageOpWorkVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 22 );

  // Разделитель >-------<
  QFrame *line = new QFrame;	// --------------------------
  line->setFrameShape(QFrame::HLine);
  pageOpWorkVboxLayout->addWidget( line );

  navigatorLabel = new NavigatorLabel( tr( "Сырье:"));
  pageOpWorkVboxLayout->addWidget( navigatorLabel );

  navigatorButton = new NavigatorButton( tr( "Приход сырья") ,
				     QIcon( piconPath + "/22/add.png") );
  pageOpWorkVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 23 );

  navigatorButton = new NavigatorButton( tr( "Остатки сырья") ,
				     QIcon( piconPath + "/32/ost.png") );
  pageOpWorkVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 24 );

  // Разделитель >-------<
  line = new QFrame;	// --------------------------
  line->setFrameShape(QFrame::HLine);
  pageOpWorkVboxLayout->addWidget( line );

  navigatorLabel = new NavigatorLabel( tr( "Продукция:"));
  pageOpWorkVboxLayout->addWidget( navigatorLabel );

  navigatorButton = new NavigatorButton( tr( "Приход со стороны") ,
				     QIcon( piconPath + "/22/add.png") );
  pageOpWorkVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 25 );

  navigatorButton = new NavigatorButton( tr( "Отгрузка продукции") ,
				     QIcon( piconPath + "/22/remove.png") );
  pageOpWorkVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 26 );

  navigatorButton = new NavigatorButton( tr( "Остатки продукции") ,
				     QIcon( piconPath + "/32/ost.png") );
  pageOpWorkVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 27 );

  // Разделитель >-------<
  line = new QFrame;	// --------------------------
  line->setFrameShape(QFrame::HLine);
  pageOpWorkVboxLayout->addWidget( line );

  navigatorButton = new NavigatorButton( tr( "Простои") ,
				     QIcon( piconPath + "/32/potP4.png") );
  pageOpWorkVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 26 );


  QSpacerItem *spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, 
					    QSizePolicy::Expanding);

  pageOpWorkVboxLayout->addItem( spacerItem );
  // Оперативная работа ...

  // Отчеты ---
  QWidget *pageRaports = new QWidget();

  QVBoxLayout *pageRaportsVboxLayout = new QVBoxLayout( pageRaports );
  pageRaportsVboxLayout->setSpacing(6);
  pageRaportsVboxLayout->setMargin(9);
   
  navigatorBox->addItem( pageRaports , tr( "Отчеты" ));
  navigatorBox->setItemIcon ( 1 , QIcon( piconPath + "/16/fileopen.png") );
   
   
  navigatorButton = new NavigatorButton( tr( "Информация о работе") ,
 					 QIcon( radiantPath + "/docs_16.png") );
  pageRaportsVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 41 );

  // Разделитель >-------<
  line = new QFrame;	// --------------------------
  line->setFrameShape(QFrame::HLine);
  pageRaportsVboxLayout->addWidget( line );

  navigatorLabel = new NavigatorLabel( tr( "Баланс общий:"));
  pageRaportsVboxLayout->addWidget( navigatorLabel );

  navigatorButton = new NavigatorButton( tr( "Нарастающий") ,
 					 QIcon( radiantPath + "/docs_16.png") );
  pageRaportsVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 42 );

  navigatorButton = new NavigatorButton( tr( "Текущий") ,
 					 QIcon( radiantPath + "/docs_16.png") );
  pageRaportsVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 43 );
   
  spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, 
 			       QSizePolicy::Expanding);
  
  pageRaportsVboxLayout->addItem( spacerItem );


  spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, 
 			       QSizePolicy::Expanding);
  pageRaportsVboxLayout->addItem( spacerItem );
  // Отчеты ..


  // Справочники ---
  QWidget *pageHandbooks = new QWidget();

  QVBoxLayout *pageHandbooksVboxLayout = new QVBoxLayout( pageHandbooks );
  pageHandbooksVboxLayout->setSpacing(6);
  pageHandbooksVboxLayout->setMargin(9);

  navigatorBox->addItem( pageHandbooks , tr( "Справочники" ));
  navigatorBox->setItemIcon ( 2 , QIcon( piconPath + "/48/s.png") );

  navigatorButton = new NavigatorButton( tr( "Баланс фосфора") ,
				     QIcon( piconPath + "/22/s_s22.png") );
  pageHandbooksVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 51 );

  navigatorButton = new NavigatorButton( tr( "Выработка") ,
				     QIcon( piconPath + "/22/s_s22.png") );
  pageHandbooksVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 52 );

  navigatorButton = new NavigatorButton( tr( "Продукция") ,
				     QIcon( piconPath + "/22/s_s22.png") );
  pageHandbooksVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 53 );

  navigatorButton = new NavigatorButton( tr( "Сырье") ,
				     QIcon( piconPath + "/22/s_s22.png") );
  pageHandbooksVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 54 );

  navigatorButton = new NavigatorButton( tr( "Причины простоев") ,
				     QIcon( piconPath + "/22/s_s22.png") );
  pageHandbooksVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 55 );

  navigatorButton = new NavigatorButton( tr( "Оборудование") ,
				     QIcon( piconPath + "/22/s_s22.png") );
  pageHandbooksVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 56 );

  navigatorButton = new NavigatorButton( tr( "Поставщики") ,
				     QIcon( piconPath + "/22/s_s22.png") );
  pageHandbooksVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 57 );

  navigatorButton = new NavigatorButton( tr( "Потребители") ,
				     QIcon( piconPath + "/22/s_s22.png") );
  pageHandbooksVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 58 );

  // Разделитель >-------<
  line = new QFrame;		// --------------------------
  line->setFrameShape(QFrame::HLine);
  pageHandbooksVboxLayout->addWidget( line );
     
  navigatorButton = new NavigatorButton( tr( "Единицы измерения массы") ,
				     QIcon( piconPath + "/22/s_s22.png") );
  pageHandbooksVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 59 );

  navigatorButton = new NavigatorButton( tr( "Цеха") ,
				     QIcon( piconPath + "/22/s_s22.png") );
  pageHandbooksVboxLayout->addWidget( navigatorButton );
  navigatorButtonGroup->addButton ( navigatorButton , 60 );

  spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, 
			       QSizePolicy::Expanding);

  pageHandbooksVboxLayout->addItem( spacerItem );

  // Справочники ..
  // Навигатор ................................................................

  // Док календарь - все остальное для работы с датами ----
  
  QDockWidget * dateTimeBrowser = new QDockWidget(tr("Дата"), this);
  dateTimeBrowser->setFeatures( dateTimeBrowser->features() ^ QDockWidget::DockWidgetClosable);
  dateTimeBrowser->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  dateTimeBrowser->setMaximumSize(QSize( NButtonWidth + 42 , 210 )); // <- Задаем фиксированный размер
  dateTimeBrowser->setMinimumSize(QSize( NButtonWidth + 42 , 0)); // <----------|

  addDockWidget(Qt::LeftDockWidgetArea , dateTimeBrowser );
  
  QWidget *pDatePickerWidget = new QWidget;
  QVBoxLayout *pDatePickerWidgetLayout = new QVBoxLayout; 
  pDatePickerWidgetLayout->setMargin(1);
  pDatePickerWidgetLayout->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  pDatePickerWidget->setLayout( pDatePickerWidgetLayout );

  PDatePicker *pDatePicker = new PDatePicker( 19 , 28 );
  pDatePicker->setFrameStyle( QFrame::Box | QFrame::Plain );
  //pDatePicker->setFocusPolicy( Qt::NoFocus );
  pDatePickerWidgetLayout->addWidget( pDatePicker );

  dateTimeBrowser->setWidget( pDatePickerWidget );

  
  // ..

  // *************************************************************************
  // Рабочая область ---------------------------------------------------------
  mainWidget = new QWidget;
  mainWidget->setMinimumSize(QSize(400, 0)); // >|----------|<
  setCentralWidget ( mainWidget );
  mainLayout = new QStackedLayout( mainWidget );

  // Оперативная работа ........................
  // Производство фосфора    
  ow_prod_phos = new OW_Prod_Phos;
  mainLayout->addWidget( ow_prod_phos );
  // .. 
  // Выработка
  ow_producing = new OW_Producing;
  mainLayout->addWidget( ow_producing );
  // .. 
  // Приход со стороны
  ow_income = new OW_Income;
  mainLayout->addWidget( ow_income );
  // .. 
  // Отгрузка
  ow_outcome = new OW_Outcome;
  mainLayout->addWidget( ow_outcome );
  // .. 
  // Отгрузка
  ow_depot = new OW_Depot;
  mainLayout->addWidget( ow_depot );
  // .. 
  //Отчеты -------------------------------------
  // Общая информация по работе предприятия
  r_infoforday = new R_InfoForDay;
  mainLayout->addWidget( r_infoforday );
  // ..
  // Баланс общий
  r_allproductionbalance = new R_AllProductionBalance;
  mainLayout->addWidget( r_allproductionbalance );
  // ..
  // Справочники -------------------------------
  // Справочник Баланс фосфора --
  s_bal_dbtable = new S_BAL_DBTable;
  mainLayout->addWidget( s_bal_dbtable );
  // ..
  // Выработка --
  s_vyr_dbtable = new S_VYR_DBTable;
  mainLayout->addWidget( s_vyr_dbtable );
  // ..
  // Продукция --
  s_prod_dbtable = new S_PROD_DBTable;
  mainLayout->addWidget( s_prod_dbtable );
  // ..
  // Сырье --
  s_syr_dbtable = new S_SYR_DBTable;
  mainLayout->addWidget( s_syr_dbtable );
  // ..
  // Причины простоев --
  s_pprost_dbtable = new S_PPROST_DBTable;
  mainLayout->addWidget( s_pprost_dbtable );
  // ..
  // Оборудование --
  s_ob_dbtable = new S_OB_DBTable;
  mainLayout->addWidget( s_ob_dbtable );
  // ..
  // Поставщики --
  s_post_dbtable = new S_POST_DBTable;
  mainLayout->addWidget( s_post_dbtable );
  // ..
  // Поставщики --
  s_potr_dbtable = new S_POTR_DBTable;
  mainLayout->addWidget( s_potr_dbtable );
  // ..
  // Цеха --
  s_dep_dbtable = new S_DEP_DBTable;
  mainLayout->addWidget( s_dep_dbtable );
  // ..
  // Справочник единиц измерения --
  s_ed_mass_dbtable = new S_ED_MASS_DBTable;
  mainLayout->addWidget( s_ed_mass_dbtable );
  // ..
  // Справочники ...............................
 
  // Рабочая область .........................................................
  resize( 800, 576);

  // Сигналы и слоты для функций этого класса *--> >>--|
  connect( pDatePicker , SIGNAL( dateChanged( const QDate & )), 
	   this , SLOT( setOWDate( const QDate & )));
}

// Установить дату 
void DSPWindow::setOWDate( const QDate & date )
{
  // Устанавливаем оперативную дату и обновляем текущий рабочий виджет
  *owDate = date;
  
  QWidget* currentWidget = mainLayout->currentWidget();

  // Оперативная работа
  if ( currentWidget == ow_prod_phos )
    {
      ow_prod_phos->setOWDate( *owDate );
      return;
    }
  if ( currentWidget == ow_producing )
    {
      ow_producing->setOWDate( date );
      return;
    }
  if ( currentWidget == ow_income )
    {
      ow_income->setOWDate( date );
      return;
    }
  if ( currentWidget == ow_outcome )
    {
      ow_outcome->setOWDate( date );
      return;
    }
  if ( currentWidget == ow_depot )
    {
      ow_depot->setOWDate( date );
      return;
    }
  
  // Отчеты
  if ( currentWidget == r_infoforday )
    {
      r_infoforday->setOWDate( date );
      return;
    }
  if ( currentWidget == r_allproductionbalance )
    {
      r_allproductionbalance->setOWDate( date );
      return;
    }

}

// Выбор нужной страницы
void DSPWindow::selectPageInMainLayout( int id )
{
  switch ( id ) {
  case 21:
    {
      ow_prod_phos->setOWDate( *owDate );
      mainLayout->setCurrentWidget ( ow_prod_phos )  ;
      return;
    }
  case 22:
    {
      ow_producing->setOWDate( *owDate );
      mainLayout->setCurrentWidget ( ow_producing )  ;
      return;
    }
  case 25:
    {
      ow_income->setOWDate( *owDate );
      mainLayout->setCurrentWidget ( ow_income )  ;
      return;
    }
  case 26:
    {
      ow_outcome->setOWDate( *owDate );
      mainLayout->setCurrentWidget ( ow_outcome )  ;
      return;
    }
  case 27:
    {
      ow_depot->setOWDate( *owDate );
      mainLayout->setCurrentWidget ( ow_depot )  ;
      return;
    }
    //-------------------------------------------------
  case 41:
    {
      r_infoforday->setOWDate( *owDate );
      mainLayout->setCurrentWidget ( r_infoforday );
      return;
    }
  case 42:
    {
      r_allproductionbalance->setOWDate( *owDate );
      mainLayout->setCurrentWidget ( r_allproductionbalance );
      return;
    }
    //-------------------------------------------------
  case 51:
    {
      mainLayout->setCurrentWidget ( s_bal_dbtable )  ;
      return;
    }
  case 52:
    {
      mainLayout->setCurrentWidget ( s_vyr_dbtable )  ;
      return;
    }
  case 53:
    {
      mainLayout->setCurrentWidget ( s_prod_dbtable )  ;
      return;
    }
  case 54:
    {
      mainLayout->setCurrentWidget ( s_syr_dbtable )  ;
      return;
    }
  case 55:
    {
      mainLayout->setCurrentWidget ( s_pprost_dbtable )  ;
      return;
    }
  case 56:
    {
      mainLayout->setCurrentWidget ( s_ob_dbtable )  ;
      return;
    }
  case 57:
    {
      mainLayout->setCurrentWidget ( s_post_dbtable )  ;
      return;
    }
  case 58:
    {
      mainLayout->setCurrentWidget ( s_potr_dbtable )  ;
      return;
    }
  case 59:
    {
      mainLayout->setCurrentWidget ( s_ed_mass_dbtable )  ;
      return;
    }
  case 60:
    {
      mainLayout->setCurrentWidget ( s_dep_dbtable )  ;
      return;
    }
  default:
    return;
  }
}

// Перехватываем закрытие окна -------------
void DSPWindow::closeEvent( QCloseEvent *e )
{
  //if (maybeSave())
  // Подключение к базе данных ---------------------------------
  QSqlDatabase db = QSqlDatabase::database("DSP"); 

  db.commit();		// Подтверждаем транзакцию
  
  e->accept();
  //else
  //    e->ignore();
}
// ***************************************************************************************
// Оперативная работа
// ***************************************************************************************
// Соглашения о именах
// -----------------------------------------------------
// production_phosphorus	- Производство фосфора
// producing			- Выработка
// income_staple		- Приход сырья
// recrement_staple		- Остатки сырья
// shipment			- Отгрузка
// recrement_production		- Остатки продукции 
// outage			- Простои 
// -----------------------------------------------------
