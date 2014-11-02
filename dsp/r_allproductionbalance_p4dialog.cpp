/*
    r_allproductionbalance_p4dialog.cpp
*/

#include <QVBoxLayout>
#include <QHBoxLayout>

#include "r_allproductionbalance_p4dialog.h"


R_AllProductionBalance_P4Dialog::R_AllProductionBalance_P4Dialog(QWidget *parent)
    : PDialog(parent)
{
  // Менеджер рабочего пространства ---
  QVBoxLayout* workLayout = new QVBoxLayout;
  workLayout->setMargin(8);

  iputObjectsLayout->addLayout( workLayout );
  // ..

  QHBoxLayout *lineLayout = new QHBoxLayout;

//   labelOnOldDate = new QLabel;
//   p4OnOldDate = new QDoubleSpinBox;
//   p4OnOldDate->setRange(0.0, 100000.0);
//   p4OnOldDate->setSingleStep(1.0);
//   p4OnOldDate->setSuffix(tr(" тн"));
//   p4OnOldDate->setValue(0.0);
//   p4OnOldDate->setAlignment(Qt::AlignRight);
//   lineLayout->addWidget( labelOnOldDate );
//   lineLayout->addStretch();
//   lineLayout->addWidget( p4OnOldDate );
//   workLayout->addLayout( lineLayout );

//   lineLayout = new QHBoxLayout;
//   QLabel* label = new QLabel( tr("Остаток фосфора"));
//   p4_in_Depot = new QDoubleSpinBox; 
//   p4_in_Depot->setRange(0.0, 100000.0);
//   p4_in_Depot->setSingleStep(1.0);
//   p4_in_Depot->setSuffix(tr(" тн"));
//   p4_in_Depot->setValue(0.0);
//   p4_in_Depot->setAlignment(Qt::AlignRight);
//   lineLayout->addWidget( label );
//   lineLayout->addStretch();
//   lineLayout->addWidget( p4_in_Depot );
//   workLayout->addLayout( lineLayout );

  QFont font;
  font.setBold(true);

  QLabel *label = new QLabel;
  label->setFont( font );
  label->setText(tr("Фосфор:"));
  workLayout->addWidget( label );

  lineLayout = new QHBoxLayout;
  label = new QLabel( tr("Остаток фосфора в шламе"));
  p4_in_Slude = new QDoubleSpinBox;
  p4_in_Slude->setRange(0.0, 100000.0);
  p4_in_Slude->setSingleStep(1.0);
  p4_in_Slude->setSuffix(tr(" тн"));
  p4_in_Slude->setValue(390.0);
  p4_in_Slude->setAlignment(Qt::AlignRight);
  lineLayout->addWidget( label );
  lineLayout->addStretch();
  lineLayout->addWidget( p4_in_Slude );

  workLayout->addLayout( lineLayout );

  // Разделитель >-------<
  QFrame *line = new QFrame;	// --------------------------
  line->setFrameShape(QFrame::HLine);
  workLayout->addWidget( line );

  label = new QLabel;
  label->setFont( font );
  label->setText(tr("Пищевая ТФК:"));
  workLayout->addWidget( label );

  lineLayout = new QHBoxLayout;
  label = new QLabel( tr("На внут. потреб. ТПФП"));
  massPshTFKToInwardTPFN = new QDoubleSpinBox;
  massPshTFKToInwardTPFN->setRange(0.0, 100000.0);
  massPshTFKToInwardTPFN->setSingleStep(1.0);
  massPshTFKToInwardTPFN->setSuffix(tr(" тн"));
  massPshTFKToInwardTPFN->setValue(0);
  massPshTFKToInwardTPFN->setAlignment(Qt::AlignRight);
  lineLayout->addWidget( label );
  lineLayout->addStretch();
  lineLayout->addWidget( massPshTFKToInwardTPFN );

  workLayout->addLayout( lineLayout );

  lineLayout = new QHBoxLayout;
  label = new QLabel( tr("На внут. потреб. ТПФН простой"));
  massPshTFKToInwardPrTPFN = new QDoubleSpinBox;
  massPshTFKToInwardPrTPFN->setRange(0.0, 100000.0);
  massPshTFKToInwardPrTPFN->setSingleStep(1.0);
  massPshTFKToInwardPrTPFN->setSuffix(tr(" тн"));
  massPshTFKToInwardPrTPFN->setValue(0);
  massPshTFKToInwardPrTPFN->setAlignment(Qt::AlignRight);
  lineLayout->addWidget( label );
  lineLayout->addStretch();
  lineLayout->addWidget( massPshTFKToInwardPrTPFN );

  workLayout->addLayout( lineLayout );


  line = new QFrame;	// --------------------------
  line->setFrameShape(QFrame::HLine);
  workLayout->addWidget( line );
  
  label = new QLabel;
  label->setFont( font );
  label->setText(tr("Феррофосфор:"));
  workLayout->addWidget( label );

  lineLayout = new QHBoxLayout;
  bttnFerroK = new QRadioButton;
  lineLayout->addWidget( bttnFerroK );
  label = new QLabel( tr("Коэффициент от производства фосфора"));
  ferroK = new QDoubleSpinBox;
  ferroK->setRange(0.0, 10.0);
  ferroK->setSingleStep(0.01);
  ferroK->setValue(0.1);
  ferroK->setAlignment(Qt::AlignRight);
  lineLayout->addWidget( label );
  lineLayout->addStretch();
  lineLayout->addWidget( ferroK );

  connect(bttnFerroK, SIGNAL(toggled( bool )), label, SLOT(setEnabled( bool )));
  connect(bttnFerroK, SIGNAL(toggled( bool )), ferroK, SLOT(setEnabled( bool )));

  workLayout->addLayout( lineLayout );
  
  lineLayout = new QHBoxLayout;
  bttnFerroProd = new QRadioButton;
  lineLayout->addWidget( bttnFerroProd );
  label = new QLabel( tr("Производство феррофосфора"));
  ferroProd = new QDoubleSpinBox;
  ferroProd->setRange(0.0, 1000000.0);
  ferroProd->setSingleStep(1.0);
  ferroProd->setSuffix(tr(" тн"));
  ferroProd->setValue(310.0);
  ferroProd->setAlignment(Qt::AlignRight);
  lineLayout->addWidget( label );
  lineLayout->addStretch();
  lineLayout->addWidget( ferroProd );

  connect(bttnFerroProd, SIGNAL(toggled( bool )), label, SLOT(setEnabled( bool )));
  connect(bttnFerroProd, SIGNAL(toggled( bool )), ferroProd, SLOT(setEnabled( bool )));

  workLayout->addLayout( lineLayout );

  bttnFerroK->toggle();

  label->setEnabled( false );
  ferroProd->setEnabled( false );

}
