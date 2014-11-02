//////////////////////////////////////////////////////////////////////////////
//�������� ��� ������ pwx 1.0  . 
//��������� ������������ ����� ������ � ����� COM1, ����������� � ��� ����� 
//���-24, � ����� ��������� ��������� � ������ ���������� ������ � ����. 
//
// ����� ��������� �.�.  		2005 �.
//
//      �������� ��� ����� ������� ��������� � ����������. GNU!!!
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

/* ��������� �������� �������� ������ ��������� � <asm/termbits.h>, 
    ������� ���������� � <termios.h> */
#define BAUDRATE B600

/* �������� ��� ���������� ��� ����������� �������� ����� */
#define MODEMDEVICE "/dev/ttyS0"
//#define _POSIX_SOURCE 1 /* POSIX-����������� �������� */

#define FALSE 0
#define TRUE 1

#define LEN_MASS 80
// ���� � ����� ������������ pwx.conf

//#define OPT_FILE "./pwx.conf"
#define OPT_FILE "/etc/pwx/pwx.conf" 

#define MAXWORDLEN 128

volatile int STOP=FALSE; 

/////////////////////////////////////////////////////////////////////////////
//��� ���� //////////////////////////////////////////////////////////////////
union ptype {
	    unsigned char p_input[LEN_MASS];
	    short int power[LEN_MASS/2];
	};

//���������� ������������---------------------------------------------------

//���������, ����������� ��� ������ � ����� ������                          
//    	 char *dbname = "192.168.0.186:/mnt/hda11/power/pwcrdb.fdb";                                 
//	 char *uname = "sysdba";                                                     
//	 char *upass = "masterkey";

//int  OPT_SERVER;
	 
char OPT_SERVERUSER[20];
char OPT_SERVERPASSWORD[20];                                                                        
char OPT_SERVERNAME[128];
 
//�������-------------------------------------------------------------------

int get_options(void);
int parse_options(FILE*);																				    
int parse_option(FILE*,char*,int*);
int getword(FILE*,char*,int*,char*);

//--------------------------------------------------------------------------

main()
{
    printf("������ ��������� pwx 1.0\n������������� ���������� COM1, ���� ���������� /dev/ttyS0 -->");
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
	
	printf("������ ����� ������������ %s ",OPT_FILE);
	
	if (get_options()) return (1); 	//��������� ��������� � ����� pwx.conf
	
	printf(" --> OK\n");
	    	
	printf("������������� ���� ������: %s ",OPT_SERVERNAME);
	//���������� ��� ������������ �������                  
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
			           *dpb++ = strlen(OPT_SERVERUSER);                                                                      
			           for (p=OPT_SERVERUSER; *p;) *dpb++ = *p++;                                                            
			               *dpb++ = isc_dpb_password;                                                                   
			           *dpb++ = strlen(OPT_SERVERPASSWORD);                                                                      
			           for (p=OPT_SERVERPASSWORD; *p;) *dpb++ = *p++;                                                            
			               dpb_length = dpb- dpb_buffer;                                                                
			    // 2. ������������ � ���� ������                                                                
			            isc_attach_database(status_vector,                                                           
			                            strlen(OPT_SERVERNAME),                                                              
			                            OPT_SERVERNAME,                                                                      
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
	printf("--> OK \n");
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
								MyTime->tm_mon+1,		//�����
								MyTime->tm_year+1900);	//���			
			    
				sprintf(ptr_query,"values ('%d.%d.%d','%d:%d:%d','%2.2f','%2.2f','%2.2f','%2.2f','%2.2f')",
							    MyTime->tm_mday,		//����
							    MyTime->tm_mon+1,		//�����
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

//////////////////////////////////////////////////////////////////////////////////////////////////
//����� ��������� �� ����� ������������ pwx.conf
//////////////////////////////////////////////////////////////////////////////////////////////////

int get_options(void)
{
	FILE* optfile = NULL;	//���������� �����
        int result;		//��������� ���������� �������
	
	 optfile = fopen(OPT_FILE,"rt");
	 
	 if(!optfile) 		//�������� �������� �����
	         {
		         printf("\n������. ������ ��������� ���� ������������:'%s'\n",
						 OPT_FILE);
			 printf("���������� ��������� � �������.\n");			 
		         return 1;
		 }
	
	//result = parse_options( optfile ); 			     
	
	parse_options(optfile);
		 
	//������� ����	
	fclose(optfile);
	
	//return result;
	return (0);
}

int parse_options(FILE* optfile)
{
    char word[MAXWORDLEN*2];
    int newlinep, result;
	
	    // ����������� �����...
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
			            printf("������: Unexpected option '%s'\n",word);
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
			    printf("������: ������������ �������� 'servername'\n" );                 
				                 return 0;                                                                                
			 }                                                                                            
		// ��� ������� ��� �����	 			
		// OPT_SERVER = atoi( word );                                                               
		strcpy( OPT_SERVERNAME, word ); 
         }
    else  if(strcasecmp(word,"user")==0)                                                 
         {                                                                                                
	    if(getword(optfile,word,newlinep,OPT_FILE) != 1 || *newlinep)                         
		         {                                                                                            
			    printf("������: ������������ �������� 'user'\n" );                 
				                 return 0;                                                                                
			 }                                                                                            
			 			                                                            
		strcpy( OPT_SERVERUSER, word ); 
         }
    else  if(strcasecmp(word,"password")==0)                                                 
         {                                                                                                
	    if(getword(optfile,word,newlinep,OPT_FILE) != 1 || *newlinep)                         
		         {                                                                                            
			    printf("������: ������������ �������� 'password'\n" );                 
				                 return 0;                                                                                
			 }                                                                                            
			 			                                                               
		strcpy( OPT_SERVERPASSWORD, word ); 
         }	 	   
      else                                                                                             
         {                                                                                                
	     fprintf( stderr, "������: ���������� �������� '%s'\n", word );                                     
		 return 0;                                                                                    
	 }                                                                                                
			                                                                                                           
     return 1;     
}

// original code by PPPD
// �������� ���� ���� �� PPPD
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
	    printf("������ ������ %s: %m", filename );
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
	printf("��������: ����� � ����� %s ����� ������� (%.20s...)",filename,word);
	len = MAXWORDLEN - 1;
    }
    word[len] = 0;

    return 1;
#undef isoctal 
}
