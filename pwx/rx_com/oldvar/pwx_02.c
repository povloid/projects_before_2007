#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

/* установки скорости передачи данных объявлены в <asm/termbits.h>, 
    который включается в <termios.h> */
#define BAUDRATE B600

/* измените эти объявления для корректного указания порта */
#define MODEMDEVICE "/dev/ttyS0"
//#define _POSIX_SOURCE 1 /* POSIX-совместимый источник */

#define FALSE 0
#define TRUE 1

#define LEN_MASS 84

volatile int STOP=FALSE; 

main()
{
	int fd; //Переменная дискриптора файла устройства
       struct termios oldtio,newtio;
      
        /* 
          Открываем устройство модема для чтения и записи как неуправляющий
          терминал (tty), поскольку мы не хотим завершать процесс когда
          помехи в линии посылают сигнал CTRL-C.
        */
         fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY ); 
         if (fd <0) {perror(MODEMDEVICE); exit(-1); }
        
         tcgetattr(fd,&oldtio); /* save current serial port settings */
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
		
	unsigned char  input1[LEN_MASS]; 
	int k=0;
	
	printf("----> Начали:\n");
	printf("Прием:\n");
	
	int i=0,str=0;
	float stove5,stove6,stove7,stove8;
	
	for (;;)
	{
	//printf("\n");
	/*------------0-------------*/
	/*Прием данных с ГПП        */
	for(i=0;i<LEN_MASS;i++) input1[i]=0; //Обнулить весь массив
			
	k=read(fd,input1,LEN_MASS);
	sleep(1);  	
	 if(input1[0]==112)
	    {  
		printf("1 -> 112\t");
		k=read(fd,input1,LEN_MASS);
		sleep(1);
	    	if(input1[0]==111)				/* выход после приема/ввода 5-ти символов */		
        	    {		        	/* для того чтобы мы могли использовать printf... */
			printf("2 -> 111\t");
			if(input1[1]==119)
			 {
			    printf("3 -> 119\n");	    
    			    input1[LEN_MASS]='\0';	
			    printf("--------------------------------------------------------------------------\n");
			    for (i=4;i<LEN_MASS;i++)
	    		    {
				printf(" %d=%d;",i-4,input1[i]);  
				str++;
				if (str>=12)
				{
					str=0;
					printf("\n");
				}
			    }
			    printf("\n--------------------------------------------------------------------------\n");
			    stove5=input1[21]*0.01;
			    stove6=input1[22]*0.01;
			    stove7=input1[23]*0.01;
			    stove8=input1[24]*0.01;
			    
			    printf("stove5 - %f  stove6 - %f  stove7 - %f  stove8 - %f\n",stove5,stove6,stove7,stove8);
			    str=0;
			}    
		    }
	    } 
	  sleep(1); /*Задержка на 1 секунду*/
	}
	
	
         /* восстановление старых установок порта */
         tcsetattr(fd,TCSANOW,&oldtio);
}

