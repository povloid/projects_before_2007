//������� 2
//������� �������� ���� ��������� ����������


#include <iostream.h>

int main(void)
    {
	cout<<"\n\t������� ����������� ������\n";
	//���������� ����������    
	    //char *Alf="abc:";    	//����������, �������� �������
	    char Alf[300];
	    char Temp[100]="";                  //���������� ��� �����.
	    char fsh[100]="";
	    int a;
	    
	//���� ������
	    cout<<"\n������� �������:";
	    cin>>Alf;
	    cout<<"\n������� ����. ����. �������� � �����:";
	    cin>>a;
	
	//�������� ����������
	    int Word_len=a-1;		//����� �����������
	    double index_W=0;
	//��������������� ����������	
	    int Alf_len=strlen(Alf)+1;	//����������� ����� ��������	
	for (int i=0; i<Word_len+1;i++)
	    {
	     Temp[i]=' ';
	     fsh[i]=Alf[Alf_len-2];
	    } 
	     Temp[Word_len+1]=0;
	     fsh[Word_len+1]=0;
	     fsh[Word_len]=Alf[Alf_len-1];     	     
     while (strcmp(Temp,fsh))
	{		
	for (int i=Word_len;i>0;--i)
	    {    	    
    		if (Temp[i]==Alf[Alf_len-1])
		     {		
		         Temp[i]=Alf[0];
			for (int j=0; j<Alf_len;j++)
			    {
				if(Temp[i-1]==' ')
			    		Temp[i-1]=Alf[0];	    
			        else	     
			    	  if(Temp[i-1]==Alf[j])
					    	{	
			    			    Temp[i-1]=Alf[j+1];
						    break;
						}    	    
			    } 
		    }	    
	    }	
	for (int i=0; i<Alf_len-1;i++)
	    {
	        Temp[Word_len]=Alf[i];
		cout<<"\n "<<++index_W<<"\t| "<<Temp<<" | ";
		//� ���� ����� �������� ���������
	    }
	    Temp[Word_len]=Alf[Alf_len-1];
	}     
}