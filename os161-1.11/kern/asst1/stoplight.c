/* 
 * stoplight.c
 *
 * 31-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: You can use any synchronization primitives available to solve
 * the stoplight problem in this file.
 */


/*
 * 
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>

struct lock *nw;
struct lock *ne;
struct lock *se;
struct lock *sw;
struct lock *acquiring;

/*
 *
 * Constants
 *
 */

/*
 * Number of cars created.
 */

#define NCARS 20


/*
 *
 * Function Definitions
 *
 */


/*
 * gostraight()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement passing straight through the
 *      intersection from any direction.
 *      Write and comment this function.
 */



static
void
gostraight(unsigned long cardirection,
           unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */
        
        kprintf("\ncar %d in intersection, going straight",carnumber);
}


/*
 * turnleft()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a left turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnleft(unsigned long cardirection,
         unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */

        kprintf("\ncar %d in intersection, turning left",carnumber);
}


/*
 * turnright()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a right turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnright(unsigned long cardirection,
          unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */

        kprintf("\ncar %d in intersection, turning right",carnumber);
}


/*
 * approachintersection()
 *
 * Arguments: 
 *      void * unusedpointer: currently unused.
 *      unsigned long carnumber: holds car id number.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Change this function as necessary to implement your solution. These
 *      threads are created by createcars().  Each one must choose a direction
 *      randomly, approach the intersection, choose a turn randomly, and then
 *      complete that turn.  The code to choose a direction randomly is
 *      provided, the rest is left to you to implement.  Making a turn
 *      or going straight should be done by calling one of the functions
 *      above.
 */
 
static
void
approachintersection(void * unusedpointer,
                     unsigned long carnumber)
{
        int cardirection, turning;

        /*
         * Avoid unused variable and function warnings.
         */

        (void) unusedpointer;
        (void) carnumber;
	(void) gostraight;
	(void) turnleft;
	(void) turnright;

        /*
         * cardirection is set randomly.
         */

        cardirection = random() % 4;
        turning = random() % 3;

        int choice = cardirection * 10 + turning;
        //acquire acquisition lock

        switch(choice)
        {
            case 0:
		kprintf("\ncar %d approaching from west, headed south",carnumber);
                lock_acquire(acquiring);
                    lock_acquire(sw);
                lock_release(acquiring);
                turnright(cardirection,carnumber);
                lock_release(sw);
		kprintf("\ncar %d leaving intersection",carnumber);
                break;
            case 1:
		kprintf("\ncar %d approaching from west, headed east",carnumber);
                lock_acquire(acquiring);
                    lock_acquire(sw);
                    lock_acquire(se);
                lock_release(acquiring);
                gostraight(cardirection,carnumber);
                lock_release(sw);
                lock_release(se);
		kprintf("\ncar %d leaving intersection",carnumber);
		break;
            case 2:
		kprintf("\ncar %d approaching from west, headed north",carnumber);
                lock_acquire(acquiring);
                    lock_acquire(sw);
                    lock_acquire(se);
                    lock_acquire(ne);
                lock_release(acquiring);
                turnleft(cardirection,carnumber);
                lock_release(sw);
                lock_release(se);
                lock_release(ne);
		kprintf("\ncar %d leaving intersection",carnumber);
                break;
            case 10:
                kprintf("\ncar %d approaching from south, headed east",carnumber);
		lock_acquire(acquiring);
                    lock_acquire(se);
                lock_release(acquiring);
                turnright(cardirection,carnumber);
                lock_release(se);
		kprintf("\ncar %d leaving intersection",carnumber);
                break;
            case 11:
		kprintf("\ncar %d approaching from south, headed north",carnumber);
                lock_acquire(acquiring);
                    lock_acquire(se);
                    lock_acquire(ne);
                lock_release(acquiring);
                gostraight(cardirection,carnumber);
                lock_release(se);
                lock_release(ne);
		kprintf("\ncar %d leaving intersection",carnumber);
                break;
            case 12:
		kprintf("\ncar %d approaching from south, headed left",carnumber);
                lock_acquire(acquiring);
                    lock_acquire(se);
                    lock_acquire(ne);
                    lock_acquire(nw);
                lock_release(acquiring);
                turnleft(cardirection,carnumber);
                lock_release(se);
                lock_release(ne);
                lock_release(nw);
		kprintf("\ncar %d leaving intersection",carnumber);
                break;
            case 20:
		kprintf("\ncar %d approaching from east, headed north",carnumber);
                lock_acquire(acquiring);
                    lock_acquire(ne);
                lock_release(acquiring);
                turnright(cardirection,carnumber);
                lock_release(ne);
		kprintf("\ncar %d leaving intersection",carnumber);
                break;
            case 21:
		kprintf("\ncar %d approaching from east, headed west",carnumber);
                lock_acquire(acquiring);
                    lock_acquire(ne);
                    lock_acquire(nw);
                lock_release(acquiring);
                gostraight(cardirection,carnumber);
                lock_release(ne);
                lock_release(nw);
		kprintf("\ncar %d leaving intersection",carnumber);
                break;
            case 22:
		kprintf("\ncar %d approaching from east, headed south",carnumber);
                lock_acquire(acquiring);
                    lock_acquire(ne);
                    lock_acquire(nw);
                    lock_acquire(sw);
                lock_release(acquiring);
                turnleft(cardirection,carnumber);
                lock_release(ne);
                lock_release(nw);
                lock_release(sw);
		kprintf("\ncar %d leaving intersection",carnumber);
                break;
            case 30:
		kprintf("\ncar %d approaching from north, headed west",carnumber);
                lock_acquire(acquiring);
                    lock_acquire(nw);
                lock_release(acquiring);
                turnright(cardirection,carnumber);
                lock_release(nw);
		kprintf("\ncar %d leaving intersection",carnumber);
                break;
            case 31:
		kprintf("\ncar %d approaching from north, headed south",carnumber);
                lock_acquire(acquiring);
                    lock_acquire(nw);
                    lock_acquire(sw);
                lock_release(acquiring);
                gostraight(cardirection,carnumber);
                lock_release(nw);
                lock_release(sw);
		kprintf("\ncar %d leaving intersection",carnumber);
                break;
            case 32:
		kprintf("\ncar %d approaching from north, headed east",carnumber);
                lock_acquire(acquiring);
                    lock_acquire(nw);
                    lock_acquire(sw);
                    lock_acquire(se);
                lock_release(acquiring);
                turnleft(cardirection,carnumber);
                lock_release(nw);
                lock_release(sw);
                lock_release(se);
		kprintf("\ncar %d leaving intersection",carnumber);
                break;
        }

        //release acquisition lock

}


/*
 * createcars()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up the approachintersection() threads.  You are
 *      free to modiy this code as necessary for your solution.
 */

int
createcars(int nargs,
           char ** args)
{
        int index, error;

        /*
         * Avoid unused variable warnings.
         */

        (void) nargs;
        (void) args;

        /*
         * Start NCARS approachintersection() threads.
         */

         nw = lock_create("nw");
         ne = lock_create("ne");
         sw = lock_create("sw");
         se = lock_create("se");
         acquiring = lock_create("acquiring");


        for (index = 0; index < NCARS; index++) {

                error = thread_fork("approachintersection thread",
                                    NULL,
                                    index,
                                    approachintersection,
                                    NULL
                                    );

                /*
                 * panic() on error.
                 */

                if (error) {
                        
                        panic("approachintersection: thread_fork failed: %s\n",
                              strerror(error)
                              );
                }
        }
        return 0;
}
