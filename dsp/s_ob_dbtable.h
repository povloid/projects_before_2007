/*
    ps_ob_dbtable.h
*/

#ifndef S_OB_DBTABLE_H
#define S_OB_DBTABLE_H

#include "pabstractdbtable.h"

class POne_Field;
class QLineEdit;
class QComboBox;
class QSpinBox;

class S_OB_DBTable : public PAbstractDBTable
{
    Q_OBJECT

public:
    S_OB_DBTable(QWidget *parent = 0);

private:
    
    QSpinBox* iobj_KOD;
    QLineEdit* iobj_NAME;
    QComboBox* iobj_DEP;

    // Создаем SQL модель для ComboBox
    QSqlQueryModel *model_in_ComboBox;

protected:

    virtual bool virtualAddRec ( void );
    virtual bool virtualEditRec ( int& );
    virtual bool virtualDelRec ( int& );

};

#endif
