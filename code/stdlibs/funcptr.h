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
		for(i = 0; i < nArgs; i++)
		{
			ptrs[i] = fptrs[nArgs - i - 1];
			asm volatile("" \
					"movl 	%0, %%eax\n\t" \
					"pushl	%%eax\n\t" \
					:
					: "r"(ptrs[i])
					: "%eax");
		}
		(*funcptr) ();
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
