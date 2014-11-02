/*
  ow_producing.h
*/

#ifndef OW_PRODUCING_H
#define OW_PRODUCING_H

#include <QWidget>
#include <QSqlDatabase>
#include <QDate>

class PDialog;
class PRecDialog;
class PTableView;

class QDate;
class QLabel;
class QSqlDatabase;
class QSqlQueryModel;
class QString;
class QLocale;
class QDoubleSpinBox;


class OW_Producing : public QWidget
{
  Q_OBJECT

 public:
  OW_Producing( QWidget *parent = 0 );

 private:
    
  QString selectSQL;
  QDate owDate;

  QSqlDatabase db;
  QSqlQueryModel *model;
  PTableView *view;
  PRecDialog *precdialog;

  QLabel *dateLabel;

  QLabel *iobj_NAME;
  QDoubleSpinBox *iobj_S_DAY_TODAY;
  QDoubleSpinBox *iobj_S_NOT_TODAY;
  QLabel *iobj_S_DAY_MONTH;

  QLocale *locale;


  void modelUpdate( void );

 private slots:
  void editRec();
  void recount( const QString & str );

 public slots:
  void setOWDate( const QDate & date );
  void update ( void );

};

#endif
