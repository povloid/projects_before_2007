/*
    ow_prod_phos_pump_dialog.h
*/

#ifndef OW_PROD_PHOS_PUMP_DIALOG_H
#define OW_PROD_PHOS_PUMP_DIALOG_H

#include "pdialog.h"

class QLineEdit;
class QString;
class QDoubleSpinBox;

class OW_Prod_Phos_Pump_Dialog : public PDialog
{
    Q_OBJECT

public:
    OW_Prod_Phos_Pump_Dialog(QWidget *parent = 0 );

    void setValuePhos( const double  & w , const int i = 0 );
    void setValueSludge( const double  & w , const int i = 0);

    double valuePhos( const int i = 0 ) const;
    double valueSludge( const int i = 0 ) const;


private:
    QDoubleSpinBox *valueEditPhos[4],
      *valueEditSludge[4];
};

#endif
