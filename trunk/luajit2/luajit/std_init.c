
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>


int errfd, outfd;
extern int _use_stderr;
extern int _use_stdout;


void __atexit()
{
    if(_use_stderr) {
        fflush(stderr);
        close(errfd);
    }

    if(_use_stdout){
        fflush(stdout);
        close(outfd);
    }
}


void std_init(const char *script)
{
    char __lua_temp[256];

    if(_use_stderr)
    {
        strcpy(__lua_temp, script);
        strcat(__lua_temp, "stderr.txt");
        int fd = open(__lua_temp, O_WRONLY | O_CREAT | O_TRUNC);
        __setup_stderr_fd(fd);
        errfd = fd;
    }

    if(_use_stdout)
    {
        strcpy(__lua_temp, script);
        strcat(__lua_temp, "stdout.txt");
        int fd = open(__lua_temp, O_WRONLY | O_CREAT | O_TRUNC);
        __setup_stdout_fd(fd);
        outfd = fd;
    }

    atexit(__atexit);
}
