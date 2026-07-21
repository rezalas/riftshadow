#ifndef SIZE_H
#define SIZE_H

/*
 * CSize - Some men would have you believe it doesn't matter.
 * 
 * -Cal
 */
class CSize
{
public:
	CSize();
	~CSize();

	RString &	GetName();
	
	CSize *		first_size;
private:
	int			size;
	CSize *		next_size;
	RString		name;
};

#endif /* SIZE_H */
