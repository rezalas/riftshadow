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
