/*
    ow_prod_phos.h
*/

#ifndef OW_PROD_PHOS_H
#define OW_PROD_PHOS_H

#include <QWidget>
#include <QSqlDatabase>
//#include <QModelIndex>

class QLabel;
class QTableView;
class OW_Prod_Phos_Tablo;
class QPushButton;

class QSqlQueryModel;
class QSqlDatabase;
class QSqlRecord;
class QModelIndex;
class QDate;

class OW_Prod_Phos : public QWidget
{
    Q_OBJECT

public:
    OW_Prod_Phos( QWidget *parent = 0 );

 private:

    QLabel *owCap;
    QLabel *dateLabel;

    QTableView *view;
    QSqlDatabase db;
    QString selectSQL;
    QSqlQueryModel *model;

    OW_Prod_Phos_Tablo *ow_prod_phos_tablo;

    QPushButton *pBtGotoCurrentDate;
    QPushButton *pBtK;
    QPushButton *pBtAddEndEdit;
    QPushButton *pBtAddEndEditPumping;
    QPushButton *pBtEditMonth;

public slots:
  void setOWDate( const QDate & date );

private slots:
  void addEdit( void );
  void addEditPumping( void );

  void update_ow_prod_phos_tablo( const QModelIndex & index );

private:
  void calc_manth_SUM( OW_Prod_Phos_Tablo * const tablo );

};

#endif
