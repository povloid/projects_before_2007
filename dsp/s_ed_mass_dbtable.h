/*
  s_ed_mass_dbtable.h
*/

#ifndef S_ED_MASS_DBTABLE_H
#define S_ED_MASS_DBTABLE_H

#include "pabstractdbtable.h"

class POne_Field;
class QLineEdit;
class QComboBox;
class QSpinBox;

class S_ED_MASS_DBTable : public PAbstractDBTable
{
    Q_OBJECT

public:
    S_ED_MASS_DBTable(QWidget *parent = 0);

private:

    
    QLineEdit* iobj_ED;
    QLineEdit* iobj_FULL_NAME;

protected:
    virtual bool virtualAddRec ( void );
    virtual bool virtualEditRec ( int& );
    virtual bool virtualDelRec ( int& );

};

#endif
