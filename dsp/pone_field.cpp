/*
    pone_field.cpp

*/

#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "pone_field.h"

POne_Field::POne_Field(QWidget *parent , QString strCaption , Type type_tmp , int sizeHorizon )
    : QWidget( parent )
{
  type = type_tmp;

  // Создаем главный менеджер размещения
  QVBoxLayout * mainLayout = new QVBoxLayout;
  setLayout( mainLayout );
  // Устанавливаем фиксированный размер 
  mainLayout->setSizeConstraint( QLayout::SetFixedSize ); 

  // Создаем компанент надписи в заголовке и устанавливаем надпись в заголовке
  caption = new QLabel( strCaption );

  // Создаем компанент ввода 
  switch (type){
  case Label:
    {
      input = new QLabel;
      ((QLabel*)input)->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
      ((QLabel*)input)->setFrameStyle( QFrame::Panel | QFrame::Sunken );
      ((QLabel*)input)->setAutoFillBackground(true); // >>+<<
      ((QLabel*)input)->setScaledContents(false); 
      ((QLabel*)input)->setMargin(2);
      break;
    }
  case LineEdit:
    {
      input = new QLineEdit;
      break;
    }
  case ComboBox:
    {
      input = new QComboBox;
      break;
    }
  case SpinBox:
    {
      input = new QSpinBox;
      break;
    }
  case DoubleSpinBox:
    {
      input = new QDoubleSpinBox;
      break;
    }
  default:
    {
      input = new QLineEdit;
      break;
    }
  }

  // Вставляем созданные компоненты в главный менеджер размещения виджета
  mainLayout->addWidget( caption );
  mainLayout->addWidget( (QWidget *) input );

  // Устанавливаем горизонтальный размер компонента ввода
  ((QWidget *) input)->setMinimumWidth ( sizeHorizon );
  ((QWidget *) input)->setMaximumWidth ( sizeHorizon );
}

// Вернуть указатель на объект
void * POne_Field::inputObject( void ) const
{
  return input;
}
// Вернуть тип объекта
POne_Field::Type POne_Field::typeObject( void ) const
{
  return type;
}
