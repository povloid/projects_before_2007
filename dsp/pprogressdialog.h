/*
    pprogressdialog.h
*/

#ifndef PPROGRESSDIALOG_H
#define PPROGRESSDIALOG_H

#include <QDialog>

class QProgressBar;
class QLabel;

class PProgressDialog : public QDialog
{
    Q_OBJECT

public:
  PProgressDialog( QDialog *parent = 0 );
  PProgressDialog( const QString & title , QDialog *parent = 0 );

  void setProgress( const int & progress , const QString & str );
  void setProgress( const int & progress );

  void setMinimum( const int & minimum );
  void setMaximum( const int & maximum );

private:
  void createProgressDialog( void );

  QLabel *label;
  QProgressBar *progressBar;

};

#endif
