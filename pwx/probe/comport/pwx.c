#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

/* установки скорости передачи данных объявлены в <asm/termbits.h>, 
    который включается в <termios.h> */
#define BAUDRATE B4800

/* измените эти объявления для корректного указания порта */
#define MODEMDEVICE "/dev/ttyS0"
//#define _POSIX_SOURCE 1 /* POSIX-совместимый источник */

#define FALSE 0
#define TRUE 1

#define LEN_MASS 20

volatile int STOP=FALSE; 

main()
{
	int fd;
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
	
	
	                 /*1h,4h,0c4h,0c9h,6h,09h,2h,7h,0abh*/
	char out0[6]={0x01,0x01,0xC0,0xC2,0x05,0x8A};
	char out1[9]={0x01,0x04,0xC8,0xCD,0x06,0x06,0x00,0x01,0xA8};
	char out2[9]={0x01,0x04,0xC8,0xCD,0x06,0x08,0x00,0x07,0xB0};
	char input1[LEN_MASS];
	
	int k=0;
	
	printf("----> Начали:\n");
	
	
	printf("TX");
	
	int i=0;
	
	for (;;)
	{
	  printf("\n");
	  printf("\n TX - 0 - ");
	  /*------------0-------------*/
	    /*Передача запроса 0*/
		write(fd,out0,6);
	    /*Прием ответа на запрос 0*/
		for(i=0;i<LEN_MASS;i++)
        	  input1[i]=0;        
		  
		k=read(fd,input1,LEN_MASS);   	/* выход после приема/ввода 5-ти символов */
        	input1[LEN_MASS]=0;               	/* для того чтобы мы могли использовать printf... */
		
		for(i=0;i<k;i++)
        	  printf("%d ",input1[i]);
		 
		    	    
	  sleep(1); /*Задержка на 1 секунду*/
	  printf("\n TX - 1 - ");
	  	  
	  /*------------1-------------*/
	    /*Передача запроса 0*/
		write(fd,out1,9);
	    /*Прием ответа на запрос 0*/
		for(i=0;i<LEN_MASS;i++)
        	  input1[i]=0;    
				
		k=read(fd,input1,LEN_MASS);   	/* выход после приема/ввода 5-ти символов */
        	input1[LEN_MASS]=0;               	/* для того чтобы мы могли использовать printf... */
        	
		for(i=0;i<k;i++)
        	  printf("%d ",input1[i]);  
        	    	    
	  sleep(1); /*Задержка на 1 секунду*/
	  printf("\n TX - 2 - ");
	  /*------------2-------------*/
	    /*Передача запроса 0*/
		write(fd,out2,9);
	    /*Прием ответа на запрос 0*/
		for(i=0;i<LEN_MASS;i++)
        	  input1[i]=0;    
		  
		k=read(fd,input1,LEN_MASS);   	/* выход после приема/ввода 5-ти символов */
        	input1[LEN_MASS]=0;               	/* для того чтобы мы могли использовать printf... */
        	
		for(i=0;i<k;i++)
        	  printf("%d ",input1[i]);
        	    	    
	  sleep(10); /*Задержка на 10 секунду*/
	}
	
	
         /* восстановление старых установок порта */
         tcsetattr(fd,TCSANOW,&oldtio);
}

