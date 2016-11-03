class CObjTemplate : public CEntity
{
public:
		
	CObjTemplate(); //note: under the current code these are NOT CALLED!
	~CObjTemplate;
	
	CObject *				CreateObject(void);
	
	RString &				GetName(void);
	RString &				GetShortDescr(void);
	RString &				GetDescription(void);
	int						GetCabal(void);
	
	CObjTemplate *			next;
	CAffectData *			affected_first;
	CAffectData *			char_affects;
	CObjApplyData *			apply;
	CArea *					area;
	CItemType *				item_type;
	CSpecFunc *				spec_prog;
	CMaterial *				material;
	
	int						level;
	int						condition;
	int						weight;
	int						count;
	int						cost;
	int						limit_total;				//max limit
	int						limit_count;				//# in circulation

	int						values[5];
	int						start_timer;				//item begins to crumble when created
	RString					wear_echo[2];
	RString					remove_echo[2];
	RString					verb;
	RString					notes;
	
	CBitvector				extra_flags;
	CBitvector				wear_flags;
	CBitvector				restrict_flags;
	CBitvector				imm_flags;
	CBitvector				res_flags;
	CBitvector				vuln_flags;
	
protected:
	RString					name;
	RString					short_descr;
	RString					description;
	int						cabal;
};
