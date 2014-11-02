/*
    pdialog.h
*/

#ifndef PDIALOG_H
#define PDIALOG_H

#include <QDialog>
#include <QLayout>

class QLabel;
class QPushButton;

class PDialog : public QDialog
{
    Q_OBJECT

public:
    PDialog( QWidget * parent = 0, Qt::WFlags f = 0 );

    void setCapPixmap ( QString strPixmapName );
    void setCapText ( QString strText );

protected:
    QHBoxLayout * iputObjectsLayout;

private:
  QLabel *imageLabelCapPixmap;
  QLabel *labelCapText;

  QPushButton *okButton;

protected slots:
  void accept();
  

};

#endif
