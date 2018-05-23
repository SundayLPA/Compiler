#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexical.h"
#include "Phead.h"


FILE *fp;             //�ʷ������洢�ļ� 
char token[40];       //���� 
char token1[40];      //���� 
char token2[40];      //�������ڵ����������������� 


int sparser()
{
	int es=0;

	if( (fp=fopen("lexi.txt","r") )==NULL )                //�ʷ������Ľ���ļ� 
	{
		printf("��δ���дʷ�������\n");
		es=10;
		return (es);
	}
	
	if(es==0)
		es=progModel();                                 //�����﷨������� 

	fclose(fp);
	
	return es;

}

int progModel()                                          //<prog> �� program <id>��<block>                                
{
	 
	int es=0;
	fscanf(fp,"%s %s %s\n",token,token1,token2);             //��ȡһ������ 
	if( strcmp("program",token) )    
	{
		es=1;
		printf("��%s��ȱ�ٳ���ͷ program ��\n",token2);		//�����ļ�ͷ�����г����� 
	}

	fscanf(fp,"%s %s %s\n",token,token1,token2);            //��ȡ��һ������ 
	if( strcmp("ID",token) )
	{
		es=2;
		printf("��%s��ȱ�ٳ�������\n",token2);             //���ʵ����Բ���ID�����г����� 
	}
	
	fscanf(fp,"%s %s %s\n",token,token1,token2);                     
	if( strcmp(";",token) )                               
	{
		es=3;
		printf("��%s��ȱ�� ; !\n",token2);				  //û��";" 
	}
	fscanf(fp,"%s %s %s\n",token,token1,token2);     
	es=blockModel();                                     //ת�뵽�ֳ���ģ�� 
	
	return es;

}

int blockModel()                                         //<block> ��[<condecl>][<vardecl>][<proc>]<body> 
{
	int es=0;
	
	if(  !strcmp("const",token) )		                         
	{
		condeclModel();			                       //ת�Ƶ���������ģ�� 
	}
	if( !strcmp("var",token) )			
	{
		vardeclModel();			                       //ת�Ƶ���������ģ�� 
	}
	if( !strcmp("procedure",token) )	
	{
		procModel();				                   //ת�Ƶ��ӳ���ģ�� 
	}
	
	es=bodyModel();			                           //ת�Ƹ������ģ�� 
 
	return es;

}
 
int condeclModel()                                       //<condecl> �� const <const>{,<const>}; 
{
	int es=0;
	
	if( strcmp("const",token) )                             
	{
		es=2;
		printf("��%s��û�г�������const��\n",token2);      //ȱ�ٳ�������const 
	}

	fscanf(fp,"%s %s %s\n",token,token1,token2);    
	es=constModel();                                       //ת�Ƶ�����ģ�� 
	
	while( strcmp(";",token) )			                   //�����ķ�Ƭ��{,<const>};
	{
		if( strcmp(",",token) )
			printf("��%s��ȱ�� , \n",token2);
			              
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		es=constModel();                                  //ת�Ƶ�����ģ��   
	}
	fscanf(fp,"%s %s %s\n",token,token1,token2);

	return es;

}

int constModel()                                         //<const> �� <id>:=<integer>
{
	int es=0,x,y;
	char name[20];     //��¼���ʵ����� 

	if( strcmp("ID",token) )                             //<id>
	{
		printf("��%s�г��������Ϸ��� \n",token2);             
		es=3;
	}

	fscanf(fp,"%s %s %s\n",token,token1,token2);       //:= 
	if( strcmp(":=",token) )
	{
		printf("��%s�г����������Ϸ��� \n",token2);     
		es=3;
	}

	fscanf(fp,"%s %s %s\n",token,token1,token2);       //<integer>
	if( strcmp("NUM",token) )
	{
		printf("��%s�г�����ʽ���Ϸ��� \n",token2);    
		es=3;
	}
	
	fscanf(fp,"%s %s %s\n",token,token1,token2);  

	return es;
}

int vardeclModel()                                       //<vardecl> �� var <id>{,<id>}; 
{
	int es=0;
	
	if( strcmp("var",token) )
	{
		es=2;
		printf("��%s��ȱ�ٱ������� var��\n",token2);      //var        
	}

	fscanf(fp,"%s %s %s\n",token,token1,token2);
	if( strcmp("ID",token) )
	{
		printf("��%s�б��������Ϸ��� \n",token2);         //<id>
		es=3;
	}
	
	fscanf(fp,"%s %s %s\n",token,token1,token2);         //{,<id>} 
	while( strcmp(";",token) )			
	{
		if( strcmp(",",token) )
			printf("��%s��ȱ�� , \n",token2);
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		if( strcmp("ID",token) )
		{
			printf("��%s�б��������Ϸ��� \n",token2);
			es=3;
		}
		fscanf(fp,"%s %s %s\n",token,token1,token2);
	}
	
	fscanf(fp,"%s %s %s\n",token,token1,token2);     
	return es;
}

int procModel()                                                       //<proc> �� procedure <id>��<id>{,<id>}��;<block>{;<proc>}
{
	int es=0,i,y;
		
	if( strcmp("procedure",token) )
	{
		es=2;
		printf("��%s��ȱ�ٷֳ������� procedure ��\n",token2);        //procedure
	}

	fscanf(fp,"%s %s %s\n",token,token1,token2);
	if( strcmp("ID",token) )                                         //<id>
	{
		es=2;
		printf("��%s�г��������Ϸ���\n",token2);                  
	}

	fscanf(fp,"%s %s %s\n",token,token1,token2);
	if( strcmp("(",token) )                                        //��
	{
		es=2;
		printf("��%s��ȱ�٣���\n",token2);                       
	}

	fscanf(fp,"%s %s %s\n",token,token1,token2);	            
	if( strcmp("ID",token) )                                      //<id>
	{
		es=2;
		printf("��%s��ȱ�ٱ�������\n",token2);                 
	}
	
	fscanf(fp,"%s %s %s\n",token,token1,token2);                
	while( strcmp( ")",token) )			                          //{,<id>}��  
	{
		if( strcmp(",",token) )
			printf("��%s��ȱ�� , \n",token2);                 
            fscanf(fp,"%s %s %s\n",token,token1,token2);
		if( strcmp("ID",token) )
		{
			printf("��%s�б��������Ϸ��� \n",token2);
			es=3;
		}

		fscanf(fp,"%s %s %s\n",token,token1,token2);
	}
	
	fscanf(fp,"%s %s %s\n",token,token1,token2);
	if( strcmp(";",token) )                                      // ;
	{
		es=2;
		printf("��%s��ȱ�� ; !\n",token2);                   
	}


	fscanf(fp,"%s %s %s\n",token,token1,token2);
	es=blockModel();                                            //ת�Ƶ����ģ�� 

	while( !strcmp(";",token) )
		es=procModel();                                         //ת�Ƶ��ӳ���ģ�� 
		
	return es;
}

int bodyModel()														//<body> �� begin <statement>{;<statement>}end
{
	int es=0;
	
	if( strcmp("begin",token) )                                        //begin
	{
		es=2;
		printf("��%s�и������ȱ�����ͷ begin ��\n",token2);        
	}
	
	fscanf(fp,"%s %s %s\n",token,token1,token2);                      //<statement> 
	es=statementModel();
	
	while( !strcmp(";",token) )				                          //{ ; statement}
	{
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		es=statementModel();
	}

	if( strcmp("end",token) )                                        //end 
	{
		es=2;
		printf("��%s�и������ȱ�����β end ��\n",token2);        
	}
	
	fscanf(fp,"%s %s %s\n",token,token1,token2);

	return es;
}


int statementModel()
{
	int es=0,y;
	char name[20];
	
    //<statement> �� <id> := <exp>                                             
	if( !strcmp("ID",token) )                   //<id> 
	{

		fscanf(fp,"%s %s %s\n",token,token1,token2);
		if( strcmp(":=",token) )               //:=
		{
			es=2;
			printf("��%s��ȱ�٣�=\n",token2);
		}
		
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		es=expModel();                        //<exp> 
	}
	
	//<statement> ��if <lexp> then <statement>[else <statement>] 
	else if( !strcmp("if",token) )          //if
	{
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		es=lexpModel();                     //<lexp>
		
		if( strcmp("then",token) )        //then
		{
			es=2;
			printf("��%s��ȱ�� then \n",token2);
		}
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		es=statementModel();             //<statement> 
		
		if( !strcmp("else",token) )	    //[else <statement>] 
		{
			fscanf(fp,"%s %s %s\n",token,token1,token2);
			es=statementModel();
		} 
	}
	
	//<statement> ��while <lexp> do <statement>
	else if( !strcmp("while",token) )     //while 
	{
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		es=lexpModel();                     //<lexp> 
		        
		if( strcmp("do",token) )            //do 
		{
			es=2;
			printf("��%s��ȱ�� do \n",token2);
		}
		
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		es=statementModel();               //<statement> 
	
	}
    //<statement> ��call <id>[��<exp>{,<exp>}��]
	else if( !strcmp("call",token) )     //call 
	{
		int x;
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		if( strcmp("ID",token) )         //<id>
		{
			es=2;
			printf("��%s��ȱ�ٺ����� \n",token2);
		}
			
		fscanf(fp,"%s %s %s\n",token,token1,token2);  //[��<exp>{,<exp>}��] 
		if( !strcmp("(",token) )	 //�� 
		{
			fscanf(fp,"%s %s %s\n",token,token1,token2);
			es=expModel();           // <exp> 
			
			while( !strcmp(",",token) ) //{,<exp>}
			{
				fscanf(fp,"%s %s %s\n",token,token1,token2);
				es=expModel();
			}
			
			if( strcmp(")",token) ) //
			{
				es=2;
				printf("��%s��ȱ�������� \n",token2);
			}
			fscanf(fp,"%s %s %s\n",token,token1,token2);
		}
		
	}
	// <statement> ��<body>
	else if( !strcmp("begin",token) )    
	{
		bodyModel();
	}
	// <statement> -> read (<id>{��<id>})
	else if( !strcmp("read",token) )     //read 
	{
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		if( strcmp("(",token) )	         //(
		{
			es=2;
			printf("��%s��ȱ��(��\n",token2);
		}
		
	    fscanf(fp,"%s %s %s\n",token,token1,token2);
		if( strcmp("ID",token) )        //<id>
		{
			es=2;
			printf("��%s��ȱ�ٱ����� \n",token2);
		}
		
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		while( !strcmp(",",token) )       //{��<id>} 
			{
				fscanf(fp,"%s %s %s\n",token,token1,token2);
				if( strcmp("ID",token) )
				{
					es=2;
					printf("��%s��ȱ�ٱ����� \n",token2);
				}
				else
					fscanf(fp,"%s %s %s\n",token,token1,token2);
			}
		
		if( strcmp(")",token) )	      //�� 
		{
			es=2;
			printf("��%s��ȱ�٣���\n",token2);
		}
		
		fscanf(fp,"%s %s %s\n",token,token1,token2);

	}
	//<statement> -> write (<exp>{,<exp>})
	else if( !strcmp("write",token) )     // write
	{
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		if( strcmp("(",token) )          //(	
		{
			es=2;
			printf("��%s��ȱ��(��\n",token2);
		}
		
		fscanf(fp,"%s %s %s\n",token,token1,token2); //<exp>
		if( expModel() )		//<exp>
			printf("��%s�б��ʽ���Ϸ���\n",token2);
		
		while( !strcmp(",",token) )           //{,<exp>}
		{
				fscanf(fp,"%s %s %s\n",token,token1,token2);
				if( expModel() )		
					printf("��%s�б��ʽ���Ϸ���\n",token2);
				fscanf(fp,"%s %s %s\n",token,token1,token2);
				
		}
		
		if( strcmp(")",token) )	         //�� 
		{
			es=2;
			printf("��%s��ȱ�٣���\n",token2);
		}
		fscanf(fp,"%s %s %s\n",token,token1,token2);
	}

	else        //������ 
	{
		es=3;
		printf("��%s��statement��䲻�Ϸ�!\n",token2);
	}

	return es;
}


//<lexp> �� <exp> <lop> <exp>|odd <exp>
int lexpModel()
{
	int es=0;
	//<lexp> �� <exp> <lop> <exp>
	if( !expModel() ) //
	{
		
		es=lopModel();  //<lop>
		
		es=expModel();
	}
	else if( !strcmp("odd",token) )			//�ڶ���
		es=expModel();
	else						//����
	{
		es=3;
		printf("��%s���������Ϸ���\n",token2);
	}

	return es;
}

int expModel()                                //<exp> �� [+|-]<term>{<aop><term>}
{
	int es=0;
	if( !(strcmp("+",token) && strcmp("-",token)) )   //[+|-]<term> 
		fscanf(fp,"%s %s %s\n",token,token1,token2);
	es=termModel();
	
	while( !aopModel() )  //{<aop><term>}
	{
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		es=termModel();
	}
	return es;
}

int termModel()                              //<term> �� <factor>{<mop><factor>} 
{
	int es=0;
	es=facterModel();
	while( !mopModel() )    //{<mop><factor>} 
	{
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		es=facterModel();
	}

	return es;
}

int facterModel()                           //<factor>��<id>|<integer>|(<exp>)
{
	int es=0,x,y;
	if(  !strcmp("ID",token) )              //<id> 
	{
		;
	}
	else if(  !strcmp("NUM",token) )      //<integer>
	{
		;
	}
	else if(  !strcmp("(",token) )      //(exp)
	{
		es=expModel();
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		if( strcmp(")",token) )   
			printf("��%s��ȱ�� ��",token2);
	}
	else                                //������ 
	{
		es=3;
		printf("��%s�����Ӷ��岻�Ϸ���\n",token2);
	}
	fscanf(fp,"%s %s %s\n",token,token1,token2);

	return es;
}

int lopModel()                    //<lop> �� =|<>|<|<=|>|>=
{
	int es=0;
	if( !strcmp("=",token) )         //=
		;
	else if( !strcmp("<>",token) )		//<> 
		;
	else if( !strcmp("<",token) )		//<
		;
	else if( !strcmp("<=",token) )		//<=
		;
	else if( !strcmp(">",token) )		//<
		;
	else if( !strcmp(">=",token) )		//>=
		;
	else								//����
	{
		es=2;
		printf("��%s�й�ϵ���������\n",token2);
	}
	
    fscanf(fp,"%s %s %s\n",token,token1,token2);
	return es;
}

int aopModel()
{
	int es=0;
	if( strcmp("+",token)&& strcmp("-",token))   //<aop> �� +|-
	{
		es=2;
	}
	
	return es;
}

int mopModel()                    //<mop> �� *|/
{
	int es=0;
	if( strcmp("*",token)&& strcmp("/",token))
	{
		es=2;     
	}
	if(!strcmp("*",token))
		;
	if(!strcmp("/",token))
		;
	return es;
}

int main()
{
	int error;
	Lexical();
	if (!sparser())
	{
			printf("�﷨�����ɹ���"); 
	}
	return 0;

}
