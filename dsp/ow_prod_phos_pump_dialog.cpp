/*
    ow_prod_phos_pump_dialog.cpp
*/

#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>

#include "ow_prod_phos_pump_dialog.h"

OW_Prod_Phos_Pump_Dialog::OW_Prod_Phos_Pump_Dialog(QWidget *parent)
    : PDialog(parent)
{
  // Менеджер рабочего пространства ---
  QVBoxLayout* workLayout = new QVBoxLayout;
  workLayout->setMargin(2);

  iputObjectsLayout->addLayout( workLayout );
  // ..

  for ( int i = 0; i < 4 ; i++ )
    {
        QHBoxLayout* lineLayout = new QHBoxLayout;
	lineLayout->addSpacing ( 50 );

	// Фосфор по печи
	QLabel *label = new QLabel( tr("Фосфор по ")
				       + QString::number( i + 5 ) 
				       + tr("-Печи:"));
	lineLayout->addWidget( label );

	valueEditPhos[i] = new QDoubleSpinBox;
	valueEditPhos[i]->setRange(0.0, 10000.0);
	valueEditPhos[i]->setSingleStep(10.0);
	valueEditPhos[i]->setSuffix(tr(" тн."));
	valueEditPhos[i]->setValue(0.0);
	valueEditPhos[i]->setAlignment(Qt::AlignRight);

	lineLayout->addWidget( valueEditPhos[i] );

	lineLayout->addStretch();
	// Шлам по печи
	label = new QLabel( tr("Шлам по ")
				       + QString::number( i + 5 ) 
				       + tr("-Печи:"));
	lineLayout->addWidget( label );

	valueEditSludge[i] = new QDoubleSpinBox;
	valueEditSludge[i]->setRange(0.0, 10000.0);
	valueEditSludge[i]->setSingleStep(10.0);
	valueEditSludge[i]->setSuffix(tr(" тн."));
	valueEditSludge[i]->setValue(0.0);
	valueEditSludge[i]->setAlignment(Qt::AlignRight);

	lineLayout->addWidget( valueEditSludge[i] );
	lineLayout->addSpacing ( 50 );
	
	workLayout->addLayout( lineLayout );
    }
}

// Задать текст в поле ввода перекачки ФОСФОРА i-того элемента
void OW_Prod_Phos_Pump_Dialog::setValuePhos( const double  & w , const int i )
{
  valueEditPhos[i]->setValue( w );
}
// Задать текст в поле ввода перекачки ШЛАМА i-того элемента
void OW_Prod_Phos_Pump_Dialog::setValueSludge( const double  & w , const int i )
{
  valueEditSludge[i]->setValue( w );
}
// Получить текст поля ввода перекачки ФОСФОРА i-того элемента
double OW_Prod_Phos_Pump_Dialog::valuePhos( const int i ) const
{
  return valueEditPhos[i]->value();
}
// Получить текст поля ввода перекачки ШЛАМА i-того элемента
double OW_Prod_Phos_Pump_Dialog::valueSludge( const int i ) const
{
  return valueEditSludge[i]->value();
}
