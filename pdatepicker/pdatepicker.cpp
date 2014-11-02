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
}

// ���������� ����
void PCellLabel::setDate( const QDate & dateTemp , const QDate & calDate )
{
  *pCellDate = dateTemp;
  setText( QString::number( dateTemp.day()) );

  // � ������������ � ���� ������ ������ ����� ���� ���� � ����������
  // ������� ������
  QPalette palette( Qt::red );
  palette.setColor( QPalette::Background , Qt::white );

  // ����
  QFont font;
      
  if ( dateTemp.month() != calDate.month() ) // ���� ������ ����� 
    palette.setColor( QPalette::Foreground , QColor( 210 , 210 , 210 ) );
  else 
    if ( dateTemp == QDate::currentDate() ) // ���� ������� ����
      {
	palette.setColor( QPalette::Background , Qt::magenta );
	palette.setColor( QPalette::Foreground , Qt::white );
	font.setBold(true);
      }
    else
      // ������ ������ � �������� ��� � ������ ����� 
      if ( dateTemp.dayOfWeek() > 5 )
	palette.setColor( QPalette::Foreground , Qt::red );

  // ���������� �������
  setPalette( palette );
  setFont(font);

}

// �������� ������������� ����
QDate & PCellLabel::date( void ) const
{
  return *pCellDate;
}
// -------------------------------------------------------------------


// weekLabels ||--------------------------------------------------------
PWeekLabel::PWeekLabel( const int & vSize , const int & hSize , QWidget *parent )
    : QLabel(parent)
{
  // ����� ����� ������ �������
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
  // ����� ����� ������ �������
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

  // ���������
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
  // ����� ����� ������ �������
  setAutoFillBackground(true); // >>+<<
  setFrameStyle( QFrame::StyledPanel | QFrame::Raised );

  // ������� ������� �������� ����������
  QGridLayout *mainLayout = new QGridLayout;
  mainLayout->setMargin( 1 );
  mainLayout->setSpacing( 1 );
  mainLayout->setSizeConstraint( QLayout::SetFixedSize );

  setLayout( mainLayout );

  // ������� ����� ���������, ������������ ��� ������ ---
  PDateCapOnTable *capOnTable;
  QHBoxLayout * capLayout = new QHBoxLayout;  
  capLayout->setMargin(1);

  QPalette palette( Qt::blue );
  palette.setColor( QPalette::Background , QColor(Qt::white));

  calDate = new QDate;
  *calDate = QDate::currentDate();
  
  QStringList days_of_week;
  days_of_week<<tr("��")<<tr("��")<<tr("��")<<tr("��")<<tr("��")<<tr("��")<<tr("��");

  for ( int i = 0; i < 7; ++i )
    {
      capOnTable = new PDateCapOnTable( hSize );
      capOnTable->setText( days_of_week.at(i));

      if ( i > 4 )		// ������ �������� ��� �� ���������
	{
	  palette.setColor ( QPalette::Background , QColor( 255 , 216 , 217 ) );
	  palette.setColor ( QPalette::Foreground , Qt::black );
	}
      
      capOnTable->setPalette( palette );

      capLayout->addWidget( capOnTable );
    }
  mainLayout->addLayout( capLayout , 1, 1, 1, cols );
  //..

  // ������� �������, ������������ ��� ������
  QVBoxLayout * weeksLayout = new QVBoxLayout;  
  weeksLayout->setMargin(1);

  for ( int i = 0; i < rows ; ++i)
    {
      weekLabels[i] = new PWeekLabel( vSize , hSize );
      weeksLayout->addWidget( weekLabels[i] );
    }

  mainLayout->addLayout( weeksLayout , 2, 0, rows, 1 );
  // ..

  // ������� ������� ����������� �������� ������
  table = new PDateTableWidget( vSize , hSize );
  mainLayout->addWidget ( table, 2, 1, rows, cols);

  // ��������� �� ����������
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

  //������� Label ������������ ����������� ����
  QFont font;
  font.setBold(true);

  palette.setColor ( QPalette::Background , QColor( Qt::white ) );
  palette.setColor ( QPalette::Foreground , Qt::black );
  
  QLabel *currDateLabel = new QLabel( tr("�������: ") + QDate::currentDate().toString("dd.MM.yyyy") );
  currDateLabel->setFrameStyle( QFrame::Box | QFrame::Plain );
  currDateLabel->setMaximumHeight( 20 );
  currDateLabel->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  currDateLabel->setMargin(1);
  currDateLabel->setFont(font);
  currDateLabel->setAutoFillBackground(true); // >>+<<
  currDateLabel->setPalette( palette );

  mainLayout->addWidget ( currDateLabel, 8, 2, 1 , 6);

  // ��������� ����� ������� ����
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

  // ������ ���������� ----[>],[>],[<], \_______________________
  // [>] - ��������� �����
  bttnNextMonth = new QToolButton;
  bttnNextMonth->setToolTip(tr("��������� �����"));
  bttnNextMonth->setFocusPolicy( Qt::NoFocus );
  bttnNextMonth->setAutoRaise ( true );
  bttnNextMonth->setMaximumSize ( QSize ( hSize , vSize ) );
  bttnNextMonth->setArrowType ( Qt::RightArrow );
  mainLayout->addWidget ( bttnNextMonth, 0, 7, 1, 1 );

  // [<] - ���������� �����
  bttnLastMonth = new QToolButton;
  bttnLastMonth->setToolTip(tr("���������� �����"));
  bttnLastMonth->setFocusPolicy( Qt::NoFocus );
  bttnLastMonth->setAutoRaise ( true );
  bttnLastMonth->setMaximumSize ( QSize ( hSize , vSize ) );
  bttnLastMonth->setArrowType ( Qt::LeftArrow );
  mainLayout->addWidget ( bttnLastMonth, 0, 0, 1, 1 );

  // [>] - ������� �� ������� ����
  bttnGotoCurrentDate = new QToolButton;
  bttnGotoCurrentDate->setToolTip(tr("������� �� ������� ����"));
  bttnGotoCurrentDate->setFocusPolicy( Qt::NoFocus );
  bttnGotoCurrentDate->setAutoRaise ( true );
  bttnGotoCurrentDate->setMaximumSize ( QSize ( hSize , vSize ) );
  bttnGotoCurrentDate->setArrowType ( Qt::RightArrow );
  mainLayout->addWidget ( bttnGotoCurrentDate , 8, 0, 1, 1 );

  // [�����]
  bttnMonth = new QPushButton;
  bttnMonth->setToolTip(tr("�����"));
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

  // [���]
  bttnYear = new QPushButton;
  bttnYear->setToolTip(tr("���"));
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

  // ������� �����-�������� ��� ������� ���������� � ����
  pMuffledObject = new PMuffledObject( this );

  // [?] - ���������� �����
  bttnHelp = new QToolButton;
  bttnHelp->setText( "?" );
  bttnHelp->setShortcut( Qt::Key_F1 );
  bttnHelp->setToolTip(tr("������� �� ���������..."));
  bttnHelp->setFocusPolicy( Qt::NoFocus );
  bttnHelp->setAutoRaise ( true );
  bttnHelp->setMaximumSize ( QSize ( hSize , vSize ) );
  mainLayout->addWidget ( bttnHelp, 1, 0 );


  // ������� � ����� *--> >----|
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

// ������� ���� >>----
void PDatePicker::currentCellChanged ( int currentRow, int currentColumn, 
				       int previousRow, int previousColumn )
{
  PCellLabel *pCellLabel;
  // ������� ���� �� �������
  if ( previousRow > -1 && previousColumn > -1 )
    {
      pCellLabel =( PCellLabel * )table->cellWidget ( previousRow, previousColumn ); 
      pCellLabel->setFrameShape ( QFrame::NoFrame );
    }

  // ������������� ���� �� �����
  pCellLabel =( PCellLabel * )table->cellWidget ( currentRow, currentColumn ); 
  pCellLabel->setFrameShape ( QFrame::Box );

  if ( pCellLabel->date().month() != calDate->month() )
    setDate( pCellLabel->date() );
  else
    if ( pCellLabel->date() != *calDate)
      {
	emit dateChanged( pCellLabel->date() ); 
	// ���������� ���������� ������� ����
	*calDate =  pCellLabel->date();
      }
}

// ���������� ���� --- 
void PDatePicker::setDate( const QDate & dateTemp )
{
  *calDate = dateTemp;
    
  //  ����� dTemp - ������������ ��� ���������
  QDate dTemp( calDate->year(), calDate->month(), 1 );

  dTemp = dTemp.addDays( - dTemp.dayOfWeek() + 1 ); // ������� �� �����������

  int index = 0 ;
  for ( int i = 0 ; i < rows ; ++i)
    {
      weekLabels[i]->setText( QString::number ( dTemp.weekNumber() )); // ����������
								       // ������
								       // ������
     
      for ( int j = 0 ; j < cols ; ++j )
	{ 
	  cellLabels[ index ]->setDate( dTemp , *calDate ); // ����,
							    // ������
							    // ������
							    // �����
							    // ����
							    // ����
	  
	  if ( dTemp == *calDate )
	    {
	      table->setCurrentCell( i , j );
	      emit dateChanged( *calDate ); 
	    }
	
	  dTemp = dTemp.addDays ( 1 );
	  ++index;
	}
    }
  
  // ���������� �������� ������ ������������ ����
  bttnMonth->setText( QDate::longMonthName( calDate->month() ) );
  // ���������� ����� ���� ������������ ����
  bttnYear->setText( QString::number( calDate->year() ) );
  spBoxYear->setValue( calDate->year() );

}

// �������� ������������� ����
QDate & PDatePicker::date( void ) const
{
  return *calDate;
}

// ������� �� ������� �����
void PDatePicker::lastMonth()
{
  setDate( calDate->addMonths ( -1 ) );
}

// ������� �� ��������� �����
void PDatePicker::nextMonth()
{
  setDate( calDate->addMonths ( 1 ) );
}

// ������� �� ��������� ���
void PDatePicker::gotoCurrentDate()
{
  setDate( QDate::currentDate() );
}

void PDatePicker::triggeredMonth ( QAction * action )
{
  int year = calDate->year();
  int month = action->data().toInt();
  int day = calDate->day();

  // ��� ���� �������� ��� ����������� �������� � 31 ������� ��
  // ������� �����
  while( !QDate::isValid ( year , month , day ) )
    --day;

  setDate( QDate ( year , month , day ) );
}
// 458048 - ����

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

  // ��������� ��� ���������� �����, ���� ���������� ���������� ��� �
  // 29 �������
  while( !QDate::isValid ( year , month , day ) )
    --day;
  
  setDate( QDate ( year , month , day ) );
}

// ����� ��������� ������ ��������� ������������� ����, ���
// ����������� �� ��������� � ����� ����� ��� ����������
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

void PDatePicker::showHelp( void ) // ������� � ������ 
{
  QWhatsThis::showText( mapToGlobal( bttnHelp->geometry().bottomLeft() ) , 
			tr("��������� ���������� "
			   "��������� ������� ��������� ������:"
			   "<ul>"
			   "<li><b>Up, Down, Left, Right</b> - ����������� "
			   "�� ����."
			   "<li><b>PageUp</b> - �� ���� ����� �����."
			   "<li><b>PageDown</b> - �� ���� ����� ������."
			   "<li><b>Home</b> - ������� �� ������� ����."
			   "<li><b>Delete</b> - ������� ����� �� ���� � ������������� �� ����."
			   "<li><b>Del</b> - ������ ��� � ������������� �� ����."
			   "</ul>"
			   "<li><b>PDATEPICKER - ���������.</b>"
			   "<li><b>������:</b> 0.01."
			   "<li><b>�����:</b> ��������� �.�. ( aka pacmanoid )."
			   "<li><b>��������:</b> - <b>GNU.</b>"
			   "<li><b>���:</b> 2006."
			   "</ul>") , 

			this );
}
