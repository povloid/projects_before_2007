#include <iostream.h>

int main(void)
{
  cout<<"\n\t������� ����������� ������\n";
  //���������� ����������    
  char Alf[100],Temp[100]="",Temp_end[100]="",temp[100]="";
  //�������� ����������
  int Word_len;		//����� �����������
  //���� ������
  cout<<"\n������� �������:";
  cin>>Alf;
  cout<<"\n������� ����� �����:";
  cin>>Word_len;
  //��������������� ����������	
  int Alf_len=strlen(Alf)+1;	//����������� ����� ��������	
  --Word_len;
  for (int i=0; i<Word_len+1;i++)
  { 
    Temp[i]=Alf[0];
    Temp_end[i]=Alf[Alf_len-2];
  } 
  Temp_end[Word_len+1]=0;
  while (strcmp(temp,Temp_end))
  {    
    for (int i=0; i<Alf_len-1;i++)
    {
      Temp[Word_len]=Alf[i];
      cout<<"\n | "<<Temp<<" | ";
      //� ���� ����� ����� �������� ���������
    }
    strcpy(temp,Temp);
    Temp[Word_len]=Alf[Alf_len-1];	
    for (int i=Word_len;i>0;--i)
    {    
      if (Temp[i]==Alf[Alf_len-1])
      {		
	Temp[i]=Alf[0];
	for (int j=0; j<Alf_len-1;j++)
	{
	  if(Temp[i-1]==Alf[j])
	  {	
	    Temp[i-1]=Alf[j+1];
	    break;

	    int s=76;

	  }    			
	}     
      }		
    }    
    Temp[Word_len+1]=0;	
  }   
}
