#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

/* ��������� �������� �������� ������ ��������� � <asm/termbits.h>, 
    ������� ���������� � <termios.h> */
#define BAUDRATE B4800

/* �������� ��� ���������� ��� ����������� �������� ����� */
#define MODEMDEVICE "/dev/ttyS0"
//#define _POSIX_SOURCE 1 /* POSIX-����������� �������� */

#define FALSE 0
#define TRUE 1

#define LEN_MASS 20

volatile int STOP=FALSE; 

main()
{
	int fd;
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
	
	
	                 /*1h,4h,0c4h,0c9h,6h,09h,2h,7h,0abh*/
	char out0[6]={0x01,0x01,0xC0,0xC2,0x05,0x8A};
	char out1[9]={0x01,0x04,0xC8,0xCD,0x06,0x06,0x00,0x01,0xA8};
	char out2[9]={0x01,0x04,0xC8,0xCD,0x06,0x08,0x00,0x07,0xB0};
	char input1[LEN_MASS];
	
	int k=0;
	
	printf("----> ������:\n");
	
	
	printf("TX");
	
	int i=0;
	
	for (;;)
	{
	  printf("\n");
	  printf("\n TX - 0 - ");
	  /*------------0-------------*/
	    /*�������� ������� 0*/
		write(fd,out0,6);
	    /*����� ������ �� ������ 0*/
		for(i=0;i<LEN_MASS;i++)
        	  input1[i]=0;        
		  
		k=read(fd,input1,LEN_MASS);   	/* ����� ����� ������/����� 5-�� �������� */
        	input1[LEN_MASS]=0;               	/* ��� ���� ����� �� ����� ������������ printf... */
		
		for(i=0;i<k;i++)
        	  printf("%d ",input1[i]);
		 
		    	    
	  sleep(1); /*�������� �� 1 �������*/
	  printf("\n TX - 1 - ");
	  	  
	  /*------------1-------------*/
	    /*�������� ������� 0*/
		write(fd,out1,9);
	    /*����� ������ �� ������ 0*/
		for(i=0;i<LEN_MASS;i++)
        	  input1[i]=0;    
				
		k=read(fd,input1,LEN_MASS);   	/* ����� ����� ������/����� 5-�� �������� */
        	input1[LEN_MASS]=0;               	/* ��� ���� ����� �� ����� ������������ printf... */
        	
		for(i=0;i<k;i++)
        	  printf("%d ",input1[i]);  
        	    	    
	  sleep(1); /*�������� �� 1 �������*/
	  printf("\n TX - 2 - ");
	  /*------------2-------------*/
	    /*�������� ������� 0*/
		write(fd,out2,9);
	    /*����� ������ �� ������ 0*/
		for(i=0;i<LEN_MASS;i++)
        	  input1[i]=0;    
		  
		k=read(fd,input1,LEN_MASS);   	/* ����� ����� ������/����� 5-�� �������� */
        	input1[LEN_MASS]=0;               	/* ��� ���� ����� �� ����� ������������ printf... */
        	
		for(i=0;i<k;i++)
        	  printf("%d ",input1[i]);
        	    	    
	  sleep(10); /*�������� �� 10 �������*/
	}
	
	
         /* �������������� ������ ��������� ����� */
         tcsetattr(fd,TCSANOW,&oldtio);
}

