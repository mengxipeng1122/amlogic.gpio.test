
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif


#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/utsname.h>
#include <asm/ioctl.h>

#include "wiringPi.h"
/*----------------------------------------------------------------------------*/
/* Debug message display function */
/*----------------------------------------------------------------------------*/
#define	MSG_ERR		-1
#define	MSG_WARN	-2

#ifndef	TRUE
#define	TRUE	(1==1)
#define	FALSE	(!TRUE)
#endif


/*----------------------------------------------------------------------------*/
//
// Return true/false if the supplied module is loaded
//
/*----------------------------------------------------------------------------*/
int moduleLoaded (char *modName)
{
	int len   = strlen (modName) ;
	int found = FALSE ;
	FILE *fd = fopen ("/proc/modules", "r") ;
	char line [80] ;

	if (fd == NULL) {
		fprintf (stderr, "gpio: Unable to check /proc/modules: %s\n",
			strerror (errno)) ;
		exit (1) ;
	}

	while (fgets (line, 80, fd) != NULL) {
		if (strncmp (line, modName, len) != 0)
			continue ;

		found = TRUE ;
		break ;
	}
	fclose (fd) ;

	return found ;
}


/*----------------------------------------------------------------------------*/
//
// ODROID System Message function
//
/*----------------------------------------------------------------------------*/
int msg (int type, const char *message, ...)
{
	va_list argp;
	char buffer [1024];

	va_start (argp, message) ;
	vsnprintf (buffer, 1023, message, argp);
	va_end (argp) ;

	fprintf (stderr, "%s : %s", type == MSG_WARN ? "warn" : "err", buffer) ;

	if (type != MSG_WARN)
		exit (EXIT_FAILURE) ;
	return 0 ;
}

int main()
{
    int pin = 249;
    struct libodroid	libwiring;
    init_odroidc2(&libwiring);
    libwiring.mode=MODE_GPIO;
    libwiring.pinMode(pin, INPUT);

    msg(MSG_WARN,"test pi input \n");
    for(int i = 0;i< 1000; i ++)
    {
        int value  = libwiring.digitalRead(pin);
        msg(MSG_WARN,"test pi %d\n", value);
        usleep(100000);
    }
}
