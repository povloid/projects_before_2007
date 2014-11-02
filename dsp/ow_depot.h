/*
    ow_depot.h
*/

#ifndef OW_DEPOT_H
#define OW_DEPOT_H

#include "ow_abstractaccumulate.h"

class QLabel;
class POne_Field;
class QComboBox;
class QDoubleSpinBox;

class OW_Depot : public OW_AbstractAccumulate
{
    Q_OBJECT

public:
    OW_Depot(QWidget *parent = 0);

private:

    QString strMainKOD;

    QLabel *tabloLabelMASSDay;
    QLabel *tabloLabelED_MASSDay;
    
    QComboBox* iobj_NAME;
    QDoubleSpinBox* iobj_MASS;
    QComboBox* iobj_KOD;

    // Создаем SQL модель для ComboBox
    QSqlQueryModel *model_in_ComboBox_KOD;

    void updateTablo( void );

 private slots:
   void update_PGR( const QModelIndex & index );

 protected:

    virtual bool virtualAddRec ( void );
    virtual bool virtualEditRec ( int& );
    virtual bool virtualDelRec ( int& );

};

#endif
