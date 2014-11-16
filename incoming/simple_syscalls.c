//simple_syscalls.c
#include <current.h>
#include <synch.h>
#include <syscall.h>
#include <thread.h>
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
	pid_t parent_pid = curthread->sPid->id;

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

	result = 0;
	//thread_fork(curthread->t_name, (void*)tf_copy, 0, md_forkentry, &child_thread);
	result = thread_fork(curthread->t_name,&tf,0,md_forkentry,&child_thread);
	if(result)
	{
		//thread fork failed
		//free any resources that were allocated to child
		//return an error code
	}

	return child_thread->sPid->id; //parent returns with child pid
}