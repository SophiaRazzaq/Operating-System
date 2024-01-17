#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include<readline/readline.h> 
#include<readline/history.h>
  

void removeWhiteSpace(char* params){
	if(params[strlen(params)-1]==' ' || params[strlen(params)-1]=='\n')
	params[strlen(params)-1]='\0';
	
	if(params[0]==' ' || params[0]=='\n') 
	memmove(params, params+1, strlen(params));
}


void tokenize_input(char** param,int *limit,char *buf,const char *c){
	char *token;
	token=strtok(buf,c);
	int pc=-1;
	while(token){
		param[++pc]=malloc(sizeof(token)+1);
		strcpy(param[pc],token);
		removeWhiteSpace(param[pc]);
		token=strtok(NULL,c);
	}
	param[++pc]=NULL;
	*limit=pc;
}

void Pipes(char** params,int limit){
	
	
	int fd[limit][2];
	int i;
	int pc;
	char *argv[100];

	for(i=0;i<limit;i++){
		tokenize_input(argv,&pc,params[i]," ");
		if(i!=limit-1){
			if(pipe(fd[i])<0){
				perror("pipe creating was not successfull\n");
				return;
			}
		}
		if(fork()==0){
			if(i!=limit-1){
				dup2(fd[i][1],1);
				close(fd[i][0]);
				close(fd[i][1]);
			}

			if(i!=0){
				dup2(fd[i-1][0],0);
				close(fd[i-1][1]);
				close(fd[i-1][0]);
			}
			execvp(argv[0],argv);
			perror("invalid input ");
			exit(1);
		}
		
		if(i!=0){
			close(fd[i-1][0]);
			close(fd[i-1][1]);
		}
		wait(NULL);
	}
}

void PipesandRedirect(char** params, int limit, char ** redirect,int mode)
{
     int fd[limit][2];
	int i;
	int pc;
	char *argv[100];
	int des;
	removeWhiteSpace(redirect[1]);
	for(i=0;i<limit;i++){
		tokenize_input(argv,&pc,params[i]," ");
		if(i!=limit-1){
			if(pipe(fd[i])<0){
				perror("pipe creating was not successfull\n");
				return;
			}
		}
		if(fork()==0){
			
			if(i==limit-1)
			{
			 if(mode==1)
			 {
			   des=open(redirect[1],O_WRONLY,O_CREAT,O_TRUNC);
			   if(fd<0){
			perror("cannot open file\n");
			return;
			}

			dup2(des,1);
			   
			  }
			  
			 if(mode==0)
			 {
			   des=open(redirect[1],O_RDONLY,O_CREAT,O_TRUNC);
			   if(fd<0){
			perror("cannot open file\n");
			return;
			}

			dup2(des,0);
			   
			  }
			  
			 
			}
			if(i!=limit-1){
				dup2(fd[i][1],1);
				close(fd[i][0]);
				close(fd[i][1]);
			}

			if(i!=0){
				dup2(fd[i-1][0],0);
				close(fd[i-1][1]);
				close(fd[i-1][0]);
			}
			execvp(argv[0],argv);
			perror("invalid input ");
			exit(1);
		}
		
		if(i!=0){
			close(fd[i-1][0]);
			close(fd[i-1][1]);
		}
		wait(NULL);
	}
}




void executeoutput(char** params,int limit){
	if(limit!=2)
	return;
	
	int counter;
	int fd;
	char *argv[100];
	removeWhiteSpace(params[1]);
	tokenize_input(argv,&counter,params[0]," ");
	if(fork()==0){

		
		  fd=open(params[1],O_RDONLY | O_CREAT | O_TRUNC);
		

		if(fd<0){
			perror("cannot open file\n");
			return;
		}

			dup2(fd,0);
		
		execvp(argv[0],argv);
		perror("invalid input ");
		exit(1);
	}
	wait(NULL);
}

void executeinput(char** params,int limit){
	if(limit!=2)
	return;
	
	int counter;
	int fd;
	char *argv[100];
	removeWhiteSpace(params[1]);
	tokenize_input(argv,&counter,params[0]," ");
	if(fork()==0){

		
		  fd=open(params[1],O_WRONLY | O_CREAT | O_TRUNC );
	

		if(fd<0){
			perror("cannot open file\n");
			return;
		}

			dup2(fd,1);
		
		execvp(argv[0],argv);
		perror("invalid input ");
		exit(1);
	}
	wait(NULL);
}



int main()
{
 
 char* parse[100];
 int limit=0;
 char* pr[100];
 int pc=0;
 printf("\n\n\n\t****KT's SHELL****");
 printf("\n");
 
 
 while(1)
 {
    char cur_dir[1024];
    getcwd(cur_dir, sizeof(cur_dir));
    printf("\nDirectory: %s", cur_dir);
    
    char* input;
    input=readline("\n>>> ");
    char str[500];
    strcpy(str,input);
    if (strlen(input) == 0) 
    {
    	printf("Not valid input");
    	break;

    } 
    else
     {
        for (int i = 0; i < 100; i++)
         {
        	parse[i] = strsep(&input, " ");
  
        	if (parse[i] == NULL)
            		break;
        	if (strlen(parse[i]) == 0)
            		i--;
    	}
    
        
       
       
 	 if (strcmp(parse[0],"exit") == 0) 
 	 {
 	    
 	    printf("\nYou Exited!\n");
 	    exit(0);
 	   
         }
         
          else if(strchr(str,'|') && strchr(str,'>'))
         {
          tokenize_input(pr,&pc,str,">");
          tokenize_input(parse,&limit,pr[0],"|");
          
          
	  PipesandRedirect(parse,limit,pr,1);
         
         }
          
          else if(strchr(str,'|') && strchr(str,'<'))
         {
          tokenize_input(pr,&pc,str,"<");
          tokenize_input(parse,&limit,pr[0],"|");
          
          
	  PipesandRedirect(parse,limit,pr,0);
         
         }
          
         else if(strchr(str,'|'))
         {
          
          tokenize_input(parse,&limit,str,"|");
          
	  Pipes(parse,limit);
         
         }
         
         else if(strchr(str,'<'))
         {
          
          tokenize_input(parse,&limit,str,"<");
	
	executeoutput(parse,limit);
			
         
         }
         
         else if(strchr(str,'>'))
         {
          
          tokenize_input(parse,&limit,str,">");
	
	executeinput(parse,limit);
	}
         
         
         
         else if (strcmp(parse[0],"cd") == 0) 
 	 {
 	    chdir(parse[1]);
            
         }
         
         else
         {
            pid_t retVal=fork();
 	 	if (retVal == -1) 
 	 	{
        		printf("\nFailed");
        	} 
        	else if (retVal == 0) 
        	{
        		if (execvp(parse[0], parse) < 0)
        		{ 
            			printf("\nCommand Failed");
            		}	
        	
        		exit(0);
        	} 
        	
     		else if(retVal>0)
     		{
        		wait(NULL);
        		
        	}
         }
     }
 
   	
}
exit(0);
return 0;
}

