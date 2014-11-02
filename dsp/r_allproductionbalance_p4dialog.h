/*
    r_allproductionbalance_p4dialog.h
*/

#ifndef R_ALLPRODUCTIONBALANCE_P4DIALOG_H
#define R_ALLPRODUCTIONBALANCE_P4DIALOG_H

#include "pdialog.h"

#include <QLabel>
#include <QDoubleSpinBox>
#include <QRadioButton>

class QLabel;
class QDoubleSpinBox;

class R_AllProductionBalance_P4Dialog : public PDialog
{
    Q_OBJECT

public:
    R_AllProductionBalance_P4Dialog(QWidget *parent = 0);

  //QLabel* labelOnOldDate;
  //QDoubleSpinBox *p4OnOldDate;

  //QDoubleSpinBox *p4_in_Depot;
  QDoubleSpinBox *p4_in_Slude;

  QRadioButton *bttnFerroK;
  QDoubleSpinBox *ferroK;
  QRadioButton *bttnFerroProd;
  QDoubleSpinBox *ferroProd;

  QDoubleSpinBox *massPshTFKToInwardTPFN;
  QDoubleSpinBox *massPshTFKToInwardPrTPFN;
};

#endif
