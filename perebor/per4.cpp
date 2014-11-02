//Вариант 2
//Вариант перебора всех возможных комбинаций


#include <iostream.h>

int main(void)
    {
	cout<<"\n\tВведите необходимые данные\n";
	//Символьные переменные    
	    //char *Alf="abc:";    	//Переменная, хранящая алфавит
	    char Alf[300];
	    char Temp[100]="";                  //Переменная для предл.
	    char fsh[100]="";
	    int a;
	    
	//Ввод данных
	    cout<<"\nВведите алфавит:";
	    cin>>Alf;
	    cout<<"\nВведите макс. колл. символов в слове:";
	    cin>>a;
	
	//Числовые переменные
	    int Word_len=a-1;		//Длина предложения
	    double index_W=0;
	//Предварительные вычисления	
	    int Alf_len=strlen(Alf)+1;	//Вычисляется длина алфавита	
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
		//В этом месте замутить сравнение
	    }
	    Temp[Word_len]=Alf[Alf_len-1];
	}     
}