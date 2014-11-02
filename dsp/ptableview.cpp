/*
    ptableview.cpp
*/

#include <QEvent>
#include <QKeyEvent>
#include "ptableview.h"

// Для отладки
//#include <iostream>

PTableView::PTableView(QWidget *parent)
    : QTableView(parent)
{
}
void PTableView::keyPressEvent(QKeyEvent *event)
{
  
  switch ( event->key() ){
  case Qt::Key_F2:		// Добавить запись
    {
      emit addRec();
      break;
    }
  case 16777220:		// Нормальный Enter
  case Qt::Key_Enter:
  case Qt::Key_F4:		// Редактировать запись
    {
      emit editRec();
      break;
    }
  case Qt::Key_F8:		// Удалить запись
    {
      emit delRec();
      break;
    }
  default:
    QTableView::keyPressEvent(event);
  }
}

bool PTableView::event(QEvent *event) 
{ 
  if (event->type() == QEvent::KeyPress) { 
    QKeyEvent *keyEvent = (QKeyEvent *)event;

    // Для отладки
    //std::cout<<keyEvent->key()<<std::endl;

    switch (keyEvent->key()) {
    case Qt::Key_Insert:
      { 
	emit addRec();
	return true;
      }
    case Qt::Key_Delete:
      { 
	emit delRec();
	return true;
      }
    }
  }
  return QTableView::event(event); 
}
