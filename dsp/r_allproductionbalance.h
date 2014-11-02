/*
    r_allproductionbalance.h
*/

#ifndef R_ALLPRODUCTIONBALANCE_H
#define R_ALLPRODUCTIONBALANCE_H

#include "pabstractraport.h"

class R_AllProductionBalance_P4Dialog;

class R_AllProductionBalance : public PAbstractRaport
{
    Q_OBJECT

public:
    R_AllProductionBalance(QWidget *parent = 0 );

 private:

    virtual void update();

  double depotOn01( const QString & kodStr , const QDate & date ) const;
  double depotOnCurrDate( const QString & kodStr , const QDate & date ) const;

  double producingOld( const QString & kodStr , const QDate & date ) const;
  double inComeOld( const QString & kodStr , const QDate & date ) const;
  double outComeOld( const QString & kodStr , const QDate & date ) const;
  
  double producing( const QString & kodStr , const QDate & date ) const;
  double inCome( const QString & kodStr , const QDate & date ) const;
  double outCome( const QString & kodStr , const QDate & date ) const;

  R_AllProductionBalance_P4Dialog *P4Dialog;

private slots:
  void recalc ( void );

private:

  double P4_in_Slude;

  bool FerroFLAG;
  double FerroK;
  double FerroProd;

  double massPshTFKToInwardTPFN;
  double massPshTFKToInwardPrTPFN;

};

#endif
