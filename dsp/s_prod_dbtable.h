/*
    s_prod_dbtable.h
*/

#ifndef S_PROD_DBTABLE_H
#define S_PROD_DBTABLE_H

#include "pabstractdbtable.h"

class POne_Field;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
class QSqlQueryModel;

/* Группа --------------------------------------------------------------------------------- */
class S_PROD_GR_DBTable : public PAbstractDBTable
{
    Q_OBJECT

public:

    S_PROD_GR_DBTable(QWidget *parent = 0);
    PTableView *ptrView;

private:
    
    QSpinBox* iobj_KOD;
    QLineEdit* iobj_NAME;
    QDoubleSpinBox* iobj_K;
    QComboBox* iobj_ED_MASS;

    // Создаем SQL модель для ComboBox
    QSqlQueryModel *model_in_ComboBox;

protected:
    virtual bool virtualAddRec ( void );
    virtual bool virtualEditRec ( int& );
    virtual bool virtualDelRec ( int& );

};
/* Группа ................................................................................. */
/* Подгруппа ------------------------------------------------------------------------------ */
class S_PROD_PGR_DBTable : public PAbstractDBTable
{
    Q_OBJECT

public:
    S_PROD_PGR_DBTable(QWidget *parent = 0);

private:
    QString strKODM;

    QLineEdit* iobj_NAME;
    QDoubleSpinBox* iobj_K;
    QComboBox* iobj_KOD_DEP;

    // Создаем SQL модель для ComboBox
    QSqlQueryModel *model_in_ComboBox_KOD_DEP;


public slots:

void update_PGR( const QModelIndex & index );

protected:
    virtual bool virtualAddRec ( void );
    virtual bool virtualEditRec ( int& );
    virtual bool virtualDelRec ( int& );

};
/* Подгруппа .............................................................................. */
/* Объединение в виджет */
class QSplitter;

class S_PROD_DBTable : public QWidget
{
    Q_OBJECT

public:
    S_PROD_DBTable(QWidget *parent = 0);

private:
    S_PROD_GR_DBTable* s_prod_gr_dbtable;
    S_PROD_PGR_DBTable* s_prod_pgr_dbtable;

    QSplitter* vert_splitter;
};

/* ... */
#endif
