//////////////////////////////////////////////////////////////////////////////
//Исходный код демона pwx 1.0  . 
//Программа осуществляет прием данных с порта COM1, поступающих с ГПП через 
//БПН-24, первичную обработку и запись полученных данных в базу. 
//
// автор Копыченко П.Г.  		2005 г.
//
// Исходный код прошу считать свободным и бесплатным. GNU!!!
//
//////////////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <ibase.h>
#include <time.h>

/* установки скорости передачи данных объявлены в <asm/termbits.h>, 
    который включается в <termios.h> */
#define BAUDRATE B600

/* измените эти объявления для корректного указания порта */
#define MODEMDEVICE "/dev/ttyS0"
//#define _POSIX_SOURCE 1 /* POSIX-совместимый источник */

#define FALSE 0
#define TRUE 1

#define LEN_MASS 80

volatile int STOP=FALSE; 

/////////////////////////////////////////////////////////////////////////////
//Мои типы //////////////////////////////////////////////////////////////////
union ptype {
	    unsigned char p_input[LEN_MASS];
	    short int power[LEN_MASS/2];
	};

//--------------------------------------------------------------------------

main()
{
    printf("Запуск программы pwx 1.0\n Инициализация устройства COM1, фаил устройства /dev/ttyS0 -->");
    //////////////////////////////////////////////////////////////////////////
    /// Инициализация порта COM 1 ////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////	
	int fd; //Переменная дискриптора файла устройства
       struct termios oldtio,newtio;
      
        /* 
          Открываем устройство модема для чтения и записи как неуправляющий
          терминал (tty), поскольку мы не хотим завершать процесс когда
          помехи в линии посылают сигнал CTRL-C.
        */
         fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY ); 
         if (fd <0) {perror(MODEMDEVICE); exit(-1); }
        
         tcgetattr(fd,&oldtio);		 /* save current serial port settings */
         bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */
        
        /* 
          BAUDRATE: устанавливает скорость передачи данных в bps.
                    Вы также могли бы использовать cfsetispeed и cfsetospeed.
          CRTSCTS : аппаратное управление выводным потоком данных (используется
                    только если кабель обладает всеми необходимыми сигнальными
                    линиями. См. секцию 7 Serial-HOWTO)
          CS8     : 8n1 (8 бит, без четности, 1 стоп-бит)
          CLOCAL  : локальное подключение, нет управления модемом
          CREAD   : разрешает прием символов
        */
         newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD | CSTOPB | PARENB;
         
        /*
          IGNPAR  : игнорировать байты с ошибками четности
          ICRNL   : отобразить символ CR на NL (иначе ввод CR на другом
                    компьютере не будет завершать ввод)
          иначе установить устройство как raw (нет обработки ввода)
        */
         newtio.c_iflag = IGNPAR;
         
        /*
         Raw вывод (нет обработки вывода).
        */
         newtio.c_oflag = 0;
         
        /*
          ICANON  : разрешить канонический ввод
          заблокировать любое функционирование "эхо", и не посылать сигналы
          к вызывающей программе
        */
         //newtio.c_lflag = ICANON;
         
	 
	 newtio.c_cc[VTIME]    = 0;   /* посимвольный таймер не используется */
         newtio.c_cc[VMIN]     = 1;   /* блокировка чтения до тех пор пока
                                        не будут приняты 5 символов */
	
        /*  
	 теперь очищаем модемную линию и активируем наши установки порта
        */
         tcflush(fd, TCIFLUSH);
         tcsetattr(fd,TCSANOW,&newtio);	
	 
	 printf(" OK \n");
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    /// Инициализация порта COM 1 завершена///////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    
    //Переменные для работы с полученными данными с порта	
	    unsigned char  input_com[LEN_MASS]; 
	    short int equipment;	    	    
	//Объединение массивов
	    union ptype pwx;		
	    int k=0;
    //Переменные для формирования запроса
	    time_t t;
	    struct tm *MyTime; //Структура tm для получения даты и времени
	    float p_float=0;
	    char p_str[5]="0.000";	    
    //////////////////////////////////////////////////////////////////////////
    /// подготовка работы с базой данных FDB /////////////////////////////////
    //////////////////////////////////////////////////////////////////////////	
    printf("Инициализация Базы данных ");
     //Константы, необходимые для работы с базой данных                          
    	 char *dbname = "192.168.0.186:/mnt/hda11/power/pwcrdb.fdb";                                 
            printf("%s --> ",dbname);
	 char *uname = "sysdba";                                                     
	 char *upass = "masterkey";                                                  
	 char query[140]="insert into  CROND (DATE1, TIME1, STOVE5, STOVE6, STOVE7, STOVE8, EQUIPMENT) values ('15.08.2005','12:12:00',0.000,0.000,0.000,0.000,0.000)";; 
	 char *ptr_query=&query[77]; //Указатель на начало массива
	 
	//Раскоментируй если хочешь узнать размер масива в памяти
	// printf("sizeof(query)=%d\n",sizeof(query));
		                                                                                      
	 //Переменные для работы с базой данных                                      
	 isc_db_handle       db_handle = NULL;                                       
	 isc_tr_handle       transaction_handle = NULL;                              
	 isc_stmt_handle     statment_handle = NULL;                                 
	 char        dpb_buffer[256], *dpb, *p;                                      
	 short       dpb_length;                                                     
	 ISC_STATUS  status_vector[20];                                              
	 
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
				             printf("1. Ошибка!\n");                                                       
				             isc_print_status(status_vector);                                             
				             printf("Приложение завершено с ошибкой.\n");                               
				             return (1); //Выход из приложения                                            
				         }
	printf("OK \n");
    //////////////////////////////////////////////////////////////////////////
    /// GO!!! ////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////	
	printf("----> Начали:\n");
	//printf("sizeof(char)=%d\tsizeof(int)=%d\n",sizeof(char),sizeof(short int));
	//printf("sizeof(pw)=%d\n",sizeof(pwx));
	printf("Прием:\n");
	
	int i=0,str=0;
		
	for (;;)  		//Цыкл приема 
	{
	/*------------0-------------*/
	/*Прием данных с ГПП        */
	for(i=0;i<LEN_MASS;i++) input_com[i]=0; //Обнулить весь массив
	
	printf("Пр. заг. ---> ");	//Прием заголовка		
	k=read(fd,input_com,LEN_MASS);  //-----^
	sleep(1);  	
	 if(input_com[0]==112)
	    {  
		printf("1 = 112 ->");
		k=read(fd,input_com,LEN_MASS);
		sleep(1);
	    	if(input_com[0]==111)						
        	    {		        			
			printf("2 = 111 ->");
			if(input_com[1]==119)
			 {
			    printf("3 = 119 ->> OK \n");	    
    			    input_com[LEN_MASS]='\0';	
			    printf("--------------------------------------------------------------------------\n");
			    for (i=0;i<LEN_MASS;i++)
	    		    {
				pwx.p_input[i]=input_com[i+4];		    
	    			printf("%2.1d =%3.1d; ",i,pwx.p_input[i]);  
				str++;
				if (str>=8)
				{
					str=0;
					printf("\n");
				}
			    }
			    //Прием массива закончен, результат приема в массиве pwx.power
			    printf("\n--------------------------------------------------------------------------\n");
			    printf("Печь 5 - %d       Печь 6 - %d       Печь 7 - %d       Печь 8 - %d\n",
							pwx.power[16],
							pwx.power[17],
							pwx.power[18],
							pwx.power[19]);
			    equipment=0;				
			    for(i=4;i<15;i++) equipment=equipment+pwx.power[i];
			    printf("Оборудование - %d\n",equipment);
			    
			    /////////////////////////////////////////////////////////////////
			    //Теперь работаем с базой данных ////////////////////////////////
			    /////////////////////////////////////////////////////////////////
			    
			    //Подготовка запроса
				t=time(NULL);          	    //Получение времени со дня рождения UNIX 
				MyTime=localtime(&t);       //Преобразовать значение времени в структуру по указателю MyTime
				printf("Время: %d:%d:%d\t Дата: %d.%d.%d\n",
								MyTime->tm_hour,	//час
							        MyTime->tm_min,		//минута
							        MyTime->tm_sec,		//секунда
								MyTime->tm_mday,	//день
								MyTime->tm_mon,		//месяц
								MyTime->tm_year+1900);	//год			
			    
				sprintf(ptr_query,"values ('%d.%d.%d','%d:%d:%d','%2.2f','%2.2f','%2.2f','%2.2f','%2.2f')",
							    MyTime->tm_mday,		//день
							    MyTime->tm_mon,		//месяц
							    MyTime->tm_year+1900,	//год
							    MyTime->tm_hour,		//час
							    MyTime->tm_min,		//минута
							    MyTime->tm_sec,		//секунда
							    (float)pwx.power[16]/100,
							    (float)pwx.power[17]/100,
							    (float)pwx.power[18]/100,
							    (float)pwx.power[19]/100,
							    (float)equipment/100);
				
				
				//printf("%s\n",ptr_query);
			    //-------------------
			    //Печать запроса 
				printf("Посылаю запрос:\n%s\n",query);
			    //----------------------------------------------------------------------
				    transaction_handle = NULL;                              
				    statment_handle = NULL;
				//Если подключение к базе денных произошло успешно, начинается транзакция                    
			             if (db_handle)                                                                       
				         {                                                                                    
				             isc_start_transaction(status_vector,                                         
				            			    &transaction_handle,                                         
				            			    1,                                                           
				            			    &db_handle,                                                  
				            			    0,                                                           
				            			    NULL);
					}
	    			 //Проверка на ошибку                                                         
				     if (status_vector[0] == 1 && status_vector[1])                               
				         {                                                                            
				             printf("2. Ошибка начала транзакции! \n");                                               
				             isc_print_status(status_vector);                                     
				             printf("Приложение завершено с ошибкой.\n");                       
				             return (1);                                                          
				         }                                                                            
			 //Подготовка к выполнению запроса сервером                                        
			           isc_dsql_allocate_statement(status_vector,                                                              
					    	     &db_handle,                                                                 
						      &statment_handle);                                                          
				 //Проверка на ошибку                                                         
				     if (status_vector[0] == 1 && status_vector[1])                               
				         {                                                                            
				             printf("3. Ошибка подготовки к выполнению запроса!\n");                                               
				             isc_print_status(status_vector);                                     
				             printf("Приложение завершено с ошибкой.\n");                       
				             return (1);                                                          
				         }
			///////////////////////////////////////////////////////////////		 
			 isc_dsql_prepare(status_vector,                                                              
			        		&transaction_handle,                                                        
			    			&statment_handle,                                                           
			    			0,                                                                          
			        		query,                                                                      
			        		SQL_DIALECT_V6,                                                             
						NULL);                                                                              
				//Проверка на ошибку                                                         
				     if (status_vector[0] == 1 && status_vector[1])                               
				         {                                                                            
				             printf("4. Ошибка!\n");                                               
				             isc_print_status(status_vector);                                     
				             printf("Приложение завершено с ошибкой.\n");                       
				             return (1);                                                          
			        	}
			//////////////////////////////////////////////////////////////		
			 isc_dsql_execute(status_vector,                                                       
				             &transaction_handle,                                                 
					     &statment_handle,                                                    
				             1,                                                                   
				             NULL);
				//Проверка на ошибку                                                         
				     if (status_vector[0] == 1 && status_vector[1])                               
				         {                                                                            
				             printf("5. Ошибка!\n");                                               
				             isc_print_status(status_vector);                                     
				             printf("Приложение завершено с ошибкой.\n");                       
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
				             printf("Ошибка завершения транзакции!\n");                                               
				             isc_print_status(status_vector);                                     
				             printf("Приложение завершено с ошибкой.\n");                       
				             return (1);                                                          
			        	}
	
				   
			    //Завершение одного цикла
			    str=0;
			    printf("\v");
			}    
		    }
	    } 
	  sleep(1); /*Задержка на 1 секунду*/
	}
	
	if (db_handle) isc_detach_database(status_vector,&db_handle);                         
				//Проверка на ошибку 
				     if (status_vector[0] == 1 && status_vector[1])                               
				         {                                                                            
				             printf("Ошибка!\n");                                               
				             isc_print_status(status_vector);                                     
				             printf("Приложение завершено с ошибкой.\n");                       
				             return (1);                                                          
			        	}
         /* восстановление старых установок порта */
         tcsetattr(fd,TCSANOW,&oldtio);
}
