//simple_syscalls.c
//#include <current.h>
#include <synch.h>
#include <syscall.h>
#include <thread.h>
#include <curthread.h>
#include <lib.h>
#include <machine/trapframe.h>
#include <addrspace.h>
#include <types.h>
//2^32-1


int
sys_getpid() {
	int output;
	output = curthread->sPid->id;
	if (output>0){
		return output;
	}
	return (-1);
}

int
sys_waitpid(pid_t pid) {
	int nPid=((int)pid);
	struct node *temp=pidroot;
	int flag=0;
	//find the matching PID
	while (!temp->next==0){
		temp=temp->next;
		if(temp->id==nPid){
			flag=1;
			break;
		}
	}
	//didnt find the pid
	if (flag==0){
		return(-1);
	}
	//not used
	if (temp->used==0){
		return (nPid);
	}
	//wait for that process to finish
	temp->waiting++;
	P(temp->sem);
	return(nPid);
}


int
sys__exit(void) {

//recovers pid and signals any waiting process
	curthread->sPid->used=0;
	int i;
	//signal any waiting threads to wake up
	for (i=0;i<curthread->sPid->waiting;i++){
		V(curthread->sPid->sem);
	}
	curthread->sPid->waiting=0;
	curthread->sPid=0;
	thread_exit();
	return (0);
}


//

//int sys_fork(struct trapframe *tf,struct addrspace* p_addr,int32_t retval)
//int sys_fork(int32_t *retval,struct trapframe *tf)
/*
	NOTE: we don't need retval as a parameter b/c that's the return value of the function
	we also do not need to pass in a pointer to address space b/c that's simple extracted from the trapframe, i think...
	
	ALSO: we may need to include a pid as a parameter in order to
	return either the child pid in the parent thread or 0 in the child thread
*/
int sys_fork(struct trapframe *tf)
{
	//save the id of the parent to figure out which value fork should return, 0 to child, pid of child to parent
	//pid_t parent_pid = curthread->sPid->id;

	//declare trapframe and address-space for new thread
	struct trapframe *new_tf;
	struct addrspace *new_addrspace;

	//copy trapframe of parent/old thread to new/child process/thread
	new_tf = kmalloc(sizeof(struct trapframe));
	bzero(new_tf,sizeof(struct trapframe));
	//int s;
	//s = splhigh();
	memcpy(new_tf,tf,sizeof(struct trapframe));
	//splx(s);

	//copy addrspace from parent/old thread to new/child process/thread
	int result = 0;
	result = as_copy(curthread->t_vmspace,&new_addrspace);
	if(result)
	{
		return result; //as_copy failed, return error code
	}

	struct thread *child_thread;
	child_thread->t_vmspace = new_addrspace;
	result = 0;
	//thread_fork(curthread->t_name, (void*)tf_copy, 0, md_forkentry, &child_thread);
	//result = thread_fork(curthread->t_name,&tf,0,&md_forkentry,&child_thread);
	//result = thread_fork(curthread->t_name,new_tf,0,&md_forkentry,&child_thread);
	result = thread_fork("child process",new_tf,(unsigned long)new_addrspace,&md_forkentry,&child_thread);
	if(result)
	{
		//thread fork failed
		//free any resources that were allocated to child
		kfree(new_tf);
		kfree(new_addrspace);
		//return an error code
		return result;
	}

	return child_thread->sPid->id; //parent returns with child pid
}

int sys_execv(userptr_t progname,userptr_t args)
{
	/*
		1. copy args from user space into kernel buffer
		(first, figuring out how much space we'll need
		to allocate in order to fit everything in the buffer)
	*/

	//may need to use copyinstr, since what we get is just a pointer to a char array, or maybe not...

	//need to read argument vector one null-terminated string at a time
	int argc = 0;
	int len = 0; //will keep track of number of int-sized blocks to allocate
	//ASSUMPTION: we don't know how many arguments there are and there could be more than 4 arguments
	//first, determine argc and total size
	while(args[argc] != '\0') //until we get a NULL pointer, keep reading arguments
	{
		int argsize = strlen(args[argc]) + 1; //length of unpadded string + NULL char
		len += (argsize / 4) + ((argsize % 4) ? 0 : 1);//keep track of total length of argument vector (actual length + 1 for NULL terminator)
		argc++; //increment argc to reflect the number of arguments
	}
	
	//create a buffer and fill it w/ our pointers and arguments
	char *kargv;

	kargv = kmalloc(sizeof(int) * (len + argc + 1));
	//copy argument pointers
	result = copyin(args,kargv,argc*4);
	if(result)
	{
		return result;
	}
	//copy arguments from argv into kargv
	int i;
	int dummy;
	for(i=0;i<argc;i++)
	{
		result = copyinstr((userptr_t)args[i],kargv[i],1024,&dummy);
		if(result)
		{
			return result;
		}
	}
	/*
		2. open executable, create new addrspace and load elf into it
	*/

	//use the read in progname to open the file
	//destroy old address space
	//create new address space
	//activate the address space
	//load_elf into the addrspace
	//close file

	/*
		3. copy args from kernel buffer into user stack
	*/

	//
	//define user stack in address space

	/*
		4. return to user mode
	*/
}

/*
 *  Simple workaround for system call read(int, char *, int).
 *  This is not how a system call should be implemented. 
 *  But, for now, it works.
 */
int
sys_read(int filehandle, char *buf, size_t size)
{
	(void) filehandle;
	(void) size;
	
	char ch = getch();	
	if(ch == '\r') {
		ch = '\n';
	}
	buf[0] = ch;
	return 1;
}

/*
 *  Implementation of system call write(int, char *, int).
 *  This is not how a system call should be implemented. 
 *  But, for now, it works.
 */
 
int
sys_write(int filehandle, char *buf, size_t size)
{
	(void) filehandle;
	
	size_t i;	
	for (i = 0; i < size; ++i) {
		putch((int)buf[i]);
	}	
	return i;
}
