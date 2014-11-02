/*
    ps_potr_dbtable.h
*/

#ifndef S_POTR_DBTABLE_H
#define S_POTR_DBTABLE_H

#include "pabstractdbtable.h"

class POne_Field;
class QLineEdit;
class QComboBox;
class QSpinBox;

class S_POTR_DBTable : public PAbstractDBTable
{
    Q_OBJECT

public:
    S_POTR_DBTable(QWidget *parent = 0);

private:

    
  QSpinBox* iobj_KOD;
  QLineEdit* iobj_NAME;

protected:
    virtual bool virtualAddRec ( void );
    virtual bool virtualEditRec ( int& );
    virtual bool virtualDelRec ( int& );

};

#endif
