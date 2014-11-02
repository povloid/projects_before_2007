/*
    ps_bal_dbtable.h
*/

#ifndef S_BAL_DBTABLE_H
#define S_BAL_DBTABLE_H

#include "pabstractdbtable.h"

class POne_Field;
class QLineEdit;
class QSpinBox;

class S_BAL_DBTable : public PAbstractDBTable
{
    Q_OBJECT

public:
    S_BAL_DBTable(QWidget *parent = 0);

private:

    
    QSpinBox* iobj_KOD;
    QLineEdit* iobj_NAME;


protected:
    virtual bool virtualAddRec ( void );
    virtual bool virtualEditRec ( int& );
    virtual bool virtualDelRec ( int& );

};

#endif
