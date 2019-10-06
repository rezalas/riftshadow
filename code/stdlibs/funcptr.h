#ifndef FUNCPTR_H
#define FUNCPTR_H

typedef void (*CPtrCast)(...);
#define MAX_FUNCTION_ARGS	8
#include <stdarg.h>

//void *fargs[MAX_FUNCTION_ARGS];

class CFuncPtr
{
public:
	void		AddImmediateArg(void *arg)
	{
		//do this if it's really necessary
	}
	void		Execute(int nArgs, ...)
	{
		if(MAX_FUNCTION_ARGS < nArgs)
			throw "Error: CFuncPtr has too many args";
		
		int i;
		void *fptrs[MAX_FUNCTION_ARGS], *ptrs[MAX_FUNCTION_ARGS];
		va_list ap;
		va_start(ap, nArgs);
		
		for(i = 0; i < nArgs; i++)
			fptrs[i] = va_arg(ap, void *);
            
        switch(nArgs)
        {
            case 0:
                (*funcptr)();
                break;
            case 1:
                (*funcptr)(fptrs[0]);
                break;
            case 2:
                (*funcptr)(fptrs[0],fptrs[1]);
                break;
            case 3: 
                (*funcptr)(fptrs[0],fptrs[1],fptrs[2]);
                break;
            case 4:
                (*funcptr)(fptrs[0],fptrs[1],fptrs[2],fptrs[3]);
                break;
            case 5:
                (*funcptr)(fptrs[0],fptrs[1],fptrs[2],fptrs[3],fptrs[4]);
                break;
            case 6:
                (*funcptr)(fptrs[0],fptrs[1],fptrs[2],fptrs[3],fptrs[4],fptrs[5]);
                break;
            case 7:
                (*funcptr)(fptrs[0],fptrs[1],fptrs[2],fptrs[3],fptrs[4],fptrs[5],fptrs[6]);
                break;
            case 8:
                (*funcptr)(fptrs[0],fptrs[1],fptrs[2],fptrs[3],fptrs[4],fptrs[5],fptrs[6],fptrs[7]);
                break;
        }
		/*for(i = 0; i < nArgs; i++)
		{
			ptrs[i] = fptrs[nArgs - i - 1];
			asm volatile("" \
					"movl 	%0, %%eax
	" \
					"pushl	%%eax
	" \
					:
					: "r"(ptrs[i])
					: "%eax");
		}
		(*funcptr) ();*/
		va_end(ap);
	}
	void		SetPtr(void *newptr)
	{
		funcptr = (CPtrCast)newptr;
	}
private:
	CPtrCast	funcptr;
	//char		nargs;
};

#endif /* FUNCPTR_H */
