/*
    ptableview.h
*/

#ifndef PTABLEVIEW_H
#define PTABLEVIEW_H

#include <QTableView>

class PTableView : public QTableView
{
    Q_OBJECT

public:
  PTableView(QWidget *parent = 0 );

private:
  void keyPressEvent(QKeyEvent *event);
  bool event(QEvent *event);

signals:
  
  void addRec(void);
  void editRec(void);
  void delRec(void);
    

};

#endif
