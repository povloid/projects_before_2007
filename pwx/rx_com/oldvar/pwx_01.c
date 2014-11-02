#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

/* ��������� �������� �������� ������ ��������� � <asm/termbits.h>, 
    ������� ���������� � <termios.h> */
#define BAUDRATE B600

/* �������� ��� ���������� ��� ����������� �������� ����� */
#define MODEMDEVICE "/dev/ttyS0"
//#define _POSIX_SOURCE 1 /* POSIX-����������� �������� */

#define FALSE 0
#define TRUE 1

#define LEN_MASS 50

volatile int STOP=FALSE; 

main()
{
	int fd; //���������� ����������� ����� ����������
       struct termios oldtio,newtio;
      
        /* 
          ��������� ���������� ������ ��� ������ � ������ ��� �������������
          �������� (tty), ��������� �� �� ����� ��������� ������� �����
          ������ � ����� �������� ������ CTRL-C.
        */
         fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY ); 
         if (fd <0) {perror(MODEMDEVICE); exit(-1); }
        
         tcgetattr(fd,&oldtio); /* save current serial port settings */
         bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */
        
        /* 
          BAUDRATE: ������������� �������� �������� ������ � bps.
                    �� ����� ����� �� ������������ cfsetispeed � cfsetospeed.
          CRTSCTS : ���������� ���������� �������� ������� ������ (������������
                    ������ ���� ������ �������� ����� ������������ �����������
                    �������. ��. ������ 7 Serial-HOWTO)
          CS8     : 8n1 (8 ���, ��� ��������, 1 ����-���)
          CLOCAL  : ��������� �����������, ��� ���������� �������
          CREAD   : ��������� ����� ��������
        */
         newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD | CSTOPB | PARENB;
         
        /*
          IGNPAR  : ������������ ����� � �������� ��������
          ICRNL   : ���������� ������ CR �� NL (����� ���� CR �� ������
                    ���������� �� ����� ��������� ����)
          ����� ���������� ���������� ��� raw (��� ��������� �����)
        */
         newtio.c_iflag = IGNPAR;
         
        /*
         Raw ����� (��� ��������� ������).
        */
         newtio.c_oflag = 0;
         
        /*
          ICANON  : ��������� ������������ ����
          ������������� ����� ���������������� "���", � �� �������� �������
          � ���������� ���������
        */
         //newtio.c_lflag = ICANON;
         
	 
	 newtio.c_cc[VTIME]    = 0;   /* ������������ ������ �� ������������ */
         newtio.c_cc[VMIN]     = 1;   /* ���������� ������ �� ��� ��� ����
                                        �� ����� ������� 5 �������� */
	
        /*  
	 ������ ������� �������� ����� � ���������� ���� ��������� �����
        */
         tcflush(fd, TCIFLUSH);
         tcsetattr(fd,TCSANOW,&newtio);	
		
	unsigned char  input1[LEN_MASS]; 
	int k=0;
	
	printf("----> ������:\n");
	printf("�����:\n");
	
	int i=0,str=0;
	
	for (;;)
	{
	  //printf("\n");
	  /*------------0-------------*/
	  /*����� ������ � ���        */
	for(i=0;i<LEN_MASS;i++) input1[i]=0; 
		  
	k=read(fd,input1,LEN_MASS);   	/* ����� ����� ������/����� 5-�� �������� */
        	                	/* ��� ���� ����� �� ����� ������������ printf... */
	input1[LEN_MASS]='\0';				    
	    for (i=0;i<LEN_MASS;i++)
	     {
	    	  printf("%d\t",input1[i]);  
		  str++;
		  if (str>=10)
		   {
			str=0;
			printf("\n");
		   }
	     }
	str=0;	
	printf("--------------------------------------------------------------------------\n"); 
	  sleep(1); /*�������� �� 1 �������*/
	}
	
	
         /* �������������� ������ ��������� ����� */
         tcsetattr(fd,TCSANOW,&oldtio);
}

