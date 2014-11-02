/*
    r_infoforday.cpp
*/
#include <QTextCursor>
#include <QTextTable>
#include <QMessageBox>

#include "r_infoforday.h"

R_InfoForDay::R_InfoForDay(QWidget *parent)
    : PAbstractRaport(parent)
{
  // Надпись в заголовке
  capLabel->setText( tr("Информация о работе") );
}

void R_InfoForDay::update()
{
  textEdit->clear();		// Отчистить все
  textEdit->setFontFamily ( "Courier New" );
  textEdit->setFontPointSize( 14 );
  textEdit->setFontWeight ( 80 );
  textEdit->setAlignment( Qt::AlignHCenter );
  textEdit->append( tr("Информация") );
  textEdit->append( tr("о работе филиала ЖФ ТОО \"Казфосфат\" (НДФЗ) за ") 
		      + owDate.toString("dd.MM.yyyy") + tr(" г.") );

  QTextCursor cursor(textEdit->textCursor());
  QTextFrame *topFrame = cursor.currentFrame();
  
  textEdit->setAlignment( Qt::AlignHCenter );
  //  cursor.movePosition(QTextCursor::Start);

  // Установка формата будущей таблицы
  QTextTableFormat myTextTableFormat;

  qreal e = 4;
  qreal d = 0;
  qreal b = 1;

  myTextTableFormat.setCellPadding( e );
  myTextTableFormat.setCellSpacing( d );
  myTextTableFormat.setBorder( b );
  myTextTableFormat.setAlignment(Qt::AlignHCenter);

  QVector<QTextLength> constraints;
  constraints << QTextLength(QTextLength::VariableLength, 30 )
	      << QTextLength(QTextLength::PercentageLength, 5 )
	      << QTextLength(QTextLength::PercentageLength, 5 )
	      << QTextLength(QTextLength::PercentageLength, 5 )
	      << QTextLength(QTextLength::PercentageLength, 5 )
	      << QTextLength(QTextLength::PercentageLength, 5 );
  myTextTableFormat.setColumnWidthConstraints(constraints);

  myTextTableFormat.setWidth ( 500 );

  // Для печати жирным шрифтом
  QTextCharFormat boldFormat;
  boldFormat.setFontWeight(QFont::Bold);

  QTextCharFormat unBoldFormat;
  unBoldFormat.setFontWeight(QFont::Normal);
  
  // Для выравнивания по середине
  QTextBlockFormat alignmentForCenterFormat;
  alignmentForCenterFormat.setAlignment( Qt::AlignHCenter );

  QTextBlockFormat alignmentForRightFormat;
  alignmentForRightFormat.setAlignment( Qt::AlignRight );

  QTextTable *table = cursor.insertTable( 6 , 6 , myTextTableFormat);

  cursor = table->cellAt( 0 , 0 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("Цех-5") , boldFormat );

  cursor = table->cellAt( 0 , 1 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("5-Печь") , boldFormat );

  cursor = table->cellAt( 0 , 2 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("6-Печь") , boldFormat );

  cursor = table->cellAt( 0 , 3 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("7-Печь") , boldFormat );

  cursor = table->cellAt( 0 , 4 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("8-Печь") , boldFormat );

  cursor = table->cellAt( 0 , 5 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("Итого") , boldFormat );

  cursor = table->cellAt( 1 , 0 ).firstCursorPosition();
  cursor.insertText(tr("Выработка за сутки МВт") , boldFormat );

  cursor = table->cellAt( 2 , 0 ).firstCursorPosition();
  cursor.insertText(tr("Выработка за ночь МВт") , boldFormat );

  cursor = table->cellAt( 3 , 0 ).firstCursorPosition();
  cursor.insertText(tr("Выработка с начала месяца МВт") , boldFormat );

  cursor = table->cellAt( 4 , 0 ).firstCursorPosition();
  cursor.insertText(tr("Фосфор за сутки тн") , boldFormat );

  cursor = table->cellAt( 5 , 0 ).firstCursorPosition();
  cursor.insertText(tr("Фосфор с начала месяца тн") , boldFormat );

  // Выбор из ьаблици и расчет ******************
  QSqlQuery query( db ); 

  query.exec("SELECT PDATE, P5_MW, P6_MW, P7_MW, P8_MW, P_MW_SUM, "
	     " P5_NIGHT_MW, P6_NIGHT_MW, P7_NIGHT_MW, P8_NIGHT_MW, P_NIGHT_MW_SUM,"
	     " P5_P4, P6_P4, P7_P4, P8_P4, P_P4_SUM "
	     " FROM FOSFOR "
	     " WHERE PDATE BETWEEN '01" + owDate.toString(".MM.yyyy") + "'" 
	     " AND '" + owDate.toString("dd.MM.yyyy") + "'"
	     " ORDER BY PDATE");

  double power_for_month_in_P5 = 0, 
    power_for_month_in_P6 = 0, 
    power_for_month_in_P7 = 0, 
    power_for_month_in_P8 = 0,
    p4_for_month_in_P5 = 0, 
    p4_for_month_in_P6 = 0, 
    p4_for_month_in_P7 = 0, 
    p4_for_month_in_P8 = 0;

  while (query.next()) {
    //   QString country = query.value(0).toString();
    
    power_for_month_in_P5 += query.value(1).toDouble();
    power_for_month_in_P6 += query.value(2).toDouble();
    power_for_month_in_P7 += query.value(3).toDouble();
    power_for_month_in_P8 += query.value(4).toDouble();
    
    p4_for_month_in_P5 += query.value(11).toDouble();
    p4_for_month_in_P6 += query.value(12).toDouble();
    p4_for_month_in_P7 += query.value(13).toDouble();
    p4_for_month_in_P8 += query.value(14).toDouble();
  }

  query.last();
  // Заполняем данные по выработке мощности
  // Суточные
  cursor = table->cellAt( 1 , 1 ).firstCursorPosition();
  cursor.insertText( query.value(1).toString() );

  cursor = table->cellAt( 1 , 2 ).firstCursorPosition();
  cursor.insertText( query.value(2).toString() );

  cursor = table->cellAt( 1 , 3 ).firstCursorPosition();
  cursor.insertText( query.value(3).toString() );

  cursor = table->cellAt( 1 , 4 ).firstCursorPosition();
  cursor.insertText( query.value(4).toString() );

  cursor = table->cellAt( 1 , 5 ).firstCursorPosition();
  cursor.insertText( query.value(5).toString() );
  // за ночь
  cursor = table->cellAt( 2 , 1 ).firstCursorPosition();
  cursor.insertText( query.value(6).toString() );

  cursor = table->cellAt( 2 , 2 ).firstCursorPosition();
  cursor.insertText( query.value(7).toString() );

  cursor = table->cellAt( 2 , 3 ).firstCursorPosition();
  cursor.insertText( query.value(8).toString() );

  cursor = table->cellAt( 2 , 4 ).firstCursorPosition();
  cursor.insertText( query.value(9).toString() );

  cursor = table->cellAt( 2 , 5 ).firstCursorPosition();
  cursor.insertText( query.value(10).toString() );
  
  // Месячные
  cursor = table->cellAt( 3 , 1 ).firstCursorPosition();
  cursor.insertText( QString::number( power_for_month_in_P5 ) );

  cursor = table->cellAt( 3 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( power_for_month_in_P6 ) );

  cursor = table->cellAt( 3 , 3 ).firstCursorPosition();
  cursor.insertText( QString::number( power_for_month_in_P7 ) );

  cursor = table->cellAt( 3 , 4 ).firstCursorPosition();
  cursor.insertText( QString::number( power_for_month_in_P8 ) );

  cursor = table->cellAt( 3 , 5 ).firstCursorPosition();
  cursor.insertText( QString::number( power_for_month_in_P5 
				      + power_for_month_in_P6 
				      + power_for_month_in_P7 
				      + power_for_month_in_P8) );

  // Заполняем данные по фосфору за месяц
  // Суточные
  cursor = table->cellAt( 4 , 1 ).firstCursorPosition();
  cursor.insertText( query.value(11).toString() );

  cursor = table->cellAt( 4 , 2 ).firstCursorPosition();
  cursor.insertText( query.value(12).toString() );

  cursor = table->cellAt( 4 , 3 ).firstCursorPosition();
  cursor.insertText( query.value(13).toString() );

  cursor = table->cellAt( 4 , 4 ).firstCursorPosition();
  cursor.insertText( query.value(14).toString() );

  cursor = table->cellAt( 4 , 5 ).firstCursorPosition();
  cursor.insertText( query.value(15).toString() );

  // Месячные
  cursor = table->cellAt( 5 , 1 ).firstCursorPosition();
  cursor.insertText( QString::number( p4_for_month_in_P5 ) );

  cursor = table->cellAt( 5 , 2 ).firstCursorPosition();
  cursor.insertText( QString::number( p4_for_month_in_P6 ) );

  cursor = table->cellAt( 5 , 3 ).firstCursorPosition();
  cursor.insertText( QString::number( p4_for_month_in_P7 ) );

  cursor = table->cellAt( 5 , 4 ).firstCursorPosition();
  cursor.insertText( QString::number( p4_for_month_in_P8 ) );

  cursor = table->cellAt( 5 , 5 ).firstCursorPosition();
  cursor.insertText( QString::number( p4_for_month_in_P5 
				      + p4_for_month_in_P6 
				      + p4_for_month_in_P7 
				      + p4_for_month_in_P8) );

  // По выработке
  constraints.clear();
  constraints << QTextLength(QTextLength::VariableLength, 30 )
	      << QTextLength(QTextLength::PercentageLength, 20 )
	      << QTextLength(QTextLength::PercentageLength, 20 )
	      << QTextLength(QTextLength::PercentageLength, 20 );
  myTextTableFormat.setColumnWidthConstraints(constraints);

  cursor.setPosition(topFrame->lastPosition());
  table = cursor.insertTable( 1 , 4 , myTextTableFormat);

  cursor = table->cellAt( 0 , 0 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("Выработка") , boldFormat );

  cursor = table->cellAt( 0 , 1 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("За сутки") , boldFormat );

  cursor = table->cellAt( 0 , 2 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("За ночь") , boldFormat );

  cursor = table->cellAt( 0 , 3 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("С начала месяца") , boldFormat );

  query.exec("SELECT S_VYR.KOD, S_VYR.NAME, MIN(A.S_DAY) AS S_DAY_TODAY, "
	     "MIN ( B.S_NOT ) AS S_NOT_TODAY, "
	     "SUM ( B.S_DAY ) AS S_DAY_MONTH, "
	     "MIN ( S_VYR.ED ) "
	     "FROM S_VYR , VYR A, VYR B "
	     "WHERE "
	     "A.PDATE = '"
	     + owDate.toString("dd.MM.yyyy") + "' "
	     "AND A.KOD = S_VYR.KOD "
	     "AND B.PDATE BETWEEN '" 
	     + "01" + owDate.toString(".MM.yyyy") 
	     + "' AND '" 
	     + owDate.toString("dd.MM.yyyy") + "' "
	     "AND B.KOD = S_VYR.KOD "
	     "GROUP BY S_VYR.NAME, S_VYR.KOD "
	     "ORDER BY S_VYR.KOD");

  int index_row = 1;
  
  while (query.next()) {
    ++index_row;
    table->insertRows ( index_row , 1 );

    cursor = table->cellAt( index_row - 1 , 0 ).firstCursorPosition();
    cursor.insertText( query.value(1).toString() , boldFormat );

    cursor = table->cellAt( index_row - 1, 1 ).firstCursorPosition();
    cursor.insertText( query.value(2).toString() );

    cursor = table->cellAt( index_row - 1, 2 ).firstCursorPosition();
    cursor.insertText( query.value(3).toString() );

    cursor = table->cellAt( index_row - 1, 3 ).firstCursorPosition();
    cursor.insertText( query.value(4).toString() );
  }

  // Остатки
  // По выработке
  constraints.clear();
  constraints << QTextLength(QTextLength::VariableLength, 30 )
	      << QTextLength(QTextLength::PercentageLength, 20 );
  myTextTableFormat.setColumnWidthConstraints(constraints);

  cursor.setPosition(topFrame->lastPosition());
  table = cursor.insertTable( 1 , 2 , myTextTableFormat);


  cursor = table->cellAt( 0 , 0 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("Остатки готовой продукции") , boldFormat );

  cursor = table->cellAt( 0 , 1 ).firstCursorPosition();
  cursor.setBlockFormat( alignmentForCenterFormat );
  cursor.insertText(tr("Масса") , boldFormat );

  
  query.exec("SELECT KOD,NAME FROM S_PROD_GR ORDER BY KOD");

  QSqlQuery tquery( db );
  
  index_row = 1;

  while (query.next()) {
    ++index_row;
    table->insertRows ( index_row , 1 );

    cursor = table->cellAt( index_row - 1 , 0 ).firstCursorPosition();
    cursor.insertText( query.value(1).toString() , boldFormat );

    tquery.exec("SELECT SUM(a.MASS) "
		"FROM OSTAT_PROD_PGR a, S_PROD_PGR b "
		"WHERE a.PDATE='"
		+ owDate.toString("dd.MM.yyyy") + "' "
		"AND a.KOD = b.KOD "
		"AND b.KODM =" + query.value(0).toString() );
    
    tquery.first();
    
    cursor = table->cellAt( index_row - 1 , 1 ).firstCursorPosition();
    cursor.insertText( tquery.value(0).toString() , boldFormat );


    tquery.exec("SELECT b.NAME,a.MASS "
		"FROM OSTAT_PROD_PGR a, S_PROD_PGR b "
		"WHERE a.PDATE='"
		+ owDate.toString("dd.MM.yyyy") + "' "
		"AND a.KOD = b.KOD "
		"AND b.KODM =" + query.value(0).toString() );

    int tindex_row = 1;
    
    while (tquery.next()) {
      ++tindex_row;

      cursor = table->cellAt( index_row - 1 , 0 ).lastCursorPosition();
      cursor.insertText( "\n + " + tquery.value(0).toString(),unBoldFormat );

      cursor = table->cellAt( index_row - 1 , 1 ).lastCursorPosition();
      cursor.insertText( "\n + " + tquery.value(1).toString(),unBoldFormat );

    }
    
  }
}
