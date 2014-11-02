/*
    ow_prod_phos_addedit_dialog.h
*/

#ifndef OW_PROD_PHOS_ADDEDIT_DIALOG_H
#define OW_PROD_PHOS_ADDEDIT_DIALOG_H

#include "pdialog.h"

class OW_Prod_Phos_Tablo;

class OW_Prod_Phos_AddEdit_Dialog : public PDialog
{
    Q_OBJECT

public:
    OW_Prod_Phos_AddEdit_Dialog(QWidget *parent = 0);

    OW_Prod_Phos_Tablo *ow_prod_phos_tablo;
};

#endif
