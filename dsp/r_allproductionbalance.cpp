/*
    r_allproductionbalance.cpp
*/

#include <QTextCursor>
#include <QTextTable>
#include <QDoubleSpinBox>
#include <QPushButton>

#include <QMessageBox>

#include "r_allproductionbalance_p4dialog.h"
#include "r_allproductionbalance.h"

#include "pprogressdialog.h"

R_AllProductionBalance::R_AllProductionBalance(QWidget *parent)
    : PAbstractRaport(parent)
{
  // Надпись в заголовке
  capLabel->setText( tr("Баланс общий - нарастающий") );

  QHBoxLayout *edLayout = new QHBoxLayout;
  mainLayout->addLayout( edLayout );

  QPushButton *button = new QPushButton( tr("Пересчет") );
  edLayout->addWidget( button );
  edLayout->addStretch();

  connect(button, SIGNAL(clicked()), this, SLOT(recalc()));

  // spP4_in_Slude = new QDoubleSpinBox;

  //   spP4_in_Slude->setRange(0.0, 100000.0);
  //   spP4_in_Slude->setSingleStep(1.0);
  //   spP4_in_Slude->setSuffix(tr(" тн"));
  //   spP4_in_Slude->setValue(390.0);
  //   spP4_in_Slude->setAlignment(Qt::AlignRight);
 
  P4_in_Slude = 390; 

  P4Dialog = new R_AllProductionBalance_P4Dialog;
  
  P4_in_Slude = P4Dialog->p4_in_Slude->value(); 
  FerroK = P4Dialog->ferroK->value();
  FerroProd = P4Dialog->ferroProd->value();
  FerroFLAG = P4Dialog->bttnFerroK->isChecked();

  massPshTFKToInwardTPFN = 0;
  massPshTFKToInwardPrTPFN = 0;
}

void R_AllProductionBalance::recalc()
{

  P4Dialog->setWindowTitle(tr("Задать параметры для расчета"));
  P4Dialog->setCapText(tr("Параметры для расчета на [") 
		      + owDate.toString("dd.MM.yyyy") + tr("]"));

  P4Dialog->p4_in_Slude->setValue(P4_in_Slude); 

  if(P4Dialog->exec())
    {
      P4_in_Slude = P4Dialog->p4_in_Slude->value(); 
      FerroK = P4Dialog->ferroK->value();
      FerroProd = P4Dialog->ferroProd->value();
      FerroFLAG = P4Dialog->bttnFerroK->isChecked();

      massPshTFKToInwardTPFN = P4Dialog->massPshTFKToInwardTPFN->value();
      massPshTFKToInwardPrTPFN = P4Dialog->massPshTFKToInwardPrTPFN->value();

      recalculate();
    }
}

void R_AllProductionBalance::update()
{
  PProgressDialog progressDialog;
  progressDialog.setMinimum( 0 );
  progressDialog.setMaximum( 11 );
  progressDialog.setMinimumWidth( 400 );
  progressDialog.show();

  progressDialog.setProgress( 0 , tr("Отчистка...") );


  textEdit->clear();		// Отчистить все
  textEdit->setFontFamily ( "Courier New" );
  textEdit->setFontPointSize( 12 );
  textEdit->setFontWeight ( 80 );
  textEdit->setAlignment( Qt::AlignHCenter );
  textEdit->append(tr("Баланс общий") );

  QTextCursor cursor(textEdit->textCursor());
  
  textEdit->setAlignment( Qt::AlignHCenter );
  //  cursor.movePosition(QTextCursor::Start);

  // Установка формата будущей таблицы
  progressDialog.setProgress( 1 , tr("Установка форматов") );
  QTextTableFormat myTextTableFormat;

  qreal e = 4;
  qreal d = 0;
  qreal b = 1;

  myTextTableFormat.setCellPadding ( e );
  myTextTableFormat.setCellSpacing( d );
  myTextTableFormat.setBorder ( b );
  myTextTableFormat.setAlignment(Qt::AlignHCenter);

  QVector<QTextLength> constraints;
  constraints << QTextLength(QTextLength::PercentageLength, 5 )
	      << QTextLength(QTextLength::VariableLength, 0 )
	      << QTextLength(QTextLength::PercentageLength, 5 );
  myTextTableFormat.setColumnWidthConstraints(constraints);

  myTextTableFormat.setWidth ( 500 );

  // Для печати жирным шрифтом
  QTextCharFormat boldFormat;
  boldFormat.setFontWeight(QFont::Bold);
  
  // Для выравнивания по середине
  QTextBlockFormat alignmentForCenterFormat;
  alignmentForCenterFormat.setAlignment( Qt::AlignHCenter );

  QTextBlockFormat alignmentForRightFormat;
  alignmentForRightFormat.setAlignment( Qt::AlignRight );

  progressDialog.setProgress( 2 , tr("Создаем таблицу") );
  QTextTable *table = cursor.insertTable( 40 , 3 , myTextTableFormat);

  // Объединяем верхние ячейки в одну ячейку
  progressDialog.setProgress( 3 , tr("Создаем заголовок") );
  table->mergeCells ( 0 , 0 , 1 , 3 );
  
  cursor = table->cellAt( 0 , 1 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("производства готовой продукции за ")
		    + owDate.toString("dd.MM.yyyy") , boldFormat );

  cursor = table->cellAt( 1 , 0 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("Номер") , boldFormat );

  cursor = table->cellAt( 1 , 1 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("Наименование") , boldFormat );

  cursor = table->cellAt( 1 , 2 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("ЖФ ТОО \"Казфосфат\"\n (НДФЗ)") , boldFormat );

  progressDialog.setProgress( 4 , tr("Рисуем рабочий шаблон") );
  // Желтый фосфор ------------------------------------
  table->mergeCells ( 2 , 0 , 8 , 1 );  
  cursor = table->cellAt( 2 , 0 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("1") , boldFormat );
  cursor.setBlockFormat( alignmentForCenterFormat );

  table->mergeCells ( 2 , 1 , 1 , 2 );  
  cursor = table->cellAt( 2 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Желтый фосфор") , boldFormat );
  cursor.setBlockFormat( alignmentForCenterFormat );

  cursor = table->cellAt( 3 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Остаток на ") + owDate.toString("01.MM.yyyy") );

  cursor = table->cellAt( 4 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Производство") );

  cursor = table->cellAt( 5 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Отгрузка") );

  cursor = table->cellAt( 6 , 1 ).firstCursorPosition();
  cursor.insertText(tr("На внутреннее потребление") );

  cursor = table->cellAt( 7 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Остаток на ") + owDate.toString("dd.MM.yyyy") );

  cursor = table->cellAt( 8 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Остаток фосфора") );

  cursor = table->cellAt( 9 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Остаток фосфора в шламе") );


  // ТФК ------------------------------------
  table->mergeCells ( 10 , 0 , 7 , 1 );  
  cursor = table->cellAt( 10 , 0 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("2") , boldFormat );
  cursor.setBlockFormat( alignmentForCenterFormat );

  table->mergeCells ( 10 , 1 , 1 , 2 );  
  cursor = table->cellAt( 10 , 1 ).firstCursorPosition();
  cursor.insertText(tr("ТФК") , boldFormat );
  cursor.setBlockFormat( alignmentForCenterFormat );

  cursor = table->cellAt( 11 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Остаток на ") + owDate.toString("01.MM.yyyy") );

  cursor = table->cellAt( 12 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Производство") );

  cursor = table->cellAt( 13 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Приход со стороны") );

  cursor = table->cellAt( 14 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Отгрузка") );

  cursor = table->cellAt( 15 , 1 ).firstCursorPosition();
  cursor.insertText(tr("На внутреннее потребление ТПФН") );

  cursor = table->cellAt( 16 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Остаток на ") + owDate.toString("dd.MM.yyyy") );

  // Пищевая ТФК -------------------------------
  table->mergeCells ( 17 , 0 , 7 , 1 );  
  cursor = table->cellAt( 17 , 0 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("3") , boldFormat );
  cursor.setBlockFormat( alignmentForCenterFormat );

  table->mergeCells ( 17 , 1 , 1 , 2 );  
  cursor = table->cellAt( 17 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Пищевая ТФК") , boldFormat );
  cursor.setBlockFormat( alignmentForCenterFormat );

  cursor = table->cellAt( 18 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Остаток на ") + owDate.toString("01.MM.yyyy") );

  cursor = table->cellAt( 19 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Производство") );

  cursor = table->cellAt( 20 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Отгрузка") );

  cursor = table->cellAt( 21 , 1 ).firstCursorPosition();
  cursor.insertText(tr("На внут. потреб. ТПФП") );

  cursor = table->cellAt( 22 , 1 ).firstCursorPosition();
  cursor.insertText(tr("На внут. потреб. ТПФН простой") );

  cursor = table->cellAt( 23 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Остаток на ") + owDate.toString("dd.MM.yyyy") );


  // ТПФН ------------------------------------
  table->mergeCells ( 24 , 0 , 5 , 1 );  
  cursor = table->cellAt( 24 , 0 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("4") , boldFormat );
  cursor.setBlockFormat( alignmentForCenterFormat );

  table->mergeCells ( 24 , 1 , 1 , 2 );  
  cursor = table->cellAt( 24 , 1 ).firstCursorPosition();
  cursor.insertText(tr("ТПФН") , boldFormat );
  cursor.setBlockFormat( alignmentForCenterFormat );

  cursor = table->cellAt( 25 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Остаток на ") + owDate.toString("01.MM.yyyy") );

  cursor = table->cellAt( 26 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Производство") );

  cursor = table->cellAt( 27 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Отгрузка") );

  cursor = table->cellAt( 28 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Остаток на ") + owDate.toString("dd.MM.yyyy") );

  // Феррофосфор ------------------------------------
  table->mergeCells ( 29 , 0 , 5 , 1 );  
  cursor = table->cellAt( 29 , 0 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("5") , boldFormat );
  cursor.setBlockFormat( alignmentForCenterFormat );

  table->mergeCells ( 29 , 1 , 1 , 2 );  
  cursor = table->cellAt( 29 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Феррофосфор") , boldFormat );
  cursor.setBlockFormat( alignmentForCenterFormat );

  cursor = table->cellAt( 30 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Остаток на ") + owDate.toString("01.MM.yyyy") );

  cursor = table->cellAt( 31 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Производство") );

  cursor = table->cellAt( 32 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Отгрузка") );

  cursor = table->cellAt( 33 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Остаток на ") + owDate.toString("dd.MM.yyyy") );

  // Граншлак ------------------------------------
  table->mergeCells ( 34 , 0 , 5 , 1 );  
  cursor = table->cellAt( 34 , 0 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("6") , boldFormat );
  cursor.setBlockFormat( alignmentForCenterFormat );

  table->mergeCells ( 34 , 1 , 1 , 2 );  
  cursor = table->cellAt( 34 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Граншлак") , boldFormat );
  cursor.setBlockFormat( alignmentForCenterFormat );

  cursor = table->cellAt( 35 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Остаток на ") + owDate.toString("01.MM.yyyy") );

  cursor = table->cellAt( 36 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Производство") );

  cursor = table->cellAt( 37 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Отгрузка") );

  cursor = table->cellAt( 38 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Остаток на ") + owDate.toString("dd.MM.yyyy") );

  // Подпись должностного лица
  cursor = table->cellAt( 39 , 1 ).firstCursorPosition();
  cursor.insertText(tr("Главный специалист по производству ") );
  cursor = table->cellAt( 39 , 2 ).firstCursorPosition();
  cursor.insertText(tr("?") );

  // **************** Расчет ****************
  progressDialog.setProgress( 5 , tr("Расчет общего баланса") );
  QSqlQuery query( db ); 
  
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // 1. ТПФН 
  progressDialog.setProgress( 6 , tr("Расчет общего баланса ТПФН") );
  double massDepotTPFNOn01 = depotOn01( "2" , owDate );

  double producingTPFN = producingOld( "2" , owDate );
  double outComeTPFN = outComeOld( "2" , owDate );

  // Остаток на 01 текущего месяца
  cursor = table->cellAt( 25 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massDepotTPFNOn01 ) );

  // Выработка - производство
  cursor = table->cellAt( 26 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( producingTPFN ));   

  // отгрузка
  cursor = table->cellAt( 27 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( outComeTPFN ) );   

  // Вычисляем новый остаток
  double massDepotTPFN = massDepotTPFNOn01 + producingTPFN - outComeTPFN;
  // Вывод значения в таблицу вычисленного остатка
  cursor = table->cellAt( 28 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massDepotTPFN ));   


  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // 2. ТФК 
  progressDialog.setProgress( 7 , tr("Расчет общего баланса ТФК") );
  double massDepotTFKOn01 = depotOn01( "3" , owDate );

  double producingTFK = producingOld( "3" , owDate );
  double inComeTFK = inComeOld( "3" , owDate );
  double outComeTFK = outComeOld( "3" , owDate );

  // Остаток на 01 текущего месяца
  cursor = table->cellAt( 11 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massDepotTFKOn01 ));

  // Выработка - производство
  cursor = table->cellAt( 12 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( producingTFK ));   

  // приход со стороны
  cursor = table->cellAt( 13 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( inComeTFK ));   

  // отгрузка
  cursor = table->cellAt( 14 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( outComeTFK ) );   

  // На внутреннее потребление
  double massTFKToInward = producingTPFN * 0.828;
  cursor = table->cellAt( 15 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massTFKToInward ) );   

  // Вычисляем новый остаток
  double massDepotTFK = massDepotTFKOn01 + producingTFK + inComeTFK 
    - outComeTFK - massTFKToInward;
  // Вывод значения в таблицу вычисленного остатка
  cursor = table->cellAt( 16 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massDepotTFK ));   

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // 2.2 Пищевая ТФК 
  progressDialog.setProgress( 8 , tr("Расчет общего баланса пищевой ТФК") );
  double massDepotPshTFKOn01 = depotOn01( "4" , owDate );

  double producingPshTFK = producingOld( "4" , owDate );
  double outComePshTFK = outComeOld( "4" , owDate );

  // Остаток на 01 текущего месяца
  cursor = table->cellAt( 18 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massDepotPshTFKOn01 ));

  // Выработка - производство
  cursor = table->cellAt( 19 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( producingPshTFK ));   

  // отгрузка
  cursor = table->cellAt( 20 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( outComePshTFK ) );   

  // На внутреннее потребление ТПФН
  cursor = table->cellAt( 21 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massPshTFKToInwardTPFN ) );   

  // На внутреннее потребление простой ТПФН
  cursor = table->cellAt( 22 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massPshTFKToInwardPrTPFN ) );   

  // Вычисляем новый остаток
  double massDepotPshTFK = massDepotPshTFKOn01 + producingPshTFK 
    - outComePshTFK - massPshTFKToInwardTPFN - massPshTFKToInwardPrTPFN;
  // Вывод значения в таблицу вычисленного остатка
  cursor = table->cellAt( 23 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massDepotPshTFK ));   

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // 3. P4 
  progressDialog.setProgress( 9 , tr("Расчет общего баланса желтого фосфора") );
  // Фосфора в шламе
  cursor = table->cellAt( 9 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( P4_in_Slude) );


  // Остаток на начало месяца
  double massDepotP4On01 = depotOn01( "1" , owDate )  + P4_in_Slude;

  cursor = table->cellAt( 3 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massDepotP4On01 ));

  // Остаток на текущую дату
  double massDepotP4OnCurrDate = depotOnCurrDate( "1" , owDate );

  cursor = table->cellAt( 8 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massDepotP4OnCurrDate ) );

  double massDepotP4 = massDepotP4OnCurrDate + P4_in_Slude;

  cursor = table->cellAt( 7 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massDepotP4 ) );

  // На внутреннее потребление
  double massP4ToInward = producingTFK * 0.33;

  cursor = table->cellAt( 6 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massP4ToInward ) );   

  // отгрузка
  double outComeP4 = outComeOld( "1" , owDate );
  cursor = table->cellAt( 5 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( outComeP4 ) ); 

  // Вычисляем новый остаток
  double  producingP4 = outComeP4 + massP4ToInward + massDepotP4 
    - massDepotP4On01;
  // Вывод значения в таблицу вычисленного остатка
  cursor = table->cellAt( 4 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( producingP4 ));   
  
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // 4. Феррофосфор
  // Остаток на начало месяца
  progressDialog.setProgress( 10 , tr("Расчет общего баланса феррофосфора") );
  double massDepotFerroOn01 = depotOn01( "5" , owDate );

  cursor = table->cellAt( 30 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massDepotFerroOn01 ));

  double  producingFerro;

  if(FerroFLAG)
    producingFerro = producingP4 * FerroK;
  else
    producingFerro = FerroProd;

  cursor = table->cellAt( 31 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( producingFerro ));

  double outComeFerro = outComeOld( "5" , owDate );
  cursor = table->cellAt( 32 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( outComeFerro ));

  double  massDepotFerro = massDepotFerroOn01 + producingFerro
    - outComeFerro;
  // Вывод значения в таблицу вычисленного остатка
  cursor = table->cellAt( 33 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massDepotFerro ));   

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // 5. Граншлак
  // Остаток на начало месяца
  progressDialog.setProgress( 11 , tr("Расчет общего баланса граншлака") );
  double massDepotGSHOn01 = depotOn01( "6" , owDate );

  cursor = table->cellAt( 35 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massDepotGSHOn01 ));
  
  double  producingGSH = producingP4 * 9.5;

  cursor = table->cellAt( 36 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( producingGSH ));

  double outComeGSH = outComeOld( "6" , owDate );
  cursor = table->cellAt( 37 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( outComeGSH ));

  double  massDepotGSH = massDepotGSHOn01 + producingGSH
    - outComeGSH;
  // Вывод значения в таблицу вычисленного остатка
  cursor = table->cellAt( 38 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( massDepotGSH ));   

}

// Остаток на первое число текущего месяца
double R_AllProductionBalance::depotOnCurrDate( const QString & kodStr , const QDate & date ) const
{
  QSqlQuery query( db );
  
  query.exec("SELECT SUM(a.MASS) "
	     "FROM OSTAT_PROD_PGR a, S_PROD_PGR b "
	     "WHERE a.PDATE='"
	     + date.toString("dd.MM.yyyy") + "' "
	     "AND a.KOD = b.KOD "
	     "AND b.KODM =" + kodStr );
    
  if(query.first())
    return query.value(0).toDouble();
  else return 0;
}

// Остаток на первое число текущего месяца
double R_AllProductionBalance::depotOn01( const QString & kodStr , const QDate & date ) const
{
  QSqlQuery query( db );
  
  query.exec("SELECT SUM(a.MASS) "
	     "FROM OSTAT_PROD_PGR a, S_PROD_PGR b "
	     "WHERE a.PDATE='"
	     + QString::number(date.addMonths(-1).daysInMonth())
	     + date.addMonths(-1).toString(".MM.yyyy") + "' "
	      "AND a.KOD = b.KOD "
	      "AND b.KODM =" + kodStr );
    
  if(query.first())
    return query.value(0).toDouble();
  else return 0;
}

// Выработано с начала месяца по текущую дату
double R_AllProductionBalance::producingOld( const QString & kodStr , const QDate & date ) const
{
  QSqlQuery query( db );

  query.exec("SELECT SUM(S_DAY) FROM VYR "
	     "WHERE PDATE BETWEEN '01" 
	     + date.toString(".MM.yyyy") + "'" 
	     " AND '" + date.toString("dd.MM.yyyy") + "'"
	     " AND KOD=" + kodStr );
  
  if(query.first())
    return query.value(0).toDouble();
  else return 0;
}

// Пришло со стороны с начала месяца по текущую дату
double R_AllProductionBalance::inComeOld( const QString & kodStr , const QDate & date ) const
{
  QSqlQuery query( db );

  query.exec("SELECT SUM(MASS) FROM PRIHOD "
	     "WHERE PDATE BETWEEN '01" 
	     + date.toString(".MM.yyyy") + "'" 
	     " AND '" + date.toString("dd.MM.yyyy") + "'"
	     " AND KOD=" + kodStr );

  if(query.first())
    return query.value(0).toDouble();
  else return 0;
}

// Отгружено на сторону с начала месяца по текущую дату
double R_AllProductionBalance::outComeOld( const QString & kodStr , const QDate & date ) const
{ 
  QSqlQuery query( db );

  query.exec("SELECT SUM(MASS) FROM OTGRUZKA "
	     "WHERE PDATE BETWEEN '01" 
	     + date.toString(".MM.yyyy") + "'" 
	     " AND '" + date.toString("dd.MM.yyyy") + "'"
	     " AND KOD=" + kodStr );

  if(query.first())
    return query.value(0).toDouble();
  else return 0;
}

// Текущая ВЫРАБОТКА
double R_AllProductionBalance::producing( const QString & kodStr , const QDate & date ) const
{
  QSqlQuery query( db );

  query.exec("SELECT S_DAY FROM VYR WHERE PDATE='" 
	     + date.toString("dd.MM.yyyy") + "' AND KOD=" + kodStr );

  if(query.first())
    return query.value(0).toDouble();
  else return 0;
}

// Текущий ПИХОД СО СТОРОНЫ
double R_AllProductionBalance::inCome( const QString & kodStr , const QDate & date ) const
{
  QSqlQuery query( db );

  query.exec("SELECT SUM(MASS) FROM PRIHOD WHERE PDATE='" 
 	     + date.toString("dd.MM.yyyy") + "' AND KOD=" + kodStr );

  if(query.first())
    return query.value(0).toDouble();
  else return 0;
}

// Текущая ОТГРУЗКА
double R_AllProductionBalance::outCome( const QString & kodStr , const QDate & date ) const
{ 
  QSqlQuery query( db );
    
  query.exec("SELECT SUM(MASS) FROM OTGRUZKA WHERE PDATE='" 
	     + date.toString("dd.MM.yyyy") + "' AND KOD=" + kodStr );

  if(query.first())
    return query.value(0).toDouble();
  else return 0;
}
