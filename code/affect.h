class CAffect
{
public:
	CAffect();
	~CAffect();

	CCharacter *		GetOwner();
	RString &			GetName();
	
	CAffect *			next;			//next on target of affect, i.e. next affect on char
	
	void				SetName(RString &nName);
	void				SetOwner(CCharacter *nOwner);
	
	bool				valid;

	int					type;
	
	int					where;
	int					level;
	int					duration;
	int					location;
	int					modifier;
	int					aftype;

	CBitvector			bitvector;
	CBitvector			properties;		//permanent?  a malady?  etc
	
	CFuncPtr			pulse_fun;
	CFuncPtr			tick_fun;
	CFuncPtr			end_fun;		//triggers on affect removal.  DO NOT DEPEND ON (due to crashes).

	void				Apply(CCharacter *ch); //overloading rocks your fucking socks off
	void				Apply(CObject *ch);
	void				Apply(CRoom *ch);
	void				Apply(CArea *ch);

	enum
	{
		TARG_CHAR, TARG_OBJ, TARG_ROOM, TARG_AREA	//affect target types
	};
	void				RemoveAffect(); //eliminates results of affect, i.e. -str and calls FreeAffect()
protected:
	void *				target;			//who the affect is placed on
	int					target_type;
	CCharacter *		owner;
	RString				name;
};
