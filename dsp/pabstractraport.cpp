/*
    pabstractraport.cpp
*/
#include "pabstractraport.h"
#include<QScrollBar>

PAbstractRaport::PAbstractRaport(QWidget *parent)
    : QWidget(parent)
{
  // Подключение к базе данных ---------------------------------
  db = QSqlDatabase::database("DSP"); 
  db.transaction();		// Начинаем транзакцию

  mainLayout = new QVBoxLayout;
  setLayout( mainLayout );

  QFont font;
  font.setBold(true);

  QPalette palette( Qt::blue );
  palette.setColor( QPalette::Background , QColor(Qt::white));

  capLabel = new QLabel;
  capLabel->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  capLabel->setFrameStyle( QFrame::Box | QFrame::Raised );
  capLabel->setAutoFillBackground(true); // >>+<<
  capLabel->setScaledContents(false); 
  capLabel->setPalette( palette );
  capLabel->setMargin(2);
  capLabel->setFont(font);
  mainLayout->addWidget( capLabel );

  textEdit = new QTextEdit;
  textEdit->setReadOnly( true );
  mainLayout->addWidget( textEdit );
}

void PAbstractRaport::setOWDate( const QDate & date )
{
  owDate = date;
  recalculate();
}

void PAbstractRaport::recalculate(void)
{
  int scroll = textEdit->verticalScrollBar()->value();

  update();

  textEdit->verticalScrollBar()->setValue( scroll );
}

