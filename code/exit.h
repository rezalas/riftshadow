class CExit
{
public:
	CExit();
	~CExit();

	CExit *			next;
	
	RString			keyword;

	short int		key; //convert to cobj?
	
	CRoom *			to_room; //?
	
	CBitvector		exit_info;

};
