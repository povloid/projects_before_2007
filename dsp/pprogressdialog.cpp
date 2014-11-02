/*
    pprogressdialog.cpp
*/

#include "pprogressdialog.h"

#include<QLabel>
#include<QProgressBar>
#include<QVBoxLayout>
#include<QApplication>

PProgressDialog::PProgressDialog( QDialog *parent )
    : QDialog(parent)
{
  setWindowTitle( tr("Подождите, выполняется работа приложения") );
  createProgressDialog();
}

PProgressDialog::PProgressDialog( const QString & title , QDialog *parent )
    : QDialog(parent)
{
  setWindowTitle( title );
  createProgressDialog();
}

void PProgressDialog::createProgressDialog( void )
{
  setModal( true );
  
  QVBoxLayout *mainLayout = new QVBoxLayout;
  setLayout( mainLayout );

  label = new QLabel;
  mainLayout->addWidget( label );
  
  progressBar = new QProgressBar;
  progressBar->setMinimum( 0 );
  progressBar->setMaximum( 100 );

  mainLayout->addWidget( progressBar );

}

void PProgressDialog::setMinimum( const int & minimum )
{
  progressBar->setMinimum( minimum );
}

void PProgressDialog::setMaximum( const int & maximum )
{
  progressBar->setMaximum( maximum );
}

void PProgressDialog::setProgress( const int & progress)
{
  progressBar->setValue( progress );

  QApplication::processEvents();
}

void PProgressDialog::setProgress( const int & progress , const QString & str )
{
  progressBar->setValue( progress );
  label->setText( str );

  QApplication::processEvents();
}
