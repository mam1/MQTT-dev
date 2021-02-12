// in modern C programs, you can simplify this another way, by putting the console I/O into a thread or lightweight process.
// Then the I/O can go on in the usual blocking fashion, but the data can be inserted into a queue to be processed on another thread.


#include <unistd.h>
#include <stdio.h>


int main(int argc, char *argv[]) {

  const char        file[]="conwatcher2";
  pid_t             fork_pid;
  char *args[]={"./EXEC",NULL}; 



  fork_pid = fork();

  if (fork_pid == 0) 
  {
    printf("Hello from the child!\n");
    execl(file,file," ");
  } 
  else 
  {
    printf("Hello from the parent!\n");
  }

  return 0;
}

// #include <unistd.h>
// int main(int argc, char *argv[]) {
//   execl("/bin/bash", "/yes/its/bash", "-c", "echo $0 && uptime", NULL);

//   return 0;
// }