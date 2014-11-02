/*
    example.cpp
*/

#include "example.h"
#include "pdatepicker.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDate>

Example::Example(QWidget *parent)
    : QDialog(parent)
{
  QHBoxLayout *mainLayout = new QHBoxLayout;
  setLayout( mainLayout );

  pDatePicker1 = new PDatePicker;
  pDatePicker2 = new PDatePicker(30,40);
  pDatePicker3 = new PDatePicker;
  pDatePicker3->setViewOnly( true );
  
  connect( pDatePicker1 , SIGNAL( dateChanged( const QDate & ) ),
	   pDatePicker2 , SLOT( setDate( const QDate & ) ) );

  connect( pDatePicker2 , SIGNAL( dateChanged( const QDate & ) ),
	   pDatePicker3 , SLOT( setDate( const QDate & ) ) );

  connect( pDatePicker3 , SIGNAL( dateChanged( const QDate & ) ),
	   this , SLOT( setDate( const QDate & ) ) );


  button = new QPushButton;
  mainLayout->addWidget( button );
  button->setFocusPolicy( Qt::NoFocus );
  connect( button , SIGNAL( clicked() ),
	   this , SLOT( setButtonText() ) );
  
  QLabel *label1 = new QLabel( tr (" setViewOnly(true) --> ") );

  mainLayout->addWidget( pDatePicker1 );
  mainLayout->addWidget( pDatePicker2 );
  mainLayout->addWidget( label1 );
  mainLayout->addWidget( pDatePicker3 );


}

void Example::setButtonText()
{
  button->setText(pDatePicker1->date().toString("dd.MM.yyyy"));
}

#include <iostream>
void Example::setDate( const QDate & date )
{
  std::cout<<date.toString("dd.MM.yyyy").toStdString()<<"  -  "
	   <<pDatePicker1->date().toString("dd.MM.yyyy").toStdString()<<std::endl;

}
