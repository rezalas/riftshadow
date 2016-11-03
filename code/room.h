#define MAX_EXIT 		6
#define MAX_TRACKS 		4
class CRoom
{
friend class CArea;
public:
	CRoom() { }
	~CRoom() { }

	static CRoom *	first;
	CRoom *			next;
	CCharacter *	people;
	CObject *		contents;
	CExtraDesc *	extra_descr;

	short int		vnum;
	short int		area_id;
	short int		index;

	CArea *			GetArea();
	CExit *			GetExit(int nExit);
	int				GetSectorType();
	int				GetCabal(void);
	int				GetGuild(void);
	RString &		GetTitle();
	RString &		GetDescription();
	bool			IsLit(void);

	void			SetArea(CArea *newArea) { area = newArea; }
	
	static void		LoadRoomTable(const char *query);

	CTrackData *	tracks[MAX_TRACKS];
	CPathFindData *	path;
	CTrapData *		trap;
	CRuneData *		rune;
	CRoomAffect *	first_affect;

	RString			owner;
	
	CBitvector		room_flags;
	CBitvector		room_aff_flags;

	short int		heal_rate;
	short int		mana_rate;
	short int		cabal;
protected:
	CArea *			area;
	short int		sector_type;
	CExit *			exits[MAX_EXIT];
	short int		room_guild;
	short int		room_light;
	RString			name;
	RString			description;
};
