/*
    pone_field.h
*/

#ifndef PONE_FIELD_H
#define PONE_FIELD_H

#include <QWidget>

class QLabel;
class QlineEdit;
class QComboBox;
class QSpinBox;
class QDoubleSpinBox;

class POne_Field : public QWidget
{
  Q_OBJECT

public:
  /* Данное перечисление описывает тип создаваемого компанента ввода */
  enum Type {
    Label,
    LineEdit,
    ComboBox,
    SpinBox,
    DoubleSpinBox
  };

public:
    POne_Field(QWidget *parent = 0 , QString strCaption = tr("Заголовок") ,
	       Type type_tmp = Type(LineEdit) , int sizeHorizon = 100 );

    void* inputObject( void ) const;
    Type typeObject ( void ) const;

private:
    /* Данное перечисление описывает тип создаваемого компанента ввода */
    Type type;

    QLabel *caption;		/* Указатель на компонент
				   выводящий надпись над полем
				   ввода */
    void *input;		/* Указатель на компонент ввода */
};

#endif
