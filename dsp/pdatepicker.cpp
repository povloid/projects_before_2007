/*
 *   pdatepicker.cpp
 *
 *   This file is part of the class - PDatePicker.
 *   PDatePicker is a class of the calendar widget.
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
 *  Authors:   Pavel Kopychenko pacmanoid <kpovlo@mail.ru>
 *  Date: 2006 year.
 */

#include <QDate>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QKeyEvent>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QStringList>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWhatsThis>

#include "pdatepicker.h"
#include "pmuffledobject.h"

const int cols = 7;
const int rows = 6;


// cellLabel ---------------------------------------------------------
PCellLabel::PCellLabel( QWidget *parent )
    : QLabel(parent)
{
  setAutoFillBackground( true ); // >>+<<
  setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  setFrameShadow( QFrame::Raised );
  setMargin(2);

  setBackgroundRole( QPalette::Window ) ;
  setFocusPolicy ( Qt::NoFocus );

  pCellDate = new QDate;
  pCellPalette = new QPalette;
}

// Установить дату
void PCellLabel::setDate( const QDate & dateTemp , const QDate & calDate )
{
  *pCellDate = dateTemp;
  // Устраняет глюк отображения (остающиеся иногда рпмки на ячейках)
  setFrameStyle( QFrame::Plain | QFrame::NoFrame );
  
  setText( QString::number( dateTemp.day()) );

  // В соответствии с днем каждая ячейка знает свой цвет и оформление
  // Палитра цветов
  QPalette palette( Qt::red );
  palette.setColor( QPalette::Background , Qt::white );

  // Фонт
  QFont font;
      
  if ( dateTemp.month() != calDate.month() ) // Если другой месяц 
    palette.setColor( QPalette::Foreground , QColor( 210 , 210 , 210 ) );
  else
    if ( dateTemp == QDate::currentDate() ) // Если текущая дата
      {
	palette.setColor( QPalette::Background , Qt::magenta );
	palette.setColor( QPalette::Foreground , Qt::white );
	font.setBold(true);
      }
    else
      // Красим будние и выходные дни в разные цвета 
      if ( dateTemp.dayOfWeek() > 5 )
	palette.setColor( QPalette::Foreground , Qt::red );      
  
  // Установить палитру
  setPalette( palette );
  setFont(font);

  *pCellPalette = palette;
  pFrameStyle = frameStyle();
}

// Получить установленную дату
QDate & PCellLabel::date( void ) const
{
  return *pCellDate;
}

void PCellLabel::setPFrameStyle( const int & frameStyle )
{
  pFrameStyle = frameStyle;
}

// Красить ячуйку под мышкой
bool PCellLabel::event( QEvent * event )
{
  switch( event->type() )
    {
    case QEvent::Leave:
      {
	setPalette( *pCellPalette );
	setFrameStyle( pFrameStyle );

	return QLabel::event( event );
      }
    case QEvent::Enter:
      {
	QPalette palette( Qt::red );
	palette.setColor( QPalette::Background , QColor( 202 , 202 , 255 ) );
	setPalette( palette );
	if( pFrameStyle != (int)(QFrame::Box | QFrame::Raised) )
	  setFrameStyle( QFrame::Plain | QFrame::Box );

	return QLabel::event( event );
      }
    default:
      return QLabel::event( event );
    }
}
// -------------------------------------------------------------------


// weekLabels ||--------------------------------------------------------
PWeekLabel::PWeekLabel( const int & vSize , const int & hSize , QWidget *parent )
    : QLabel(parent)
{
  // Стиль рамки вокруг виджета
  setAutoFillBackground(true); // >>+<<
  setFrameStyle( QFrame::Panel | QFrame::Sunken );
  setMaximumWidth( hSize );
  setMaximumHeight( vSize - 2 );
  setMinimumWidth( hSize );
  setMinimumHeight( vSize - 2 );
  setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  setMargin(2);
}

// capOnTable ||--------------------------------------------------------
PDateCapOnTable::PDateCapOnTable( const int & hSize , QWidget *parent )
    : QLabel(parent)
{
  // Стиль рамки вокруг виджета
  setAutoFillBackground(true); // >>+<<
  setFrameStyle( QFrame::Box | QFrame::Raised );
  setMaximumWidth( hSize - 2 );
  setMaximumHeight( 20 );
  setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  setMargin(1);

  QFont font;
  font.setBold(true);
  setFont(font);
}

// table class ||-------------------------------------------------------
PDateTableWidget::PDateTableWidget( const int & vSize , const int & hSize , QWidget *parent )
    : QTableWidget(parent)
{
  setFrameShape( QFrame::Box );
  setFrameShadow( QFrame::Plain );

  ((QHeaderView *)this->verticalHeader())->setDefaultSectionSize( vSize );
  ((QHeaderView *)this->horizontalHeader())->setDefaultSectionSize( hSize );

  // Геометрия
  setColumnCount( cols );
  setRowCount( rows );

  int widgetVSize = vSize * rows + 2;
  int widgetHSize = hSize * cols + 2;
  
  setMinimumSize(  widgetHSize , widgetVSize);
  setMaximumSize(  widgetHSize , widgetVSize);  

  ((QHeaderView *)this->verticalHeader())->hide();
  ((QHeaderView *)this->horizontalHeader())->hide();

  setAutoFillBackground(true); // >>+<<
  setShowGrid ( false );

}

void PDateTableWidget::keyPressEvent(QKeyEvent *event)
{
  switch ( event->key() ){

  case Qt::Key_Home:
    {
      emit gotoCurrentDate();
      break;
    }
  case Qt::Key_PageUp:
    {
      emit lastMonth();
      break;
    }
  case Qt::Key_PageDown:
    {
      emit nextMonth();
      break;
    }
  case Qt::Key_End:
    {
      emit selectYear();
      break;
    }
  case Qt::Key_Delete:
    {
      emit selectMonth();
      break;
    }
  default:
    QTableWidget::keyPressEvent(event);
  }
}
// -------------------------------------------------------------------


// main class ||--------------------------------------------------------
PDatePicker::PDatePicker( int vSize , int hSize , QWidget *parent)
    : QFrame(parent)
{
  // Стиль рамки вокруг виджета
  setAutoFillBackground(true); // >>+<<
  setFrameStyle( QFrame::StyledPanel | QFrame::Raised );

  // Создаем главный менеджер размещения
  QGridLayout *mainLayout = new QGridLayout;
  mainLayout->setMargin( 1 );
  mainLayout->setSpacing( 1 );
  mainLayout->setSizeConstraint( QLayout::SetFixedSize );

  setLayout( mainLayout );

  // Создаем шапку заголовка, отображающую дни недели ---
  PDateCapOnTable *capOnTable;
  QHBoxLayout * capLayout = new QHBoxLayout;  
  capLayout->setMargin(1);

  QPalette palette( Qt::blue );
  palette.setColor( QPalette::Background , QColor(Qt::white));

  calDate = new QDate;
  *calDate = QDate::currentDate();
  
  QStringList days_of_week;
  days_of_week<<tr("Пн")<<tr("Вт")<<tr("Ср")<<tr("Чт")<<tr("Пт")<<tr("Сб")<<tr("Вс");

  for ( int i = 0; i < 7; ++i )
    {
      capOnTable = new PDateCapOnTable( hSize );
      capOnTable->setText( days_of_week.at(i));

      if ( i > 4 )		// Красим выходные дни от быдлодней
	{
	  palette.setColor ( QPalette::Background , QColor( 255 , 216 , 217 ) );
	  palette.setColor ( QPalette::Foreground , Qt::black );
	}
      
      capOnTable->setPalette( palette );

      capLayout->addWidget( capOnTable );
    }
  mainLayout->addLayout( capLayout , 1, 1, 1, cols );
  //..

  // Создаем линейку, отображающую дни недели
  QVBoxLayout * weeksLayout = new QVBoxLayout;  
  weeksLayout->setMargin(1);

  for ( int i = 0; i < rows ; ++i)
    {
      weekLabels[i] = new PWeekLabel( vSize , hSize );
      weeksLayout->addWidget( weekLabels[i] );
    }

  mainLayout->addLayout( weeksLayout , 2, 0, rows, 1 );
  // ..

  // Создаем таблицу отображения текущего месяца
  table = new PDateTableWidget( vSize , hSize );
  mainLayout->addWidget ( table, 2, 1, rows, cols);

  // Заполняем ее элементами
  // cellLabels[42];
  
  int index = 0 ;
  for ( int i = 0 ; i < rows ; ++i )
    for ( int j = 0 ; j < cols ; ++j )
      {
	cellLabels[ index ] = new PCellLabel;
	cellLabels[ index ]->setText( QString::number( index ));
	table->setCellWidget ( i , j, cellLabels[ index ] );
	++index;
      }

  //Создаем Label отображающий сегодняшнюю дату
  QFont font;
  font.setBold(true);

  palette.setColor ( QPalette::Background , QColor( Qt::white ) );
  palette.setColor ( QPalette::Foreground , Qt::black );
  
  QLabel *currDateLabel = new QLabel( tr("Сегодня: ") + QDate::currentDate().toString("dd.MM.yyyy") );
  currDateLabel->setFrameStyle( QFrame::Box | QFrame::Plain );
  currDateLabel->setMaximumHeight( 20 );
  currDateLabel->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  currDateLabel->setMargin(1);
  currDateLabel->setFont(font);
  currDateLabel->setAutoFillBackground(true); // >>+<<
  currDateLabel->setPalette( palette );

  mainLayout->addWidget ( currDateLabel, 8, 2, 1 , 6);

  // Миниметка цвета текущей даты
  palette.setColor( QPalette::Background , Qt::magenta );
  palette.setColor( QPalette::Foreground , Qt::white );

  currDateLabel = new QLabel;

  currDateLabel->setFrameStyle( QFrame::Box | QFrame::Plain );
  currDateLabel->setMaximumWidth ( 14 );
  currDateLabel->setMaximumHeight( 14 );
  currDateLabel->setMinimumWidth ( 14 );
  currDateLabel->setMinimumHeight( 14 );
  currDateLabel->setLineWidth(2);
  currDateLabel->setAutoFillBackground(true); // >>+<<
  currDateLabel->setPalette( palette );
  mainLayout->addWidget( currDateLabel, 8, 1, 1 , 1);
  mainLayout->setAlignment( currDateLabel , Qt::AlignHCenter | Qt::AlignVCenter );

  // Кнопки управления ----[>],[>],[<], \_______________________
  // [>] - следующий месяц
  bttnNextMonth = new QToolButton;
  bttnNextMonth->setToolTip(tr("Следующий месяц"));
  bttnNextMonth->setFocusPolicy( Qt::NoFocus );
  bttnNextMonth->setAutoRaise ( true );
  bttnNextMonth->setMaximumSize ( QSize ( hSize , vSize ) );
  bttnNextMonth->setArrowType ( Qt::RightArrow );
  mainLayout->addWidget ( bttnNextMonth, 0, 7, 1, 1 );

  // [<] - предидущий месяц
  bttnLastMonth = new QToolButton;
  bttnLastMonth->setToolTip(tr("Предидущий месяц"));
  bttnLastMonth->setFocusPolicy( Qt::NoFocus );
  bttnLastMonth->setAutoRaise ( true );
  bttnLastMonth->setMaximumSize ( QSize ( hSize , vSize ) );
  bttnLastMonth->setArrowType ( Qt::LeftArrow );
  mainLayout->addWidget ( bttnLastMonth, 0, 0, 1, 1 );

  // [>] - Перейти на текущую дату
  bttnGotoCurrentDate = new QToolButton;
  bttnGotoCurrentDate->setToolTip(tr("Перейти на текущую дату"));
  bttnGotoCurrentDate->setFocusPolicy( Qt::NoFocus );
  bttnGotoCurrentDate->setAutoRaise ( true );
  bttnGotoCurrentDate->setMaximumSize ( QSize ( hSize , vSize ) );
  bttnGotoCurrentDate->setArrowType ( Qt::RightArrow );
  mainLayout->addWidget ( bttnGotoCurrentDate , 8, 0, 1, 1 );

  // [месяц]
  bttnMonth = new QPushButton;
  bttnMonth->setToolTip(tr("Месяц"));
  bttnMonth->setFocusPolicy( Qt::NoFocus );
  bttnMonth->setFlat(true);
  bttnMonth->setMaximumHeight( vSize );
  bttnMonth->setMaximumWidth ( hSize * 4 );
  bttnMonth->setMinimumHeight( vSize );
  bttnMonth->setMinimumWidth ( hSize * 4 );

  mainLayout->addWidget ( bttnMonth , 0, 1, 1, 4 );

  QMenu *menu = new QMenu(this);
  for (int month = 1; month <= 12; ++month)
    {
      QAction * action = new QAction( QDate::longMonthName(month) , this);
      action->setData( QVariant ( month ) );
      menu->addAction( action );
    }

  bttnMonth->setMenu( menu );

  // [год]
  bttnYear = new QPushButton;
  bttnYear->setToolTip(tr("Год"));
  bttnYear->setFocusPolicy( Qt::NoFocus );
  bttnYear->setFlat(true);
  bttnYear->setMaximumHeight( vSize );
  bttnYear->setMaximumWidth ( hSize * 2 );
  mainLayout->addWidget ( bttnYear , 0, 5, 1, 2 );

  spBoxYearFrame = new QFrame;
  spBoxYearFrame->setWindowFlags( Qt::Popup );
  spBoxYearFrame->setFrameStyle( QFrame::WinPanel | QFrame::Plain );

  QVBoxLayout *spBoxYearFrameLayout = new QVBoxLayout;
  spBoxYearFrameLayout->setMargin( 2 );
  spBoxYearFrame->setLayout( spBoxYearFrameLayout );

  spBoxYear = new QSpinBox;
  spBoxYear->setAcceptDrops( false );
  spBoxYear->setWrapping( false );
  spBoxYear->setGeometry( 5, 5, 60, 20 );
  spBoxYear->setButtonSymbols( QSpinBox::UpDownArrows );
  spBoxYear->setMaximum( 3000 );
  spBoxYear->setMinimum( 1800 );
  spBoxYearFrameLayout->addWidget( spBoxYear );

  // Создаем класс-заглушку для событий клавиатура и мышы
  pMuffledObject = new PMuffledObject( this );

  // [?] - предидущий месяц
  bttnHelp = new QToolButton;
  bttnHelp->setText( "?" );
  bttnHelp->setShortcut( Qt::Key_F1 );
  bttnHelp->setToolTip(tr("Справка по календарю..."));
  bttnHelp->setFocusPolicy( Qt::NoFocus );
  bttnHelp->setAutoRaise ( true );
  bttnHelp->setMaximumSize ( QSize ( hSize , vSize ) );
  mainLayout->addWidget ( bttnHelp, 1, 0 );


  // Сигналы и слоты *--> >----|
  connect( table , SIGNAL( currentCellChanged ( int , int , int , int ))
	   , this, SLOT( currentCellChanged ( int , int , int , int )));

  connect( table , SIGNAL( gotoCurrentDate() ),
	   this , SLOT( gotoCurrentDate() ) );

  connect( table , SIGNAL( lastMonth() ),
	   this , SLOT( lastMonth() ) );

  connect( table , SIGNAL( nextMonth() ),
	   this , SLOT( nextMonth() ) );

  connect( table , SIGNAL( selectYear() ),
	   bttnYear , SLOT( click () ) );

  connect( table , SIGNAL( selectMonth() ),
	   bttnMonth , SLOT( click () ) );

  connect( menu , SIGNAL( triggered ( QAction * ) ),
	   this , SLOT( triggeredMonth ( QAction * ) ) );

  connect( bttnLastMonth, SIGNAL(clicked()), this, SLOT( lastMonth()));
  connect( bttnNextMonth, SIGNAL(clicked()), this, SLOT( nextMonth())); 
  connect( bttnGotoCurrentDate, SIGNAL(clicked()), this, SLOT( gotoCurrentDate())); 
  connect( bttnYear, SIGNAL(clicked()), this, SLOT( showSpBoxYearFrame()));
  connect( spBoxYear, SIGNAL( editingFinished () ), this , SLOT( hideSpBoxYearFrame() ) );
  connect( bttnHelp, SIGNAL(clicked()), this, SLOT( showHelp()));

  setDate( *calDate );

}

// Функция слот >>----
void PDatePicker::currentCellChanged ( int currentRow, int currentColumn, 
				       int previousRow, int previousColumn )
{
  PCellLabel *pCellLabel;
  // Убираем цвет со старого
  if ( previousRow > -1 && previousColumn > -1 )
    {
      pCellLabel =( PCellLabel * )table->cellWidget ( previousRow, previousColumn ); 
      pCellLabel->setFrameShape ( QFrame::NoFrame );
      pCellLabel->setPFrameStyle( QFrame::NoFrame );
    }

  // Устанавливаем цвет на новом
  pCellLabel =( PCellLabel * )table->cellWidget ( currentRow, currentColumn ); 
  pCellLabel->setFrameStyle( QFrame::Box | QFrame::Raised );
  pCellLabel->setPFrameStyle( QFrame::Box | QFrame::Raised );

  if ( pCellLabel->date().month() != calDate->month() )
    setDate( pCellLabel->date() );
  else
    if ( pCellLabel->date() != *calDate)
      {
	emit dateChanged( pCellLabel->date() ); 
	// Установить внутреннюю текущую дату
	*calDate =  pCellLabel->date();
      }
}

// Установить дату --- 
void PDatePicker::setDate( const QDate & dateTemp )
{
  *calDate = dateTemp;
    
  //  Далее dTemp - используется как временная
  QDate dTemp( calDate->year(), calDate->month(), 1 );

  dTemp = dTemp.addDays( - dTemp.dayOfWeek() + 1 ); // Перейти на понедельник

  int index = 0 ;
  for ( int i = 0 ; i < rows ; ++i)
    {
      weekLabels[i]->setText( QString::number ( dTemp.weekNumber() )); // Выставляем
								       // номера
								       // недель
     
      for ( int j = 0 ; j < cols ; ++j )
	{ 
	  cellLabels[ index ]->setDate( dTemp , *calDate ); // Итак,
							    // каждая
							    // ячейка
							    // знает
							    // свою
							    // дату
	  
	  if ( dTemp == *calDate )
	    {
	      table->setCurrentCell( i , j );
	      emit dateChanged( *calDate ); 
	    }
	
	  dTemp = dTemp.addDays ( 1 );
	  ++index;
	}
    }
  
  // Установить название месяца выставленной даты
  bttnMonth->setText( QDate::longMonthName( calDate->month() ) );
  // Установить номер года выставленной даты
  bttnYear->setText( QString::number( calDate->year() ) );
  spBoxYear->setValue( calDate->year() );

}

// Получить установленную дату
QDate & PDatePicker::date( void ) const
{
  return *calDate;
}

// Перейти на прошлый месяц
void PDatePicker::lastMonth()
{
  setDate( calDate->addMonths ( -1 ) );
}

// Перейти на следующий месяц
void PDatePicker::nextMonth()
{
  setDate( calDate->addMonths ( 1 ) );
}

// Перейти на следующий год
void PDatePicker::gotoCurrentDate()
{
  setDate( QDate::currentDate() );
}

void PDatePicker::triggeredMonth ( QAction * action )
{
  int year = calDate->year();
  int month = action->data().toInt();
  int day = calDate->day();

  // Это надо например для корректного перехода с 31 августа на
  // ФЕВРАЛЬ месяц
  while( !QDate::isValid ( year , month , day ) )
    --day;

  setDate( QDate ( year , month , day ) );
}
// 458048 - Баха

void PDatePicker::showSpBoxYearFrame( void )
{
  spBoxYearFrame->move( mapToGlobal( bttnYear->geometry().bottomLeft() ) );
  spBoxYear->setFocus();
  spBoxYearFrame->show();
}

void PDatePicker::hideSpBoxYearFrame( void )
{
  spBoxYearFrame->hide();

  int year = spBoxYear->value();
  int month = calDate->month();
  int day = calDate->day();

  // Коррекция для высокосных годов, если установлен высокосный год и
  // 29 ФЕВРАЛЯ
  while( !QDate::isValid ( year , month , day ) )
    --day;
  
  setDate( QDate ( year , month , day ) );
}

// Режим включения только просмотра установленной даты, без
// возможности ее изменения а также режим его отключения
void PDatePicker::setViewOnly( bool viewOnly )
{
  if ( viewOnly )
    {
      bttnNextMonth->hide();
      bttnLastMonth->hide();
      bttnGotoCurrentDate->hide();

      bttnMonth->installEventFilter ( pMuffledObject );
      bttnYear->installEventFilter ( pMuffledObject );
      table->installEventFilter ( pMuffledObject );

      for ( int i = 0 ; i< 42 ; ++i )
	cellLabels[i]->installEventFilter ( pMuffledObject );
    }
  else
    {
      bttnNextMonth->show();
      bttnLastMonth->show();
      bttnGotoCurrentDate->show();

      bttnMonth->removeEventFilter ( pMuffledObject );
      bttnYear->removeEventFilter ( pMuffledObject );
      table->removeEventFilter ( pMuffledObject );

      for ( int i = 0 ; i< 42 ; ++i )
	cellLabels[i]->removeEventFilter ( pMuffledObject );
    }
}

void PDatePicker::showHelp( void ) // Справка и помощь 
{
  QWhatsThis::showText( mapToGlobal( bttnHelp->geometry().bottomLeft() ) , 
			tr("Календарь использует "
			   "следующие быстрые сочетания клавиш:"
			   "<ul>"
			   "<li><b>Up, Down, Left, Right</b> - перемещение "
			   "по дням."
			   "<li><b>PageUp</b> - на один месяц назад."
			   "<li><b>PageDown</b> - на один месяц вперед."
			   "<li><b>Home</b> - перейти на текущую дату."
			   "<li><b>Delete</b> - выбрать месяц из меню и переместиться на него."
			   "<li><b>Del</b> - ввести год и переместиться на него."
			   "</ul>"
			   "<li><b>PDATEPICKER - календарь.</b>"
			   "<li><b>Версия:</b> 0.01."
			   "<li><b>Автор:</b> Копыченко П.Г. ( aka pacmanoid )."
			   "<li><b>Лицензия:</b> - <b>GNU.</b>"
			   "<li><b>год:</b> 2006."
			   "</ul>") , 

			this );
}
