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

#define LEN_MASS 84

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
	float stove5,stove6,stove7,stove8;
	
	for (;;)
	{
	//printf("\n");
	/*------------0-------------*/
	/*����� ������ � ���        */
	for(i=0;i<LEN_MASS;i++) input1[i]=0; //�������� ���� ������
			
	k=read(fd,input1,LEN_MASS);
	sleep(1);  	
	 if(input1[0]==112)
	    {  
		printf("1 -> 112\t");
		k=read(fd,input1,LEN_MASS);
		sleep(1);
	    	if(input1[0]==111)				/* ����� ����� ������/����� 5-�� �������� */		
        	    {		        	/* ��� ���� ����� �� ����� ������������ printf... */
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
	  sleep(1); /*�������� �� 1 �������*/
	}
	
	
         /* �������������� ������ ��������� ����� */
         tcsetattr(fd,TCSANOW,&oldtio);
}

