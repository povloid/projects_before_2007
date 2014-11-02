#include <ibase.h>
#include <iostream>
#include <string>

using namespace std;

//��� ��������� ������������� ��� �������� ���������� ���� SQL_VARYING
#define SQL_VARCHAR(len) struct {short vary_length; char vary_string[(len)+1];}	


//������ ������� ��������� ���������
int main (void)
{
    //���������, ����������� ��� ������ � ����� ������
    //char *dbname = "192.168.0.77:/home/fdb/PWCRDB.FDB";
    char *dbname = "192.168.0.2:c:\\PowerNew\\PWCRDB.FDB";
    char *uname = "sysdba"; 
    char *upass = "masterkey";
    char *query = "select ID, DATE1, TIME1, F_HZ, STOVE5, STOVE6, STOVE7, STOVE8, EQUIPMENT from CROND where date1='15.08.2005'";
    
    //���������� ��� ������ � ����� ������
    isc_db_handle 	db_handle = NULL;
    isc_tr_handle	transaction_handle = NULL;
    isc_stmt_handle 	statment_handle = NULL;
    char 	dpb_buffer[256], *dpb, *p;
    short 	dpb_length;
    ISC_STATUS	status_vector[20];
    XSQLDA	*isqlda, *osqlda;
    long 	fetch_code;
    short	o_ind[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    //��������� ����������
    	struct tm	added_date;
	struct tm	added_time;
	ISC_DATE	pwx_date;
	ISC_TIME	pwx_time;
	char 	decodedDate[50],decodedTime[50];
	int 	id;
	float	f_hz;
	short int sstove5,sstove6,sstove7,sstove8,sequipment;
	
	float stove5,stove6,stove7,stove8,equipment,plant=0;
	
	
    //����� ��������� �� �����

    cout<<"������"<<endl;
    
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
			cout<<"������!"<<endl;
			isc_print_status(status_vector);
			cout<<"���������� ��������� � �������."<<endl;
			return (1); //����� �� ����������
		}
	//���� ����������� � ���� ������ ��������� �������, ���������� ����������
		if (db_handle)
		{
			isc_start_transaction(status_vector,
					&transaction_handle,
					1,
					&db_handle,
					0,
					NULL);
			//�������� �� ������
			if (status_vector[0] == 1 && status_vector[1])
			{
				cout<<"������!"<<endl;
				isc_print_status(status_vector);
				cout<<"���������� ��������� � �������."<<endl;
				return (1);
			}
		}
	//����� ���������������� ���������, ������� ����� ����������� ������������ 
	//�������
	 osqlda = (XSQLDA *) malloc (XSQLDA_LENGTH(9));
	 osqlda->version=SQLDA_VERSION1;
	 osqlda->sqln=9; 
	 //������	 
	 osqlda->sqlvar[0].sqldata = (char *)&id;
	 osqlda->sqlvar[0].sqltype = SQL_LONG;
	 osqlda->sqlvar[0].sqlind = &o_ind[0];
	 //����
	 osqlda->sqlvar[1].sqldata = (char *)&pwx_date;
	 osqlda->sqlvar[1].sqltype = SQL_TYPE_DATE;
	 osqlda->sqlvar[1].sqlind = &o_ind[1];
	 //�����
	 osqlda->sqlvar[2].sqldata = (char *)&pwx_time;
	 osqlda->sqlvar[2].sqltype = SQL_TYPE_TIME;
	 osqlda->sqlvar[2].sqlind = &o_ind[2];
	 //�������
	 osqlda->sqlvar[3].sqldata = (char *)&f_hz;
	 osqlda->sqlvar[3].sqltype = SQL_FLOAT;
	 osqlda->sqlvar[3].sqlind = &o_ind[3];
  	 //���� 5
	 osqlda->sqlvar[4].sqldata = (char *)&sstove5;
	 osqlda->sqlvar[4].sqltype = SQL_FLOAT;
	 osqlda->sqlvar[4].sqlind = &o_ind[4];
	 //���� 6
	 osqlda->sqlvar[5].sqldata = (char *)&sstove6;
	 osqlda->sqlvar[5].sqltype = SQL_FLOAT;
	 osqlda->sqlvar[5].sqlind = &o_ind[5];
	 //���� 7
	 osqlda->sqlvar[6].sqldata = (char *)&sstove7;
	 osqlda->sqlvar[6].sqltype = SQL_FLOAT;
	 osqlda->sqlvar[6].sqlind = &o_ind[6];
	 //���� 8
	 osqlda->sqlvar[7].sqldata = (char *)&sstove8;
	 osqlda->sqlvar[7].sqltype = SQL_FLOAT;
	 osqlda->sqlvar[7].sqlind = &o_ind[7];
	 //������������
	 osqlda->sqlvar[8].sqldata = (char *)&sequipment;
	 osqlda->sqlvar[8].sqltype = SQL_FLOAT;
	 osqlda->sqlvar[8].sqlind = &o_ind[8];
	 
	 
	//���������� � ���������� ������� ��������
	 isc_dsql_allocate_statement(
			 status_vector,
			 &db_handle,
			 &statment_handle);
	 	//�������� �� ������
	 	if (status_vector[0] == 1 && status_vector[1])
			{
				cout<<"������!"<<endl;
				isc_print_status(status_vector);
				cout<<"���������� ��������� � �������."<<endl;
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
		//�������� �� ������
	 	if (status_vector[0] == 1 && status_vector[1])
			{
				cout<<"������!"<<endl;
				isc_print_status(status_vector);
				cout<<"���������� ��������� � �������."<<endl;
				return (1);
			}
	 isc_dsql_execute2(
			 status_vector,
			 &transaction_handle,
			 &statment_handle,
			 1,
			 NULL,
			 NULL);
		//�������� �� ������
	 	if (status_vector[0] == 1 && status_vector[1])
			{
				cout<<"������!"<<endl;
				isc_print_status(status_vector);
				cout<<"���������� ��������� � �������."<<endl;
				return (1);
			}
	//����� ���������� ������� HTML-���������
		cout<<"��������� �������:"<<endl;
		cout<<"���.\t�����.\t\t������"<<endl;
	//����� ���������� ������� ������ ����� � �������� ������. "���������" ���-
	//��� ���������� ������� isc_dsql_fetch():
	int index=0;
	while ((fetch_code = isc_dsql_fetch(
						status_vector,
						&statment_handle,
						1,
						osqlda))==0)
	//��� ��������� ���������� ��������� ��������� ���������� �����, ��� ��� ������ 
	//������������ ������ �� ������ ������������� ����������� ����������, � ���� �����
	//�� �������, �� ������ � ��������� ������� ����� �������� "�����" �� ������ ��� �������
	//���������� �����:
	
	{
		//pwx_date.vary_string[pwx_date.vary_length]='\0';			
		//pwx_time.vary_string[pwx_time.vary_length]='\0';
		isc_decode_sql_date(&pwx_date,&added_date);
		isc_decode_sql_time(&pwx_time,&added_time);

		strftime(decodedDate,sizeof(decodedDate),"%d-%b-%Y",&added_date);
		strftime(decodedTime,sizeof(decodedTime),"%H:%M:%S",&added_time);
		
		stove5=sstove5*0.01;
		stove6=sstove6*0.01;
		stove7=sstove7*0.01;
		stove8=sstove8*0.01;
		equipment=sequipment*0.01;
		plant=stove5+stove6+stove7+stove8+equipment;
				
		cout<<id<<" "<<decodedDate<<" "<<decodedTime<<"\t"<<f_hz<<"\t"<<stove5<<"\t"<<stove6\
		<<"\t"<<stove7<<"\t"<<stove8<<"\t"<<equipment<<"\t"<<plant<<endl;
	}

	//����� ������ ���� ������ ���������� ��������� ��������
	cout<<"\n ������ ��������."<<endl;
	//�������� �� ������
	 	if (status_vector[0] == 1 && status_vector[1])
			{
				cout<<"������!"<<endl;
				isc_print_status(status_vector);
				cout<<"��������� ��������� � �������."<<endl;
				return (1);
			}
	free(osqlda);
	isc_dsql_free_statement(status_vector,&statment_handle,DSQL_drop);
	//�������� �� ������
	 	if (status_vector[0] == 1 && status_vector[1])
			{
				cout<<"������!"<<endl;
				isc_print_status(status_vector);
				cout<<"���������� ��������� � �������."<<endl;
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
				cout<<"������!"<<endl;
				isc_print_status(status_vector);
				cout<<"���������� ��������� � �������."<<endl;
				return (1);
			}
	if (db_handle) isc_detach_database(status_vector,&db_handle);
	//�������� �� ������
	 	if (status_vector[0] == 1 && status_vector[1])
			{
				cout<<"������!"<<endl;
				isc_print_status(status_vector);
				cout<<"���������� ��������� � �������."<<endl;
				return (1);
			}
	cout<<"���������� ���������."<<endl;
	
	
	//time_t t=time(NULL);		//��������� ������� �� ��� �������� UNIX
	//struct tm *MyTime;		//������������� ��������� tm
	//MyTime = localtime(&t);	//������������� �������� ������� � ��������� �� ��������� MyTime

	//cout<<t;
	//cout<<ctime(&t)<<endl;	
	//cout<<MyTime->tm_hour<<":"<<MyTime->tm_min<<":"<<MyTime->tm_sec<<endl;
	//cout<<MyTime->tm_year-100<<endl;

	
	return 0;
}// end of main
