/*
    ow_prod_phos_addedit_dialog.cpp
*/

#include <QVBoxLayout>

#include "ow_prod_phos_addedit_dialog.h"
#include "ow_prod_phos_tablo.h"

OW_Prod_Phos_AddEdit_Dialog::OW_Prod_Phos_AddEdit_Dialog(QWidget *parent)
    : PDialog(parent)
{
  // Менеджер рабочего пространства ---
  QVBoxLayout* workLayout = new QVBoxLayout;
  iputObjectsLayout->addLayout( workLayout );
  // ..
   
  ow_prod_phos_tablo = new OW_Prod_Phos_Tablo( NULL , true );
  workLayout->addWidget( ow_prod_phos_tablo );

  ow_prod_phos_tablo->setFocus();
}
