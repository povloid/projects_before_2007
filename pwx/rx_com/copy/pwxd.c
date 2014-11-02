//////////////////////////////////////////////////////////////////////////////
//Исходный код демона pwx 1.0  . 
//Программа осуществляет прием данных с порта COM1, поступающих с ГПП через 
//БПН-24, а также первичную обработку и запись полученных данных в базу. 
//
// автор Копыченко П.Г.  		2005 г.
//
//      Исходный код прошу считать свободным и бесплатным. GNU!!!
//
//////////////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <ibase.h>
#include <time.h>
#include <errno.h>

/* установки скорости передачи данных объявлены в <asm/termbits.h>, 
    который включается в <termios.h> */
#define BAUDRATE B600

/* измените эти объявления для корректного указания порта */
#define MODEMDEVICE "/dev/ttyS0"
//#define _POSIX_SOURCE 1 /* POSIX-совместимый источник */

#define FALSE 0
#define TRUE 1

#define LEN_MASS 80
// Путь к файлу конфигурации pwx.conf

//#define OPT_FILE "./pwx.conf"
#define OPT_FILE "/etc/pwx/pwx.conf" 

#define MAXWORDLEN 128

volatile int STOP=FALSE; 

/////////////////////////////////////////////////////////////////////////////
//Мои типы //////////////////////////////////////////////////////////////////
union ptype {
	    unsigned char p_input[LEN_MASS];
	    short int power[LEN_MASS/2];
	};

//Переменные конфигурации---------------------------------------------------

//Константы, необходимые для работы с базой данных                          
//    	 char *dbname = "192.168.0.186:/mnt/hda11/power/pwcrdb.fdb";                                 
//	 char *uname = "sysdba";                                                     
//	 char *upass = "masterkey";

//int  OPT_SERVER;
	 
char OPT_SERVERUSER[20];
char OPT_SERVERPASSWORD[20];                                                                        
char OPT_SERVERNAME[128];
 
//Функции-------------------------------------------------------------------

int get_options(void);
int parse_options(FILE*);																				    
int parse_option(FILE*,char*,int*);
int getword(FILE*,char*,int*,char*);

//--------------------------------------------------------------------------

main()
{
    printf("Запуск программы pwx 1.0\nИнициализация устройства COM1, фаил устройства /dev/ttyS0 -->");
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
	
	printf("Чтение файла конфигурации %s ",OPT_FILE);
	
	if (get_options()) return (1); 	//Прочитать параметры с файла pwx.conf
	
	printf(" --> OK\n");
	    	
	printf("Инициализация Базы данных: %s ",OPT_SERVERNAME);
	//Переменные для формирования запроса                  
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
			           *dpb++ = strlen(OPT_SERVERUSER);                                                                      
			           for (p=OPT_SERVERUSER; *p;) *dpb++ = *p++;                                                            
			               *dpb++ = isc_dpb_password;                                                                   
			           *dpb++ = strlen(OPT_SERVERPASSWORD);                                                                      
			           for (p=OPT_SERVERPASSWORD; *p;) *dpb++ = *p++;                                                            
			               dpb_length = dpb- dpb_buffer;                                                                
			    // 2. Подключаемся к базе денных                                                                
			            isc_attach_database(status_vector,                                                           
			                            strlen(OPT_SERVERNAME),                                                              
			                            OPT_SERVERNAME,                                                                      
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
	printf("--> OK \n");
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
								MyTime->tm_mon+1,		//месяц
								MyTime->tm_year+1900);	//год			
			    
				sprintf(ptr_query,"values ('%d.%d.%d','%d:%d:%d','%2.2f','%2.2f','%2.2f','%2.2f','%2.2f')",
							    MyTime->tm_mday,		//день
							    MyTime->tm_mon+1,		//месяц
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

//////////////////////////////////////////////////////////////////////////////////////////////////
//Взять параметры из файла конфигурации pwx.conf
//////////////////////////////////////////////////////////////////////////////////////////////////

int get_options(void)
{
	FILE* optfile = NULL;	//Дискриптор файла
        int result;		//результат выполнения функции
	
	 optfile = fopen(OPT_FILE,"rt");
	 
	 if(!optfile) 		//Проверка открытия файла
	         {
		         printf("\nОшибка. Немогу прочитать фаил конфигурации:'%s'\n",
						 OPT_FILE);
			 printf("Приложение завершено с ошибкой.\n");			 
		         return 1;
		 }
	
	//result = parse_options( optfile ); 			     
	
	parse_options(optfile);
		 
	//Закрыть фаил	
	fclose(optfile);
	
	//return result;
	return (0);
}

int parse_options(FILE* optfile)
{
    char word[MAXWORDLEN*2];
    int newlinep, result;
	
	    // Сканировать опции...
	        for( ;; )
		    {
		            // no options - no actions
		            result=getword(optfile,word,& newlinep,OPT_FILE);
		            if(result==-1)      // error
				return 0;
			    else if( result == 0 )  // no words
			        return 1;
									    
			    // not a line beginning
			    if(!newlinep)
			        {
			            printf("Ошибка: Unexpected option '%s'\n",word);
			            return 0;
			        }
			    // oki, check the option
			    if(!parse_option(optfile,word,& newlinep))
			        return 0;
		    }
}

int parse_option(FILE* optfile,char* word,int* newlinep)
{
     if(strcasecmp(word,"servername")==0)                                                 
         {                                                                                                
	    if(getword(optfile,word,newlinep,OPT_FILE) != 1 || *newlinep)                         
		         {                                                                                            
			    printf("Ошибка: Неправильный параметр 'servername'\n" );                 
				                 return 0;                                                                                
			 }                                                                                            
		// Это вариант для числа	 			
		// OPT_SERVER = atoi( word );                                                               
		strcpy( OPT_SERVERNAME, word ); 
         }
    else  if(strcasecmp(word,"user")==0)                                                 
         {                                                                                                
	    if(getword(optfile,word,newlinep,OPT_FILE) != 1 || *newlinep)                         
		         {                                                                                            
			    printf("Ошибка: Неправильный параметр 'user'\n" );                 
				                 return 0;                                                                                
			 }                                                                                            
			 			                                                            
		strcpy( OPT_SERVERUSER, word ); 
         }
    else  if(strcasecmp(word,"password")==0)                                                 
         {                                                                                                
	    if(getword(optfile,word,newlinep,OPT_FILE) != 1 || *newlinep)                         
		         {                                                                                            
			    printf("Ошибка: Неправильный параметр 'password'\n" );                 
				                 return 0;                                                                                
			 }                                                                                            
			 			                                                               
		strcpy( OPT_SERVERPASSWORD, word ); 
         }	 	   
      else                                                                                             
         {                                                                                                
	     fprintf( stderr, "Ошибка: Неизвесный параметр '%s'\n", word );                                     
		 return 0;                                                                                    
	 }                                                                                                
			                                                                                                           
     return 1;     
}

// original code by PPPD
// оригинал кода взял из PPPD
/*
 * Read a word from a file.
 * Words are delimited by white-space or by quotes (" or ').
 * Quotes, white-space and \ may be escaped with \.
 * \<newline> is ignored.
 */

// returns 1 on success, 0 - no morr words, -1 - error 
int getword( FILE* f, char* word, int* newlinep, char* filename )
{
    int c, len, escape;
    int quoted, comment;
    int value, digit, got, n;

#define isoctal(c) ((c) >= '0' && (c) < '8') 

    *newlinep = 0;
    len = 0;
    escape = 0;
    comment = 0;

    /*
     * First skip white-space and comments.
     */
    for (;;) {
	c = getc(f);
	if (c == EOF)
	    break;

	/*
	 * A newline means the end of a comment; backslash-newline
	 * is ignored.  Note that we cannot have escape && comment.
	 */
	if (c == '\n') {
	    if (!escape) {
		*newlinep = 1;
		comment = 0;
	    } else
		escape = 0;
	    continue;
	}

	/*
	 * Ignore characters other than newline in a comment.
	 */
	if (comment)
	    continue;

	/*
	 * If this character is escaped, we have a word start.
	 */
	if (escape)
	    break;

	/*
	 * If this is the escape character, look at the next character.
	 */
	if (c == '\\') {
	    escape = 1;
	    continue;
	}

	/*
	 * If this is the start of a comment, ignore the rest of the line.
	 */
	if( c == '#' || c == ';' )
	{
	    comment = 1;
	    continue;
	}

	/*
	 * A non-whitespace character is the start of a word.
	 */
	if (!isspace(c))
	    break;
    }

    /*
     * Save the delimiter for quoted strings.
     */
    if (!escape && (c == '"' || c == '\'')) {
        quoted = c;
	c = getc(f);
    } else
        quoted = 0;

    /*
     * Process characters until the end of the word.
     */
    while (c != EOF) {
	if (escape) {
	    /*
	     * This character is escaped: backslash-newline is ignored,
	     * various other characters indicate particular values
	     * as for C backslash-escapes.
	     */
	    escape = 0;
	    if (c == '\n') {
	        c = getc(f);
		continue;
	    }

	    got = 0;
	    switch (c) {
	    case 'a':
		value = '\a';
		break;
	    case 'b':
		value = '\b';
		break;
	    case 'f':
		value = '\f';
		break;
	    case 'n':
		value = '\n';
		break;
	    case 'r':
		value = '\r';
		break;
	    case 's':
		value = ' ';
		break;
	    case 't':
		value = '\t';
		break;

	    default:
		if (isoctal(c)) {
		    /*
		     * \ddd octal sequence
		     */
		    value = 0;
		    for (n = 0; n < 3 && isoctal(c); ++n) {
			value = (value << 3) + (c & 07);
			c = getc(f);
		    }
		    got = 1;
		    break;
		}

		if (c == 'x') {
		    /*
		     * \x<hex_string> sequence
		     */
		    value = 0;
		    c = getc(f);
		    for (n = 0; n < 2 && isxdigit(c); ++n) {
			digit = toupper(c) - '0';
			if (digit > 10)
			    digit += '0' + 10 - 'A';
			value = (value << 4) + digit;
			c = getc (f);
		    }
		    got = 1;
		    break;
		}

		/*
		 * Otherwise the character stands for itself.
		 */
		value = c;
		break;
	    }

	    /*
	     * Store the resulting character for the escape sequence.
	     */
	    if (len < MAXWORDLEN-1)
		word[len] = value;
	    ++len;

	    if (!got)
		c = getc(f);
	    continue;

	}

	/*
	 * Not escaped: see if we've reached the end of the word.
	 */
	if (quoted) {
	    if (c == quoted)
		break;
	} else {
	    if (isspace(c) || c == '#') {
		ungetc (c, f);
		break;
	    }
	}

	/*
	 * Backslash starts an escape sequence.
	 */
	if (c == '\\') {
	    escape = 1;
	    c = getc(f);
	    continue;
	}

	/*
	 * An ordinary character: store it in the word and get another.
	 */
	if (len < MAXWORDLEN-1)
	    word[len] = c;
	++len;

	c = getc(f);
    }

    /*
     * End of the word: check for errors.
     */
    if (c == EOF) {
	if (ferror(f)) {
	    if (errno == 0)
		errno = EIO;
	    printf("Ошибка чтения %s: %m", filename );
	    return -1;
	}
	/*
	 * If len is zero, then we didn't find a word before the
	 * end of the file.
	 */
	if (len == 0)
	    return 0;
    }

    /*
     * Warn if the word was too long, and append a terminating null.
     */
    if (len >= MAXWORDLEN) {
	printf("Внимание: слово в файле %s очень длинное (%.20s...)",filename,word);
	len = MAXWORDLEN - 1;
    }
    word[len] = 0;

    return 1;
#undef isoctal 
}
