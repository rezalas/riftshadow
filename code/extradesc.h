#ifndef EXTRADESC_H
#define EXTRADESC_H

class CExtraDesc
{
public:
	CExtraDesc();
	~CExtraDesc();
	
	CExtraDesc *			next;
	
	RString					keyword;
	RString					description;
	
	bool					IsValid();
private:
	bool					valid;
};

#endif /* EXTRADESC_H */
