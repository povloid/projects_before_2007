/*
    pabstractraport.h
*/

#ifndef PABSTRACTRAPORT_H
#define PABSTRACTRAPORT_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>

class QLabel;
class QTextEdit;
class QVBoxLayout;

class PAbstractRaport : public QWidget
{
    Q_OBJECT

 public:
    PAbstractRaport(QWidget *parent = 0 );

 public slots:
   void setOWDate( const QDate & date );

 protected:

  QVBoxLayout *mainLayout;
  
  QDate owDate;
  
  QSqlDatabase db;
  
  QLabel *capLabel;
  QTextEdit *textEdit;
  
  virtual void update() = 0;

  void recalculate(void);

};

#endif
