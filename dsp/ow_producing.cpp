/*
    ow_producing.cpp
*/


#include "ow_producing.h"
#include "pdatepicker.h"
#include "precdialog.h"
#include "ptableview.h"

#include "p_ru_itemdelegate.h"

#include <QLocale>
#include <QDate>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlQueryModel>

#include <QSqlRecord>

#include "dspconst.h"

OW_Producing::OW_Producing(QWidget *parent)
    : QWidget(parent)
{
  locale = new QLocale;

  owDate = QDate::currentDate();

  //owDate = QDate::fromString("15.08.2006","dd.MM.yyyy");

  QFont font;
  font.setBold(true);

  QPalette palette( Qt::blue );
  palette.setColor( QPalette::Background , QColor(Qt::white));
  //palette.setColor( QPalette::Active , QPalette::Background , QColor(Qt::white));

  QLabel *captionViewLabel = new QLabel(tr("Выработка"));
  captionViewLabel->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  captionViewLabel->setFrameStyle( QFrame::Box | QFrame::Raised );
  captionViewLabel->setAutoFillBackground(true); // >>+<<
  captionViewLabel->setScaledContents(false); 
  captionViewLabel->setPalette( palette );
  captionViewLabel->setMargin(2);

  captionViewLabel->setFont(font);

  dateLabel = new QLabel;
  
  QFont font2("Lucida", 13 );  
  palette.setColor( QPalette::Background , QColor( 170, 255, 255 ));
  palette.setColor( QPalette::Foreground , Qt::blue );

  dateLabel = new QLabel( tr("00.00.0000"));
  dateLabel->setPalette( palette );
  dateLabel->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  dateLabel->setFont( font2 );
  dateLabel->setMargin( 4 );
  dateLabel->setAutoFillBackground(true); // >>+<<
  dateLabel->setFrameStyle( QFrame::Box | QFrame::Raised );



  // Подключение к базе данных ---------------------------------
  db = QSqlDatabase::database("DSP"); 
  db.transaction();		// Начинаем транзакцию

  // Создаем SQL модель
  model = new QSqlQueryModel;

  // Обновить модель --
  modelUpdate();

  model->setHeaderData(0, Qt::Horizontal, tr("Код"));
  model->setHeaderData(1, Qt::Horizontal, tr("Наименование"));
  model->setHeaderData(2, Qt::Horizontal, tr("За сутки"));
  model->setHeaderData(3, Qt::Horizontal, tr("За ночь"));
  model->setHeaderData(4, Qt::Horizontal, tr("За месяц"));
  model->setHeaderData(5, Qt::Horizontal, tr("Ед."));

  view = new PTableView;

  view->setModel(model);
  view->setColumnWidth( 0, 50 );
  view->setColumnWidth( 1, 90 );
  view->setColumnWidth( 2, 70 );
  view->setColumnWidth( 3, 70 );
  view->setColumnWidth( 4, 70 );
  view->setColumnWidth( 5, 30 );
  view->setSelectionBehavior ( QAbstractItemView::SelectRows );
  view->setSelectionMode ( QAbstractItemView::SingleSelection ); 
  view->setAlternatingRowColors ( true ); // отображать с чередующимися цветами строк
  view->setTabKeyNavigation ( false ); // отключить навигацию по кнопке TAB
  ((QHeaderView *)view->verticalHeader())->setDefaultSectionSize( 20 );
  ((QHeaderView *)view->verticalHeader())->hide();

  P_RU_ItemDelegate *p_ru_ItemDelegate = new P_RU_ItemDelegate;
  view->setItemDelegate( p_ru_ItemDelegate );
  
  QHBoxLayout * mainLayout = new QHBoxLayout;
  setLayout( mainLayout ); // Назначаем его главным на форме

  QVBoxLayout * tabLayout = new QVBoxLayout;
  tabLayout->addWidget( captionViewLabel );
  tabLayout->addWidget( view );
  mainLayout->addLayout( tabLayout );

  QVBoxLayout * buttonsLayout = new QVBoxLayout;
  mainLayout->addLayout( buttonsLayout );

  // Разделитель >-------<
  QFrame *line = new QFrame;	// --------------------------
  line->setFrameShape(QFrame::HLine);

  QPushButton *button = new QPushButton(tr("F4 - Ред. зап."));
  button->setShortcut( Qt::Key_F4 );
  button->setIconSize ( QSize( 24, 24 ) );
  button->setIcon( QIcon( radiantPath + "/edit_24.png") ); 
  button->setMaximumHeight ( buttonsMaximumHeight );

  // Сигналы и слоты *--> >----|
  connect( button, SIGNAL(clicked()), this, SLOT( editRec()));
  connect( view , SIGNAL( editRec()), this, SLOT( editRec()));
  //connect( pDatePicker, SIGNAL( dateChanged( const QDate & )), 
  //   this, SLOT( setDate( const QDate & )));

  buttonsLayout->addSpacing ( 30 );
  buttonsLayout->addWidget( dateLabel );
  buttonsLayout->addWidget( button );
  buttonsLayout->addStretch();


  // Создаем диалог для работы с записями
  precdialog = new PRecDialog;

  POne_Field* ptrPRD;

  ptrPRD = precdialog->addInputObject( tr("Наименование") , 
				       POne_Field::Type(POne_Field::Label) , 200 );
  iobj_NAME = (QLabel*)ptrPRD->inputObject();

  ptrPRD = precdialog->addInputObject( tr("За сутки:") , 
				       POne_Field::Type(POne_Field::DoubleSpinBox) , 80 );
  iobj_S_DAY_TODAY = (QDoubleSpinBox*)ptrPRD->inputObject();
  iobj_S_DAY_TODAY->setRange(0.0, 100000.0);
  iobj_S_DAY_TODAY->setSingleStep(10.0);
  iobj_S_DAY_TODAY->setValue(0.0);
  iobj_S_DAY_TODAY->setAlignment(Qt::AlignRight);
  iobj_S_DAY_TODAY->setFocus();

  ptrPRD = precdialog->addInputObject( tr("За ночь:") , 
				       POne_Field::Type(POne_Field::DoubleSpinBox) , 80 );
  iobj_S_NOT_TODAY = (QDoubleSpinBox*)ptrPRD->inputObject();
  iobj_S_NOT_TODAY->setRange(0.0, 100000.0);
  iobj_S_NOT_TODAY->setSingleStep(10.0);
  iobj_S_NOT_TODAY->setValue(0.0);
  iobj_S_NOT_TODAY->setAlignment(Qt::AlignRight);

  ptrPRD = precdialog->addInputObject( tr("За месяц:") , 
				       POne_Field::Type(POne_Field::Label) , 80 );
  iobj_S_DAY_MONTH = (QLabel*)ptrPRD->inputObject();
  iobj_S_DAY_MONTH->setAlignment(Qt::AlignRight);

  connect( iobj_S_DAY_TODAY, SIGNAL( valueChanged ( const QString & ) ), 
	       this, SLOT( recount( const QString & )));
}

// Обновить согласно справочнику ---
void OW_Producing::update( )
{
  // Запрашиваем все коды объектов из справочника
  QSqlQuery query("SELECT KOD FROM S_VYR ORDER BY KOD", db );
    
  if(!query.first())		// Если записи в справочнике не имеются
    {
      QMessageBox::warning( this , tr("Внимание:"),   
			    tr("В справочнике по ВЫРАБОТКЕ нет ни одной записи! "
			       "Ввод данных невозможен."));
      return;
    }

  //  9650 11925 

  // Проверка на наличие записи на нужное число 
 
  QSqlQuery query_out( db );

  // Вставить записи
  do
    {
      query_out.exec("INSERT INTO VYR (KOD, PDATE) VALUES ( '"
		     + query.value(0).toString() + "','" 
		     + owDate.toString("dd.MM.yyyy") + "')");
    }
  while (query.next());
  
  modelUpdate( );

}

void OW_Producing::setOWDate( const QDate & date )
{
  owDate = date;
  
  if ( date <= QDate::currentDate() )
    update();

  modelUpdate();
}

// Обновить модель ( повторить запрос ) ---------
void OW_Producing::modelUpdate(  )
{
  QString currDate = owDate.toString("dd.MM.yyyy");

  dateLabel->setText( currDate );

  selectSQL = "SELECT S_VYR.KOD, S_VYR.NAME, MIN(A.S_DAY) AS S_DAY_TODAY, "
    "MIN ( A.S_NOT ) AS S_NOT_TODAY, "
    "SUM ( B.S_DAY ) AS S_DAY_MONTH, "
    "MIN ( S_VYR.ED ) AS ED_MASS "
    "FROM S_VYR , VYR A, VYR B "
    "WHERE "
    "A.PDATE = '"
    + currDate 
    + "' "
    "AND A.KOD = S_VYR.KOD "
    "AND B.PDATE BETWEEN '" 
    + "01" + owDate.toString(".MM.yyyy") 
    + "' AND '" 
    + currDate + "' "
    "AND B.KOD = S_VYR.KOD "
    "GROUP BY S_VYR.NAME, S_VYR.KOD "
    "ORDER BY S_VYR.KOD";

  // Обновить модель --
  model->setQuery( selectSQL , db );

}

// Редактировать запись ------------------
void OW_Producing::editRec()
{
  int row = view->currentIndex().row();
  
  if( row != -1 ) // Если выбрана запись
    {
      QSqlRecord record = model->record( row );
	
      QString currDate = owDate.toString("dd.MM.yyyy");
      
      precdialog->setWindowTitle(tr("Редактировать запись"));
      precdialog->setCapText(tr("Редактировать запись на [") + currDate + tr("]"));
      precdialog->setCapPixmap( radiantPath + "/edit_32.png" );

      // Заполняем элементы диалога
      iobj_NAME->setText( record.value("NAME").toString() );

      iobj_S_DAY_TODAY->setValue( record.value("S_DAY_TODAY").toDouble() );
      iobj_S_DAY_TODAY->setSuffix(" " +  record.value("ED_MASS").toString() );
      iobj_S_DAY_TODAY->setFocus();

      iobj_S_NOT_TODAY->setValue( record.value("S_NOT_TODAY").toDouble() );
      iobj_S_NOT_TODAY->setSuffix(" " + record.value("ED_MASS").toString() );

      // Выполнить дилог
      recount( "0" );
      if (!precdialog->exec())
	return;

        QSqlQuery query( db );
       
       if (!query.exec( "UPDATE VYR SET " 
			"S_DAY = '" 
			+ QString::number(iobj_S_DAY_TODAY->value())
			+ "', S_NOT = '" 
			+ QString::number(iobj_S_NOT_TODAY->value())
			+ "' WHERE PDATE = '" + currDate + "' " 
			+ "AND KOD = ( SELECT KOD FROM S_VYR WHERE NAME = '"
			+ record.value("NAME").toString()
			+"')"))
	 QMessageBox::warning( this, tr ("Ошибка"), tr("SQL запрос не прошел!"));
       else
	 {
	   modelUpdate();
	   view->selectRow ( row );
	   return;
	 }
       view->setFocus();
  } 
  else
    QMessageBox::information( this, tr("Внимание"),
  		       tr("Нет выбранной записи.") );
}

// Функция пересчета ------------------
void OW_Producing::recount( const QString & str )
{
  if ( str == "" )
    return;

  int row = view->currentIndex().row();
  
  QSqlRecord record = model->record( row );

  double rez = record.value("S_DAY_MONTH").toDouble() - record.value("S_DAY_TODAY").toDouble()
    + iobj_S_DAY_TODAY->value();

  iobj_S_DAY_MONTH->setText( locale->toString( rez ) + " " 
			     + record.value("ED_MASS").toString() );
}
