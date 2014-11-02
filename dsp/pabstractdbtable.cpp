/*
    pabstractdbtable.cpp
*/
#include <QHeaderView>

#include "pabstractdbtable.h"

#include "dspconst.h"

PAbstractDBTable::PAbstractDBTable( QWidget * parent )
    : QWidget(parent)
{
  locale = new QLocale;

  QPalette palette( Qt::blue );
  palette.setColor( QPalette::Background , QColor(Qt::white));
  //palette.setColor( QPalette::Active , QPalette::Background , QColor(Qt::white));

  captionViewLabel = new QLabel;
  captionViewLabel->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  captionViewLabel->setFrameStyle( QFrame::Box | QFrame::Raised );
  captionViewLabel->setAutoFillBackground(true); // >>+<<
  captionViewLabel->setScaledContents(false); 
  captionViewLabel->setPalette( palette );
  captionViewLabel->setMargin(2);

  QFont font;
  font.setBold(true);
  captionViewLabel->setFont(font);

  // Подключение к базе данных ---------------------------------
  db = QSqlDatabase::database("DSP"); 
  db.transaction();		// Начинаем транзакцию
  
  // Создаем SQL модель
  model = new QSqlQueryModel;

  // Создаем таблицу
  view = new PTableView;
  view->setModel(model);
  view->setSelectionBehavior ( QAbstractItemView::SelectRows );
  view->setSelectionMode ( QAbstractItemView::SingleSelection ); 
  view->setAlternatingRowColors ( true ); // отображать с чередующимися цветами строк
  view->setTabKeyNavigation ( false ); // отключить навигацию по кнопке TAB
  ((QHeaderView *)view->verticalHeader())->setDefaultSectionSize( 20 );
  ((QHeaderView *)view->verticalHeader())->hide();
  view->horizontalHeader()->setClickable(false);
  //((QHeaderView *)view->horizontalHeader())->setMaximumHeight( 20 );

  p_ru_ItemDelegate = new P_RU_ItemDelegate;
  view->setItemDelegate( p_ru_ItemDelegate );

  // Создаем кнопки [добавить зап.], [Редактировать зап],[Удалить зап]
  // Создаем кнопки

  bttnAddRec = new QPushButton(tr("F2 - Доб. зап.")); 
  bttnAddRec->setIconSize ( QSize( 24, 24 ) );
  bttnAddRec->setIcon( QIcon( radiantPath + "/add_24.png") );
  bttnAddRec->setMaximumHeight ( buttonsMaximumHeight );

  bttnEditRec = new QPushButton(tr("F4 - Ред. зап."));
  bttnEditRec->setIconSize ( QSize( 24, 24 ) );
  bttnEditRec->setIcon( QIcon( radiantPath + "/edit_24.png") ); 
  bttnEditRec->setMaximumHeight ( buttonsMaximumHeight );

  bttnDelRec = new QPushButton(tr("F8 - Уд.  зап."));
  bttnDelRec->setIconSize ( QSize( 24, 24 ) );
  bttnDelRec->setIcon( QIcon( radiantPath + "/remov_24.png") ); 
  bttnDelRec->setMaximumHeight ( buttonsMaximumHeight );


  // Сигналы и слоты *--> >----|
  connect( bttnAddRec, SIGNAL(clicked()), this, SLOT( addRec()));
  connect( bttnEditRec, SIGNAL(clicked()), this, SLOT( editRec())); 
  connect( bttnDelRec, SIGNAL(clicked()), this, SLOT( delRec())); 
  //............................

  // Создаем диалог для работы с записями
  precdialog = new PRecDialog;
   
  //setupLayouts();

  // Сигналы и слоты *-> >>---| -------------------------------------------------------------
  connect( view , SIGNAL( addRec()), this, SLOT( addRec()));
  connect( view , SIGNAL( editRec()), this, SLOT( editRec()));
  connect( view , SIGNAL( delRec()), this, SLOT( delRec()));

  //.........................................................................................
}

void PAbstractDBTable::setupLayouts( void )
{
  // Расмещаем все в соответствующих менеджерах размещения
  QVBoxLayout * tabLayout = new QVBoxLayout;
  tabLayout->addWidget( captionViewLabel );
  tabLayout->addWidget( view );

  QVBoxLayout * buttonLayout = new QVBoxLayout;
  buttonLayout->addSpacing ( 30 );
  buttonLayout->addWidget( bttnAddRec );
  buttonLayout->addWidget( bttnEditRec );
  buttonLayout->addWidget( bttnDelRec );
  buttonLayout->addStretch();

  // Создаем главный менеджер размещения
  QHBoxLayout * mainLayout = new QHBoxLayout;
  setLayout( mainLayout ); // Назначаем его главным на форме

  mainLayout->addLayout( tabLayout );
  mainLayout->addLayout( buttonLayout );
}

// Добавить запись -----------------------
void PAbstractDBTable::addRec()
{
  precdialog->setWindowTitle(tr("Добавить новую запись"));
  precdialog->setCapText(tr("Добавить запись"));
  precdialog->setCapPixmap( radiantPath + "/add_32.png" );
  
  if (!virtualAddRec())
    QMessageBox::warning( this, tr ("Ошибка"), tr("SQL запрос не прошел!"));
  else
    goto_past_EditORNewRec();
}

// Редактировать запись ------------------
void PAbstractDBTable::editRec()
{
  precdialog->setWindowTitle(tr("Редактировать запись"));
  precdialog->setCapText(tr("Редактировать запись"));
  precdialog->setCapPixmap( radiantPath + "/edit_32.png" );

  int row = view->currentIndex().row();

  if( row != -1 ) // Если выбрана запись
    {
      QSqlRecord record = model->record( row );
      strKOD = record.value(0).toString();
	
      if (!virtualEditRec( row ))
	QMessageBox::warning( this, tr ("Ошибка"), tr("SQL запрос не прошел!"));
      else
	goto_past_EditORNewRec();
      return;
    } 
  else
    QMessageBox::information( this, tr("Внимание"),
			      tr("Нет выбранной записи.") );
}
// Удалить запись ------------------------
void PAbstractDBTable::delRec()
{
  int row = view->currentIndex().row();

  if( row != -1 ) // Если выбрана запись
    {
      if ( !virtualDelRec( row ) ) 
	QMessageBox::warning( this, tr("Ошибка"), tr("SQL запрос не прошел!"));
      else 
	{
	  model->setQuery( selectSQL , db );
	  view->setFocus();
	  
	  if ( row >= model->rowCount() )
	    --row;
	  
	    view->selectRow ( row );
	  
	}
      return;
    } QMessageBox::information( this, tr("Внимание"),
				tr("Нет выбранной записи.") );
}

//------------------------------------------------------------------------------------
//Функции, необходимые для быстрого перемещения по таблице к (записи) месту
//проведения последней операции
void PAbstractDBTable::goto_past_EditORNewRec()
{
  model->setQuery( selectSQL , db );
  view->setFocus();
  
  int index = 0;

  while( model->record( index ).value(0).toString() != strKOD 
	 && index++ < model->rowCount() )
    ;;

    view->selectRow ( index );
}
