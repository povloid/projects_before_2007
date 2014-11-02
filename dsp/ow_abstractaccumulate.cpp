/*
    ow_abstractaccumulate.cpp
*/

#include "ow_abstractaccumulate.h"
#include <QGroupBox>
#include <QHeaderView>

OW_AbstractAccumulate::OW_AbstractAccumulate(QWidget *parent)
    : PAbstractDBTable(parent)
{
  owDate = QDate::currentDate();
  
  boldFont = new QFont;
  boldFont->setBold(true);

  capPalette  = new QPalette( Qt::blue );
  capPalette->setColor( QPalette::Background , Qt::white );
  capPalette->setColor( QPalette::Foreground , Qt::black );


  // Компанент, отображающий дату в табло ----------------------
  // QFont font("Lucida", 13, QFont::Bold);
  QFont font("Lucida", 14 );
  //QFont font;
  //font.setBold(true);

  QPalette palette( Qt::white );
  palette.setColor( QPalette::Background , QColor( 0, 85, 255 ));
  palette.setColor( QPalette::Foreground , Qt::white );

  tabloCapLabel = new QLabel( tr("Заголовок"));
  tabloCapLabel->setPalette( palette );
  //  tabloDateLabel->setSizePolicy( QSizePolicy::Expanding );
  tabloCapLabel->setAutoFillBackground(true); // >>+<<
  tabloCapLabel->setFont( font );
  tabloCapLabel->setFrameStyle( QFrame::Box | QFrame::Raised );

  palette.setColor( QPalette::Background , QColor( 170, 255, 255 ));
  palette.setColor( QPalette::Foreground , Qt::blue );

  tabloDateLabel = new QLabel( tr("00.00.0000"));
  tabloDateLabel->setPalette( palette );
  tabloDateLabel->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  tabloDateLabel->setFont( font );
  tabloDateLabel->setMargin( 4 );
  //  tabloDateLabel->setSizePolicy( QSizePolicy::Expanding );
  tabloDateLabel->setAutoFillBackground(true); // >>+<<
  tabloDateLabel->setFrameStyle( QFrame::Box | QFrame::Raised );

  // Создаем SQL модель
  mainModel = new QSqlQueryModel;

  // 7  - 20 - 6..
  // Создаем таблицу
  mainView = new QTableView;
  mainView->setModel( mainModel );
  mainView->setSelectionBehavior ( QAbstractItemView::SelectRows );
  mainView->setSelectionMode ( QAbstractItemView::SingleSelection ); 
  mainView->setAlternatingRowColors ( true ); // отображать с чередующимися цветами строк
  mainView->setTabKeyNavigation ( false ); // отключить навигацию по кнопке TAB
  ((QHeaderView *)mainView->verticalHeader())->setDefaultSectionSize( 20 );
  ((QHeaderView *)mainView->verticalHeader())->hide();
  mainView->horizontalHeader()->setClickable(false);
  mainView->setItemDelegate( p_ru_ItemDelegate );
  
  // Создаем элементы для отображения текущенй информации
  
  setupLayouts();
}

void OW_AbstractAccumulate::setupLayouts( void )
{
  // Расмещаем все в соответствующих менеджерах размещения

  // Верхняя часть -----

  QHBoxLayout * capWidgetsLayout = new QHBoxLayout;

  QVBoxLayout * mainTableLayout = new QVBoxLayout;
  mainTableLayout->setMargin( 2 );
  mainTableLayout->addWidget( mainView );

  QGroupBox *mainTableGroupBox = new QGroupBox;
  mainTableGroupBox->setTitle( tr("Группа:") );
  mainTableGroupBox->setMaximumWidth( 300 );
  mainTableGroupBox->setMaximumHeight( 180 );
  mainTableGroupBox->setLayout(mainTableLayout);

  tabloLayout = new QGridLayout;
  tabloLayout->setSizeConstraint(QLayout::SetMaximumSize);
  tabloLayout->setMargin( 2 );

  QGroupBox *tabloGroupBox = new QGroupBox;
  tabloGroupBox->setTitle( tr("Показатели:") );
  tabloGroupBox->setMaximumHeight( 180 );
  tabloGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  tabloGroupBox->setLayoutDirection(Qt::LeftToRight);
  tabloGroupBox->setLayout(tabloLayout);

  capWidgetsLayout->addWidget( mainTableGroupBox );
  capWidgetsLayout->addWidget( tabloGroupBox );
  //...
  // Нижняя часть ----
  QVBoxLayout * tabLayout = new QVBoxLayout;
  tabLayout->addWidget( view );

  QVBoxLayout * buttonLayout = new QVBoxLayout;
  buttonLayout->addSpacing ( 30 );
  buttonLayout->addWidget( bttnAddRec );
  buttonLayout->addWidget( bttnEditRec );
  buttonLayout->addWidget( bttnDelRec );
  buttonLayout->addStretch();


  QHBoxLayout * subTableLayout = new QHBoxLayout;
  subTableLayout->addLayout( tabLayout );
  subTableLayout->addLayout( buttonLayout );
  subTableLayout->setMargin( 4 );

  QGroupBox *subTableGroupBox = new QGroupBox;
  subTableGroupBox->setTitle( tr("Подгруппа:") );
  subTableGroupBox->setLayout(subTableLayout);
  // ..
  //Создаем главный менеджер размещения
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget( captionViewLabel );
  mainLayout->addLayout( capWidgetsLayout );
  mainLayout->addWidget( subTableGroupBox );

  setLayout( mainLayout ); //Назначаем его главным на форме
}


void OW_AbstractAccumulate::setOWDate( const QDate & date )
{
  int row = mainView->currentIndex().row();
  
  owDate = date;
  mainModel->setQuery( selectMainSQL , db );

  mainView->selectRow ( row );
}
