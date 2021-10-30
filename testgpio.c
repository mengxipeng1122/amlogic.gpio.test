
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
#include "odroidc2.h"
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


// all 681s pads gpio defines
#define GPIO_1P_START (C2_GPIOY_PIN_START+0 )
#define GPIO_1P_UP    (C2_GPIOY_PIN_START+1 )
#define GPIO_1P_DOWN  (C2_GPIOY_PIN_START+2 )
#define GPIO_1P_LEFT  (C2_GPIOY_PIN_START+3 )
#define GPIO_1P_RIGHT (C2_GPIOY_PIN_START+4 )
#define GPIO_1P_A     (C2_GPIOY_PIN_START+5 )
#define GPIO_1P_B     (C2_GPIOY_PIN_START+6 )
#define GPIO_1P_C     (C2_GPIOY_PIN_START+7 )
#define GPIO_1P_D     (C2_GPIOY_PIN_START+8 )
#define GPIO_1P_E     (C2_GPIOY_PIN_START+9 )
#define GPIO_1P_F     (C2_GPIOY_PIN_START+10)

#define GPIO_2P_START (C2_GPIOZ_PIN_START+2 )
#define GPIO_2P_UP    (C2_GPIOZ_PIN_START+3 )
#define GPIO_2P_DOWN  (C2_GPIOZ_PIN_START+4 )
#define GPIO_2P_LEFT  (C2_GPIOZ_PIN_START+5 )
#define GPIO_2P_RIGHT (C2_GPIOZ_PIN_START+6 )
#define GPIO_2P_A     (C2_GPIOZ_PIN_START+8 )
#define GPIO_2P_B     (C2_GPIOZ_PIN_START+9 )
#define GPIO_2P_C     (C2_GPIOZ_PIN_START+10)
#define GPIO_2P_D     (C2_GPIOZ_PIN_START+11)
#define GPIO_2P_E     (C2_GPIOZ_PIN_START+12)
#define GPIO_2P_F     (C2_GPIOZ_PIN_START+13)

#define GPIO_COIN     (C2_GPIOY_PIN_START+15)
#define GPIO_MODE     (C2_GPIOY_PIN_START+16)


int main()
{
    int pin = (C2_GPIOY_PIN_START + 1);
    struct libodroid	libwiring;
    init_odroidc2(&libwiring);
    libwiring.mode=MODE_GPIO;

    // pin direction 
    libwiring.pinMode(GPIO_1P_START, INPUT);
    libwiring.pinMode(GPIO_1P_UP   , INPUT);
    libwiring.pinMode(GPIO_1P_DOWN , INPUT);
    libwiring.pinMode(GPIO_1P_LEFT , INPUT);
    libwiring.pinMode(GPIO_1P_RIGHT, INPUT);
    libwiring.pinMode(GPIO_1P_A    , INPUT);
    libwiring.pinMode(GPIO_1P_B    , INPUT);
    libwiring.pinMode(GPIO_1P_C    , INPUT);
    libwiring.pinMode(GPIO_1P_D    , INPUT);
    libwiring.pinMode(GPIO_1P_E    , INPUT);
    libwiring.pinMode(GPIO_1P_F    , INPUT);

    libwiring.pinMode(GPIO_2P_START, INPUT);
    libwiring.pinMode(GPIO_2P_UP   , INPUT);
    libwiring.pinMode(GPIO_2P_DOWN , INPUT);
    libwiring.pinMode(GPIO_2P_LEFT , INPUT);
    libwiring.pinMode(GPIO_2P_RIGHT, INPUT);
    libwiring.pinMode(GPIO_2P_A    , INPUT);
    libwiring.pinMode(GPIO_2P_B    , INPUT);
    libwiring.pinMode(GPIO_2P_C    , INPUT);
    libwiring.pinMode(GPIO_2P_D    , INPUT);
    libwiring.pinMode(GPIO_2P_E    , INPUT);
    libwiring.pinMode(GPIO_2P_F    , INPUT);

    libwiring.pinMode(GPIO_MODE    , INPUT);
    libwiring.pinMode(GPIO_COIN    , INPUT);

    msg(MSG_WARN,"test pi input \n");
    for(int i = 0;; i ++)
    {
        int value  = libwiring.digitalRead(pin);
        msg(MSG_WARN,"1p %d %d %d %d %d %d %d %d %d %d %d 2p %d %d %d %d %d %d %d %d %d %d %d coin %d mode %d %d\n"
            , libwiring.digitalRead(GPIO_1P_START)
            , libwiring.digitalRead(GPIO_1P_UP   )
            , libwiring.digitalRead(GPIO_1P_DOWN )
            , libwiring.digitalRead(GPIO_1P_LEFT )
            , libwiring.digitalRead(GPIO_1P_RIGHT)
            , libwiring.digitalRead(GPIO_1P_A    )
            , libwiring.digitalRead(GPIO_1P_B    )
            , libwiring.digitalRead(GPIO_1P_C    )
            , libwiring.digitalRead(GPIO_1P_D    )
            , libwiring.digitalRead(GPIO_1P_E    )
            , libwiring.digitalRead(GPIO_1P_F    )
            , libwiring.digitalRead(GPIO_2P_START)
            , libwiring.digitalRead(GPIO_2P_UP   )
            , libwiring.digitalRead(GPIO_2P_DOWN )
            , libwiring.digitalRead(GPIO_2P_LEFT )
            , libwiring.digitalRead(GPIO_2P_RIGHT)
            , libwiring.digitalRead(GPIO_2P_A    )
            , libwiring.digitalRead(GPIO_2P_B    )
            , libwiring.digitalRead(GPIO_2P_C    )
            , libwiring.digitalRead(GPIO_2P_D    )
            , libwiring.digitalRead(GPIO_2P_E    )
            , libwiring.digitalRead(GPIO_2P_F    )
            , libwiring.digitalRead(GPIO_COIN    )
            , libwiring.digitalRead(GPIO_MODE    )
            , libwiring.analogRead (0            )
            );


        usleep(100000);
    }
}
