#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
//global variablesss
int laps=0;
int lapTime=0;
int totalTime=0;

void signalfunc(int signal) 
{
    //printf("okok");
    printf("received SIGCHLD\n");
    printf("total time in seconds: %d\n", laps*lapTime);
    exit(0);
        //printf("okok");
}

//cout<<"ok"; 

void child() 
{
    //printf("okok");
for (int i=1; i <=laps; i++) {
        //printf("okok");
        sleep(lapTime);
          //printf("okok");
        printf("lap: %d completed\n", i);
}
    exit(0);
}



int main(int argc, char *argv[]) {
if (argc != 3) {
        printf("wrong input: <number of laps> <lap time>\n");
        return 1;
}

  laps = atoi(argv[1]);
  lapTime = atoi(argv[2]);
   
  signal(SIGCHLD, signalfunc);  //signalhandler :)

   // printf("enter number of laps: ");
    //scanf("%d", &laps);
   // printf("enter lap time: ");
   // scanf("%d", &lapTime);

  int pid = fork();
    //if(pid>0)
    //cout<<"child process is not created";    
    
    
    //checking fork system calls
  if (pid == 0) 
    {
        child();
    } else {
        wait(NULL);
    }
    return 0;
}
