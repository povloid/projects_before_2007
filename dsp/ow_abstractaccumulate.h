/*
    ow_abstractaccumulate.h
*/

#ifndef OW_ABSTRACTACCUMULATE_H
#define OW_ABSTRACTACCUMULATE_H

#include <QDate>
#include "pabstractdbtable.h"

class QTableView;
class QGridLayout;

class OW_AbstractAccumulate : public PAbstractDBTable
{
    Q_OBJECT

 public:
    OW_AbstractAccumulate(QWidget *parent = 0);

 public slots:
   void setOWDate( const QDate & date );

 protected:
    QDate owDate;

    virtual void setupLayouts( void );

    QTableView *mainView;
    QSqlQueryModel *mainModel;
    QString selectMainSQL;

    QGridLayout * tabloLayout;
    QLabel *tabloCapLabel;
    QLabel *tabloDateLabel;

    QFont *boldFont;
    QPalette *capPalette;

};

#endif
