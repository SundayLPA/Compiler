#include<stdio.h>
#include<string.h>

//�����ֱ�
#define keywordSum  16     //�����ָ��� 
char * keyword[keywordSum]={"program" , "if" , "else" , "while" , "do" , "int" , "call"  , "read" , "write" , "const" , "var" , "procedure" , "begin" , "end"  ,  "then", "odd"};

char singleword[50]="+-*/()=,.;";   //���ֽ��
char doubleword[10]="><!:";        //˫�ֽ��
int row=1;                         //����������ʱ�ṩ���� 

FILE *fin, *fout;                //fin�����ļ�(Դ�����ļ�)��fout����ļ�(�ʷ������Ľ��) 
char finname[30]="source.txt",foutname[30]="lexi.txt";



int isAlpha(char x)		//�ж��ַ��Ƿ�Ϊ��ĸ
{
	if( (x>='a' && x<='z') || (x>='A' && x<='Z') )
		return 1;
	else 
		return 0;
}

int isNumber(char x)		//�ж��ַ��Ƿ�Ϊ����
{
	if( x>='0' && x<='9' )
		return 1;
	else 
		return 0;
}

int openFile()
{
	if( (fin=fopen(finname,"r"))==NULL )     
	{
		printf("Դ�����ļ���ʧ�ܣ�\n");         //Դ�����ļ���ʧ�� 
		return(1);
	}
	if( (fout=fopen(foutname,"w"))==NULL )
	{
		printf("�ʷ���������ļ���ʧ�ܣ�\n");   //�ʷ���������ļ���ʧ�� 
		return(2);
	}
}

int closeFile() 
{
	fclose(fin);
	fclose(fout);
	
}
int Lexical()
{
	char ch;                         //��Դ��������ַ���ch 
	char token[40];                  //��Ŵ������ַ������������ 	
	int es=0;                              
	int j,n;      //�Ա��� 
	
	openFile();                  
	ch=getc(fin);
	while(ch!=EOF)
	{
		while(ch==' '||ch=='\n'||ch==9)           //�����ո񣬻��У��Ʊ�� 
		{
			if(ch=='\n')
				row++;     //���к�����+1 
			ch=getc(fin);			
		}
		
		if(isAlpha(ch))                      //������ĸ 
		{
			token[0]=ch;
			j=1;
			ch=getc(fin);
			while(isAlpha(ch)||isNumber(ch))   //ƴ���ַ���   
			{
				token[j]=ch;
				j++;
				ch=getc(fin);
			}
			token[j]='\0';
							
			n=0; 
			while(n<keywordSum && strcmp(token,keyword[n]) )    //���ҷ��ű�n��¼���ű��λ�� 
				n++;
				
			if(n>=keywordSum)	
				fprintf(fout,"%s\t%s\t%d\n","ID",token,row);   //�ַ���Ϊ��ʶ�� 
			else					
				fprintf(fout,"%s\t%s\t%d\n",token,token,row);  //�ַ���Ϊ�ؼ��� 
		}
		else if( isNumber(ch) )               //��������   
		{
			token[0]=ch;
			j=1;
			ch=getc(fin);
			while( isNumber(ch) ) 
			{
				token[j]=ch;
				j++;
				ch=getc(fin);
			}                         
			token[j]='\0';         //ƴ�����֣����ַ�������ʽ�洢
			
			fprintf(fout,"%s\t%s\t%d\n" , "NUM" ,token,row);  //������������ļ� 

		}

		else if(strchr(singleword,ch)>0)    //�����ֽ�� 
		{
			token[0]=ch;
			token[1]='\0';
			ch=getc(fin);
			fprintf(fout,"%s\t%s\t%d\n",token,token,row);

		}
		
		else if( strchr(doubleword,ch)>0 ) //����˫�ֽ�������и��õķ��� 
		{
			token[0]=ch;
			ch=getc(fin);  //��ǰ����һλ�ַ� 
			if(ch=='=' || ( token[0]=='<' && ch=='>' ) )     //˫��� 
			{
				token[1]=ch;
				token[2]='\0';
				ch=getc(fin);
			}
			else			            
				token[1]='\0';                               //���ֽ�� 
			fprintf(fout,"%s\t%s\t%d\n",token,token,row);
		}
		
		else                    
		{
			token[0]=ch;
			token[1]='\0';
			ch=getc(fin);		                                 //����һ���ַ������򱨴�����ִ��
			while(ch!=EOF && (ch!=' '&&ch!='\n') )
				ch=getc(fin);
			es=3;
			fprintf(fout,"%s\t%s\t%d\n","Error","Error",row);   //��¼��������� 
		}


	}
	
	closeFile();
	
	return es;

}

