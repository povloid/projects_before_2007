/*
    ps_vyr_dbtable.h
*/

#ifndef S_VYR_DBTABLE_H
#define S_VYR_DBTABLE_H

#include "pabstractdbtable.h"

class POne_Field;
class QLineEdit;
class QComboBox;
class QSpinBox;

class S_VYR_DBTable : public PAbstractDBTable
{
    Q_OBJECT

public:
    S_VYR_DBTable(QWidget *parent = 0);

private:

    QSpinBox* iobj_KOD;
    QLineEdit* iobj_NAME;
    QComboBox* iobj_ED_MASS;
    QComboBox* iobj_KOD_DEP;

    // Создаем SQL модель для ComboBox
    QSqlQueryModel *model_in_ComboBox_ED_MASS;
    QSqlQueryModel *model_in_ComboBox_KOD_DEP;


protected:

    virtual bool virtualAddRec ( void );
    virtual bool virtualEditRec ( int& );
    virtual bool virtualDelRec ( int& );

};

#endif
