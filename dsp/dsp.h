
#ifndef DSP_H
#define DSP_H

#include <QMainWindow>
#include <QToolButton>
#include <QLabel>

class QAction;
class QButtonGroup;
class QDate;
class QDockWidget;
class QMenu;
class QPushButton;
class QStackedLayout;
class QToolBox;
class QWidget;

class OW_Prod_Phos;
class OW_Producing;
class OW_Outcome;
class OW_Income;
class OW_Depot;

class R_AllProductionBalance;
class R_InfoForDay;

class S_BAL_DBTable;
class S_DEP_DBTable;
class S_ED_MASS_DBTable;
class S_OB_DBTable;
class S_POST_DBTable;
class S_POTR_DBTable;
class S_PPROST_DBTable;
class S_PROD_DBTable;
class S_SYR_DBTable;
class S_VYR_DBTable;

// The My button class
class NavigatorLabel : public QLabel
{
    Q_OBJECT

public:
    NavigatorLabel( const QString & text , QWidget *parent = 0 );
};

// The My button class
class NavigatorButton : public QToolButton
{
    Q_OBJECT

public:
    NavigatorButton( const QString & text, const QIcon & icon , QWidget *parent = 0 );
};

// The main class of the DSP
class DSPWindow : public QMainWindow
{
    Q_OBJECT

public:
    DSPWindow(QWidget *parent = 0);

protected:
    virtual void closeEvent(QCloseEvent *e);

private:
    //void setupFileActions();

signals:
  void owDateChanged( const QDate & date );

private slots:
  //void fileNew();
  void selectPageInMainLayout( int );
  void setOWDate( const QDate & date ); 

 private:
 
 QDate *owDate; 

 OW_Prod_Phos *ow_prod_phos;
 OW_Producing *ow_producing;
 OW_Outcome *ow_outcome;
 OW_Income *ow_income;
 OW_Depot *ow_depot;

 R_AllProductionBalance *r_allproductionbalance;
 R_InfoForDay *r_infoforday;

 S_BAL_DBTable *s_bal_dbtable;
 S_ED_MASS_DBTable *s_ed_mass_dbtable;
 S_VYR_DBTable *s_vyr_dbtable;
 S_PROD_DBTable *s_prod_dbtable;
 S_SYR_DBTable *s_syr_dbtable;
 S_PPROST_DBTable *s_pprost_dbtable;
 S_DEP_DBTable *s_dep_dbtable;
 S_OB_DBTable *s_ob_dbtable;
 S_POST_DBTable *s_post_dbtable;
 S_POTR_DBTable *s_potr_dbtable;

 QWidget *mainWidget;
 QStackedLayout *mainLayout;
 QDockWidget* navigator; 
 QToolBox* navigatorBox; 

 QButtonGroup* navigatorButtonGroup;

 QAction *actionSave;
    
};

#endif
