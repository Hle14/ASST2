#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);
int sys_waitpid(pid_t pid);
int sys_getpid(void);
int sys__exit(void);



//int sys_fork(void (*entrypoint)(void *data1, unsigned long data2),
//int sys_fork(struct trapframe *tf,struct addrspace* p_addr,int32_t retval);
int sys_fork(struct trapframe *tf);
//int sys_execv(char *prog, char **argv,int *retval);


#endif /* _SYSCALL_H_ */
