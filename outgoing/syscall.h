#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

#include <types.h>


int sys_reboot(int code);
int sys_waitpid(pid_t pid);
int sys_getpid(void);
int sys__exit(void);

int sys_read(int filehandle, char *buf, size_t size);
int sys_write(int filehandle, char *buf, size_t size);

//int sys_fork(void (*entrypoint)(void *data1, unsigned long data2),
//int sys_fork(struct trapframe *tf,struct addrspace* p_addr,int32_t retval);
struct trapframe;
int sys_fork(struct trapframe *tf);
//int sys_execv(char *prog, char **argv,int *retval);
int sys_execv(userptr_t progname,userptr_t argv);


#endif /* _SYSCALL_H_ */
