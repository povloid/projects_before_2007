/*
    ow_outcome.h
*/

#ifndef OW_OUTCOME_H
#define OW_OUTCOME_H

#include "ow_abstractaccumulate.h"

class QLabel;
class POne_Field;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QDoubleSpinBox;

class OW_Outcome : public OW_AbstractAccumulate
{
    Q_OBJECT

public:
    OW_Outcome(QWidget *parent = 0);

private:

    QString strMainKOD;

    QLabel *tabloLabelVAGDay;
    QLabel *tabloLabelVAGMonth;
    QLabel *tabloLabelMASSDay;
    QLabel *tabloLabelMASSMonth;
    QLabel *tabloLabelED_MASSDay;
    QLabel *tabloLabelED_MASSMonth;
    
    QComboBox* iobj_NAME;
    QSpinBox* iobj_VAG;
    QDoubleSpinBox* iobj_MASS;
    QComboBox* iobj_KOD_POTR;

    // Создаем SQL модель для ComboBox
    QSqlQueryModel *model_in_ComboBox_NAME;
    QSqlQueryModel *model_in_ComboBox_KOD_POTR;

    void updateTablo( void );

 private slots:
   void update_PGR( const QModelIndex & index );

 protected:

    virtual bool virtualAddRec ( void );
    virtual bool virtualEditRec ( int& );
    virtual bool virtualDelRec ( int& );

private slots:
  void setTextKOD_POTR( const QString & text );

private:
  QString textKOD_POTR() const;
  QString KOD_POTRtext;

};

#endif
