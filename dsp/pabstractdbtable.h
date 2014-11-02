/*
    pabstractdbtable.h
*/

#ifndef PABSTRACTDBTABLE_H
#define PABSTRACTDBTABLE_H

#include <QLabel>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QLayout>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QWidget>
#include <QPushButton>
#include <QLocale>

#include "precdialog.h"
#include "ptableview.h"
#include "p_ru_itemdelegate.h"

#include "dspconst.h"

class PRecDialog;
class QLabel;
class QSqlQueryModel;
class PTableView;
class PDialog;
class QSqlDatabase;
class QPushButton;

class QLocale;
class P_RU_ItemDelegate;

/* class PAbstractDBTableSetupLayouts */
/* { */
/*  protected: */
/*   virtual void setupLayouts( void ) = 0; */
/* }; */

class PAbstractDBTable : public QWidget //, protected PAbstractDBTableSetupLayouts
{
  Q_OBJECT

 public:
  PAbstractDBTable( QWidget * parent = 0 );

 protected:

  QSqlDatabase db;
  
  QSqlQueryModel *model;
  PTableView *view;
  P_RU_ItemDelegate *p_ru_ItemDelegate;
  QLocale *locale;
  QLabel *captionViewLabel;
  PRecDialog *precdialog;

  QPushButton *bttnAddRec;
  QPushButton *bttnEditRec;
  QPushButton *bttnDelRec;

 private slots:
  void addRec();
  void editRec();
  void delRec();
 
 protected:
  virtual bool virtualAddRec ( void ) = 0;
  virtual bool virtualEditRec ( int& ) = 0;
  virtual bool virtualDelRec ( int& ) = 0;

  virtual void setupLayouts( void );

  QString selectSQL;
  QString strKOD;

 private:
  void goto_past_EditORNewRec( void );

};

#endif
