/*
    example.h
*/

#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <QDialog>

class QDate;
class PDatePicker;
class QPushButton;

class Example : public QDialog
{
    Q_OBJECT

public:
    Example(QWidget *parent = 0);

private slots:
  void setDate( const QDate & date );

 void setButtonText(); 

 private:
 QPushButton *button;

 PDatePicker *pDatePicker1; 
 PDatePicker *pDatePicker2; 
 PDatePicker *pDatePicker3; 

};

#endif
