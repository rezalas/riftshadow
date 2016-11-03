#include "rift.h"
#include "weather.h"

#define MAX_ADJACENT	25

class CArea
{
public:
	CArea();
	~CArea();
	CArea *			next;
	static CArea *	first;
	
	static void		LoadAreaData();
	static void		LoadAreaTable(const char *query); /*this is reserved for bootstrap use*/
	
	int				GetPlayerCount(void);
	int				GetAreaAge(void);
	CAreaAffect *	GetFirstAffect(void);
	
	short int		low_range;	/* lowest safe level */
	short int		high_range; /* highest useful level */
	short int		area_type;
	short int		index;

	short int		min_vnum;
	short int		max_vnum;
	
	short int		type;
	short int		climate;
	
	RString			name;
	RString			area_file;
	RString			credits;

	CBitvector		flags;

	CArea *			adjacent_areas[MAX_ADJACENT];
	CWeather *		area_weather;
private:
	int				area_age;
};
