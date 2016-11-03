class CObject //not a CEntity because it doesn't have a UID, just its ObjTemplate UID
{
	CObject();
	~CObject();

	CObject *			next;
	
	CObjTemplate *		objtemplate;
	RString &			GetUID();				/* we overload getuid to refer to mommy's uid */
	
	CObject *			GetContained();			/* get first object inside this one */
	CObject *			GetContainer();			/* object this is inside */
	CObject *			GetNextOnChar();		/* get next item on the character with this, for loops */
	CObject *			GetNextInRoom();		/* next item in room, for loops */
	CObject *			GetOn();				/* what is the object on top of? */
	
	RString &			GetName(void);
	RString &			GetShortDescr(void);
	RString &			GetDescription(void);
	int					GetWeight(void);
	int					GetLevel(void);
	int					GetTimer(void);					//ticks till crumble
	
	CObjApplyData *		GetFirstApply(void);
	
	void				SetWeight(int nNewWeight);
	void				SetLevel(int nNewWeight);
	void				SetTimer(int nNewTimer);		//ticks till crumble
	CCharacter *		carried_by;
	
	CExtraDesc *		extra_descr_first;
	CAffectData *		affected_first;
	CAffectData *		char_affects;
	CRoom *				in_room;
	CItemType *			item_type;
	CMaterial *			material;
	
	RString				owner;

	CBitvector			extra_flags;
	CBitvector			wear_flags;
	CBitvector			restrict_flags;
	CBitvector			imm_flags;
	CBitvector			res_flags;
	CBitvector			vuln_flags;

protected:
	CObject *			next_content;
	CObject *			contains;
	CObject *			in_obj;
	CObject *			on;

	CObjApplyData *		apply;
	bool				valid;
	int					weight;
	unsigned int		cost;
	int					level;
	int					timer;
	
	RString				name;
	RString				short_descr;
	RString 			description;
};
