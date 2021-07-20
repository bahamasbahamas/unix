#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
   int fd , pid2 , pid3 ;
   char result [1337];
   int fd_pipe[2];
   pipe(fd_pipe);
   fd = open ( "buffer", O_CREAT );
   pid2 = fork () ;
    if (pid2 <0){
        perror("error: fork pid2");
    }
    if ( pid2 == 0) {
        close(1); // close stdout
        close(fd_pipe[0]);
        dup(fd_pipe[1]);
        pid3 = fork ();
        if (pid3 <0){
            perror("error: fork pid3");
        }
        if ( pid3 == 0) {
            result[0] =  's';
            write ( fd , result , 1337) ;
            unlink ( " buffer " ) ;
        }
        else{
            wait(NULL);
            char buf[10];
            sprintf(buf, "%d", pid3);
            write(1, buf,10);
       }
    }
    else {
        wait (NULL) ;
        // P1 Parent
        close(0); // close stdin
        close(fd_pipe[1]); // close pipe (write)
        dup(fd_pipe[0]); // copy fd pipe (read) to stdin
        char buf[10];
        read(fd_pipe[0], buf, 10);
        printf ( " PID von P2 : %d , PID von P3 : %s " , pid2 , buf ) ;
        read ( fd , result , 1337) ;
    }
    return 0;
