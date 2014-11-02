/*
    pdialog.cpp
*/

#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QBitmap>
#include <QBrush>

#include "pdialog.h"

#include "dspconst.h"

PDialog::PDialog( QWidget * parent , Qt::WFlags f )
    : QDialog( parent , f )
{
  // Создаем главный менеджер размещения
  QVBoxLayout * mainLayout = new QVBoxLayout;
  setLayout( mainLayout );
  // Устанавливаем фиксированный размер 
  mainLayout->setSizeConstraint( QLayout::SetFixedSize ); 

  // Шапка диалога ----------------------
  QPalette palette( Qt::white );
  palette.setColor( QPalette::Background , QColor( 0, 85, 255 ));


  QFrame *frameCap = new QFrame;
  frameCap->setPalette( palette );
  frameCap->setAutoFillBackground(true); // >>+<<
  frameCap->setFrameStyle( QFrame::Box | QFrame::Raised );

  QHBoxLayout* frameCapLayout = new QHBoxLayout;
  frameCapLayout->setMargin( 2 );
  frameCap->setLayout( frameCapLayout );

  imageLabelCapPixmap = new QLabel;
  imageLabelCapPixmap->setBackgroundRole(QPalette::Base);
  imageLabelCapPixmap->setPixmap( piconPath + "/apps_32.png" );

  frameCapLayout->addWidget( imageLabelCapPixmap );

  QFont font("Lucida", 14 );
  //font.setBold(true);
  palette.setColor( QPalette::Foreground , QColor( "white" ));

  labelCapText = new QLabel("Надпись в заголовке");
  labelCapText->setFont( font );
  labelCapText->setPalette( palette );
  //  labelCapText->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
  frameCapLayout->addWidget( labelCapText );
  frameCapLayout->addStretch();

  mainLayout->addWidget( frameCap ); 
  // ..

  // Создаем менеджер размещения для компонентов ввода
  iputObjectsLayout = new QHBoxLayout;
  iputObjectsLayout->setMargin(2);
    
  QFrame *frame = new QFrame;
 
//   QBrush brush( Qt::TexturePattern );
//   brush.setTexture( QPixmap( tilesPath + "/kenwimer.png" ));
//   palette.setBrush ( QPalette::Background , brush );
//   palette.setBrush ( QPalette::Foreground , Qt::black );
//   frame->setPalette( palette );
  
//   QPalette palette2;
//   palette2.setBrush ( QPalette::Background , QColor( 255 , 226 , 199 ) );
//   palette2.setBrush ( QPalette::Foreground , Qt::black );

//   frame->setPalette( palette2 );

  frame->setAutoFillBackground(true); // >>+<<
  frame->setFrameStyle( QFrame::WinPanel | QFrame::Raised );
  frame->setLayout( iputObjectsLayout );
  mainLayout->addWidget( frame );

  // Создаем кнопки
  okButton = new QPushButton(tr("Принять")); 
  okButton->setIconSize ( QSize( 24, 24 ) );
  okButton->setIcon( QIcon( radiantPath + "/opts_24.png") );
  okButton->setMaximumHeight ( buttonsMaximumHeight );

  QPushButton *cancelButton = new QPushButton(tr("Закрыть"));
  cancelButton->setIconSize ( QSize( 24, 24 ) );
  cancelButton->setIcon( QIcon( radiantPath + "/cancl_24.png") ); 
  cancelButton->setMaximumHeight ( buttonsMaximumHeight );

  // Сигналы и слоты *--> >----|
  connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject())); 
  //............................

  // Создаем менеджер размещения для кнопок диалога
  QHBoxLayout* buttonsLayout = new QHBoxLayout;
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(okButton); 
  buttonsLayout->addWidget(cancelButton);
  mainLayout->addLayout(buttonsLayout);


}

void PDialog::accept()
{
  QDialog::accept();
}

// Установить иконку в шапке
void PDialog::setCapPixmap( QString strPixmapName )
{
  imageLabelCapPixmap->setPixmap( strPixmapName );
  return;
}
// Задать текст в шапке 
void PDialog::setCapText( QString strText )
{
  labelCapText->setText( strText + tr("    "));
  return;
}
