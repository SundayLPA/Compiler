#include<stdio.h>
#include<string.h>

//保留字表
#define keywordSum  16     //保留字个数 
char * keyword[keywordSum]={"program" , "if" , "else" , "while" , "do" , "int" , "call"  , "read" , "write" , "const" , "var" , "procedure" , "begin" , "end"  ,  "then", "odd"};

char singleword[50]="+-*/()=,.;";   //单分界符
char doubleword[10]="><!:";        //双分界符
int row=1;                         //行数，报错时提供行数 

FILE *fin, *fout;                //fin输入文件(源程序文件)，fout输出文件(词法分析的结果) 
char finname[30]="source.txt",foutname[30]="lexi.txt";



int isAlpha(char x)		//判断字符是否为字母
{
	if( (x>='a' && x<='z') || (x>='A' && x<='Z') )
		return 1;
	else 
		return 0;
}

int isNumber(char x)		//判断字符是否为数字
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
		printf("源程序文件打开失败！\n");         //源程序文件打开失败 
		return(1);
	}
	if( (fout=fopen(foutname,"w"))==NULL )
	{
		printf("词法分析输出文件打开失败！\n");   //词法分析输出文件打开失败 
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
	char ch;                         //从源程序读入字符到ch 
	char token[40];                  //存放处理后的字符串，运算符等 	
	int es=0;                              
	int j,n;      //自变量 
	
	openFile();                  
	ch=getc(fin);
	while(ch!=EOF)
	{
		while(ch==' '||ch=='\n'||ch==9)           //跳过空格，换行，制表符 
		{
			if(ch=='\n')
				row++;     //换行后行数+1 
			ch=getc(fin);			
		}
		
		if(isAlpha(ch))                      //处理字母 
		{
			token[0]=ch;
			j=1;
			ch=getc(fin);
			while(isAlpha(ch)||isNumber(ch))   //拼接字符串   
			{
				token[j]=ch;
				j++;
				ch=getc(fin);
			}
			token[j]='\0';
							
			n=0; 
			while(n<keywordSum && strcmp(token,keyword[n]) )    //查找符号表，n记录符号表的位置 
				n++;
				
			if(n>=keywordSum)	
				fprintf(fout,"%s\t%s\t%d\n","ID",token,row);   //字符串为标识符 
			else					
				fprintf(fout,"%s\t%s\t%d\n",token,token,row);  //字符串为关键字 
		}
		else if( isNumber(ch) )               //处理数字   
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
			token[j]='\0';         //拼接数字，以字符串的形式存储
			
			fprintf(fout,"%s\t%s\t%d\n" , "NUM" ,token,row);  //将数字输出到文件 

		}

		else if(strchr(singleword,ch)>0)    //处理单分界符 
		{
			token[0]=ch;
			token[1]='\0';
			ch=getc(fin);
			fprintf(fout,"%s\t%s\t%d\n",token,token,row);

		}
		
		else if( strchr(doubleword,ch)>0 ) //处理双分界符，还有更好的方法 
		{
			token[0]=ch;
			ch=getc(fin);  //超前搜索一位字符 
			if(ch=='=' || ( token[0]=='<' && ch=='>' ) )     //双界符 
			{
				token[1]=ch;
				token[2]='\0';
				ch=getc(fin);
			}
			else			            
				token[1]='\0';                               //单分界符 
			fprintf(fout,"%s\t%s\t%d\n",token,token,row);
		}
		
		else                    
		{
			token[0]=ch;
			token[1]='\0';
			ch=getc(fin);		                                 //读下一个字符，程序报错后继续执行
			while(ch!=EOF && (ch!=' '&&ch!='\n') )
				ch=getc(fin);
			es=3;
			fprintf(fout,"%s\t%s\t%d\n","Error","Error",row);   //记录出错的行数 
		}


	}
	
	closeFile();
	
	return es;

}

