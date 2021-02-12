// in modern C programs, you can simplify this another way, by putting the console I/O into a thread or lightweight process.
// Then the I/O can go on in the usual blocking fashion, but the data can be inserted into a queue to be processed on another thread.


#include <unistd.h>
#include <stdio.h>


int main(int argc, char *argv[]) {

  char              file[]="conwatcher";
  pid_t             fork_pid;
  char              *args[]={file,NULL}; 

  fork_pid = fork();

  if (fork_pid == 0) 
  {
    printf("Hello from the child!\n");
    execvp(args[0],args);
  } 

  printf("Hello from the parent!\n");
  printf("%s\n", "main process running");
  
  

  return 0;
}

// #include <unistd.h>
// int main(int argc, char *argv[]) {
//   execl("/bin/bash", "/yes/its/bash", "-c", "echo $0 && uptime", NULL);

//   return 0;
// }