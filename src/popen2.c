/*
	popen2.c
	--------------
	Base code taken from http://dzone.com/snippets/simple-popen2-implementation
	
	Modified by Thomas Maurice <tmaurice59@gmail.com> for error handling with
	errno and perror(). Also modified to impelment the redirect of the
	stderr error stream to which was in my opinion a lack in the original code.

	This code is obviously not portable, it is for *NIX systems only because
	windows-like systems don't implement pipes
*/

/**
	\file popen2.c
	\brief An implementation of popen2()
	
	\author Thomas Maurice
	\version 0.2
*/

#include <popen2.h>

/**
	\brief Implementation of popen2
	
	This function will start a new thread by forking. But when the standard
	popen() function allow you only one way communication with the child
	process, popen2() enables you to have both writing **and** reading
	access to it. In case of error, some calls to perror() will be done
	before returning a negative pid, so that you can know what went wrong
	during the call to the function.
	
	For exemple, to list all the files of the directory using it, you
	will use :
	~~~~~~~~~~~~~~{.c}
	int in, out, err; // Pointers to file descriptors
	int nbyte; // Bytes read
	char buffer[512];
	pid_t pid = popen2("ls", &in, &out, &err);
	
	if(pid < 0) {
		printf("Something went wrong");
		exit(0);
	}
	
	while((nbyte = read(out, buffer, sizeof(buffer))) > 0) {
		printf("%s", buffer);
	}
	
	close(in);
	close(out);
	
	~~~~~~~~~~~~~~
	
	Note that in C++ the parameters *outfp* and *errfp* are not mandatory and can
	be omitted, they will be treated as NULL in the code function. So the following
	calls are valid too in C++:
	
	~~~~~~~~~~~~~~{.cpp}
	int in, out, err; // Pointers to file descriptors
	pid_t pid = popen2("ls", &in, &out, &err);
	pid = popen2("ls", &in);
	pid = popen2("ls", &in, &out);
	~~~~~~~~~~~~~~
	
	\param[in] command the command to execute
	\param[out] infp pointer to the input stream of the process 
	\param[out] outfp pointer to the output stream of the process
	\param[out] errfp pointer to the output *error* stream of the process
	
	\return the pid of the launched process, a negative number otherwise
*/

pid_t popen2(const char *command, int *infp, int *outfp, int *errfp)
{
    int p_stdin[2], p_stdout[2], p_stderr[2];
    pid_t pid;

    if (pipe(p_stdin) != 0 || pipe(p_stdout) != 0 || pipe(p_stderr) != 0) {
				perror("pipe() : ");
        return -1;
		}
		
		// Try to fork 30 times in case of failure
		int tries = 30;

		do {
    	pid = fork();
			tries--;
		} while (tries >= 0 && pid == -1 && errno == EAGAIN);

    if (pid < 0) {
				perror("fork() : ");
        return pid;
		} else if (pid == 0) {
        close(p_stdin[WRITE]);
        dup2(p_stdin[READ], READ);
        close(p_stdout[READ]);
        dup2(p_stdout[WRITE], WRITE);
        close(p_stderr[READ]);
        dup2(p_stderr[WRITE], ERR);

        execl("/bin/sh", "sh", "-c", command, NULL);
        perror("execl() : ");
        exit(1);
    }

    if (infp == NULL)
        close(p_stdin[WRITE]);
    else
        *infp = p_stdin[WRITE];

    if (outfp == NULL)
        close(p_stdout[READ]);
    else
        *outfp = p_stdout[READ];
        
    if (errfp == NULL)
        close(p_stderr[READ]);
    else
        *errfp = p_stderr[READ];
	
    return pid;
}

