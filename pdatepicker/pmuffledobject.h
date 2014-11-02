/*
 *  pmuffledobject.h
 *
 *   This file is part of the class - MuffledObject.
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
 *  Authors:   Pavel Kopychenko pacmanoid <kpovlo@mail.ru>
 *  Date: 2006 year.
 *
 *  ����� MuffledObject - ���� �������� ������� ���������� � ����
 */

#ifndef PMUFFLEDOBJECT_H
#define PMUFFLEDOBJECT_H

#include <QEvent>
#include <QObject>

// �������� �������
class PMuffledObject: public QObject
{
  Q_OBJECT

 public:
  PMuffledObject ( QObject * parent = 0 )
    :QObject( parent )
    {
    }

  bool eventFilter(QObject *obj, QEvent *event)
    {
      switch (event->type()) 
	{
	case QEvent::KeyPress:
	case QEvent::MouseButtonPress:
	case QEvent::MouseButtonRelease:
	case QEvent::MouseButtonDblClick:
	case QEvent::MouseMove:
	case QEvent::MouseTrackingChange:
	  return true;
	default:
	  // standard event processing
	  return QObject::eventFilter(obj, event);
	}
    }
};

#endif
