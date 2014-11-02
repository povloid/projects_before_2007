/*
    precdialog.cpp
*/
#include <QPushButton>
#include "precdialog.h"

PRecDialog::PRecDialog(QWidget *parent)
    : PDialog(parent)
{
  
}

//  Добавить один элемент ввода
//
// Пример вызова функции:
// addInputObject( tr("Привет") , POne_Field::Type(POne_Field::LineEdit) , 40 );
// addInputObject( tr("Сало") , POne_Field::Type(POne_Field::ComboBox) , 100 );
// addInputObject( tr("Украина") , POne_Field::Type(POne_Field::SpinBox) , 150 );

POne_Field* PRecDialog::addInputObject( QString strCaption , POne_Field::Type type_tmp , 
				     int sizeHorizon ) const
{
  POne_Field * pone_Field = new POne_Field( NULL , 
					    strCaption, type_tmp, sizeHorizon );
  
  PDialog::iputObjectsLayout->addWidget( pone_Field );

  return pone_Field;
}
