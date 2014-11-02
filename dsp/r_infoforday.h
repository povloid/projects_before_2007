/*
    r_infoforday.h
*/

#ifndef R_INFOFORDAY_H
#define R_INFOFORDAY_H

#include "pabstractraport.h"

class R_InfoForDay : public PAbstractRaport
{
    Q_OBJECT

public:
    R_InfoForDay(QWidget *parent = 0 );

 private:
    virtual void update();

};

#endif
