/*
    p_ru_itemdelegate.cpp
*/
#include <QLocale>
#include <QModelIndex>
#include <QDate>
#include "p_ru_itemdelegate.h"

P_RU_ItemDelegate::P_RU_ItemDelegate(QWidget *parent)
    : QItemDelegate( parent )
{
  locale = new QLocale( QLocale::Russian, QLocale::RussianFederation );
}

void P_RU_ItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, 
			      const QModelIndex& index) const 
{ 
  switch ( index.data().type() ) 
    {
 
    case QVariant::Date:
    case QVariant::DateTime: 
      { 
	QStyleOptionViewItem opt = option; 
	QDate dt = index.data().toDate(); 
	QString s = dt.toString("dd.MM.yyyy"); 
	QVariant value = index.data(Qt::TextColorRole); 

	//if (value.isValid() && qvariant_cast<QColor>(value).isValid()) 
	//  opt.palette.setColor(QPalette::Text, qvariant_cast<QColor>(value)); 
	opt.palette.setColor( QPalette::Text, Qt::blue ); 
	opt.font.setBold( true );

	opt.displayAlignment = Qt::AlignVCenter | Qt::AlignHCenter; 
	drawDisplay(painter, opt, opt.rect, s); 
	drawFocus(painter, opt, opt.rect); 
	return; 
      } 
    case QVariant::Int:
    case QVariant::Double:
      { 
	QStyleOptionViewItem opt = option; 
	double dt = index.data().toDouble(); 

	QString s;
	
	if( dt >= 1000000 )
	  s = locale->toString(dt ,'f', 2);
	else
	  s = locale->toString(dt);	

	QVariant value = index.data(Qt::TextColorRole); 
	//if (value.isValid() && qvariant_cast<QColor>(value).isValid()) 
	//  opt.palette.setColor(QPalette::Text, qvariant_cast<QColor>(value)); 
	opt.displayAlignment = Qt::AlignVCenter | Qt::AlignRight; 
	drawDisplay(painter, opt, opt.rect, s); 
	drawFocus(painter, opt, opt.rect); 
	return; 
      } 
      
    default:
      {    
	QItemDelegate::paint(painter, option, index); 
	return;
      }
    }

}
// --------------------------------------------
