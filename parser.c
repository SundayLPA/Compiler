#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexical.h"
#include "Phead.h"


FILE *fp;             //词法分析存储文件 
char token[40];       //类型 
char token1[40];      //名称 
char token2[40];      //单词所在的行数，用作出错处理 


int sparser()
{
	int es=0;

	if( (fp=fopen("lexi.txt","r") )==NULL )                //词法分析的结果文件 
	{
		printf("还未进行词法分析！\n");
		es=10;
		return (es);
	}
	
	if(es==0)
		es=progModel();                                 //进入语法语义分析 

	fclose(fp);
	
	return es;

}

int progModel()                                          //<prog> → program <id>；<block>                                
{
	 
	int es=0;
	fscanf(fp,"%s %s %s\n",token,token1,token2);             //读取一个单词 
	if( strcmp("program",token) )    
	{
		es=1;
		printf("第%s行缺少程序头 program ！\n",token2);		//不是文件头，进行出错处理 
	}

	fscanf(fp,"%s %s %s\n",token,token1,token2);            //读取下一个单词 
	if( strcmp("ID",token) )
	{
		es=2;
		printf("第%s行缺少程序名！\n",token2);             //单词的属性不是ID，进行出错处理 
	}
	
	fscanf(fp,"%s %s %s\n",token,token1,token2);                     
	if( strcmp(";",token) )                               
	{
		es=3;
		printf("第%s行缺少 ; !\n",token2);				  //没有";" 
	}
	fscanf(fp,"%s %s %s\n",token,token1,token2);     
	es=blockModel();                                     //转入到分程序模块 
	
	return es;

}

int blockModel()                                         //<block> →[<condecl>][<vardecl>][<proc>]<body> 
{
	int es=0;
	
	if(  !strcmp("const",token) )		                         
	{
		condeclModel();			                       //转移到常量声明模块 
	}
	if( !strcmp("var",token) )			
	{
		vardeclModel();			                       //转移到变量声明模块 
	}
	if( !strcmp("procedure",token) )	
	{
		procModel();				                   //转移到子程序模块 
	}
	
	es=bodyModel();			                           //转移复合语句模块 
 
	return es;

}
 
int condeclModel()                                       //<condecl> → const <const>{,<const>}; 
{
	int es=0;
	
	if( strcmp("const",token) )                             
	{
		es=2;
		printf("第%s行没有常量声明const！\n",token2);      //缺少常量声明const 
	}

	fscanf(fp,"%s %s %s\n",token,token1,token2);    
	es=constModel();                                       //转移到常量模块 
	
	while( strcmp(";",token) )			                   //处理文法片段{,<const>};
	{
		if( strcmp(",",token) )
			printf("第%s行缺少 , \n",token2);
			              
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		es=constModel();                                  //转移到常量模块   
	}
	fscanf(fp,"%s %s %s\n",token,token1,token2);

	return es;

}

int constModel()                                         //<const> → <id>:=<integer>
{
	int es=0,x,y;
	char name[20];     //记录单词的名称 

	if( strcmp("ID",token) )                             //<id>
	{
		printf("第%s行常量名不合法！ \n",token2);             
		es=3;
	}

	fscanf(fp,"%s %s %s\n",token,token1,token2);       //:= 
	if( strcmp(":=",token) )
	{
		printf("第%s行常量声明不合法！ \n",token2);     
		es=3;
	}

	fscanf(fp,"%s %s %s\n",token,token1,token2);       //<integer>
	if( strcmp("NUM",token) )
	{
		printf("第%s行常量格式不合法！ \n",token2);    
		es=3;
	}
	
	fscanf(fp,"%s %s %s\n",token,token1,token2);  

	return es;
}

int vardeclModel()                                       //<vardecl> → var <id>{,<id>}; 
{
	int es=0;
	
	if( strcmp("var",token) )
	{
		es=2;
		printf("第%s行缺少变量声明 var！\n",token2);      //var        
	}

	fscanf(fp,"%s %s %s\n",token,token1,token2);
	if( strcmp("ID",token) )
	{
		printf("第%s行变量名不合法！ \n",token2);         //<id>
		es=3;
	}
	
	fscanf(fp,"%s %s %s\n",token,token1,token2);         //{,<id>} 
	while( strcmp(";",token) )			
	{
		if( strcmp(",",token) )
			printf("第%s行缺少 , \n",token2);
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		if( strcmp("ID",token) )
		{
			printf("第%s行变量名不合法！ \n",token2);
			es=3;
		}
		fscanf(fp,"%s %s %s\n",token,token1,token2);
	}
	
	fscanf(fp,"%s %s %s\n",token,token1,token2);     
	return es;
}

int procModel()                                                       //<proc> → procedure <id>（<id>{,<id>}）;<block>{;<proc>}
{
	int es=0,i,y;
		
	if( strcmp("procedure",token) )
	{
		es=2;
		printf("第%s行缺少分程序声明 procedure ！\n",token2);        //procedure
	}

	fscanf(fp,"%s %s %s\n",token,token1,token2);
	if( strcmp("ID",token) )                                         //<id>
	{
		es=2;
		printf("第%s行程序名不合法！\n",token2);                  
	}

	fscanf(fp,"%s %s %s\n",token,token1,token2);
	if( strcmp("(",token) )                                        //（
	{
		es=2;
		printf("第%s行缺少（！\n",token2);                       
	}

	fscanf(fp,"%s %s %s\n",token,token1,token2);	            
	if( strcmp("ID",token) )                                      //<id>
	{
		es=2;
		printf("第%s行缺少变量名！\n",token2);                 
	}
	
	fscanf(fp,"%s %s %s\n",token,token1,token2);                
	while( strcmp( ")",token) )			                          //{,<id>}）  
	{
		if( strcmp(",",token) )
			printf("第%s行缺少 , \n",token2);                 
            fscanf(fp,"%s %s %s\n",token,token1,token2);
		if( strcmp("ID",token) )
		{
			printf("第%s行变量名不合法！ \n",token2);
			es=3;
		}

		fscanf(fp,"%s %s %s\n",token,token1,token2);
	}
	
	fscanf(fp,"%s %s %s\n",token,token1,token2);
	if( strcmp(";",token) )                                      // ;
	{
		es=2;
		printf("第%s行缺少 ; !\n",token2);                   
	}


	fscanf(fp,"%s %s %s\n",token,token1,token2);
	es=blockModel();                                            //转移到语句模块 

	while( !strcmp(";",token) )
		es=procModel();                                         //转移到子程序模块 
		
	return es;
}

int bodyModel()														//<body> → begin <statement>{;<statement>}end
{
	int es=0;
	
	if( strcmp("begin",token) )                                        //begin
	{
		es=2;
		printf("第%s行复合语句缺少语句头 begin ！\n",token2);        
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
		printf("第%s行复合语句缺少语句尾 end ！\n",token2);        
	}
	
	fscanf(fp,"%s %s %s\n",token,token1,token2);

	return es;
}


int statementModel()
{
	int es=0,y;
	char name[20];
	
    //<statement> → <id> := <exp>                                             
	if( !strcmp("ID",token) )                   //<id> 
	{

		fscanf(fp,"%s %s %s\n",token,token1,token2);
		if( strcmp(":=",token) )               //:=
		{
			es=2;
			printf("第%s行缺少：=\n",token2);
		}
		
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		es=expModel();                        //<exp> 
	}
	
	//<statement> →if <lexp> then <statement>[else <statement>] 
	else if( !strcmp("if",token) )          //if
	{
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		es=lexpModel();                     //<lexp>
		
		if( strcmp("then",token) )        //then
		{
			es=2;
			printf("第%s行缺少 then \n",token2);
		}
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		es=statementModel();             //<statement> 
		
		if( !strcmp("else",token) )	    //[else <statement>] 
		{
			fscanf(fp,"%s %s %s\n",token,token1,token2);
			es=statementModel();
		} 
	}
	
	//<statement> →while <lexp> do <statement>
	else if( !strcmp("while",token) )     //while 
	{
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		es=lexpModel();                     //<lexp> 
		        
		if( strcmp("do",token) )            //do 
		{
			es=2;
			printf("第%s行缺少 do \n",token2);
		}
		
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		es=statementModel();               //<statement> 
	
	}
    //<statement> →call <id>[（<exp>{,<exp>}）]
	else if( !strcmp("call",token) )     //call 
	{
		int x;
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		if( strcmp("ID",token) )         //<id>
		{
			es=2;
			printf("第%s行缺少函数名 \n",token2);
		}
			
		fscanf(fp,"%s %s %s\n",token,token1,token2);  //[（<exp>{,<exp>}）] 
		if( !strcmp("(",token) )	 //（ 
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
				printf("第%s行缺少右括号 \n",token2);
			}
			fscanf(fp,"%s %s %s\n",token,token1,token2);
		}
		
	}
	// <statement> →<body>
	else if( !strcmp("begin",token) )    
	{
		bodyModel();
	}
	// <statement> -> read (<id>{，<id>})
	else if( !strcmp("read",token) )     //read 
	{
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		if( strcmp("(",token) )	         //(
		{
			es=2;
			printf("第%s行缺少(！\n",token2);
		}
		
	    fscanf(fp,"%s %s %s\n",token,token1,token2);
		if( strcmp("ID",token) )        //<id>
		{
			es=2;
			printf("第%s行缺少变量名 \n",token2);
		}
		
		fscanf(fp,"%s %s %s\n",token,token1,token2);
		while( !strcmp(",",token) )       //{，<id>} 
			{
				fscanf(fp,"%s %s %s\n",token,token1,token2);
				if( strcmp("ID",token) )
				{
					es=2;
					printf("第%s行缺少变量名 \n",token2);
				}
				else
					fscanf(fp,"%s %s %s\n",token,token1,token2);
			}
		
		if( strcmp(")",token) )	      //） 
		{
			es=2;
			printf("第%s行缺少）！\n",token2);
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
			printf("第%s行缺少(！\n",token2);
		}
		
		fscanf(fp,"%s %s %s\n",token,token1,token2); //<exp>
		if( expModel() )		//<exp>
			printf("第%s行表达式不合法！\n",token2);
		
		while( !strcmp(",",token) )           //{,<exp>}
		{
				fscanf(fp,"%s %s %s\n",token,token1,token2);
				if( expModel() )		
					printf("第%s行表达式不合法！\n",token2);
				fscanf(fp,"%s %s %s\n",token,token1,token2);
				
		}
		
		if( strcmp(")",token) )	         //） 
		{
			es=2;
			printf("第%s行缺少）！\n",token2);
		}
		fscanf(fp,"%s %s %s\n",token,token1,token2);
	}

	else        //错误处理 
	{
		es=3;
		printf("第%s行statement语句不合法!\n",token2);
	}

	return es;
}


//<lexp> → <exp> <lop> <exp>|odd <exp>
int lexpModel()
{
	int es=0;
	//<lexp> → <exp> <lop> <exp>
	if( !expModel() ) //
	{
		
		es=lopModel();  //<lop>
		
		es=expModel();
	}
	else if( !strcmp("odd",token) )			//第二种
		es=expModel();
	else						//错误
	{
		es=3;
		printf("第%s行条件不合法！\n",token2);
	}

	return es;
}

int expModel()                                //<exp> → [+|-]<term>{<aop><term>}
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

int termModel()                              //<term> → <factor>{<mop><factor>} 
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

int facterModel()                           //<factor>→<id>|<integer>|(<exp>)
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
			printf("第%s行缺少 ）",token2);
	}
	else                                //错误处理 
	{
		es=3;
		printf("第%s行因子定义不合法！\n",token2);
	}
	fscanf(fp,"%s %s %s\n",token,token1,token2);

	return es;
}

int lopModel()                    //<lop> → =|<>|<|<=|>|>=
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
	else								//错误
	{
		es=2;
		printf("第%s行关系运算符错误！\n",token2);
	}
	
    fscanf(fp,"%s %s %s\n",token,token1,token2);
	return es;
}

int aopModel()
{
	int es=0;
	if( strcmp("+",token)&& strcmp("-",token))   //<aop> → +|-
	{
		es=2;
	}
	
	return es;
}

int mopModel()                    //<mop> → *|/
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
			printf("语法分析成功！"); 
	}
	return 0;

}
