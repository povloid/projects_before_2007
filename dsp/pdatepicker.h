/*
 *   pdatepicker.h
 *
 *   This file is part of the class - PDatePicker.
 *   PDatePicker is a class of the calendar widget.
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
 *  Authors:   Pavel Kopychenko pacmanoid <kpovlo@mail.ru>
 *  Date: 2006 year.
 */

/* -------------------------------------------------
 *  Main functions:
 *
 * signals: 
 *     void dateChanged( const QDate & dateTemp ); 
 *  
 *  public:
 *     QDate & date( void ) const;
 *  
 *  public slots: 
 *     void setDate( const QDate & dateTemp ); 
 *     void gotoCurrentDate( void ); 
 *     void setViewOnly( bool ); 
 * -------------------------------------------------
 */

#ifndef PDATEPICKER_H
#define PDATEPICKER_H

#include <QLabel>
#include <QTableWidget>

class QDate;
class QPalette;
class QPushButton;
class QToolButton;
class QSpinBox;
class QFarme;
class QPalette;

class PMuffledObject;

const int VSize = 19;
const int HSize = 22;

// cellLabel
class PCellLabel : public QLabel
{
    Q_OBJECT

 public:
  PCellLabel(QWidget *parent = 0 );

  void setDate( const QDate & dateTemp , const QDate & calDate );
  QDate & date( void ) const;

  void setPFrameStyle( const int & frameStyle );

 private:

  QDate * pCellDate;

  int pFrameStyle;

  QPalette* pCellPalette;
  bool event ( QEvent * event );
};


// weekLabels
class PWeekLabel : public QLabel
{
    Q_OBJECT

public:
    PWeekLabel( const int & vSize , const int & hSize , QWidget *parent = 0 );
};


// capOnTable
class PDateCapOnTable : public QLabel
{
    Q_OBJECT

public:
    PDateCapOnTable( const int & hSize , QWidget *parent = 0 );
};

// table
class PDateTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    PDateTableWidget( const int & vSize , const int & hSize , QWidget *parent = 0 );

 private:
    void keyPressEvent(QKeyEvent *event);

signals:
    void gotoCurrentDate();
    void lastMonth();
    void nextMonth();
    void selectYear();
    void selectMonth();

};

// main widget -----------------------------------------------------------------------
class PDatePicker : public QFrame
{
    Q_OBJECT

 public:
    PDatePicker( int vSize = VSize , int hSize = HSize , QWidget *parent = 0 );

 protected:

    PDateTableWidget *table;

 private:
    QDate * calDate;

    PWeekLabel * weekLabels[6];
    PCellLabel * cellLabels[42];

    QFrame *spBoxYearFrame;
    QPushButton *bttnMonth;
    QPushButton *bttnYear;
    QSpinBox *spBoxYear;
    QToolButton *bttnGotoCurrentDate;
    QToolButton *bttnHelp;
    QToolButton *bttnLastMonth;
    QToolButton *bttnNextMonth;

    PMuffledObject *pMuffledObject;

 signals:
    void dateChanged( const QDate & dateTemp );
    
 public:
    QDate & date( void ) const;

 public slots:
    void setDate( const QDate & dateTemp );
    void gotoCurrentDate( void );
    void setViewOnly( bool );

 private slots:
   void currentCellChanged ( int currentRow, int currentColumn, 
			     int previousRow, int previousColumn );
   void lastMonth( void );
   void nextMonth( void );
   void showSpBoxYearFrame( void );
   void hideSpBoxYearFrame( void );
   void triggeredMonth ( QAction * action );
   void showHelp( void );
};

#endif
