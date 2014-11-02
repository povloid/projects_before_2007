/*
  ow_prod_phos_tablo.cpp
*/
#include <QLocale>

#include "ow_prod_phos_tablo.h"

const int MaximumHeight = 200;

OW_Prod_Phos_Tablo::OW_Prod_Phos_Tablo( QWidget *parent , bool bool_edit_tmp )
  : QWidget(parent)
{
  bool_edit = bool_edit_tmp;

  locale = new QLocale;

  // Соглашение об именах
  // Выработка--
  //
  //   power_for_day_in_P5
  //   power_for_night_in_P5
  //   power_for_month_in_P5
  //
  // Производство фосфора--
  //
  //   p4_for_day_in_P5
  //   p4_for_month_in_P5
  //
  // Суммарные 
  // 
  // добавлять _SUM вместо например _in_P5
  
  // Создаем главный менеджер размещения
  QGridLayout *mainLayout = new QGridLayout;
  setLayout( mainLayout );

  QFont boldFont;
  boldFont.setBold(true);

  QPalette capPalette( Qt::blue );
  capPalette.setColor( QPalette::Background , Qt::white );
  capPalette.setColor( QPalette::Foreground , Qt::black );

  QLabel* label = new QLabel( tr("5-Цех"));
  label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  label->setFrameStyle( QFrame::Box | QFrame::Raised );
  label->setAutoFillBackground(true); // >>+<<
  label->setScaledContents(false); 
  label->setMargin(2);
  label->setFont( boldFont );
  mainLayout->addWidget ( label, 0, 0, 1, 1 );

  label = new QLabel( tr("Выр. за сут. (мВт):"));
  label->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
  label->setAutoFillBackground(true); // >>+<<
  label->setScaledContents(false); 
  label->setMargin(2);
  mainLayout->addWidget ( label, 2, 0, 1, 1 );

  label = new QLabel( tr("Выр. за ночь (мВт):"));
  label->setAutoFillBackground(true); // >>+<<
  label->setScaledContents(false); 
  label->setMargin(2);
  mainLayout->addWidget ( label, 3, 0, 1, 1 );

  label = new QLabel( tr("Выр. с нач. мес. (мВт):"));
  label->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
  label->setAutoFillBackground(true); // >>+<<
  label->setScaledContents(false); 
  label->setMargin(2);
  mainLayout->addWidget ( label, 4, 0, 1, 1 );

  label = new QLabel( tr("Фосфор за сут. (тн):"));
  label->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
  label->setAutoFillBackground(true); // >>+<<
  label->setScaledContents(false); 
  label->setMargin(2);
  mainLayout->addWidget ( label, 6, 0, 1, 1 );
  
  label = new QLabel( tr("Фосфор с нач. мес. (тн):"));
  label->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
  label->setAutoFillBackground(true); // >>+<<
  label->setScaledContents(false); 
  label->setMargin(2);
  mainLayout->addWidget ( label, 7, 0, 1, 1 );

  label = new QLabel( tr("5-Печь"));
  label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  label->setFrameStyle( QFrame::Panel | QFrame::Raised );
  label->setAutoFillBackground(true); // >>+<<
  label->setScaledContents(false); 
  label->setMargin(2);
  label->setPalette( capPalette );
  label->setFont( boldFont );
  mainLayout->addWidget ( label, 0, 1 );

  label = new QLabel( tr("6-Печь"));
  label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  label->setFrameStyle( QFrame::Panel | QFrame::Raised );
  label->setAutoFillBackground(true); // >>+<<
  label->setScaledContents(false); 
  label->setMargin(2);
  label->setPalette( capPalette );
  label->setFont( boldFont );
  mainLayout->addWidget ( label, 0, 2 );

  label = new QLabel( tr("7-Печь"));
  label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  label->setFrameStyle( QFrame::Panel | QFrame::Raised );
  label->setAutoFillBackground(true); // >>+<<
  label->setScaledContents(false); 
  label->setMargin(2);
  label->setPalette( capPalette );
  label->setFont( boldFont );
  mainLayout->addWidget ( label, 0, 3 );

  label = new QLabel( tr("8-Печь"));
  label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  label->setFrameStyle( QFrame::Panel | QFrame::Raised );
  label->setAutoFillBackground(true); // >>+<<
  label->setScaledContents(false); 
  label->setMargin(2);
  label->setPalette( capPalette );
  label->setFont( boldFont );
  mainLayout->addWidget ( label, 0, 4 );

  capPalette.setColor( QPalette::Background , Qt::yellow );

  label = new QLabel( tr("И того"));
  label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  label->setFrameStyle( QFrame::Panel | QFrame::Raised );
  label->setAutoFillBackground(true); // >>+<<
  label->setScaledContents(false); 
  label->setMargin(2);
  label->setPalette( capPalette );
  label->setFont( boldFont );
  mainLayout->addWidget ( label, 0, 5 );

  QFrame *line;
  // Разделитель >-------<
  line = new QFrame;	// --------------------------
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow ( QFrame::Sunken );
  mainLayout->addWidget ( line , 1, 0, 1, 6 );

  line = new QFrame;	// --------------------------
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow ( QFrame::Sunken );
  mainLayout->addWidget ( line , 5, 0, 1, 6 );


  // Активные этементы

  // 5-Печь --------------------------------------
  power_for_month_in_P5 = new QLabel( tr("0"));
  power_for_month_in_P5->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  power_for_month_in_P5->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  power_for_month_in_P5->setAutoFillBackground(true); // >>+<<
  power_for_month_in_P5->setScaledContents(false); 
  power_for_month_in_P5->setMargin(2);
  mainLayout->addWidget ( power_for_month_in_P5, 4, 1 );

  p4_for_day_in_P5 = new QLabel( tr("0"));
  p4_for_day_in_P5->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  p4_for_day_in_P5->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  p4_for_day_in_P5->setAutoFillBackground(true); // >>+<<
  p4_for_day_in_P5->setScaledContents(false); 
  p4_for_day_in_P5->setMargin(2);
  mainLayout->addWidget ( p4_for_day_in_P5, 6, 1 );

  p4_for_month_in_P5 = new QLabel( tr("0"));
  p4_for_month_in_P5->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  p4_for_month_in_P5->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  p4_for_month_in_P5->setAutoFillBackground(true); // >>+<<
  p4_for_month_in_P5->setScaledContents(false); 
  p4_for_month_in_P5->setMargin(2);
  mainLayout->addWidget ( p4_for_month_in_P5, 7, 1 );
  //..

  // 6-Печь --------------------------------------
  power_for_month_in_P6 = new QLabel( tr("0"));
  power_for_month_in_P6->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  power_for_month_in_P6->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  power_for_month_in_P6->setAutoFillBackground(true); // >>+<<
  power_for_month_in_P6->setScaledContents(false); 
  power_for_month_in_P6->setMargin(2);
  mainLayout->addWidget ( power_for_month_in_P6, 4, 2 );

  p4_for_day_in_P6 = new QLabel( tr("0"));
  p4_for_day_in_P6->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  p4_for_day_in_P6->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  p4_for_day_in_P6->setAutoFillBackground(true); // >>+<<
  p4_for_day_in_P6->setScaledContents(false); 
  p4_for_day_in_P6->setMargin(2);
  mainLayout->addWidget ( p4_for_day_in_P6, 6, 2 );

  p4_for_month_in_P6 = new QLabel( tr("0"));
  p4_for_month_in_P6->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  p4_for_month_in_P6->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  p4_for_month_in_P6->setAutoFillBackground(true); // >>+<<
  p4_for_month_in_P6->setScaledContents(false); 
  p4_for_month_in_P6->setMargin(2);
  mainLayout->addWidget ( p4_for_month_in_P6, 7, 2 );
  //..

  // 7-Печь --------------------------------------
  power_for_month_in_P7 = new QLabel( tr("0"));
  power_for_month_in_P7->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  power_for_month_in_P7->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  power_for_month_in_P7->setAutoFillBackground(true); // >>+<<
  power_for_month_in_P7->setScaledContents(false); 
  power_for_month_in_P7->setMargin(2);
  mainLayout->addWidget ( power_for_month_in_P7, 4, 3 );

  p4_for_day_in_P7 = new QLabel( tr("0"));
  p4_for_day_in_P7->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  p4_for_day_in_P7->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  p4_for_day_in_P7->setAutoFillBackground(true); // >>+<<
  p4_for_day_in_P7->setScaledContents(false); 
  p4_for_day_in_P7->setMargin(2);
  mainLayout->addWidget ( p4_for_day_in_P7, 6, 3 );

  p4_for_month_in_P7 = new QLabel( tr("0"));
  p4_for_month_in_P7->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  p4_for_month_in_P7->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  p4_for_month_in_P7->setAutoFillBackground(true); // >>+<<
  p4_for_month_in_P7->setScaledContents(false); 
  p4_for_month_in_P7->setMargin(2);
  mainLayout->addWidget ( p4_for_month_in_P7, 7, 3 );
  //..

  // 8-Печь --------------------------------------
  power_for_month_in_P8 = new QLabel( tr("0"));
  power_for_month_in_P8->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  power_for_month_in_P8->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  power_for_month_in_P8->setAutoFillBackground(true); // >>+<<
  power_for_month_in_P8->setScaledContents(false); 
  power_for_month_in_P8->setMargin(2);
  mainLayout->addWidget ( power_for_month_in_P8, 4, 4 );

  p4_for_day_in_P8 = new QLabel( tr("0"));
  p4_for_day_in_P8->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  p4_for_day_in_P8->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  p4_for_day_in_P8->setAutoFillBackground(true); // >>+<<
  p4_for_day_in_P8->setScaledContents(false); 
  p4_for_day_in_P8->setMargin(2);
  mainLayout->addWidget ( p4_for_day_in_P8, 6, 4 );

  p4_for_month_in_P8 = new QLabel( tr("0"));
  p4_for_month_in_P8->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  p4_for_month_in_P8->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  p4_for_month_in_P8->setAutoFillBackground(true); // >>+<<
  p4_for_month_in_P8->setScaledContents(false); 
  p4_for_month_in_P8->setMargin(2);
  mainLayout->addWidget ( p4_for_month_in_P8, 7, 4 );
  //..
  
  // И того  --------------------------------------
  power_for_day_SUM = new QLabel( tr("0"));
  power_for_day_SUM->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  power_for_day_SUM->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  power_for_day_SUM->setAutoFillBackground(true); // >>+<<
  power_for_day_SUM->setScaledContents(false); 
  power_for_day_SUM->setMargin(2);
  mainLayout->addWidget ( power_for_day_SUM, 2, 5 );

  power_for_night_SUM = new QLabel( tr("0"));
  power_for_night_SUM->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  power_for_night_SUM->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  power_for_night_SUM->setAutoFillBackground(true); // >>+<<
  power_for_night_SUM->setScaledContents(false); 
  power_for_night_SUM->setMargin(2);
  mainLayout->addWidget ( power_for_night_SUM, 3, 5 );

  power_for_month_SUM = new QLabel( tr("0"));
  power_for_month_SUM->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  power_for_month_SUM->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  power_for_month_SUM->setAutoFillBackground(true); // >>+<<
  power_for_month_SUM->setScaledContents(false); 
  power_for_month_SUM->setMargin(2);
  mainLayout->addWidget ( power_for_month_SUM, 4, 5 );

  p4_for_day_SUM = new QLabel( tr("0"));
  p4_for_day_SUM->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  p4_for_day_SUM->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  p4_for_day_SUM->setAutoFillBackground(true); // >>+<<
  p4_for_day_SUM->setScaledContents(false); 
  p4_for_day_SUM->setMargin(2);
  mainLayout->addWidget ( p4_for_day_SUM, 6, 5 );

  p4_for_month_SUM = new QLabel( tr("0"));
  p4_for_month_SUM->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  p4_for_month_SUM->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  p4_for_month_SUM->setAutoFillBackground(true); // >>+<<
  p4_for_month_SUM->setScaledContents(false); 
  p4_for_month_SUM->setMargin(2);
  mainLayout->addWidget ( p4_for_month_SUM, 7, 5 );
  //..

  // ----------------------------------------------------------------------
  // По типу назначения виджета,
  // то есть чисто для отображения информации или же для ввода ее 
  // QLabel или QLineEdit
  // S5 ****
  if ( bool_edit ) 		// Если использовать для ввода
    {
      powerED_for_day_in_P5 = new QDoubleSpinBox;
      powerED_for_day_in_P5->setRange(0.0, 100000.0);
      powerED_for_day_in_P5->setSingleStep(10.0);
      powerED_for_day_in_P5->setSuffix(tr(" МВт."));
      powerED_for_day_in_P5->setValue(0.0);
      powerED_for_day_in_P5->setAlignment(Qt::AlignRight);
      mainLayout->addWidget ( powerED_for_day_in_P5, 2, 1 );

      powerED_for_night_in_P5 = new QDoubleSpinBox;
      powerED_for_night_in_P5->setRange(0.0, 100000.0);
      powerED_for_night_in_P5->setSingleStep(10.0);
      powerED_for_night_in_P5->setSuffix(tr(" МВт."));
      powerED_for_night_in_P5->setValue(0.0);
      powerED_for_night_in_P5->setAlignment(Qt::AlignRight);
      mainLayout->addWidget ( powerED_for_night_in_P5, 3, 1 );

      connect( powerED_for_day_in_P5, SIGNAL( valueChanged ( const QString & ) ), 
	       this, SLOT( recount( const QString & )));
      connect( powerED_for_night_in_P5, SIGNAL( valueChanged ( const QString & ) ), 
	       this, SLOT( recount( const QString & )));

      // S6 ****
      powerED_for_day_in_P6 = new QDoubleSpinBox;
      powerED_for_day_in_P6->setRange(0.0, 100000.0);
      powerED_for_day_in_P6->setSingleStep(10.0);
      powerED_for_day_in_P6->setSuffix(tr(" МВт."));
      powerED_for_day_in_P6->setValue(0.0);
      powerED_for_day_in_P6->setAlignment(Qt::AlignRight);
      mainLayout->addWidget ( powerED_for_day_in_P6, 2, 2 );

      powerED_for_night_in_P6 = new QDoubleSpinBox;
      powerED_for_night_in_P6->setRange(0.0, 100000.0);
      powerED_for_night_in_P6->setSingleStep(10.0);
      powerED_for_night_in_P6->setSuffix(tr(" МВт."));
      powerED_for_night_in_P6->setValue(0.0);
      powerED_for_night_in_P6->setAlignment(Qt::AlignRight);
      mainLayout->addWidget ( powerED_for_night_in_P6, 3, 2 );

      connect( powerED_for_day_in_P6, SIGNAL( valueChanged ( const QString & ) ), 
	       this, SLOT( recount( const QString & )));
      connect( powerED_for_night_in_P6, SIGNAL( valueChanged ( const QString & ) ), 
	       this, SLOT( recount( const QString & )));

      // S7 ****
      powerED_for_day_in_P7 = new QDoubleSpinBox;
      powerED_for_day_in_P7->setRange(0.0, 100000.0);
      powerED_for_day_in_P7->setSingleStep(10.0);
      powerED_for_day_in_P7->setSuffix(tr(" МВт."));
      powerED_for_day_in_P7->setValue(0.0);
      powerED_for_day_in_P7->setAlignment(Qt::AlignRight);
      mainLayout->addWidget ( powerED_for_day_in_P7, 2, 3 );

      powerED_for_night_in_P7 = new QDoubleSpinBox;
      powerED_for_night_in_P7->setRange(0.0, 100000.0);
      powerED_for_night_in_P7->setSingleStep(10.0);
      powerED_for_night_in_P7->setSuffix(tr(" МВт."));
      powerED_for_night_in_P7->setValue(0.0);
      powerED_for_night_in_P7->setAlignment(Qt::AlignRight);
      mainLayout->addWidget ( powerED_for_night_in_P7, 3, 3 );

      connect( powerED_for_day_in_P7, SIGNAL( valueChanged ( const QString & ) ), 
	       this, SLOT( recount( const QString & )));
      connect( powerED_for_night_in_P7, SIGNAL( valueChanged ( const QString & ) ), 
	       this, SLOT( recount( const QString & )));
      // S8 ****
      powerED_for_day_in_P8 = new QDoubleSpinBox;
      powerED_for_day_in_P8->setRange(0.0, 100000.0);
      powerED_for_day_in_P8->setSingleStep(10.0);
      powerED_for_day_in_P8->setSuffix(tr(" МВт."));
      powerED_for_day_in_P8->setValue(0.0);
      powerED_for_day_in_P8->setAlignment(Qt::AlignRight);
      mainLayout->addWidget ( powerED_for_day_in_P8, 2, 4 );

      powerED_for_night_in_P8 = new QDoubleSpinBox;
      powerED_for_night_in_P8->setRange(0.0, 100000.0);
      powerED_for_night_in_P8->setSingleStep(10.0);
      powerED_for_night_in_P8->setSuffix(tr(" МВт."));
      powerED_for_night_in_P8->setValue(0.0);
      powerED_for_night_in_P8->setAlignment(Qt::AlignRight);
      mainLayout->addWidget ( powerED_for_night_in_P8, 3, 4 );

      connect( powerED_for_day_in_P8, SIGNAL( valueChanged ( const QString & ) ), 
	       this, SLOT( recount( const QString & )));
      connect( powerED_for_night_in_P8, SIGNAL( valueChanged ( const QString & ) ), 
	       this, SLOT( recount( const QString & )));
    }
  else
    {
      power_for_day_in_P5 = new QLabel( tr("0"));
      power_for_day_in_P5->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
      power_for_day_in_P5->setFrameStyle( QFrame::Panel | QFrame::Sunken );
      power_for_day_in_P5->setAutoFillBackground(true); // >>+<<
      power_for_day_in_P5->setScaledContents(false); 
      power_for_day_in_P5->setMargin(2);
      mainLayout->addWidget ( power_for_day_in_P5, 2, 1 );

      power_for_night_in_P5 = new QLabel( tr("0"));
      power_for_night_in_P5->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
      power_for_night_in_P5->setFrameStyle( QFrame::Panel | QFrame::Sunken );
      power_for_night_in_P5->setAutoFillBackground(true); // >>+<<
      power_for_night_in_P5->setScaledContents(false); 
      power_for_night_in_P5->setMargin(2);
      mainLayout->addWidget ( power_for_night_in_P5, 3, 1 );
      // S6 ****
      power_for_day_in_P6 = new QLabel( tr("0"));
      power_for_day_in_P6->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
      power_for_day_in_P6->setFrameStyle( QFrame::Panel | QFrame::Sunken );
      power_for_day_in_P6->setAutoFillBackground(true); // >>+<<
      power_for_day_in_P6->setScaledContents(false); 
      power_for_day_in_P6->setMargin(2);
      mainLayout->addWidget ( power_for_day_in_P6, 2, 2 );

      power_for_night_in_P6 = new QLabel( tr("0"));
      power_for_night_in_P6->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
      power_for_night_in_P6->setFrameStyle( QFrame::Panel | QFrame::Sunken );
      power_for_night_in_P6->setAutoFillBackground(true); // >>+<<
      power_for_night_in_P6->setScaledContents(false); 
      power_for_night_in_P6->setMargin(2);
      mainLayout->addWidget ( power_for_night_in_P6, 3, 2 );
      // S7 ****
      power_for_day_in_P7 = new QLabel( tr("0"));
      power_for_day_in_P7->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
      power_for_day_in_P7->setFrameStyle( QFrame::Panel | QFrame::Sunken );
      power_for_day_in_P7->setAutoFillBackground(true); // >>+<<
      power_for_day_in_P7->setScaledContents(false); 
      power_for_day_in_P7->setMargin(2);
      mainLayout->addWidget ( power_for_day_in_P7, 2, 3 );

      power_for_night_in_P7 = new QLabel( tr("0"));
      power_for_night_in_P7->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
      power_for_night_in_P7->setFrameStyle( QFrame::Panel | QFrame::Sunken );
      power_for_night_in_P7->setAutoFillBackground(true); // >>+<<
      power_for_night_in_P7->setScaledContents(false); 
      power_for_night_in_P7->setMargin(2);
      mainLayout->addWidget ( power_for_night_in_P7, 3, 3 );
      // S8 ****
      power_for_day_in_P8 = new QLabel( tr("0"));
      power_for_day_in_P8->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
      power_for_day_in_P8->setFrameStyle( QFrame::Panel | QFrame::Sunken );
      power_for_day_in_P8->setAutoFillBackground(true); // >>+<<
      power_for_day_in_P8->setScaledContents(false); 
      power_for_day_in_P8->setMargin(2);
      mainLayout->addWidget ( power_for_day_in_P8, 2, 4 );

      power_for_night_in_P8 = new QLabel( tr("0"));
      power_for_night_in_P8->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
      power_for_night_in_P8->setFrameStyle( QFrame::Panel | QFrame::Sunken );
      power_for_night_in_P8->setAutoFillBackground(true); // >>+<<
      power_for_night_in_P8->setScaledContents(false); 
      power_for_night_in_P8->setMargin(2);
      mainLayout->addWidget ( power_for_night_in_P8, 3, 4 );
    }
  
  // Подстройка размеров
  mainLayout->setColumnMinimumWidth ( 5 , 80 );
  mainLayout->setMargin ( 2 );
  setMaximumHeight( MaximumHeight );
}
// Перекрываем функцию получения фокуса
void OW_Prod_Phos_Tablo::setFocus()
{
  powerED_for_day_in_P5->setFocus();
}

// Функции - необходимые для ввода текста
// S5 -----------------------------------
void OW_Prod_Phos_Tablo::setText_for_day_in_P5( const double & w )
{
  QString str = locale->toString( w );

  if ( bool_edit ) 		// Если использовать для ввода
    powerED_for_day_in_P5->setValue( w );
  else
    power_for_day_in_P5->setText( str );
  // Пересчет
  recount( str );
}

void OW_Prod_Phos_Tablo::setText_for_night_in_P5( const double & w )
{
  QString str = locale->toString( w );

  if ( bool_edit ) 		// Если использовать для ввода
    powerED_for_night_in_P5->setValue( w );
  else
    power_for_night_in_P5->setText( str );
  // Пересчет
  recount( str );
}

// S6 -----------------------------------
void OW_Prod_Phos_Tablo::setText_for_day_in_P6( const double & w )
{
  QString str = locale->toString( w );

  if ( bool_edit ) 		// Если использовать для ввода
    powerED_for_day_in_P6->setValue( w );
  else
    power_for_day_in_P6->setText( str );
  // Пересчет
  recount( str );
}

void OW_Prod_Phos_Tablo::setText_for_night_in_P6( const double & w )
{
  QString str = locale->toString( w );

  if ( bool_edit ) 		// Если использовать для ввода
    powerED_for_night_in_P6->setValue( w );
  else
    power_for_night_in_P6->setText( str );
  // Пересчет
  recount( str );
}

// S7 -----------------------------------
void OW_Prod_Phos_Tablo::setText_for_day_in_P7( const double & w )
{
  QString str = locale->toString( w );

  if ( bool_edit ) 		// Если использовать для ввода
    powerED_for_day_in_P7->setValue( w );
  else
    power_for_day_in_P7->setText( str );
  // Пересчет
  recount( str );
}

void OW_Prod_Phos_Tablo::setText_for_night_in_P7( const double & w )
{
  QString str = locale->toString( w );

  if ( bool_edit ) 		// Если использовать для ввода
    powerED_for_night_in_P7->setValue( w );
  else
    power_for_night_in_P7->setText( str );
  // Пересчет
  recount( str );
}

// S8 -----------------------------------
void OW_Prod_Phos_Tablo::setText_for_day_in_P8( const double & w )
{
  QString str = locale->toString( w );

  if ( bool_edit ) 		// Если использовать для ввода
    powerED_for_day_in_P8->setValue( w );
  else
    power_for_day_in_P8->setText( str );
  // Пересчет
  recount( str );
}

void OW_Prod_Phos_Tablo::setText_for_night_in_P8( const double & w )
{
  QString str = locale->toString( w );

  if ( bool_edit ) 		// Если использовать для ввода
    powerED_for_night_in_P8->setValue( w );
  else
    power_for_night_in_P8->setText( str );
  // Пересчет
  recount( str );
}

// Функции - необходимые для вывода текста *<--<<
// S5 -----------------------------------
double OW_Prod_Phos_Tablo::value_for_day_in_P5( void ) const
{
  if ( bool_edit ) 		// Если использовать для ввода
    return powerED_for_day_in_P5->value();
  else
    return 0;
}

double OW_Prod_Phos_Tablo::value_for_night_in_P5( void ) const
{
 if ( bool_edit ) 		// Если использовать для ввода
    return powerED_for_night_in_P5->value();
  else
    return 0;
}

// S6 -----------------------------------
double OW_Prod_Phos_Tablo::value_for_day_in_P6( void ) const
{
  if ( bool_edit ) 		// Если использовать для ввода
    return powerED_for_day_in_P6->value();
  else
    return 0;
}

double OW_Prod_Phos_Tablo::value_for_night_in_P6( void ) const
{
  if ( bool_edit ) 		// Если использовать для ввода
    return powerED_for_night_in_P6->value();
  else
    return 0;
}

// S7 -----------------------------------
double OW_Prod_Phos_Tablo::value_for_day_in_P7( void ) const
{
  if ( bool_edit ) 		// Если использовать для ввода
    return powerED_for_day_in_P7->value();
  else
    return 0;
}

double OW_Prod_Phos_Tablo::value_for_night_in_P7( void ) const
{
  if ( bool_edit ) 		// Если использовать для ввода
    return powerED_for_night_in_P7->value();
  else
    return 0;
}


// S8 -----------------------------------
double OW_Prod_Phos_Tablo::value_for_day_in_P8( void ) const
{
  if ( bool_edit ) 		// Если использовать для ввода
    return powerED_for_day_in_P8->value();
  else
    return 0;
}

double OW_Prod_Phos_Tablo::value_for_night_in_P8( void ) const
{
  if ( bool_edit ) 		// Если использовать для ввода
    return powerED_for_night_in_P8->value();
  else
    return 0;
}

// Функция пересчета ------------------
void OW_Prod_Phos_Tablo::recount( const QString & str )
{
  if ( str == "" )
    return;

  double powerP5Day, powerP6Day, powerP7Day, powerP8Day;
  double powerP5Night, powerP6Night, powerP7Night, powerP8Night;

  if ( bool_edit ) 		// Если использовать для ввода
    {
      powerP5Day = powerED_for_day_in_P5->value();
      powerP6Day = powerED_for_day_in_P6->value();
      powerP7Day = powerED_for_day_in_P7->value();
      powerP8Day = powerED_for_day_in_P8->value();

      powerP5Night = powerED_for_night_in_P5->value();
      powerP6Night = powerED_for_night_in_P6->value();
      powerP7Night = powerED_for_night_in_P7->value();
      powerP8Night = powerED_for_night_in_P8->value();
    }
  else
    {
      powerP5Day = locale->toDouble( power_for_day_in_P5->text() );
      powerP6Day = locale->toDouble( power_for_day_in_P6->text() );
      powerP7Day = locale->toDouble( power_for_day_in_P7->text() );
      powerP8Day = locale->toDouble( power_for_day_in_P8->text() );

      powerP5Night = locale->toDouble( power_for_night_in_P5->text() );
      powerP6Night = locale->toDouble( power_for_night_in_P6->text() );
      powerP7Night = locale->toDouble( power_for_night_in_P7->text() );
      powerP8Night = locale->toDouble( power_for_night_in_P8->text() );
    }

  power_for_day_SUM->setText( locale->toString( powerP5Day + powerP6Day 
						+ powerP7Day + powerP8Day ));

  power_for_night_SUM->setText( locale->toString( powerP5Night + powerP6Night 
						  + powerP7Night + powerP8Night ));

  p4_for_day_SUM->setText( locale->toString( locale->toDouble( p4_for_day_in_P5->text())
					     + locale->toDouble( p4_for_day_in_P6->text())
					     + locale->toDouble( p4_for_day_in_P7->text()) 
					     + locale->toDouble( p4_for_day_in_P8->text())));
  
  // Подсчет данных за месяц --
  // Power
  double powerP5Month = power_For_Month_in_P5 + powerP5Day;
  double powerP6Month = power_For_Month_in_P6 + powerP6Day;
  double powerP7Month = power_For_Month_in_P7 + powerP7Day;
  double powerP8Month = power_For_Month_in_P8 + powerP8Day;

  power_for_month_in_P5->setText( locale->toString( powerP5Month ));
  power_for_month_in_P6->setText( locale->toString( powerP6Month ));
  power_for_month_in_P7->setText( locale->toString( powerP7Month ));
  power_for_month_in_P8->setText( locale->toString( powerP8Month ));
      
  power_for_month_SUM->setText( locale->toString( powerP5Month
						 + powerP6Month
						 + powerP7Month
						 + powerP8Month ));
  // P4

  double p4P5Month = p4_For_Month_in_P5 + locale->toDouble( p4_for_day_in_P5->text());
  double p4P6Month = p4_For_Month_in_P6 + locale->toDouble( p4_for_day_in_P6->text());
  double p4P7Month = p4_For_Month_in_P7 + locale->toDouble( p4_for_day_in_P7->text());
  double p4P8Month = p4_For_Month_in_P8 + locale->toDouble( p4_for_day_in_P8->text());

  p4_for_month_in_P5->setText( locale->toString( p4P5Month ));
  p4_for_month_in_P6->setText( locale->toString( p4P6Month ));
  p4_for_month_in_P7->setText( locale->toString( p4P7Month ));
  p4_for_month_in_P8->setText( locale->toString( p4P8Month ));
      
  p4_for_month_SUM->setText( locale->toString( p4P5Month
					       + p4P6Month
					       + p4P7Month
					       + p4P8Month ));

}

// Отображение месячных показателей ВЫРАБОТКЕ ЭЛЕКТРОЭНЕРГИИ -----
void OW_Prod_Phos_Tablo::setPower_For_Month_in_P5( const double & w )
{
  power_For_Month_in_P5 = w;
}
void OW_Prod_Phos_Tablo::setPower_For_Month_in_P6( const double & w )
{
  power_For_Month_in_P6 = w;
}
void OW_Prod_Phos_Tablo::setPower_For_Month_in_P7( const double & w )
{
  power_For_Month_in_P7 = w;
}
void OW_Prod_Phos_Tablo::setPower_For_Month_in_P8( const double & w )
{
  power_For_Month_in_P8 = w;
}
// ..
// Отображение показателей по ПРОИЗВОДСТВУ фосфора -----
void OW_Prod_Phos_Tablo::setText_P4_For_Day_in_P5( const double & w )
{
  QString str = locale->toString( w );
  
  p4_for_day_in_P5->setText( str );
  // Пересчет
  recount( str );
}
void OW_Prod_Phos_Tablo::setText_P4_For_Day_in_P6( const double & w )
{
  QString str = locale->toString( w );

  p4_for_day_in_P6->setText( str );
  // Пересчет
  recount( str );
}
void OW_Prod_Phos_Tablo::setText_P4_For_Day_in_P7( const double & w )
{
  QString str = locale->toString( w );

  p4_for_day_in_P7->setText( str );
  // Пересчет
  recount( str );
}
void OW_Prod_Phos_Tablo::setText_P4_For_Day_in_P8( const double & w )
{
  QString str = locale->toString( w );

  p4_for_day_in_P8->setText( str );
  // Пересчет
  recount( str );
}
// ..
// Отображение МЕСЯЧНЫХ показателей по ПРОИЗВОДСТВУ фосфора -----
void OW_Prod_Phos_Tablo::setP4_For_Month_in_P5( const double & w )
{
  p4_For_Month_in_P5 = w;
}
void OW_Prod_Phos_Tablo::setP4_For_Month_in_P6( const double & w )
{
  p4_For_Month_in_P6 = w;
}
void OW_Prod_Phos_Tablo::setP4_For_Month_in_P7( const double & w )
{
  p4_For_Month_in_P7 = w;
}
void OW_Prod_Phos_Tablo::setP4_For_Month_in_P8( const double & w )
{
  p4_For_Month_in_P8 = w;
}
// ..
