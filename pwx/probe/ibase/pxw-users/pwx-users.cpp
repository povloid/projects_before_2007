#include <ibase.h>
#include <iostream>
#include <string>

using namespace std;

//Эта структура предназначена для хранения переменных типа SQL_VARYING
#define SQL_VARCHAR(len) struct {short vary_length; char vary_string[(len)+1];}	


//Начало главной процедуры программы
int main (void)
{
    //Константы, необходимые для работы с базой данных
    //char *dbname = "192.168.0.77:/home/fdb/PWCRDB.FDB";
    char *dbname = "192.168.0.2:c:\\PowerNew\\PWCRDB.FDB";
    char *uname = "sysdba"; 
    char *upass = "masterkey";
    char *query = "select us, passw from us";
    
    //Переменные для работы с базой данных
    isc_db_handle 	db_handle = NULL;
    isc_tr_handle	transaction_handle = NULL;
    isc_stmt_handle 	statment_handle = NULL;
    char 	dpb_buffer[256], *dpb, *p;
    short 	dpb_length;
    ISC_STATUS	status_vector[20];
    XSQLDA	*isqlda, *osqlda;
    long 	fetch_code;
    short	o_ind[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    //Остальные переменные
   	SQL_VARCHAR(20)   us_us; 
	SQL_VARCHAR(20)   us_passw; 
    //вывод сообщения на экран
    cout<<"Начали"<<endl;
    
    //Подключаемся к базе данных в два этапа:
     // 1. Создаем такназываемый database parametr buffer, необходимый
     //    для подключения к базе данных
        dpb=dpb_buffer;
	*dpb++ = isc_dpb_version1;
	*dpb++ = isc_dpb_user_name;
	*dpb++ = strlen(uname);
	for (p=uname; *p;) *dpb++ = *p++;	
	*dpb++ = isc_dpb_password;
	*dpb++ = strlen(upass);
	for (p=upass; *p;) *dpb++ = *p++;
	dpb_length = dpb- dpb_buffer;
     // 2. Подключаемся к базе денных
      	isc_attach_database(status_vector,
			strlen(dbname),
			dbname,
			&db_handle,
			dpb_length,
			dpb_buffer);

	//Далее идет проверка стандартная для большенства API-функций проверка и анализ результата
		if (status_vector[0] == 1 && status_vector[1])
		{
			cout<<"Ошибка!"<<endl;
			isc_print_status(status_vector);
			cout<<"Приложение завершено с ошибкой."<<endl;
			return (1); //Выход из приложения
		}
	//Если подключение к базе денных произошло успешно, начинается транзакция
		if (db_handle)
		{
			isc_start_transaction(status_vector,
					&transaction_handle,
					1,
					&db_handle,
					0,
					NULL);
			//Проверка на ошибку
			if (status_vector[0] == 1 && status_vector[1])
			{
				cout<<"Ошибка!"<<endl;
				isc_print_status(status_vector);
				cout<<"Приложение завершено с ошибкой."<<endl;
				return (1);
			}
		}
	//Далее инициализируются структуры, которые будут заполняться результатами 
	//запроса
	 osqlda = (XSQLDA *) malloc (XSQLDA_LENGTH(6));
	 osqlda->version=SQLDA_VERSION1;
	 osqlda->sqln=2;
	 //Имя пользователя
	 osqlda->sqlvar[0].sqldata = (char *)&us_us;
	 osqlda->sqlvar[0].sqltype = SQL_VARYING;
	 osqlda->sqlvar[0].sqlind = &o_ind[0];
	 //Пароль
	 osqlda->sqlvar[1].sqldata = (char *)&us_passw;
	 osqlda->sqlvar[1].sqltype = SQL_VARYING;
	 osqlda->sqlvar[1].sqlind = &o_ind[1];

	//Подготовка к выполнению запроса сервером
	 isc_dsql_allocate_statement(
			 status_vector,
			 &db_handle,
			 &statment_handle);
	 	//Проверка на ошибку
	 	if (status_vector[0] == 1 && status_vector[1])
			{
				cout<<"Ошибка!"<<endl;
				isc_print_status(status_vector);
				cout<<"Приложение завершено с ошибкой."<<endl;
				return (1);
			}
	 
	 isc_dsql_prepare(
			 status_vector,
			 &transaction_handle,
			 &statment_handle,
			 0,
			 query,
			 SQL_DIALECT_V6,
			 osqlda);
		//Проверка на ошибку
	 	if (status_vector[0] == 1 && status_vector[1])
			{
				cout<<"Ошибка!"<<endl;
				isc_print_status(status_vector);
				cout<<"Приложение завершено с ошибкой."<<endl;
				return (1);
			}
	 isc_dsql_execute2(
			 status_vector,
			 &transaction_handle,
			 &statment_handle,
			 1,
			 NULL,
			 NULL);
		//Проверка на ошибку
	 	if (status_vector[0] == 1 && status_vector[1])
			{
				cout<<"Ошибка!"<<endl;
				isc_print_status(status_vector);
				cout<<"Приложение завершено с ошибкой."<<endl;
				return (1);
			}
	//здесь начинается таблица HTML-документа
		cout<<"Результат запроса:"<<endl;
		cout<<"Ном.\tПольз.\t\tПароль"<<endl;
	//После исполнения запроса сервер готов к передаче данных. "Доставкой" дан-
	//ных занимается функция isc_dsql_fetch():
	int index=0;
	while ((fetch_code = isc_dsql_fetch(
						status_vector,
						&statment_handle,
						1,
						osqlda))==0)
	//Для строковых переменных требуется корректно установить длину, так как размер 
	//возвращаемых данных не всегда соответствуем максимально возможному, и если этого
	//не сделать, то вместе с реальными данными можно получить "мусор" из памяти или остатки
	//предидущих строк:
	
	{
		us_us.vary_string[us_us.vary_length]='\0';			
		us_passw.vary_string[us_passw.vary_length]='\0';
		cout<<index++<<"\t"<<us_us.vary_string<<"\t"<<us_passw.vary_string<<endl;
	}

	//После вывода всех данных необходимо завершить документ
	cout<<"\n Запрос выполнен."<<endl;
	//Проверка на ошибку
	 	if (status_vector[0] == 1 && status_vector[1])
			{
				cout<<"Ошибка!"<<endl;
				isc_print_status(status_vector);
				cout<<"Приложение завершено с ошибкой."<<endl;
				return (1);
			}
	free(osqlda);
	isc_dsql_free_statement(status_vector,&statment_handle,DSQL_drop);
	//Проверка на ошибку
	 	if (status_vector[0] == 1 && status_vector[1])
			{
				cout<<"Ошибка!"<<endl;
				isc_print_status(status_vector);
				cout<<"Приложение завершено с ошибкой."<<endl;
				return (1);
			}
	
	//Затем завершить транзакцию и выйти
	if (transaction_handle)
		{
			isc_commit_transaction(status_vector,&transaction_handle);
		}
	//Проверка на ошибку
	 	if (status_vector[0] == 1 && status_vector[1])
			{
				cout<<"Ошибка!"<<endl;
				isc_print_status(status_vector);
				cout<<"Приложение завершено с ошибкой."<<endl;
				return (1);
			}
	if (db_handle) isc_detach_database(status_vector,&db_handle);
	//Проверка на ошибку
	 	if (status_vector[0] == 1 && status_vector[1])
			{
				cout<<"Ошибка!"<<endl;
				isc_print_status(status_vector);
				cout<<"Приложение завершено с ошибкой."<<endl;
				return (1);
			}
	cout<<"Транзакция завершена."<<endl;
		


	
	return 0;
}// end of main
