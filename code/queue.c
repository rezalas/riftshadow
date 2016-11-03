#include "queue.h"
#define NOP asm volatile("nop");
void bug( const char *str, int param );
/*
* DO NOT TOUCH ANYTHING IN THIS FILE
* ____ANYTHING____
*/

CQueue *CQueue::queue_first = 0; /*null*/

CQueue::CQueue()
{
}

CQueue::~CQueue()
{
}
void CQueue::ProcessQueue()
{
	int i;
	void *j;
	CQueue *qf, *qf_next;
	for(qf = queue_first; qf; qf = qf_next)
	{
		qf_next = qf->queue_next;
		if(qf->queue_delay < 0) /* weird bug i don't want to find, probably a fucked up addtoq somewhere in the code */
		{
			qf->FreeQueue();
			continue;
		}
		if(--qf->queue_delay == 0)
		{
		/*
			asm volatile("
						subl 	$64, %esp
						pushl	%edx
						pushl	%eax
    					movl    $0, %eax
					    .p2align 2
					.J14:
					    cmpl    44(%ebx), %eax
					    jl  	.J17
					    jmp 	.J15
					.J17:
						movl	%eax, -24(%ebp)
						movl	%edx, -28(%ebp)
						movl	%ecx, -32(%ebp)
	
						movl	%ebx, %edx

						imul	$4, %eax
						addl	$12, %edx
						addl	%eax, %edx
						movl	(%edx), %edx
						pushl	%edx
						movl	-24(%ebp), %eax
						movl    -28(%ebp), %edx
						movl	-32(%ebp), %ecx
						incl    %eax
						jmp 	.J14
					.J15:
						");
		   */
		   	for(int r = 0; r < qf->queue_numargs; r++)
			{
                        	asm volatile("" \
                                "movl    %0, %%eax\n\t" \
                                "pushl   %%eax\n\t"
				:
                                : "r"(qf->queue_args[r])
                                : "%eax");
			}
			(*qf->queue_function) ();
			for(int r = 0; r < qf->queue_numargs; r++)
				asm volatile("popl %%eax" : : : "%eax");
			qf->FreeQueue();
			/*asm volatile("
						popl	%eax
						popl	%edx
						");*/
		}
	}
}
void CQueue::AddToQueue(int nTimer, int nArgs, ...)
{
	va_list ap;
	int i;
	void *hax[MAX_QUEUE_ARGS];
	if(nTimer < 0)
		bug("Negative Queue Timer - NumArgs: %d", nArgs);	
	CQueue *nq = new CQueue;
	nq->queue_delay = nTimer;
	nq->queue_numargs = nArgs;
	nq->queue_next = queue_first;
	queue_first = nq;
	va_start(ap, nArgs);
	nq->queue_function = (QUEUE_FUNCTION)va_arg(ap, void *);
	for(i = 0; i < MAX_QUEUE_ARGS; i++)
		nq->queue_args[i] = 0; //null
	for(i = 0; i < nArgs; i++)
		hax[i] = va_arg(ap, void *);
	for(i = 0; i < nArgs; i++)
		nq->queue_args[i] = hax[nArgs - i - 1]; //don't even ask why
	va_end(ap);
}

void CQueue::FreeQueue(void)
{
	if(this == queue_first)
	{
		queue_first = this->queue_next;
		delete this;
		return;
	}
	CQueue *r;
	for(r = queue_first; r && r->queue_next != this; r = r->queue_next)
		;
	if(!r)
		throw("FreeQueue(): Invalid linked list");
	r->queue_next = this->queue_next;
	delete this;
}

bool CQueue::HasQueuePending(void *qChar)
{
	CQueue *r;
	for(r = queue_first; r; r = r->queue_next)
		for(int i = 0; i < r->queue_numargs; i++)
			if(r->queue_args[i] == qChar && r->queue_delay > 0)
				return TRUE;
	return FALSE;
}

void CQueue::DeleteQueuedEventsInvolving(void *qChar)
{
	CQueue *r, *r_next = 0;//null
	int deleted = 0;
	for(r = queue_first; r != 0 /*null*/; r = r_next)
	{
		r_next = r->queue_next;
		for(int i = 0; i < r->queue_numargs; i++)
			if(r->queue_args[i] == qChar && r->queue_delay > 0)
			{
				r->FreeQueue();
				deleted++;
				break;
			}
	}
	bug("%d events deleted.", deleted);
}
