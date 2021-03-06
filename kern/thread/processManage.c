#include <kern/processManage.h>
#include <lib.h>
#include <kern/errno.h>

/* Process Create Function */
int process_create(pid_t ppid, pid_t cpid, struct thread * selfThread) {
	
	/* initialize the process structure */	
	struct process * proc;
	proc = kmalloc(sizeof(struct process));
	KASSERT(proc != NULL);

	proc->exitcv = waitpidcv;
	proc->exited = false;
	proc->self = selfThread;
	proc->ppid = ppid;	
	
	/* initialize the process table with child's pid*/	
	p_table[cpid] = proc;

	return 0;
}


/* pid allocator function */
int pid_alloc(pid_t * pidValue) {
	
	for (int i=1; i < MAX_RUNNING_PROCS; i++) {
		if (p_table[i] == NULL) {
			 *pidValue = i;
			 return 0;	// return 0 upon success
		}	
	}

	return ENPROC;	// case of error, process table is full
}

/* Process Destroy Function*/
void process_destroy(pid_t pidValue) {
	
	/* Destroy the process structure and set the p_table value of the corresponding process to NULL*/
	struct process * procToDestroy = p_table[pidValue];
	KASSERT(procToDestroy != NULL);

	kfree(procToDestroy);
	p_table[pidValue] = NULL;

}

/* Initialize synch primitives for waitpid */
void waitpid_init(){
	waitpidlock = lock_create("waitpid lock");
	waitpidcv = cv_create("waitpid cv");
	p_tablelock = lock_create("ptable lock");
	p_table[1]->exitcv = waitpidcv;
}
