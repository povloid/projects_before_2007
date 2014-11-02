/*
    ow_prod_phos_tablo.h
*/

#ifndef OW_PROD_PHOS_TABLO_H
#define OW_PROD_PHOS_TABLO_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QDoubleValidator>

class QLocale;

class OW_Prod_Phos_Tablo : public QWidget
{
    Q_OBJECT

public:
    OW_Prod_Phos_Tablo( QWidget *parent = 0 , bool bool_edit_tmp = false );

    void setFocus();

private:    
  
    QLocale *locale;

    double power_For_Month_in_P5;
    double power_For_Month_in_P6;
    double power_For_Month_in_P7;
    double power_For_Month_in_P8;

    double p4_For_Month_in_P5;
    double p4_For_Month_in_P6;
    double p4_For_Month_in_P7;
    double p4_For_Month_in_P8;

    bool bool_edit; // QLabel или QLineEdit
    
    QLabel *power_for_month_in_P5;
    QLabel *p4_for_day_in_P5;
    QLabel *p4_for_month_in_P5;

    QLabel *power_for_month_in_P6;
    QLabel *p4_for_day_in_P6;
    QLabel *p4_for_month_in_P6;

    QLabel *power_for_month_in_P7;
    QLabel *p4_for_day_in_P7;
    QLabel *p4_for_month_in_P7;

    QLabel *power_for_month_in_P8;
    QLabel *p4_for_day_in_P8;
    QLabel *p4_for_month_in_P8;

    QLabel *power_for_day_SUM;
    QLabel *power_for_night_SUM;
    QLabel *power_for_month_SUM;
    QLabel *p4_for_day_SUM;
    QLabel *p4_for_month_SUM;

    /* Элементы ввода информации */

    /* При  bool_edit = false */

    QLabel *power_for_day_in_P5;
    QLabel *power_for_night_in_P5;

    QLabel *power_for_day_in_P6;
    QLabel *power_for_night_in_P6;

    QLabel *power_for_day_in_P7;
    QLabel *power_for_night_in_P7;

    QLabel *power_for_day_in_P8;
    QLabel *power_for_night_in_P8;

    /* При  bool_edit = true */

    QDoubleSpinBox *powerED_for_day_in_P5;
    QDoubleSpinBox *powerED_for_night_in_P5;

    QDoubleSpinBox *powerED_for_day_in_P6;
    QDoubleSpinBox *powerED_for_night_in_P6;

    QDoubleSpinBox *powerED_for_day_in_P7;
    QDoubleSpinBox *powerED_for_night_in_P7;

    QDoubleSpinBox *powerED_for_day_in_P8;
    QDoubleSpinBox *powerED_for_night_in_P8;

 public:

    void setText_for_day_in_P5( const double & w );
    void setText_for_night_in_P5( const double & w );

    void setText_for_day_in_P6( const double & w );
    void setText_for_night_in_P6( const double & w );

    void setText_for_day_in_P7( const double & w );
    void setText_for_night_in_P7( const double & w );

    void setText_for_day_in_P8( const double & w );
    void setText_for_night_in_P8( const double & w );
    
    double value_for_day_in_P5( void ) const;
    double value_for_night_in_P5( void ) const;

    double value_for_day_in_P6( void ) const;
    double value_for_night_in_P6( void ) const;

    double value_for_day_in_P7( void ) const;
    double value_for_night_in_P7( void ) const;

    double value_for_day_in_P8( void ) const;
    double value_for_night_in_P8( void ) const;

    void setPower_For_Month_in_P5( const double & w );
    void setPower_For_Month_in_P6( const double & w );
    void setPower_For_Month_in_P7( const double & w );
    void setPower_For_Month_in_P8( const double & w );

    void setText_P4_For_Day_in_P5( const double & w );
    void setText_P4_For_Day_in_P6( const double & w );
    void setText_P4_For_Day_in_P7( const double & w );
    void setText_P4_For_Day_in_P8( const double & w );

    void setP4_For_Month_in_P5( const double & w );
    void setP4_For_Month_in_P6( const double & w );
    void setP4_For_Month_in_P7( const double & w );
    void setP4_For_Month_in_P8( const double & w );

private slots:
    
    void recount ( const QString & str );

};

#endif
