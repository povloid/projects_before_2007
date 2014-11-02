/*
    p_ru_itemdelegate.h
*/

#ifndef P_RU_ITEMDELEGATE_H
#define P_RU_ITEMDELEGATE_H

#include <QItemDelegate>

class QLocale;

class P_RU_ItemDelegate : public QItemDelegate
{
public:
    P_RU_ItemDelegate(QWidget *parent = 0 );
  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const; 

private:
  QLocale *locale;
};

#endif
