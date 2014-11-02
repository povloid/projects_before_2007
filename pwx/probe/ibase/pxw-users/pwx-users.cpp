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
    char *query = "select us, passw from us";
    
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
   	SQL_VARCHAR(20)   us_us; 
	SQL_VARCHAR(20)   us_passw; 
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
	 osqlda = (XSQLDA *) malloc (XSQLDA_LENGTH(6));
	 osqlda->version=SQLDA_VERSION1;
	 osqlda->sqln=2;
	 //��� ������������
	 osqlda->sqlvar[0].sqldata = (char *)&us_us;
	 osqlda->sqlvar[0].sqltype = SQL_VARYING;
	 osqlda->sqlvar[0].sqlind = &o_ind[0];
	 //������
	 osqlda->sqlvar[1].sqldata = (char *)&us_passw;
	 osqlda->sqlvar[1].sqltype = SQL_VARYING;
	 osqlda->sqlvar[1].sqlind = &o_ind[1];

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
		us_us.vary_string[us_us.vary_length]='\0';			
		us_passw.vary_string[us_passw.vary_length]='\0';
		cout<<index++<<"\t"<<us_us.vary_string<<"\t"<<us_passw.vary_string<<endl;
	}

	//����� ������ ���� ������ ���������� ��������� ��������
	cout<<"\n ������ ��������."<<endl;
	//�������� �� ������
	 	if (status_vector[0] == 1 && status_vector[1])
			{
				cout<<"������!"<<endl;
				isc_print_status(status_vector);
				cout<<"���������� ��������� � �������."<<endl;
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
		


	
	return 0;
}// end of main
