#define MAX_FORM	 22

#define FORM_QUEST 	 0
#define FORM_PLANT 	 1
#define FORM_MYTH_ANIMAL 2
#define FORM_EXTRAPLANAR 3

#define FORM_QUEST	 0
#define FORM_OFFENSE	 1
#define FORM_DEFENSE     2
#define FORM_UTILITY	 3

extern const struct form_type form_table[MAX_FORM];

struct  form_type
{
    char *      name;                   // Form name, i.e. lion
    char *	short_descr;		// Form short, i.e. a grizzled ram
    char *	long_descr;		// form long: A lion whips around to glare at you threateningly. */
    sh_int      form_stats[5];  	// form stats, increase this if a new stat is added
    sh_int	armor[4];		// form ACs
    sh_int	dam_mod;		// form dam mod, 100 is normal dam
    sh_int	attacks;		// max attacks form gets per round
    char *	attack_type;    	// damage name, i.e. claw, must be on attack_table
    sh_int      parry_modifier;         // how difficult is it to parry? (% modifier, 50 is 50% easier, -50 is 50% harder)
    sh_int	dice_number;		// damage dice number, in 5d4+35 the 5
    sh_int	dice_type;		// damage dice type, in 5d4+35 the 4
    sh_int	dice_add;		// damage dice addition, the 35
    sh_int	hit_roll;		// form hitroll
    sh_int	saves_spell;		// form saves
    long	imm_flags[MAX_BITVECTOR];		// form immunities
    long	res_flags[MAX_BITVECTOR];		// form resistances
    long	vuln_flags[MAX_BITVECTOR];		// form vulns
    long	affected_by[MAX_BITVECTOR];		// form affected_by	
    long	specials[MAX_BITVECTOR];		// form special bits, like juzam terror or whatever
    sh_int	form_category;		// quest, plant, extraplanar, animal
    sh_int	form_foci;		// offense, quest, utility, etc.
    char *	yell;			// what the form echoes when it's attacked
    char *	description;		// form description
    char *	spec_one;		// name of first special, like "rake"
    char *	spec_two;		// second special
    char *	spec_three;		// third special
};
