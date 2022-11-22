/* poll_input.c

  Licensed under GNU General Public License v2 or later.
*/
#include <poll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
		       } while (0)
extern char **environ;
char *ourenv[] = {
	"DISPLAY=:0"
};

int main(int argc, char *argv[])
{
   int nfds, num_open_fds;
   struct pollfd *pfds;
   time_t *ptimes;
   const char keys[]={'b','e'};


   /*
   char **x;
   for (x=environ;*x;x++) {
	   printf("ENV: %s\n",*x);
   }
   */


   if (argc < 2) {
      fprintf(stderr, "Usage: %s file...\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   num_open_fds = nfds = argc - 1;
   pfds = calloc(nfds, sizeof(struct pollfd));
   ptimes = calloc(nfds, sizeof(time_t));
   if (pfds == NULL)
       errExit("malloc");

   /* Open each file on command line, and add it 'pfds' array */

   for (int j = 0; j < nfds; j++) {
       pfds[j].fd = open(argv[j + 1], O_RDONLY);
       if (pfds[j].fd == -1)
	   errExit("open");

       printf("Opened \"%s\" on fd %d\n", argv[j + 1], pfds[j].fd);

       pfds[j].events = POLLPRI ;
       time(&ptimes[j]);
   }

   /* Keep calling poll() as long as at least one file descriptor is
      open */

   while (num_open_fds > 0) {
       int ready;
	time_t now;

       ready = poll(pfds, nfds, -1);
       if (ready == -1)
	   errExit("poll");

       	time(&now);
       //printf("Ready: %d\n", ready);

       /* Deal with array returned by poll() */

       for (int j = 0; j < nfds; j++) {
	   char buf[10];

	   if (pfds[j].revents != 0) {
		   /*
	       printf("  fd=%d; events: %s%s%s ", pfds[j].fd,
		       (pfds[j].revents & POLLIN)  ? "POLLIN "  : "",
		       (pfds[j].revents & POLLPRI)  ? "POLLPRI "  : "",
		       (pfds[j].revents & POLLHUP) ? "POLLHUP " : "",
		       (pfds[j].revents & POLLERR) ? "POLLERR " : "");
		       */

	       if (pfds[j].revents & (POLLPRI | POLLIN) ) {
		       lseek(pfds[j].fd,0,SEEK_SET);
		   ssize_t s = read(pfds[j].fd, buf, sizeof(buf));
		   if (s == -1)
		       errExit("read");
		   //printf("    read %zd bytes: %.*s", s, (int) s, buf);

		   if (buf[0] ==  '0') {
			   if (now  <= ptimes[j]+1) {
				   //printf("DEBOUNCE\n");
				} else {
				   printf("\n\n\nKEYPRESS\n");

				   int pid = fork();
				   printf("PID returnd %d\n",pid);
				   if (pid == 0) {
					   char keybuf[2];
					   keybuf[0]=keys[j];
					   keybuf[1]=(char)0;
					   setuid(1001);
					   int r = execle("/usr/bin/xdotool","xdotool","key",keybuf,0L,ourenv);
					   printf("Execle returned %d errno %d\n",r,errno);
					   perror("EXECLE: ");
				   } else {
					   int status;
					   waitpid(pid,&status,0);
					   printf("Status pid %d returned %d\n",pid,status);
				   }

				}
			   }
		   time(&ptimes[j]);

	       } 
	       /* else {                
		   printf("    closing fd %d\n", pfds[j].fd);
		   if (close(pfds[j].fd) == -1)
		       errExit("close");
		   num_open_fds--;
	       } */
	   }
       }
   }

   printf("All file descriptors closed; bye\n");
   exit(EXIT_SUCCESS);
}

