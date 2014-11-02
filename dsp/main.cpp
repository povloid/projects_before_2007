// 

#include "dsp.h"

#include <QMessageBox>
#include <QSplashScreen>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QObject>
#include <QLocale>

#include <QApplication>
#include <QTextEncoder> 

#include "dspconst.h"

int main( int argc, char ** argv )
{
  QApplication a( argc, argv );

  // Создаем заставку
  QPixmap pixmap(piconPath +"/splash.png");
  QSplashScreen *splash = new QSplashScreen(pixmap);
  splash->show();
    
  splash->clearMessage();
  splash->showMessage( QObject::tr("Initialize codecs (UTF-8) ..."),
		       Qt::AlignLeft | Qt::AlignTop, Qt::blue);
  a.processEvents();

  //Настройка приложения на KOI8-R
  //QTextCodec *russCodec = QTextCodec::codecForName("KOI8-R");
  QTextCodec *russCodec = QTextCodec::codecForName("UTF-8");
  QTextCodec::setCodecForTr(russCodec); 
  QTextCodec::setCodecForCStrings(russCodec);
  //QTextCodec::setCodecForLocale(russCodec);

  splash->clearMessage();
  splash->showMessage(QObject::tr("Кодеки установлены."), 	
		  Qt::AlignLeft | Qt::AlignTop, Qt::blue);
  a.processEvents();

  QLocale::setDefault( QLocale::Russian );

  // Создание соединения с базой данных -----------------------------------------------
  splash->clearMessage();
  splash->showMessage(QObject::tr("Устанавливаем соединение с базой данных."), 	
		  Qt::AlignLeft | Qt::AlignTop, Qt::blue);
  a.processEvents();

  // Создаем соединение
  QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE","DSP");
  // >>
  //   Место для вставки диалога соединения с базой данных
  // <<

  db.setHostName("127.0.0.1");
  db.setDatabaseName("d:\\dsp.fdb");
  db.setUserName("SYSDBA");
  db.setPassword("masterkey");

  //     db.setHostName("localhost");
  //     db.setDatabaseName("/home/mybase/dsp.fdb");
  //     db.setUserName("SYSDBA");
  //     db.setPassword("masterkey");


  if (!db.open()) {
    QMessageBox::critical(0, qApp->tr("Cannot open database"),
			  qApp->tr("Unable to establish a database connection.\n"
				   "This example needs SQLite support. Please read "
				   "the Qt SQL driver documentation for information how "
				   "to build it.\n\n"
				   "Click Cancel to exit."), QMessageBox::Cancel,
			  QMessageBox::NoButton);
    return false;
  }
  // ..................................................................................
  splash->clearMessage();
  splash->showMessage(QObject::tr("Соединение с базой данных установлено."),
		  Qt::AlignLeft | Qt::AlignTop, Qt::blue);
  a.processEvents();

  QSqlQuery query( db );
  // Создаем жестко прошитые записи, необходимые для расчетов
  query.exec( QObject::tr("INSERT INTO S_ED_MASS ( ED, FULL_NAME ) VALUES ( 'тн', 'тонн' )"));

  query.exec( QObject::tr("INSERT INTO S_POST ( KOD, NAME ) VALUES ( 1, 'Жанатас' )"));
  query.exec( QObject::tr("INSERT INTO S_POST ( KOD, NAME ) VALUES ( 2, 'Новотроитск' )"));
  query.exec( QObject::tr("INSERT INTO S_POST ( KOD, NAME ) VALUES ( 3, 'Смычка' )"));

  query.exec( QObject::tr("INSERT INTO S_POTR ( KOD, NAME ) VALUES ( 1, 'Кант' )"));
  query.exec( QObject::tr("INSERT INTO S_POTR ( KOD, NAME ) VALUES ( 2, 'Беканор' )"));

  query.exec( QObject::tr("INSERT INTO S_DEP ( KOD, NAME ) VALUES ( 0 , 'Неопределенный' )"));
  query.exec( QObject::tr("INSERT INTO S_DEP ( KOD, NAME ) VALUES ( 1 , '?' )"));
  query.exec( QObject::tr("INSERT INTO S_DEP ( KOD, NAME ) VALUES ( 4 , '?' )"));
  query.exec( QObject::tr("INSERT INTO S_DEP ( KOD, NAME ) VALUES ( 5 , 'Печной' )"));
  query.exec( QObject::tr("INSERT INTO S_DEP ( KOD, NAME ) VALUES ( 8 , '?' )"));
  query.exec( QObject::tr("INSERT INTO S_DEP ( KOD, NAME ) VALUES ( 7 , '?' )"));
  query.exec( QObject::tr("INSERT INTO S_DEP ( KOD, NAME ) VALUES ( 28 , '?' )"));
  query.exec( QObject::tr("INSERT INTO S_DEP ( KOD, NAME ) VALUES ( 32 , '?' )"));
  query.exec( QObject::tr("INSERT INTO S_DEP ( KOD, NAME ) VALUES ( 37 , 'ИВЦ' )"));

  //query.exec( QObject::tr("INSERT INTO S_VYR ( KOD, NAME, KOD_DEP, ED) VALUES ( 1, 'Фосфор',5,'тн')"));
  query.exec( QObject::tr("INSERT INTO S_VYR ( KOD, NAME, KOD_DEP, ED) VALUES ( 2, 'ТПФП',0,'тн')"));
  query.exec( QObject::tr("INSERT INTO S_VYR ( KOD, NAME, KOD_DEP, ED) VALUES ( 3, 'ТФК',8,'тн')"));
  query.exec( QObject::tr("INSERT INTO S_VYR ( KOD, NAME, KOD_DEP, ED) VALUES ( 4, 'ТФК пищевая',0,'тн')"));
  query.exec( QObject::tr("INSERT INTO S_VYR ( KOD, NAME, KOD_DEP, ED) VALUES ( 5, 'Феррофосфор',5,'тн')"));
  query.exec( QObject::tr("INSERT INTO S_VYR ( KOD, NAME, KOD_DEP, ED) VALUES ( 6, 'Граншлак',0,'тн')"));
  query.exec( QObject::tr("INSERT INTO S_VYR ( KOD, NAME, KOD_DEP, ED) VALUES ( 7, 'Перекачка шлама',0,'тн')"));
  query.exec( QObject::tr("INSERT INTO S_VYR ( KOD, NAME, KOD_DEP, ED) VALUES ( 8, 'Перекачка фосфора',0,'тн')"));

  query.exec( QObject::tr("INSERT INTO S_PROD_GR ( KOD, NAME, K , ED) VALUES ( 1, 'Фосфор общий', 0 ,'тн')"));
  query.exec( QObject::tr("INSERT INTO S_PROD_GR ( KOD, NAME, K , ED) VALUES ( 2, 'ТПФП всего', 0 ,'тн')"));
  query.exec( QObject::tr("INSERT INTO S_PROD_GR ( KOD, NAME, K , ED) VALUES ( 3, 'ТФК всего', 0 ,'тн')"));
  query.exec( QObject::tr("INSERT INTO S_PROD_GR ( KOD, NAME, K , ED) VALUES ( 4, 'ТФК пищевая', 0 ,'тн')"));
  query.exec( QObject::tr("INSERT INTO S_PROD_GR ( KOD, NAME, K , ED) VALUES ( 5, 'Феррофосфор', 0 ,'тн')"));
  query.exec( QObject::tr("INSERT INTO S_PROD_GR ( KOD, NAME, K , ED) VALUES ( 6, 'Граншлак', 0 ,'тн')"));
  query.exec( QObject::tr("INSERT INTO S_PROD_GR ( KOD, NAME, K , ED) VALUES ( 7, 'Шлам всего', 0 ,'тн')"));

  query.exec( QObject::tr("INSERT INTO S_PROD_PGR ( NAME, K , KODM , KOD_DEP ) " 
			  "VALUES ( 'залито', 0 , 1 , 32 )"));
  query.exec( QObject::tr("INSERT INTO S_PROD_PGR ( NAME, K , KODM , KOD_DEP ) " 
			  "VALUES ( 'в хранилищах', 0 , 1 , 7 )"));
  query.exec( QObject::tr("INSERT INTO S_PROD_PGR ( NAME, K , KODM , KOD_DEP ) " 
			  "VALUES ( 'покраска', 0 , 1 , 28 )"));
  query.exec( QObject::tr("INSERT INTO S_PROD_PGR ( NAME, K , KODM , KOD_DEP ) " 
			  "VALUES ( 'склад на колесах', 0 , 1 , 22 )"));
  query.exec( QObject::tr("INSERT INTO S_PROD_PGR ( NAME, K , KODM , KOD_DEP ) " 
			  "VALUES ( 'в дозаторе', 0 , 1 , 8 )"));

  query.exec( QObject::tr("INSERT INTO S_PROD_PGR ( NAME, K , KODM , KOD_DEP ) " 
			  "VALUES ( 'в т.ч. в силосах', 0 , 2 , 0 )"));
  query.exec( QObject::tr("INSERT INTO S_PROD_PGR ( NAME, K , KODM , KOD_DEP ) " 
			  "VALUES ( 'на пантусе', 0 , 2 , 0 )"));
  query.exec( QObject::tr("INSERT INTO S_PROD_PGR ( NAME, K , KODM , KOD_DEP ) " 
			  "VALUES ( 'на колесах', 0 , 2 , 1 )"));
  query.exec( QObject::tr("INSERT INTO S_PROD_PGR ( NAME, K , KODM , KOD_DEP ) " 
			  "VALUES ( 'пищевой ТПФН', 0 , 2 , 0 )"));

  query.exec( QObject::tr("INSERT INTO S_PROD_PGR ( NAME, K , KODM , KOD_DEP ) " 
			  "VALUES ( 'в т.ч. ТФК', 0 , 3 , 0 )"));

  query.exec( QObject::tr("INSERT INTO S_PROD_PGR ( NAME, K , KODM , KOD_DEP ) " 
			  "VALUES ( 'Всего', 0 , 4 , 0 )"));

  query.exec( QObject::tr("INSERT INTO S_PROD_PGR ( NAME, K , KODM , KOD_DEP ) " 
			  "VALUES ( 'Всего', 0 , 5 , 5 )"));

  query.exec( QObject::tr("INSERT INTO S_PROD_PGR ( NAME, K , KODM , KOD_DEP ) " 
			  "VALUES ( 'Всего', 0 , 6 , 0 )"));

  query.exec( QObject::tr("INSERT INTO S_PROD_PGR ( NAME, K , KODM , KOD_DEP ) " 
			  "VALUES ( 'в т.ч.', 0 , 7 , 7 )"));
  query.exec( QObject::tr("INSERT INTO S_PROD_PGR ( NAME, K , KODM , KOD_DEP ) " 
			  "VALUES ( 'в дозаторах', 0 , 7 , 8 )"));
  splash->clearMessage();
  splash->showMessage(QObject::tr("Создаем главное окно приложения."), 	
		      Qt::AlignLeft | Qt::AlignTop, Qt::blue);
  a.processEvents();

  // Создаем главное окно
  DSPWindow dsp;

  dsp.show();

  // Убираем заставку
  splash->finish(&dsp);
  delete splash;

  return a.exec();
}
