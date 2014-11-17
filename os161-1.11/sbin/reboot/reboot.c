#include <unistd.h>

/*
 * reboot - shut down system and reboot it.
 * Usage: reboot
 *
 * Just calls reboot() with the RB_REBOOT flag.
 */

int
main()
{
	printf("Just stopping execution");
	reboot(RB_REBOOT);
	return 0;
}
