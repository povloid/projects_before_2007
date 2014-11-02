/*
    s_syr_dbtable.h
*/

#ifndef S_SYR_DBTABLE_H
#define S_SYR_DBTABLE_H

#include "pabstractdbtable.h"

class POne_Field;
class QLineEdit;
class QComboBox;
class QSpinBox;
/* Группа --------------------------------------------------------------------------------- */
class S_SYR_GR_DBTable : public PAbstractDBTable
{
    Q_OBJECT

public:

    S_SYR_GR_DBTable(QWidget *parent = 0);
    PTableView *ptrView;

private:
    
    QLineEdit* iobj_KOD;
    QLineEdit* iobj_NAME;
    QLineEdit* iobj_K;

protected:
    virtual bool virtualAddRec ( void );
    virtual bool virtualEditRec ( int& );
    virtual bool virtualDelRec ( int& );

};
/* Группа ................................................................................. */
/* Подгруппа ------------------------------------------------------------------------------ */
class S_SYR_PGR_DBTable : public PAbstractDBTable
{
    Q_OBJECT

public:
    S_SYR_PGR_DBTable(QWidget *parent = 0);

private:
    QString strKODM;

    QLineEdit* iobj_KOD;
    QLineEdit* iobj_NAME;
    QLineEdit* iobj_K;

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

class S_SYR_DBTable : public QWidget
{
    Q_OBJECT

public:
    S_SYR_DBTable(QWidget *parent = 0);

private:
    S_SYR_GR_DBTable* s_syr_gr_dbtable;
    S_SYR_PGR_DBTable* s_syr_pgr_dbtable;

    QSplitter* vert_splitter;
};

/* ... */
#endif
