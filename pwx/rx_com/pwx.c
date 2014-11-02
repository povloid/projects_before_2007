//////////////////////////////////////////////////////////////////////////////
//�������� ��� ������ pwx 1.0  . 
//��������� ������������ ����� ������ � ����� COM1, ����������� � ��� ����� 
//���-24, ��������� ��������� � ������ ���������� ������ � ����. 
//
// ����� ��������� �.�.  		2005 �.
//
// �������� ��� ����� ������� ��������� � ����������. GNU!!!
//
//////////////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <ibase.h>
#include <time.h>

/* ��������� �������� �������� ������ ��������� � <asm/termbits.h>, 
    ������� ���������� � <termios.h> */
#define BAUDRATE B600

/* �������� ��� ���������� ��� ����������� �������� ����� */
#define MODEMDEVICE "/dev/ttyS0"
//#define _POSIX_SOURCE 1 /* POSIX-����������� �������� */

#define FALSE 0
#define TRUE 1

#define LEN_MASS 80

volatile int STOP=FALSE; 

/////////////////////////////////////////////////////////////////////////////
//��� ���� //////////////////////////////////////////////////////////////////
union ptype {
	    unsigned char p_input[LEN_MASS];
	    short int power[LEN_MASS/2];
	};

//--------------------------------------------------------------------------

main()
{
    printf("������ ��������� pwx 1.0\n ������������� ���������� COM1, ���� ���������� /dev/ttyS0 -->");
    //////////////////////////////////////////////////////////////////////////
    /// ������������� ����� COM 1 ////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////	
	int fd; //���������� ����������� ����� ����������
       struct termios oldtio,newtio;
      
        /* 
          ��������� ���������� ������ ��� ������ � ������ ��� �������������
          �������� (tty), ��������� �� �� ����� ��������� ������� �����
          ������ � ����� �������� ������ CTRL-C.
        */
         fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY ); 
         if (fd <0) {perror(MODEMDEVICE); exit(-1); }
        
         tcgetattr(fd,&oldtio);		 /* save current serial port settings */
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
	 
	 printf(" OK \n");
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    /// ������������� ����� COM 1 ���������///////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    
    //���������� ��� ������ � ����������� ������� � �����	
	    unsigned char  input_com[LEN_MASS]; 
	    short int equipment;	    	    
	//����������� ��������
	    union ptype pwx;		
	    int k=0;
    //���������� ��� ������������ �������
	    time_t t;
	    struct tm *MyTime; //��������� tm ��� ��������� ���� � �������
	    float p_float=0;
	    char p_str[5]="0.000";	    
    //////////////////////////////////////////////////////////////////////////
    /// ���������� ������ � ����� ������ FDB /////////////////////////////////
    //////////////////////////////////////////////////////////////////////////	
    printf("������������� ���� ������ ");
     //���������, ����������� ��� ������ � ����� ������                          
    	 char *dbname = "192.168.0.186:/mnt/hda11/power/pwcrdb.fdb";                                 
            printf("%s --> ",dbname);
	 char *uname = "sysdba";                                                     
	 char *upass = "masterkey";                                                  
	 char query[140]="insert into  CROND (DATE1, TIME1, STOVE5, STOVE6, STOVE7, STOVE8, EQUIPMENT) values ('15.08.2005','12:12:00',0.000,0.000,0.000,0.000,0.000)";; 
	 char *ptr_query=&query[77]; //��������� �� ������ �������
	 
	//������������� ���� ������ ������ ������ ������ � ������
	// printf("sizeof(query)=%d\n",sizeof(query));
		                                                                                      
	 //���������� ��� ������ � ����� ������                                      
	 isc_db_handle       db_handle = NULL;                                       
	 isc_tr_handle       transaction_handle = NULL;                              
	 isc_stmt_handle     statment_handle = NULL;                                 
	 char        dpb_buffer[256], *dpb, *p;                                      
	 short       dpb_length;                                                     
	 ISC_STATUS  status_vector[20];                                              
	 
     //������������ � ���� ������ � ��� �����:                                                        
			     // 1. ������� ������������� database parametr buffer, �����������                               
			     //    ��� ����������� � ���� ������                                                             
			           dpb=dpb_buffer;                                                                              
			           *dpb++ = isc_dpb_version1;                                                                   
			           *dpb++ = isc_dpb_user_name;                                                                  
			           *dpb++ = strlen(uname);                                                                      
			           for (p=uname; *p;) *dpb++ = *p++;                                                            
			               *dpb++ = isc_dpb_password;                                                                   
			           *dpb++ = strlen(upass);                                                                      
			           for (p=upass; *p;) *dpb++ = *p++;                                                            
			               dpb_length = dpb- dpb_buffer;                                                                
			    // 2. ������������ � ���� ������                                                                
			            isc_attach_database(status_vector,                                                           
			                            strlen(dbname),                                                              
			                            dbname,                                                                      
			                            &db_handle,                                                                  
			                            dpb_length,                                                                  
			                            dpb_buffer);   
			
				//����� ���� �������� ����������� ��� ����������� API-������� �������� � ������ ����������   
			             if (status_vector[0] == 1 && status_vector[1])                                       
				         {                                                                                    
				             printf("1. ������!\n");                                                       
				             isc_print_status(status_vector);                                             
				             printf("���������� ��������� � �������.\n");                               
				             return (1); //����� �� ����������                                            
				         }
	printf("OK \n");
    //////////////////////////////////////////////////////////////////////////
    /// GO!!! ////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////	
	printf("----> ������:\n");
	//printf("sizeof(char)=%d\tsizeof(int)=%d\n",sizeof(char),sizeof(short int));
	//printf("sizeof(pw)=%d\n",sizeof(pwx));
	printf("�����:\n");
	
	int i=0,str=0;
		
	for (;;)  		//���� ������ 
	{
	/*------------0-------------*/
	/*����� ������ � ���        */
	for(i=0;i<LEN_MASS;i++) input_com[i]=0; //�������� ���� ������
	
	printf("��. ���. ---> ");	//����� ���������		
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
			    //����� ������� ��������, ��������� ������ � ������� pwx.power
			    printf("\n--------------------------------------------------------------------------\n");
			    printf("���� 5 - %d       ���� 6 - %d       ���� 7 - %d       ���� 8 - %d\n",
							pwx.power[16],
							pwx.power[17],
							pwx.power[18],
							pwx.power[19]);
			    equipment=0;				
			    for(i=4;i<15;i++) equipment=equipment+pwx.power[i];
			    printf("������������ - %d\n",equipment);
			    
			    /////////////////////////////////////////////////////////////////
			    //������ �������� � ����� ������ ////////////////////////////////
			    /////////////////////////////////////////////////////////////////
			    
			    //���������� �������
				t=time(NULL);          	    //��������� ������� �� ��� �������� UNIX 
				MyTime=localtime(&t);       //������������� �������� ������� � ��������� �� ��������� MyTime
				printf("�����: %d:%d:%d\t ����: %d.%d.%d\n",
								MyTime->tm_hour,	//���
							        MyTime->tm_min,		//������
							        MyTime->tm_sec,		//�������
								MyTime->tm_mday,	//����
								MyTime->tm_mon,		//�����
								MyTime->tm_year+1900);	//���			
			    
				sprintf(ptr_query,"values ('%d.%d.%d','%d:%d:%d','%2.2f','%2.2f','%2.2f','%2.2f','%2.2f')",
							    MyTime->tm_mday,		//����
							    MyTime->tm_mon,		//�����
							    MyTime->tm_year+1900,	//���
							    MyTime->tm_hour,		//���
							    MyTime->tm_min,		//������
							    MyTime->tm_sec,		//�������
							    (float)pwx.power[16]/100,
							    (float)pwx.power[17]/100,
							    (float)pwx.power[18]/100,
							    (float)pwx.power[19]/100,
							    (float)equipment/100);
				
				
				//printf("%s\n",ptr_query);
			    //-------------------
			    //������ ������� 
				printf("������� ������:\n%s\n",query);
			    //----------------------------------------------------------------------
				    transaction_handle = NULL;                              
				    statment_handle = NULL;
				//���� ����������� � ���� ������ ��������� �������, ���������� ����������                    
			             if (db_handle)                                                                       
				         {                                                                                    
				             isc_start_transaction(status_vector,                                         
				            			    &transaction_handle,                                         
				            			    1,                                                           
				            			    &db_handle,                                                  
				            			    0,                                                           
				            			    NULL);
					}
	    			 //�������� �� ������                                                         
				     if (status_vector[0] == 1 && status_vector[1])                               
				         {                                                                            
				             printf("2. ������ ������ ����������! \n");                                               
				             isc_print_status(status_vector);                                     
				             printf("���������� ��������� � �������.\n");                       
				             return (1);                                                          
				         }                                                                            
			 //���������� � ���������� ������� ��������                                        
			           isc_dsql_allocate_statement(status_vector,                                                              
					    	     &db_handle,                                                                 
						      &statment_handle);                                                          
				 //�������� �� ������                                                         
				     if (status_vector[0] == 1 && status_vector[1])                               
				         {                                                                            
				             printf("3. ������ ���������� � ���������� �������!\n");                                               
				             isc_print_status(status_vector);                                     
				             printf("���������� ��������� � �������.\n");                       
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
				//�������� �� ������                                                         
				     if (status_vector[0] == 1 && status_vector[1])                               
				         {                                                                            
				             printf("4. ������!\n");                                               
				             isc_print_status(status_vector);                                     
				             printf("���������� ��������� � �������.\n");                       
				             return (1);                                                          
			        	}
			//////////////////////////////////////////////////////////////		
			 isc_dsql_execute(status_vector,                                                       
				             &transaction_handle,                                                 
					     &statment_handle,                                                    
				             1,                                                                   
				             NULL);
				//�������� �� ������                                                         
				     if (status_vector[0] == 1 && status_vector[1])                               
				         {                                                                            
				             printf("5. ������!\n");                                               
				             isc_print_status(status_vector);                                     
				             printf("���������� ��������� � �������.\n");                       
				             return (1);                                                          
			        	}
			//����� ��������� ���������� � �����                                                  
			         if (transaction_handle)                                                               
				                 {                                                                             
						    isc_commit_transaction(status_vector,&transaction_handle);            
						 } 
				//�������� �� ������                                                         
				     if (status_vector[0] == 1 && status_vector[1])                               
				         {                                                                            
				             printf("������ ���������� ����������!\n");                                               
				             isc_print_status(status_vector);                                     
				             printf("���������� ��������� � �������.\n");                       
				             return (1);                                                          
			        	}
	
				   
			    //���������� ������ �����
			    str=0;
			    printf("\v");
			}    
		    }
	    } 
	  sleep(1); /*�������� �� 1 �������*/
	}
	
	if (db_handle) isc_detach_database(status_vector,&db_handle);                         
				//�������� �� ������ 
				     if (status_vector[0] == 1 && status_vector[1])                               
				         {                                                                            
				             printf("������!\n");                                               
				             isc_print_status(status_vector);                                     
				             printf("���������� ��������� � �������.\n");                       
				             return (1);                                                          
			        	}
         /* �������������� ������ ��������� ����� */
         tcsetattr(fd,TCSANOW,&oldtio);
}
