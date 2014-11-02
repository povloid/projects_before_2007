/*
    precdialog.h
*/

#ifndef PRECDIALOG_H
#define PRECDIALOG_H

#include"pdialog.h"
#include"pone_field.h"

class PRecDialog : public PDialog
{
    Q_OBJECT

public:
    PRecDialog( QWidget *parent = 0 );

    POne_Field* addInputObject(QString strCaption = tr("Заголовок") , 
			       POne_Field::Type type_tmp = POne_Field::Type(POne_Field::LineEdit ), 
			       int sizeHorizon = 100) const;

private:
    QHBoxLayout * iputObjectsLayout;
};

#endif
