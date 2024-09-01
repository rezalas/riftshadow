// #include "queue.h"
// #define NOP asm volatile("nop");

// CQueue *CQueue::queue_first = 0; /*null*/

// CQueue::CQueue()
// {
// }

// CQueue::~CQueue()
// {
// }
// void CQueue::ProcessQueue()
// {
// 	int i;
// 	void *j;
// 	CQueue *qf, *qf_next;
// 	for(qf = queue_first; qf; qf = qf_next)
// 	{
// 		qf_next = qf->queue_next;
// 		if(qf->queue_delay < 0) /* weird bug i don't want to find, probably a fucked up addtoq somewhere in the code */
// 		{
// 			qf->FreeQueue();
// 			continue;
// 		}
// 		if(--qf->queue_delay == 0)
// 		{
// 			switch(qf->queue_numargs)
// 			{
// 				case 0:
// 					(*qf->queue_function)();
// 					break;
// 				case 1:
// 					(*qf->queue_function)(qf->queue_args[0]);
// 					break;
// 				case 2:
// 					(*qf->queue_function)(qf->queue_args[0],qf->queue_args[1]);
// 					break;
// 				case 3: 
// 					(*qf->queue_function)(qf->queue_args[0],qf->queue_args[1],qf->queue_args[2]);
// 					break;
// 				case 4:
// 					(*qf->queue_function)(qf->queue_args[0],qf->queue_args[1],qf->queue_args[2],qf->queue_args[3]);
// 					break;
// 				case 5:
// 					(*qf->queue_function)(qf->queue_args[0],qf->queue_args[1],qf->queue_args[2],qf->queue_args[3],qf->queue_args[4]);
// 					break;
// 				case 6:
// 					(*qf->queue_function)(qf->queue_args[0],qf->queue_args[1],qf->queue_args[2],qf->queue_args[3],qf->queue_args[4],qf->queue_args[5]);
// 					break;
// 				case 7:
// 					(*qf->queue_function)(qf->queue_args[0],qf->queue_args[1],qf->queue_args[2],qf->queue_args[3],qf->queue_args[4],qf->queue_args[5],qf->queue_args[6]);
// 					break;
// 				case 8:
// 					(*qf->queue_function)(qf->queue_args[0],qf->queue_args[1],qf->queue_args[2],qf->queue_args[3],qf->queue_args[4],qf->queue_args[5],qf->queue_args[6],qf->queue_args[7]);
// 					break;
// 			}		
			
// 			qf->FreeQueue();
// 		}
// 	}
// }
// void CQueue::AddToQueue(int nTimer, int nArgs, ...)
// {
// 	va_list ap;
// 	int i;
// 	void *hax[MAX_QUEUE_ARGS];

// 	if(nTimer < 0)
// 	 	Logger.Warn("Negative Queue Timer - NumArgs: {}", nArgs);	
// 	CQueue *nq = new CQueue;
// 	nq->queue_delay = nTimer;
// 	nq->queue_numargs = nArgs;
// 	nq->queue_next = queue_first;
// 	queue_first = nq;
// 	va_start(ap, nArgs);
// 	nq->queue_function = (QUEUE_FUNCTION)va_arg(ap, void *);
// 	for(i = 0; i < MAX_QUEUE_ARGS; i++)
// 		nq->queue_args[i] = va_arg(ap, void *);
// 	va_end(ap);
// }

// void CQueue::FreeQueue(void)
// {
// 	if(this == queue_first)
// 	{
// 		queue_first = this->queue_next;
// 		delete this;
// 		return;
// 	}
// 	CQueue *r;
// 	for(r = queue_first; r && r->queue_next != this; r = r->queue_next)
// 		;
// 	if(!r)
// 		throw("FreeQueue(): Invalid linked list");
// 	r->queue_next = this->queue_next;
// 	delete this;
// }

// bool CQueue::HasQueuePending(void *qChar)
// {
// 	CQueue *r;
// 	for(r = queue_first; r; r = r->queue_next)
// 		for(int i = 0; i < r->queue_numargs; i++)
// 			if(r->queue_args[i] == qChar && r->queue_delay > 0)
// 				return true;
// 	return false;
// }

// void CQueue::DeleteQueuedEventsInvolving(void *qChar)
// {
// 	CQueue *r, *r_next = 0;//null
// 	int deleted = 0;
// 	for(r = queue_first; r != 0 /*null*/; r = r_next)
// 	{
// 		r_next = r->queue_next;
// 		for(int i = 0; i < r->queue_numargs; i++)
// 			if(r->queue_args[i] == qChar && r->queue_delay > 0)
// 			{
// 				r->FreeQueue();
// 				deleted++;
// 				break;
// 			}
// 	}

// 	Logger.Warn("{} events deleted.", deleted);
// }
