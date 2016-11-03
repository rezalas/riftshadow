#include "rift.h"
#include <stdarg.h>
/*
* DO NOT TOUCH ANYTHING
* this is all in assembly and fucking around in queue.h or .c will break it
* then I WILL BREAK YOU
*/
#define MAX_QUEUE_ARGS		8	
typedef void (*QUEUE_FUNCTION)(...);
class CQueue
{
public:
	CQueue();
	~CQueue();
	
	static void 		ProcessQueue(void);
	static void		AddToQueue(int nTimer, int nArgs, ...);
	static bool		HasQueuePending(void *qChar);	
	static void		DeleteQueuedEventsInvolving(void *qChar);
private:
	void			FreeQueue();

	static CQueue *	queue_first;
	CQueue *		queue_next;
	QUEUE_FUNCTION 	queue_function;
	int				queue_delay;				/* seconds _remaining_ */
	void *			queue_args[MAX_QUEUE_ARGS];	/* Queue function args */
	int				queue_numargs;
};
