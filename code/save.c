
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"
#include "lookup.h"
#include "tables.h"

extern  int     _filbuf         args( (FILE *) );


int rename(const char *oldfname, const char *newfname);
long lvuln_flags[MAX_BITVECTOR];
long limmune_flags[MAX_BITVECTOR];
long lres_flags[MAX_BITVECTOR];
bool check_parse_name args((char *name));

char *print_flags(long flag[])
{
    int count, pos = 0;
    static char buf[52];

	/* Morg - Valgrind fix */
	buf[0] = '\0';

    for (count = 0; count < (MAX_BITVECTOR * 32);  count++)
    {
        if (IS_SET(flag,count))
        {
            if (count < 26)
                buf[pos] = 'A' + count;
            else
                buf[pos] = 'a' + (count - 26);
            pos++;
        }
    }

    if (pos == 0)
    {
        buf[pos] = '0';
        pos++;
    }

    buf[pos] = '\0';

    return buf;
}


/*
 * Array of containers read for proper re-nesting of objects.
 */
#define MAX_NEST	100
static	OBJ_DATA *	rgObjNest	[MAX_NEST];



/*
 * Local functions.
 */
void	fwrite_char	args( ( CHAR_DATA *ch,  FILE *fp ) );
void	fwrite_obj	args( ( CHAR_DATA *ch,  OBJ_DATA  *obj,
			    FILE *fp, int iNest ) );
void	fwrite_pet	args( ( CHAR_DATA *pet, FILE *fp) );
void    fwrite_charmie  args( ( CHAR_DATA *ch, FILE *fp) );
void	fread_char	args( ( CHAR_DATA *ch,  FILE *fp ) );
void    fread_pet	args( ( CHAR_DATA *ch,  FILE *fp ) );
void	fread_obj	args( ( CHAR_DATA *ch,  FILE *fp ) );
void    fread_charmie   args( ( CHAR_DATA *ch, FILE *fp ) );


int isAftSpell (int aftype) {
	if ((aftype != AFT_SKILL) &&
	(aftype != AFT_POWER) &&
	(aftype != AFT_MALADY) &&
	(aftype != AFT_INVIS) &&
	(aftype != AFT_RUNE) &&
	(aftype != AFT_COMMUNE) &&
	(aftype != AFT_TIMER)) {
		aftype = AFT_SPELL;
	}
	return aftype;
}

/*
 * Save a character and inventory.
 * Would be cool to save NPC's too for quest purposes,
 *   some of the infrastructure is provided.
 */
extern int mPort;
void save_char_obj( CHAR_DATA *ch )
{
    char strsave[MAX_INPUT_LENGTH], filenm[MSL], query[MSL*2];
    FILE *fp;
    CHAR_DATA *search;
	int res;

    if ( IS_NPC(ch) || mPort == 4000) //do not save, sir!!!
	return;

    if ( ch->desc != NULL && ch->desc->original != NULL )
	ch = ch->desc->original;

	if (!check_parse_name(ch->true_name)) {
		wiznet("ALERT!! $N/$F name corrupt!!",ch,NULL,0,0,0);
	}

    /* create god log */
    fclose( fpReserve );
	if(!IS_IMMORTAL(ch))
	{
		res = RS.SQL.Update("players SET pks=%.2f,level=%d,class=%d,race=%d,cabal=%d,sex=%d,hours=%ld,align=%d,ethos=%d,gold=%ld WHERE name='%s' LIMIT 1",
		ch->pcdata->frags[PK_KILLS], ch->level, ch->Class()->GetIndex(), ch->race, ch->cabal, ch->pcdata->true_sex,
		(ch->played + current_time - ch->logon) / 3600,ch->alignment,ch->pcdata->ethos,ch->gold+ch->gold_bank,ch->true_name);
	}	

	sprintf(strsave, "%s%s.plr",PLAYER_DIR, capitalize(ch->true_name));
    if ( ( fp = fopen( TEMP_FILE, "w" ) ) == NULL )
    {
	bug( "Save_char_obj: fopen", 0 );
	perror( strsave );
    }
    else
    {

	fwrite_char( ch, fp );
	if(ch->carrying)
	    fwrite_obj( ch, ch->carrying, fp, 0 );
	if(!ch->carrying && ch->pcdata->old && ch->pcdata->old->carrying)
	    fwrite_obj( ch, ch->pcdata->old->carrying, fp, 0);
	/* save the pets */
	if (ch->pet != NULL && ch->pet->in_room == ch->in_room)
	    fwrite_pet(ch->pet,fp);
        for (search = char_list; search != NULL; search = search->next)
                if (IS_NPC(search) && (search->master == ch) && 
			(IS_SET(search->act,ACT_UNDEAD) || (!strcmp(ch->Class()->name,"necromancer") && 
				!IS_SET(search->act, ACT_PET)))
                      /*  && search->in_room->vnum==ch->in_room->vnum */ && IS_AFFECTED(search,AFF_CHARM))
				fwrite_charmie(search,fp);
	fprintf( fp, "#END\n" );
    }
    fclose( fp );
    rename(TEMP_FILE,strsave);
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}



/*
 * Write the char.
 */
void fwrite_charmie(CHAR_DATA *ch, FILE *fp)
{
    fprintf(fp, "#CHARMED\n");
    fprintf(fp, "%d %d %s~ %s~\n",	ch->pIndexData->vnum, ch->in_room->vnum, ch->name, ch->short_descr);
    fprintf(fp, "%s~\n",	ch->long_descr);
    fprintf(fp, "%d %d %d %d\n",	ch->level, ch->max_hit, ch->hit, ch->size);
    fprintf(fp, "%d %d %d %d %d\n",ch->alignment, ch->damage[DICE_NUMBER], ch->damage[DICE_TYPE], ch->damroll, ch->defense_mod);
}
void fread_charmie(CHAR_DATA *ch, FILE *fp)
{
	int vnum, roomvnum;
	CHAR_DATA *charmed;
	AFFECT_DATA af;
	vnum = fread_number(fp);
	if (get_mob_index(vnum) == NULL)
	{
            bug("Fread_charmie: bad vnum %d!!!!",vnum);
            charmed = create_mobile(get_mob_index(24594));
        }
        else
            charmed = create_mobile(get_mob_index(vnum));
	roomvnum = fread_number(fp);
	free_pstring(charmed->short_descr);
	free_pstring(charmed->long_descr);
	free_pstring(charmed->name);
	charmed->name		= fread_string(fp);
	charmed->short_descr= fread_string(fp);
	charmed->long_descr	= fread_string(fp);
	charmed->level		= fread_number(fp);
	charmed->max_hit	= fread_number(fp);
	charmed->hit		= fread_number(fp);
	charmed->size		= fread_number(fp);
	charmed->alignment		= fread_number(fp);
	charmed->damage[DICE_NUMBER]	= fread_number(fp);
	charmed->damage[DICE_TYPE]	= fread_number(fp);
	charmed->damroll		= fread_number(fp);
	charmed->defense_mod	= fread_number(fp);
	char_to_room(charmed,get_room_index(roomvnum));
	add_follower(charmed,ch);
	charmed->leader		= ch;
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.level	= ch->level;
	af.aftype	= AFT_SPELL;
	af.location	= 0;
	af.modifier	= 0;
	af.duration	= -1;
	SET_BIT(af.bitvector, AFF_CHARM);
	af.type		= skill_lookup("charm person");
	if(charmed->pIndexData->vnum==MOB_VNUM_ZOMBIE)
		af.type	= skill_lookup("animate dead");
	affect_to_char(charmed,&af);
}

void fwrite_char( CHAR_DATA *ch, FILE *fp )
{
    AFFECT_DATA *paf;
    int sn, gn, pos;
    int i,j;
    OBJ_DATA *obj;
	OBJ_DATA *belt;
	TROPHY_DATA *placeholder;

    fprintf( fp, "#%s\n", IS_NPC(ch) ? "MOB" : "PLAYER"	);
    fprintf( fp, "Name %s~\n",	ch->true_name		);
    fprintf( fp, "Ghost %d~\n", ch->ghost		);
    fprintf( fp, "Id   %ld\n", ch->id			);
    fprintf( fp, "LogO %ld\n",	current_time		);
    fprintf( fp, "Vers %d\n",   6			);
    if (!ch->pcdata->old && ch->short_descr[0] != '\0')
      	fprintf( fp, "ShD  %s~\n",	ch->short_descr	);
    if (ch->pcdata->old && ch->pcdata->old->short_descr[0]!='\0')
	fprintf(fp,"ShD  %s~\n",	ch->pcdata->old->short_descr);
    if(!ch->pcdata->old && ch->long_descr[0] != '\0')
	fprintf( fp, "LnD  %s~\n",	ch->long_descr	);
    if (ch->pcdata->old && ch->pcdata->old->long_descr[0]!='\0')
        fprintf(fp,"LnD  %s~\n",        ch->pcdata->old->long_descr);
    if(!ch->pcdata->old && ch->description[0] != '\0')
    	fprintf( fp, "Desc %s~\n",	ch->description	);
    if(ch->pcdata->old && ch->pcdata->old->description[0] != '\0')
	fprintf( fp, "Desc %s~\n",	ch->pcdata->old->description);
    if (ch->prompt != NULL || !str_cmp(ch->prompt,"<%hhp %mm %vmv> "))
        fprintf( fp, "Prom %s~\n",      ch->prompt  	);
    fprintf( fp, "Race %s~\n", pc_race_table[ch->race].name );
    if (ch->cabal)
	{
        fprintf( fp, "Cabal %s~\n",cabal_table[ch->cabal].name);
		fprintf( fp, "CabalLevel %d\n",ch->pcdata->cabal_level);
	}
	if (ch->cabal == CABAL_HORDE)
		fprintf( fp, "Tribe %d\n",ch->pcdata->tribe	);
    fprintf( fp, "Sex  %d\n",	ch->sex			);
	fprintf( fp, "Beauty %d\n", ch->pcdata->beauty);
	fprintf( fp, "Cla  %s~\n",	(RSTR)ch->Class()->name);
    fprintf( fp, "Levl %d\n",	ch->level		);
    if (ch->trust != 0)
	fprintf( fp, "Tru  %d\n",	ch->trust	);
    fprintf( fp, "Sec  %d\n",    ch->pcdata->security	);	/* OLC */
    fprintf( fp, "Plyd %d\n",
	ch->played + (int) (current_time - ch->logon)	);
    if (!IS_NPC(ch))
	{
    fprintf( fp, "Agemod %d\n",	ch->pcdata->age_mod);
	if (ch->pcdata->death_status == HAS_DIED)
	{
	fprintf(fp,"Died %d\n",	ch->pcdata->death_status);
	fprintf(fp,"Dtime %d\n",ch->pcdata->death_timer);
	}
	}
    fprintf( fp, "Not  %ld %ld %ld %ld %ld\n",		
	ch->pcdata->last_note,ch->pcdata->last_idea,ch->pcdata->last_penalty,
	ch->pcdata->last_news,ch->pcdata->last_changes	);
    fprintf( fp, "Scro %d\n", 	ch->lines		);

    fprintf( fp, "HomeTown %d\n",ch->hometown);

    fprintf( fp, "Room %d\n",
        (  ch->in_room == get_room_index( ROOM_VNUM_LIMBO )
        && ch->was_in_room != NULL )
            ? ch->was_in_room->vnum
            : ch->in_room == NULL ? ROOM_VNUM_TEMPLE : ch->in_room->vnum );

    fprintf( fp, "HMV  %d %d %d %d %d %d\n",
	ch->hit, ch->max_hit, ch->mana, ch->max_mana, ch->move, ch->max_move );
    if (!IS_NPC(ch))
    {
    if (ch->pcdata->death_count == 0)
	fprintf( fp,"Deaths %d\n", 0);
    else
	fprintf( fp, "Deaths %d\n", ch->pcdata->death_count);
     if (ch->pcdata->sp == 0)
	fprintf( fp, "Sp %d\n",0);
    else
	fprintf( fp, "Sp %d\n",ch->pcdata->sp);
    fprintf( fp, "RollTime %ld\n",	ch->pcdata->roll_time);
    fprintf( fp, "Born %d\n",		ch->pcdata->birth_date);
    fprintf( fp, "DeathTime %d\n",	ch->pcdata->death_time);
	fprintf(fp, "Styles %s\n", print_flags(ch->pcdata->styles));
    fprintf( fp, "Instyle %d\n",	ch->pcdata->style);
    fprintf( fp, "Special %d\n",	ch->pcdata->special);
    fprintf( fp, "Bounty %ld\n",  	ch->pcdata->bounty	);
    fprintf( fp, "BountyKilled %d\n",	ch->pcdata->bounty_killed);
    fprintf( fp, "Paladin_path %d\n",	ch->pcdata->paladin_path);
	if (ch->Class()->GetIndex() == CLASS_SORCERER) {
		fprintf( fp, "EleMaj %d\n",	ch->pcdata->ele_major);
		fprintf( fp, "ElePar %d\n", ch->pcdata->ele_para);
	}
     if(ch->Class()->GetIndex() == CLASS_NECROMANCER)
	fprintf(fp, "Souls %d\n", ch->pcdata->souls);
     if (ch->gold > 0)
      fprintf( fp, "Gold %ld\n",	ch->gold		);
    else
      fprintf( fp, "Gold %d\n", 0			);
    if (ch->gold_bank > 0)
      fprintf( fp, "Bgold %ld\n",	ch->gold_bank		);
    else
      fprintf( fp, "Bgold %d\n", 0			);
    fprintf( fp, "Exp  %d\n",	ch->exp			);
    if (ch->act != 0)
	fprintf( fp, "Act  %s\n",   print_flags(ch->act));
    if (ch->affected_by != 0)
	fprintf( fp, "AfBy %s\n",   print_flags(ch->affected_by));
    fprintf( fp, "Comm %s\n",       print_flags(ch->comm));
    if (ch->wiznet)
    	fprintf( fp, "Wizn %s\n",   print_flags(ch->wiznet));
    if (ch->invis_level)
	fprintf( fp, "Invi %d\n", 	ch->invis_level	);
    if (ch->incog_level)
	fprintf(fp,"Inco %d\n",ch->incog_level);
    fprintf( fp, "Pos  %d\n",	
	ch->position == POS_FIGHTING ? POS_STANDING : ch->position );
    if (ch->practice != 0)
    	fprintf( fp, "Prac %d\n",	ch->practice	);
    if (ch->train != 0)
	fprintf( fp, "Trai %d\n",	ch->train	);
    if (ch->saving_throw != 0)
	fprintf( fp, "Save  %d\n",	ch->saving_throw);
    fprintf( fp, "Alig  %d\n",	ch->alignment		);
    fprintf( fp, "Etho  %d\n",  ch->pcdata->ethos       );
    fprintf( fp, "OAli  %d\n",  ch->pcdata->oalign      );
    fprintf( fp, "OEth  %d\n",  ch->pcdata->oethos      );
    fprintf( fp, "Indu  %d\n",  ch->pcdata->induct      );
    if (ch->hitroll != 0)
	fprintf( fp, "Hit   %d\n",	ch->hitroll	);
    if (ch->damroll != 0)
	fprintf( fp, "Dam   %d\n",	ch->damroll	);
    fprintf( fp, "ACs %d %d %d %d\n", ch->armor[0],ch->armor[1],ch->armor[2],ch->armor[3]);

    fprintf( fp, "BCredits %d\n",	ch->pcdata->bounty_credits);
	fprintf( fp, "TrSet %s\n", print_flags(ch->pcdata->trust));
	for (pos = 0; pos < MAX_QUESTS; pos++)
		if (ch->pcdata->quests[pos] > 0)
			fprintf(fp,"Quest %d %d\n", pos, ch->pcdata->quests[pos]);
	fprintf(fp,"Rep %d\n",  ch->pcdata->reputation);
    for(pos = 0; pos < MAX_EVENTS; pos++)
		if(get_trust(ch) >= color_event[pos].min_level)
    		fprintf(fp, "CScheme %s %s\n",	color_event[pos].event_name, ch->pcdata->color_scheme[pos]);
    fprintf( fp, "Sect ");
    for(pos = 0; pos < SECT_MAX; pos++)
	fprintf(fp,"%ld ",ch->pcdata->sect_time[pos]);
    fprintf( fp, "-1\n");
    fprintf( fp, "Newbie %d~\n",	ch->pcdata->newbie );
	fprintf( fp, "Immune %s\n", print_flags(ch->imm_flags));
	fprintf( fp, "Vuln %s\n", print_flags(ch->vuln_flags));
	fprintf( fp, "Resist %s\n", print_flags(ch->res_flags));
    if(ch->pcdata->start_weapon >= 0)
    	fprintf( fp, "StartWeap %d\n",	ch->pcdata->start_weapon);
    if (ch->wimpy !=0 )
		fprintf( fp, "Wimp  %d\n",	ch->wimpy	);
    fprintf(fp,"DeposItems ");
    for(i=0; i<MAX_STORED_ITEMS; i++)
	fprintf(fp,"%d%c",ch->pcdata->deposited_items[i], i == MAX_STORED_ITEMS - 1 ? '\n' : ' ');
    fprintf( fp, "Attr %d %d %d %d %d\n",
	ch->perm_stat[STAT_STR],
	ch->perm_stat[STAT_INT],
	ch->perm_stat[STAT_WIS],
	ch->perm_stat[STAT_DEX],
	ch->perm_stat[STAT_CON] );
    fprintf (fp, "AMod %d %d %d %d %d\n",
	ch->mod_stat[STAT_STR],
	ch->mod_stat[STAT_INT],
	ch->mod_stat[STAT_WIS],
	ch->mod_stat[STAT_DEX],
	ch->mod_stat[STAT_CON] );
    }
    if ( IS_NPC(ch) )
	fprintf( fp, "Vnum %d\n",	ch->pIndexData->vnum	);
    else
    {
	fprintf( fp, "Pass %s~\n",	ch->pcdata->pwd		);
	if (ch->pcdata->bamfin[0] != '\0')
	    fprintf( fp, "Bin  %s~\n",	ch->pcdata->bamfin);
	if (ch->pcdata->bamfout[0] != '\0')
		fprintf( fp, "Bout %s~\n",	ch->pcdata->bamfout);
	if (ch->pcdata->imm_death)
		fprintf( fp, "Dmsg %s~\n", ch->pcdata->imm_death);
	fprintf( fp, "Titl %s~\n",	ch->pcdata->title	);
	if(ch->pcdata->extitle) fprintf( fp, "EXTitl %s~\n", ch->pcdata->extitle);

	fprintf( fp, "TSex %d\n",	ch->pcdata->true_sex	);
	fprintf( fp, "LLev %d\n",	ch->pcdata->last_level	);
	fprintf( fp, "HMVP %d %d %d\n", ch->pcdata->perm_hit,
						   ch->pcdata->perm_mana,
						   ch->pcdata->perm_move);
	fprintf( fp, "Cnd  %d %d %d %d %d %d\n",
	    ch->pcdata->condition[0],
	    ch->pcdata->condition[1],
	    ch->pcdata->condition[2],
	    ch->pcdata->condition[3],
	    ch->pcdata->condition[4],
	    ch->pcdata->condition[5]);
	/* kills and killed scores */
	fprintf( fp, "kls %d %d %d %d\n",
	        ch->pcdata->kills[PK_KILLS],
		ch->pcdata->kills[PK_GOOD],
		ch->pcdata->kills[PK_NEUTRAL],
		ch->pcdata->kills[PK_EVIL]);

	fprintf( fp, "frg %d %d %d %d\n",
		(int)((float)(ch->pcdata->frags[PK_KILLS] * 1000000)),
		(int)((float)(ch->pcdata->frags[PK_GOOD] * 1000000)),
		(int)((float)(ch->pcdata->frags[PK_NEUTRAL] * 1000000)),
		(int)((float)(ch->pcdata->frags[PK_EVIL]) * 1000000));

	fprintf( fp, "kld %d %d\n",
	        ch->pcdata->killed[PK_KILLED],
		ch->pcdata->killed[MOB_KILLED]);

	fprintf( fp, "frgd %d\n", (int)((float)(ch->pcdata->fragged * 1000000)));

	if (ch->pcdata->recentkills[0] && ch->pcdata->recentkills[0][0] != '\0')  {
		fprintf(fp, "rkls ");
		for (i = 0;i < 100;i++) {
			if (ch->pcdata->recentkills[i] == NULL)
				break;
			fprintf(fp, "%s ", ch->pcdata->recentkills[i]);
		}
		fprintf(fp, "EndRKLS\n");
	}
	
	if (ch->Class()->name == "anti-paladin") {
		fprintf(fp, "Lssr ");
		for (i=0; i < MAX_LESSER; i++)
			fprintf(fp, "%d ", ch->pcdata->lesserdata[i]);
		fprintf(fp, "\n");
		
		fprintf(fp, "Grtr ");
		for (i=0; i< MAX_GREATER; i++)
			fprintf(fp, "%d ", ch->pcdata->greaterdata[i]);
		fprintf(fp, "\n");

		fprintf(fp, "Arch ");
		for (i=0; i< MAX_DEVIL; i++)
			fprintf(fp, "%d ", ch->pcdata->devildata[i]);
		fprintf(fp, "\n");
	}
	
        for (pos = 0; pos < MAX_ALIAS; pos++)
	{
	    if (ch->pcdata->alias[pos] == NULL
	    ||  ch->pcdata->alias_sub[pos] == NULL)
		break;

	    fprintf(fp,"Alias %s %s~\n",ch->pcdata->alias[pos],
		    ch->pcdata->alias_sub[pos]);
	}

	ch->Profs()->WriteProfs(fp);

	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name != NULL && (ch->pcdata->learned[sn] > 0
		|| ch->pcdata->learned[sn] == -2 ))
	    {
		fprintf( fp, "Sk %d '%s'\n",
		    ch->pcdata->learned[sn], skill_table[sn].name );
	    }
	}

	for ( gn = 0; gn < MAX_GROUP; gn++ )
        {
            if ( group_table[gn].name != NULL && ch->pcdata->group_known[gn])
            {
                fprintf( fp, "Gr '%s'\n",group_table[gn].name);
            }
        }
    }

    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	if (paf->type < 0 || paf->type>= MAX_SKILL)
	    continue;
	if (paf->type == gsn_word_of_command || paf->type == gsn_disguise || paf->type == gsn_indomitable_spirit)
		continue;

	paf->aftype = isAftSpell(paf->aftype);
	fprintf( fp, "Affc '%s' %3d %3d %3d %3d %3d %s %3d %s '%s'\n",
	    skill_table[paf->type].name,
	    paf->where,
	    paf->level,
	    paf->duration,
	    paf->modifier,
	    paf->location,
		print_flags(paf->bitvector),
	    paf->aftype,
		(paf->owner) ? (paf->owner->name) : "none",
		(paf->name) ? (paf->name) : "none"
	    );
    }

	if (ch->pcdata->trophy && ch->pcdata->trophy->victname
		&& ch->cabal == CABAL_HORDE
		&& (belt = get_eq_char(ch,WEAR_WAIST)) != NULL
		&& belt->pIndexData->vnum == OBJ_VNUM_TROPHY_BELT
		&& belt->value[4] >= 1)
	{
		fprintf(fp,"Trophies ");
		fprintf(fp,"%d ",belt->value[4]);
		placeholder = ch->pcdata->trophy;
		for (j=1;j<=belt->value[4];j++) {
			if (!ch->pcdata->trophy) 		break;
			fprintf(fp,"%s%s",ch->pcdata->trophy->victname," ");
			if (!ch->pcdata->trophy->next)	break;
			ch->pcdata->trophy = ch->pcdata->trophy->next;
		}
		fprintf(fp,"XYZ\n\r");
		ch->pcdata->trophy = placeholder;
	}
	
	if (ch->pcdata->logon_time)
		fprintf(fp,"LogonTime %s~\n",ch->pcdata->logon_time);

	fprintf(fp,"TimePlayed %d~\n",(int) ((current_time - ch->logon) / 60 ));

	if (ch->pcdata->history_buffer != NULL)
		fprintf(fp,"History %s~\n",ch->pcdata->history_buffer);
        if (ch->pcdata->role != NULL)
                fprintf(fp,"Role %s~\n",ch->pcdata->role);
	fprintf(fp, "TrackAObj %d~\n",count_carried(ch,FALSE));
	fprintf(fp, "TrackLObj %d~\n",count_carried(ch,TRUE));

	fprintf(fp, "FingEQ");
	for (i = 0; i < MAX_WEAR; i++)
	{
		if ((obj = get_eq_char(ch,i)) == NULL)
			fprintf(fp, " 0");
		else
			fprintf(fp, " %d", obj->pIndexData->vnum);
	}
	fprintf(fp, "\n");

    fprintf( fp, "End\n\n" );
    return;
}

/* write a pet */
void fwrite_pet( CHAR_DATA *pet, FILE *fp)
{

    fprintf(fp,"#PET\n");

    fprintf(fp,"Vnum %d\n",pet->pIndexData->vnum);

    fprintf(fp,"Name %s~\n", pet->name);
    fprintf(fp,"LogO %ld\n", current_time);
    if (pet->short_descr != pet->pIndexData->short_descr)
    	fprintf(fp,"ShD  %s~\n", pet->short_descr);
    if (pet->long_descr != pet->pIndexData->long_descr)
    	fprintf(fp,"LnD  %s~\n", pet->long_descr);
    if (pet->description != pet->pIndexData->description)
    	fprintf(fp,"Desc %s~\n", pet->description);
    if (pet->race != pet->pIndexData->race)
    	fprintf(fp,"Race %s~\n", race_data_lookup(pet->race)->name);
    if (pet->cabal)
        fprintf( fp, "Cabal %s~\n",cabal_table[pet->cabal].name);
    fprintf(fp,"Sex  %d\n", pet->sex);
    if (pet->level != pet->pIndexData->level)
    	fprintf(fp,"Levl %d\n", pet->level);
    fprintf(fp, "HMV  %d %d %d %d %d %d\n",
    	pet->hit, pet->max_hit, pet->mana, pet->max_mana, pet->move, pet->max_move);
    if (pet->gold > 0)
    	fprintf(fp,"Gold %ld\n",pet->gold);
    if (pet->exp > 0)
    	fprintf(fp, "Exp  %d\n", pet->exp);
    if (pet->act != pet->pIndexData->act)
    	fprintf(fp, "Act  %s\n", print_flags(pet->act));
    if (pet->affected_by != pet->pIndexData->affected_by)
    	fprintf(fp, "AfBy %s\n", print_flags(pet->affected_by));
    if (pet->comm != 0)
    	fprintf(fp, "Comm %s\n", print_flags(pet->comm));
    fprintf(fp,"Pos  %d\n", pet->position = POS_FIGHTING ? POS_STANDING : pet->position);
    if (pet->saving_throw != 0)
    	fprintf(fp, "Save %d\n", pet->saving_throw);
    if (pet->alignment != pet->pIndexData->alignment)
    	fprintf(fp, "Alig %d\n", pet->alignment);
    if (pet->hitroll != pet->pIndexData->hitroll)
    	fprintf(fp, "Hit  %d\n", pet->hitroll);
    if (pet->damroll != pet->pIndexData->damage[DICE_BONUS])
    	fprintf(fp, "Dam  %d\n", pet->damroll);
    fprintf(fp, "ACs  %d %d %d %d\n",
    	pet->armor[0],pet->armor[1],pet->armor[2],pet->armor[3]);
    fprintf(fp, "Attr %d %d %d %d %d\n",
    	pet->perm_stat[STAT_STR], pet->perm_stat[STAT_INT],
    	pet->perm_stat[STAT_WIS], pet->perm_stat[STAT_DEX],
    	pet->perm_stat[STAT_CON]);
    fprintf(fp, "AMod %d %d %d %d %d\n",
    	pet->mod_stat[STAT_STR], pet->mod_stat[STAT_INT],
    	pet->mod_stat[STAT_WIS], pet->mod_stat[STAT_DEX],
    	pet->mod_stat[STAT_CON]);
/*
    for ( paf = pet->affected; paf != NULL; paf = paf->next )
    {
    	if (paf->type < 0 || paf->type >= MAX_SKILL)
    	    continue;
    	
	paf->aftype = isAftSpell(paf->aftype);

    	fprintf(fp, "Affc '%s' %3d %3d %3d %3d %3d %s %3d %s '%s'\n",
    	    skill_table[paf->type].name,
    	    paf->where,
			paf->level,
			paf->duration,
			paf->modifier,
			paf->location,
			print_flags(paf->bitvector),
			paf->aftype,
			(paf->owner) ? (paf->owner->name) : "none",
			(paf->name) ? (paf->name) : "none");
    }
 THIS IS VARY VARY BAD. */
    fprintf(fp,"End\n");
    return;
}

/*
 * Write an object and its contents.
 */
void fwrite_obj( CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest )
{
    EXTRA_DESCR_DATA *ed = NULL;
    OBJ_AFFECT_DATA *paf;
    OBJ_APPLY_DATA *oad;
    /*
     * Slick recursion to write lists backwards,
     *   so loading them will load in forwards order.
     */
    if ( obj->next_content != NULL )
	fwrite_obj( ch, obj->next_content, fp, iNest );

    /*
     * Castrate storage characters.
     */
    if ( obj->item_type == ITEM_KEY
    ||   (obj->item_type == ITEM_MAP && !obj->value[0]))
	return;
    fprintf( fp, "#O\n" );
    fprintf( fp, "Vnum %d\n",   obj->pIndexData->vnum        );
    if (!obj->pIndexData->new_format)
	fprintf( fp, "Oldstyle\n");
    fprintf( fp, "Nest %d\n",	iNest	  	     );

    /* these data are only used if they do not match the defaults */

    if ( str_cmp(obj->name, obj->pIndexData->name))
    	fprintf( fp, "Name %s~\n",	obj->name		     );
    if ( str_cmp(obj->short_descr, obj->pIndexData->short_descr))
        fprintf( fp, "ShD  %s~\n",	obj->short_descr	     );
    if ( str_cmp(obj->description,obj->pIndexData->description))
        fprintf( fp, "Desc %s~\n",	obj->description	     );
    if (obj->owner != "")
		fprintf( fp, "Owner %s~\n",	obj->owner );

    if ( !vector_equal(obj->extra_flags, obj->pIndexData->extra_flags))
		fprintf(fp, "ExtF %s\n", print_flags(obj->extra_flags));
    if ( !vector_equal(obj->wear_flags, obj->pIndexData->wear_flags))
		fprintf(fp, "WeaF %s\n", print_flags(obj->wear_flags));
    if ( obj->item_type != obj->pIndexData->item_type)
		fprintf( fp, "Ityp %d\n",   obj->item_type           );
    if ( obj->weight != obj->pIndexData->weight)
        fprintf( fp, "Wt   %d\n",	obj->weight		     );
	if ( str_cmp (obj->material, obj->pIndexData->material))
		fprintf( fp, "Mat  %s~\n",	obj->material			 );
	if (obj->wear_loc_name)
		fprintf( fp, "WLName %s~\n", obj->wear_loc_name);
    if ( obj->condition != obj->pIndexData->condition)
		fprintf( fp, "Cond %d\n",	obj->condition		     );

    /* variable data */

    fprintf( fp, "Wear %d\n",   obj->wear_loc                );
    if (obj->level != obj->pIndexData->level)
        fprintf( fp, "Lev  %d\n",	obj->level		     );
    if (obj->timer != 0)
        fprintf( fp, "Time %d\n",	obj->timer	     );
    fprintf( fp, "Cost %d\n",	obj->cost		     );
    if (obj->value[0] != obj->pIndexData->value[0]
    ||  obj->value[1] != obj->pIndexData->value[1]
    ||  obj->value[2] != obj->pIndexData->value[2]
    ||  obj->value[3] != obj->pIndexData->value[3]
    ||  obj->value[4] != obj->pIndexData->value[4])
    	fprintf( fp, "Val  %d %d %d %d %d\n",
	    obj->value[0], obj->value[1], obj->value[2], obj->value[3],
	    obj->value[4]	     );

    switch ( obj->item_type )
    {
    case ITEM_POTION:
    case ITEM_SCROLL:
    case ITEM_PILL:
	if ( obj->value[1] > 0 )
	{
	    fprintf( fp, "Spell 1 '%s'\n",
		skill_table[obj->value[1]].name );
	}

	if ( obj->value[2] > 0 )
	{
	    fprintf( fp, "Spell 2 '%s'\n",
		skill_table[obj->value[2]].name );
	}

	if ( obj->value[3] > 0 )
	{
	    fprintf( fp, "Spell 3 '%s'\n",
		skill_table[obj->value[3]].name );
	}

	break;

    case ITEM_STAFF:
    case ITEM_WAND:
	if ( obj->value[3] > 0 )
	{
	    fprintf( fp, "Spell 3 '%s'\n",
		skill_table[obj->value[3]].name );
	}

	break;
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if (paf->type < 0 || paf->type >= MAX_SKILL)
	    continue;

	paf->aftype = isAftSpell(paf->aftype);

        fprintf( fp, "Affc '%s' %3d %3d %3d %3d %3d %s %d %s\n",
            skill_table[paf->type].name,
            paf->where,
            paf->level,
            paf->duration,
            paf->modifier,
            paf->location,
			print_flags(paf->bitvector),
            paf->aftype,
			(paf->owner) ? (paf->owner->name) : "none"
            );
    }
    OBJ_APPLY_DATA *ioad;

    for(oad = obj->apply; oad; oad = oad->next)
    {
	for(ioad = obj->pIndexData->apply; ioad; ioad = ioad->next)
		if(ioad == oad)
			break;
	if(!ioad)
		fprintf(fp, "AddApp %d %d %d\n", oad->location, oad->modifier, oad->type);
    }
	if ((ed = obj->extra_descr) != NULL)
		if ((!obj->pIndexData->extra_descr)
			|| (str_cmp(ed->keyword,obj->pIndexData->extra_descr->keyword)))
				fprintf( fp, "ExDe %s~ %s~\n", ed->keyword, ed->description );

    fprintf( fp, "End\n\n" );

    if ( obj->contains != NULL )
	fwrite_obj( ch, obj->contains, fp, iNest + 1 );

    return;
}



/*
 * Load a char and inventory into a new ch structure.
 */
bool load_char_obj( DESCRIPTOR_DATA *d, char *name )
{
    char strsave[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *ch;
    FILE *fp;
    bool found;
    int stat;
    int pos;
	int i;

	CHAR_DATA *charg = new CHAR_DATA;

    ch = new_char();
    ch->pcdata = new_pcdata();

	d->character			= ch;
    ch->desc				= d;
    ch->pcdata->entering_text		= FALSE;
    ch->name				= palloc_string( name );
    ch->id				= get_pc_id();
    ch->race				= race_lookup("human");
    ch->level				= 0;
    SET_BIT(ch->act, PLR_NOSUMMON);
    SET_BIT(ch->comm, COMM_COMBINE);
    SET_BIT(ch->comm, COMM_PROMPT);
    zero_vector(ch->pcdata->trust);
    ch->prompt 				= palloc_string("<%hhp %mm %vmv %L> ");
	for (i = 0; i < 20; i++)
		ch->pcdata->queue[i][0] = '\0';
	ch->pcdata->read_next = 0;
	ch->pcdata->write_next = 0;
	ch->pcdata->pending = 0;
    ch->Profs()->SetChar(ch);
    ch->Profs()->SetPATime(0);
    ch->pcdata->confirm_delete		= FALSE;
    ch->pcdata->pwd			= NULL;
    ch->pcdata->bamfin			= palloc_string( "" );
    ch->pcdata->bamfout			= palloc_string( "" );
    ch->pcdata->title			= palloc_string( "" );
    ch->pcdata->roll_time		= current_time;
    ch->pcdata->birth_date		= 1516; //rs starts in 1515
    ch->pcdata->death_time		= 500;
    ch->pcdata->style			= 0;
    zero_vector(ch->pcdata->styles);
    for (stat =0; stat < MAX_STATS; stat++)
	ch->perm_stat[stat]		= 13;
    for (stat = 0; stat < SECT_MAX; stat++)
	ch->pcdata->sect_time[stat] = 0;
    for (stat = 0; stat < MAX_STORED_ITEMS; stat++)
	ch->pcdata->deposited_items[stat] = 0;
    ch->pcdata->condition[COND_THIRST]	= 0;
    ch->pcdata->condition[COND_FULL]	= 0;
    ch->pcdata->condition[COND_HUNGER]	= 0;
    ch->pcdata->condition[COND_STARVING] = 0;
    ch->pcdata->condition[COND_DEHYDRATED] = 0;
	ch->pcdata->security      = 0;    /* OLC */
	ch->pcdata->bounty	    = 0;
    ch->pcdata->bounty_credits = 0;
	ch->bounty_timer		=	0;
    ch->pcdata->end_fun		= NULL;
    ch->pcdata->energy_state	= 0;
    ch->dam_mod = 100;
    ch->pcdata->shifted	    = -1;
    ch->defense_mod = 0;
    ch->pcdata->tribe	= 0;
    ch->pcdata->save_timer = 0;
	ch->disrupted = FALSE;
	ch->stolen_from = FALSE;
	ch->pcdata->souls = 0;
	ch->position = 0;
	ch->pcdata->cabal_level = 0;
	ch->pcdata->bounty_killed = 0;
	ch->pcdata->paladin_path = 0;
	ch->pcdata->editing_item = 0;
	for(pos = 0; pos < 100;pos++)
    	ch->pcdata->recentkills[pos] = NULL;
	for(pos = 0; pos < MAX_EVENTS; pos++)
		ch->pcdata->color_scheme[pos] = color_event[pos].default_color;
	for(pos = 0; pos < MAX_QUESTS; pos++)
		ch->pcdata->quests[pos] = 0;
    found = FALSE;
    fclose( fpReserve );

    /* decompress if .gz file exists */
    sprintf( strsave, "%s%s%s", PLAYER_DIR, capitalize(name),".gz");
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {
	fclose(fp);
	sprintf(buf,"gzip -dfq %s",strsave);
	system(buf);
    }

    sprintf( strsave, "%s%s.plr", PLAYER_DIR, capitalize( name ) );
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {
	int iNest;

	for ( iNest = 0; iNest < MAX_NEST; iNest++ )
	    rgObjNest[iNest] = NULL;

	found = TRUE;
	for ( ; ; )
	{
	    char letter;
	    char *word;

	    letter = fread_letter( fp );
	    if ( letter == '*' )
	    {
		fread_to_eol( fp );
		continue;
	    }

	    if ( letter != '#' )
	    {
		bug( "Load_char_obj: # not found.", 0 );
		break;
	    }

	    word = fread_word( fp );
	    if      ( !str_cmp( word, "PLAYER" ) ) fread_char ( ch, fp );
	    else if ( !str_cmp( word, "OBJECT" ) ) fread_obj  ( ch, fp );
	    else if ( !str_cmp( word, "O"      ) ) fread_obj  ( ch, fp );
	    else if ( !str_cmp( word, "CHARMED") ) fread_charmie ( ch, fp );
	    else if ( !str_cmp( word, "PET"    ) ) fread_pet  ( ch, fp );
	    else if ( !str_cmp( word, "END"    ) ) break;
	    else
	    {
		bug( "Load_char_obj: bad section.", 0 );
		break;
	    }
	}
	fclose( fp );
    }


	free_pstring(ch->true_name);
	ch->true_name = palloc_string(name);
	free_pstring(ch->backup_true_name);
	ch->backup_true_name = palloc_string(name);
    fpReserve = fopen( NULL_FILE, "r" );

	/* Morg - Valgrind fix */
	zero_vector(ch->imm_flags);
	zero_vector(ch->res_flags);
	zero_vector(ch->vuln_flags);

    /* initialize race */
    if (found)
    {
		int i;

		if (ch->race == 0)
	    	ch->race = race_lookup("human");

		ch->size = pc_race_table[ch->race].size;
		ch->dam_type = 17; /*punch */

		for (i = 0; i < 5; i++)
		{
	    	if (pc_race_table[ch->race].skills[i] == NULL)
				break;
	    	group_add(ch,pc_race_table[ch->race].skills[i],FALSE);
		}
	
		BITWISE_OR(ch->affected_by, race_data_lookup(ch->race)->aff);
		BITWISE_OR(ch->imm_flags, race_data_lookup(ch->race)->imm);
		BITWISE_OR(ch->res_flags, race_data_lookup(ch->race)->res);
		BITWISE_OR(ch->vuln_flags, race_data_lookup(ch->race)->vuln);
		copy_vector(ch->form, race_data_lookup(ch->race)->form);
		copy_vector(ch->parts, race_data_lookup(ch->race)->parts);
    }

	BITWISE_OR(ch->imm_flags, limmune_flags);
	BITWISE_OR(ch->res_flags, lres_flags);
	BITWISE_OR(ch->vuln_flags, lvuln_flags);

	/* RT initialize skills */

    if (found && ch->version < 2)  /* need to add the new skills */
    {
		group_add(ch,ch->Class()->base_group,FALSE);
		group_add(ch,ch->Class()->default_group,TRUE);
		ch->pcdata->learned[gsn_recall] = 50;
    }
    return found;
}



/*
 * Read in a char.
 */

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = TRUE;			\
				    break;				\
				}

/* provided to free strings */
#if defined(KEYS)
#undef KEYS
#endif

#define KEYS( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    free_pstring(field);			\
				    field  = value;			\
				    fMatch = TRUE;			\
				    break;				\
				}

#if defined(KEYV)
#undef KEYV
#endif

#define KEYV( literal, field )						\
				if ( !str_cmp( word, literal ) )	\
				{									\
					fread_flag_new(field, fp);		\
					fMatch = TRUE;					\
					break;							\
				}

void fread_char( CHAR_DATA *ch, FILE *fp )
{
    char buf[MAX_STRING_LENGTH];
    char *word;
    bool fMatch;
    int count = 0;
    int lastlogoff = current_time;
    int percent;
	int i, scalps;
	TROPHY_DATA *placeholder;
    sprintf(buf,"Loading %s.",ch->name);
    log_string(buf);

	zero_vector(limmune_flags);
	zero_vector(lres_flags);
	zero_vector(lvuln_flags);

    ch->pcdata->newbie = FALSE;

    for ( ; ; )
    {
	word   = feof( fp ) ? (char*)"End" : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;
	case 'A':
	    KEYV( "Act",		ch->act);
	    KEYV( "AffectedBy",	ch->affected_by);
	    KEY( "Agemod",	ch->pcdata->age_mod,	fread_number( fp ));
	    KEYV( "AfBy",	ch->affected_by);
	    KEY( "Alignment",	ch->alignment,		fread_number( fp ) );
	    KEY( "Alig",	ch->alignment,		fread_number( fp ) );

	    if (!str_cmp( word, "Alia"))
	    {
		if (count >= MAX_ALIAS)
		{
		    fread_to_eol(fp);
		    fMatch = TRUE;
		    break;
		}

		ch->pcdata->alias[count] 	= fread_word(fp);
		ch->pcdata->alias_sub[count]	= fread_string(fp);
		count++;
		fMatch = TRUE;
		break;
	    }


		if (!str_cmp( word, "Alias"))
		{
			if (count >= MAX_ALIAS)
			{
				fread_to_eol(fp);
				fMatch = TRUE;
				break;
			}

			ch->pcdata->alias[count]        = fread_word(fp);
			ch->pcdata->alias_sub[count]    = fread_string(fp);
			count++;
			fMatch = TRUE;
			break;
		}

	    if (!str_cmp( word, "AC") || !str_cmp(word,"Armor"))
	    {
			fread_to_eol(fp);
			fMatch = TRUE;
			break;
	    }

	    if (!str_cmp(word,"ACs"))
	    {
		int i;

		for (i = 0; i < 4; i++)
		    ch->armor[i] = fread_number(fp);
		fMatch = TRUE;
		break;
	    }

            if (!str_cmp(word, "Affc"))
            {
                AFFECT_DATA *paf;
		CHAR_DATA *wch;
		char *owner;
		char *afname;
                int sn;

                paf = new_affect();
		init_affect(paf);
                sn = skill_lookup(fread_word(fp));
                if (sn < 0)
                    bug("Fread_char: unknown skill.",0);
                else
                    paf->type = sn;

                paf->where  	= fread_number(fp);
                paf->level      = fread_number( fp );
                paf->duration   = fread_number( fp );
                paf->modifier   = fread_number( fp );
                paf->location   = fread_number( fp );
		fread_flag_new(paf->bitvector, fp);
                paf->aftype  	= fread_number( fp );

		owner = fread_word( fp );
		for (wch = char_list; wch; wch = wch->next) {
			if (!str_cmp(wch->name,owner)) {
				paf->owner = wch;
					break;
			}
		}

		if (!str_cmp(ch->name,owner))
			paf->owner = ch;
		
		afname = fread_word(fp);

		if (str_cmp(afname,"none"))
			paf->name = palloc_string(afname);

                paf->next       = ch->affected;
                ch->affected    = paf;
                fMatch = TRUE;
                break;
            }

		if (!str_cmp(word, "Arch")) {
			for (i = 0; i < MAX_DEVIL; i++)
				ch->pcdata->devildata[i] = fread_number( fp);
			fMatch = TRUE;
			break;
		}
			
	    if ( !str_cmp( word, "AttrMod"  ) || !str_cmp(word,"AMod"))
	    {
		int stat;
		for (stat = 0; stat < MAX_STATS; stat ++)
		   ch->mod_stat[stat] = fread_number(fp);
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "AttrPerm" ) || !str_cmp(word,"Attr"))
	    {
		int stat;

		for (stat = 0; stat < MAX_STATS; stat++)
		    ch->perm_stat[stat] = fread_number(fp);
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'B':
	    KEY( "Bgold",	ch->gold_bank,		fread_number( fp ) );
	    KEY( "Bamfin",	ch->pcdata->bamfin,	fread_string( fp ) );
	    KEY( "Bamfout",	ch->pcdata->bamfout,	fread_string( fp ) );
	    KEY( "Bin",		ch->pcdata->bamfin,	fread_string( fp ) );
	    KEY( "Bout",	ch->pcdata->bamfout,	fread_string( fp ) );
	    KEY( "Bounty",	ch->pcdata->bounty,	fread_number( fp ) );
	    KEY( "BCredits",	ch->pcdata->bounty_credits, fread_number( fp ));
	    KEY( "Born",	ch->pcdata->birth_date,	fread_number( fp ) );
		KEY( "Beauty",	ch->pcdata->beauty, fread_number( fp ) );
		KEY( "BountyKilled", ch->pcdata->bounty_killed, fread_number( fp));
		break;

	case 'C':
		if(!str_cmp(word, "Cla"))
		{
			ch->SetClass(CClass::Lookup(fread_string(fp)));
			fMatch = TRUE;
			break;
		}
        KEY( "Cabal",        ch->cabal, cabal_lookup(fread_string(fp)));
		KEY( "CabalLevel",	ch->pcdata->cabal_level,	fread_number( fp ) );
		if ( !str_cmp( word, "Condition" ) || !str_cmp(word,"Cond"))
	    {
		ch->pcdata->condition[0] = fread_number( fp );
		ch->pcdata->condition[1] = fread_number( fp );
		ch->pcdata->condition[2] = fread_number( fp );
		ch->pcdata->condition[3] = fread_number( fp );
		if (ch->version > 5)
		{
		ch->pcdata->condition[4] = fread_number( fp );
		ch->pcdata->condition[5] = fread_number( fp );
		}
		fMatch = TRUE;
		break;
	    }
            if (!str_cmp(word,"Cnd"))
            {
		ch->pcdata->condition[0] = fread_number( fp );
		ch->pcdata->condition[1] = fread_number( fp );
		ch->pcdata->condition[2] = fread_number( fp );
		ch->pcdata->condition[3] = fread_number( fp );
		if (ch->version > 5)
		{
		ch->pcdata->condition[4] = fread_number( fp );
		ch->pcdata->condition[5] = fread_number( fp );
		}
		else
		{
		ch->pcdata->condition[4] = 0;
		ch->pcdata->condition[5] = 0;
			}
                fMatch = TRUE;
                break;
            }
	    // Zzz
	    if (!str_cmp(word,"CScheme"))
			ch->pcdata->color_scheme[get_event_number(fread_word(fp))] = get_color_name(fread_word(fp));
	    KEYV("Comm",		ch->comm);

	    break;

	case 'D':
   	KEY( "Deaths",	ch->pcdata->death_count,	fread_number( fp ) );
  	KEY( "Died",	ch->pcdata->death_status,	fread_number( fp ) );
	KEY( "Dtime",	ch->pcdata->death_timer,	fread_number( fp ) );
	KEY( "Damroll",	ch->damroll,		fread_number( fp ) );
	KEY( "Dam",		ch->damroll,		fread_number( fp ) );
    KEY( "Description",	ch->description,	fread_string( fp ) );
	KEY( "Desc",	ch->description,	fread_string( fp ) );
	KEY( "DeathTime", ch->pcdata->death_time,	fread_number( fp ) );
	KEY( "Dmsg",	ch->pcdata->imm_death,		fread_string( fp ) );

	if(!str_cmp(word,"DeposItems"))
		for(i = 0; i<MAX_STORED_ITEMS; i++)
			ch->pcdata->deposited_items[i] = fread_number(fp);

	break;

	case 'E':
		KEY ( "EleMaj",	ch->pcdata->ele_major,	fread_number( fp ) );
		KEY ( "ElePar",	ch->pcdata->ele_para,	fread_number( fp ) );
	    if ( !str_cmp( word, "EXTitle" )  || !str_cmp( word, "EXTitl"))
            {
                ch->pcdata->extitle = fread_string( fp );
		/* Kargoth */
                if (ch->pcdata->extitle[0] != '.' && ch->pcdata->extitle[0] != ','
                &&  ch->pcdata->extitle[0] != '!' && ch->pcdata->extitle[0] != '?')
                {
                    sprintf( buf, " %s", ch->pcdata->extitle );
                    free_pstring( ch->pcdata->extitle );
                    ch->pcdata->extitle = palloc_string( buf );
                }
				
				if(ch->pcdata->extitle[0]=='\0')
					free_pstring(ch->pcdata->extitle);
              	fMatch = TRUE;
                break;
            }

	    if ( !str_cmp( word, "End" ) )
	    {
    		/* adjust hp mana move up  -- here for speed's sake */
    		percent = (current_time - lastlogoff) * 25 / ( 2 * 60 * 60);

			percent = UMIN(percent,100);

    		if (percent > 0 && !IS_AFFECTED(ch,AFF_POISON)
    		&&  !IS_AFFECTED(ch,AFF_PLAGUE))
    		{
        	    ch->hit	+= (ch->max_hit - ch->hit) * percent / 100;
        	    ch->mana    += (ch->max_mana - ch->mana) * percent / 100;
        	    ch->move    += (ch->max_move - ch->move)* percent / 100;
    		}
		return;
            }
            KEY( "Etho",        ch->pcdata->ethos,      fread_number( fp ) );
            KEY( "Exp",         ch->exp,                fread_number( fp ) );
		break;
	case 'F':
	    if(!str_cmp(word,"Form")) {
			fread_string(fp);
			fread_number(fp);
		}
			/*if((form = form_lookup(fread_string(fp)))>-1)
				ch->pcdata->forms[form] = fread_number(fp);*/
		if(!str_cmp(word,"frg")) {
			ch->pcdata->frags[PK_KILLS] = (float)((float)fread_number(fp) / (float)1000000);
			ch->pcdata->frags[PK_GOOD] = (float)((float)fread_number(fp) / (float)1000000);
			ch->pcdata->frags[PK_NEUTRAL] = (float)((float)fread_number(fp) / (float)1000000);
			ch->pcdata->frags[PK_EVIL] = (float)((float)fread_number(fp) / (float)1000000);
		}
		if (!str_cmp(word,"frgd"))
			ch->pcdata->fragged = (float)(fread_number(fp) / 1000000);
	case 'G':
		KEY( "Gold",	ch->gold,		fread_number( fp ) );
		KEY( "Ghost",	ch->ghost,		fread_number( fp ) );
		if ( !str_cmp( word, "Group" )  || !str_cmp(word,"Gr")) {
			int gn;
			char *temp;

			temp = fread_word( fp ) ;
			gn = group_lookup(temp);

			if ( gn < 0 ) {
				fprintf(stderr,"%s",temp);
				bug( "Fread_char: unknown group. ", 0 );
			} else {
				gn_add(ch,gn);
			}
				
			fMatch = TRUE;
			break;
		}

		if (!str_cmp(word,"Grtr")) {
			for (i = 0; i < MAX_GREATER; i++)
				ch->pcdata->greaterdata[i] = fread_number( fp );
			fMatch = TRUE;
			break;
		}
		
	break;

	case 'H':
            KEY( "History",	ch->pcdata->history_buffer,	fread_string( fp ) );
	    KEY( "Hitroll",	ch->hitroll,		fread_number( fp ) );
	    KEY( "Hit",		ch->hitroll,		fread_number( fp ) );
	    KEY( "HomeTown",	ch->hometown,		fread_number( fp ) );

		ch->pcdata->temp_history_buffer = NULL;
	    if ( !str_cmp( word, "HpManaMove" ) || !str_cmp(word,"HMV"))
	    {
		ch->hit		= fread_number( fp );
		ch->max_hit	= fread_number( fp );
		ch->mana	= fread_number( fp );
		ch->max_mana	= fread_number( fp );
		ch->move	= fread_number( fp );
		ch->max_move	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }

            if ( !str_cmp( word, "HpManaMovePerm" ) || !str_cmp(word,"HMVP"))
            {
                ch->pcdata->perm_hit	= fread_number( fp );
                ch->pcdata->perm_mana   = fread_number( fp );
                ch->pcdata->perm_move   = fread_number( fp );
                fMatch = TRUE;
                break;
            }

	    break;

	case 'I':
	    KEY( "Id",		ch->id,			fread_number( fp ) );
            KEY( "Indu",        ch->pcdata->induct,     fread_number( fp ) );
	    KEY( "InvisLevel",	ch->invis_level,	fread_number( fp ) );
	    KEY( "Inco",	ch->incog_level,	fread_number( fp ) );
	    KEY( "Invi",	ch->invis_level,	fread_number( fp ) );
	    KEYV( "Immune",	limmune_flags);
	    KEY( "Instyle",	ch->pcdata->style,	fread_number( fp ) );
	    break;

	case 'K':
	    if (!str_cmp(word,"kls"))
	    {
		ch->pcdata->kills[PK_KILLS] = fread_number( fp );
		ch->pcdata->kills[PK_GOOD] = fread_number( fp );
		ch->pcdata->kills[PK_NEUTRAL] = fread_number( fp );
		ch->pcdata->kills[PK_EVIL] = fread_number( fp );
	     }
	    if (!str_cmp(word,"kld"))
 	    {
		ch->pcdata->killed[PK_KILLED] = fread_number( fp );
		ch->pcdata->killed[MOB_KILLED] = fread_number( fp );
	    }		
	    break;

	case 'L':
	    KEY( "LastLevel",	ch->pcdata->last_level, fread_number( fp ) );
	    KEY( "LLev",	ch->pcdata->last_level, fread_number( fp ) );
	    KEY( "Level",	ch->level,		fread_number( fp ) );
	    KEY( "Lev",		ch->level,		fread_number( fp ) );
	    KEY( "Levl",	ch->level,		fread_number( fp ) );
	    KEY( "LogO",	lastlogoff,		fread_number( fp ) );
	    KEY( "LongDescr",	ch->long_descr,		fread_string( fp ) );
	    KEY( "LnD",		ch->long_descr,		fread_string( fp ) );
		if (!str_cmp(word,"Lssr")) {
			for (i = 0; i < MAX_LESSER; i++)
				ch->pcdata->lesserdata[i] = fread_number( fp );
			fMatch = TRUE;
			break;
		}
	    break;

	case 'N':
	    KEY( "Name",	ch->name,		fread_string( fp ) );
	    KEY( "Note",	ch->pcdata->last_note,	fread_number( fp ) );
	    if (!str_cmp(word,"Not"))
	    {
		ch->pcdata->last_note			= fread_number(fp);
		ch->pcdata->last_idea			= fread_number(fp);
		ch->pcdata->last_penalty		= fread_number(fp);
		ch->pcdata->last_news			= fread_number(fp);
		ch->pcdata->last_changes		= fread_number(fp);
		fMatch = TRUE;
		break;
	    }
	    KEY( "Newbie",	ch->pcdata->newbie,	fread_number( fp ) );
	    break;
	case 'O':
            KEY( "OAli",        ch->pcdata->oalign,     fread_number( fp ) );
            KEY( "OEth",        ch->pcdata->oethos,     fread_number( fp ) );
	    break;

	case 'P':
		KEY( "Paladin_path",	ch->pcdata->paladin_path,	fread_number( fp ));
	    KEY( "Password",	ch->pcdata->pwd,	fread_string( fp ) );
	    KEY( "Pass",	ch->pcdata->pwd,	fread_string( fp ) );
	    KEY( "Played",	ch->played,		fread_number( fp ) );
	    KEY( "Plyd",	ch->played,		fread_number( fp ) );
	    KEY( "Position",	ch->position,		fread_number( fp ) );
	    KEY( "Pos",		ch->position,		fread_number( fp ) );
	    KEY( "Practice",	ch->practice,		fread_number( fp ) );
	    KEY( "Prac",	ch->practice,		fread_number( fp ) );
            KEY( "Prompt",      ch->prompt,             fread_string( fp ) );
 	    KEY( "Prom",	ch->prompt,		fread_string( fp ) );
	    if(!str_cmp(word,"Prof"))
		ch->Profs()->SetProf(CProficiencies::ProfIndexLookup(fread_string(fp)), fread_number(fp));
	    if(!str_cmp(word,"ProfPoints"))
	    {
		ch->Profs()->SetPoints(fread_number(fp));
		ch->Profs()->SetPATime(fread_number(fp));
	    }
	    break;
	case 'Q':
	    if(!str_cmp(word,"Quest"))
			ch->pcdata->quests[fread_number(fp)] = fread_number(fp);
	    break;
	case 'R':
	    KEY( "Race",        ch->race,	
				race_lookup(fread_string( fp )) );
		KEY( "Rep", 		ch->pcdata->reputation,	fread_number(fp));
	    KEYV( "Resist",	lres_flags);
        KEY( "Role",     	ch->pcdata->role,     	fread_string( fp ) );
	    KEY( "RollTime",	ch->pcdata->roll_time,	fread_number(fp));
	    if ( !str_cmp( word, "Room" ) )
	    {
		ch->in_room = get_room_index( fread_number( fp ) );
		if ( ch->in_room == NULL )
		    ch->in_room = get_room_index( ROOM_VNUM_LIMBO );
		fMatch = TRUE;
		break;
	    }

		if (!str_cmp(word,"rkls")) {
			i = 0;
			word = fread_word(fp);
			for (i=0;i<100;i++) {
				word = fread_word(fp);
				if (!str_cmp(word,"EndRKLS"))
					break;
				ch->pcdata->recentkills[i] = palloc_string(word);
			}
			break;
		}

	    break;

	case 'S':
	    KEY( "SavingThrow",	ch->saving_throw,	fread_number( fp ) );
	    KEY( "Save",	ch->saving_throw,	fread_number( fp ) );
	    KEY( "Scro",	ch->lines,		fread_number( fp ) );
	    KEY( "Sex",		ch->sex,		fread_number( fp ) );
	    KEY( "ShortDescr",	ch->short_descr,	fread_string( fp ) );
	    KEY( "ShD",		ch->short_descr,	fread_string( fp ) );
		KEY( "Sec",         ch->pcdata->security,   fread_number( fp ) );   /* OLC */
		KEY( "Sp",		ch->pcdata->sp,		fread_number( fp ) );
	    KEYV( "Styles",	ch->pcdata->styles);
	    KEY( "Special",	ch->pcdata->special,	fread_number( fp ) );
		KEY( "Souls",	ch->pcdata->souls, 	fread_number( fp ) );
	    KEY( "StartWeap",	ch->pcdata->start_weapon,fread_number(fp ) );
	 if ( !str_cmp( word, "Skill" ) || !str_cmp(word,"Sk"))
	    {
			int sn;
			int value;
			char *temp;

			value = fread_number( fp );
			temp = fread_word( fp ) ;
			sn = skill_lookup(temp);
			/* sn    = skill_lookup( fread_word( fp ) ); */
			if ( sn < 0 )
			{
		    	fprintf(stderr,"%s",temp);
		    	bug( "Fread_char: unknown skill. ", 0 );
			}
			else
		    	ch->pcdata->learned[sn] = value;
			fMatch = TRUE;
	    }
	    if(!str_cmp(word,"Sect"))
	    {
			int i = 0, x = 0;
			while((i=fread_number(fp))!=-1)
			{
				ch->pcdata->sect_time[x] = i;
				x++;
			}
	    }
	    break;

	case 'T':
            KEY( "TrueSex",     ch->pcdata->true_sex,  	fread_number( fp ) );
	    KEY( "TSex",	ch->pcdata->true_sex,   fread_number( fp ) );
	    KEY( "Trai",	ch->train,		fread_number( fp ) );
	    KEY( "Trust",	ch->trust,		fread_number( fp ) );
	    KEYV( "TrSet",	ch->pcdata->trust);
	    KEY( "Tru",		ch->trust,		fread_number( fp ) );
	    KEY( "Tribe",	ch->pcdata->tribe,	fread_number( fp ) );
	    if ( !str_cmp( word, "Title" )  || !str_cmp( word, "Titl"))
	    {
		ch->pcdata->title = fread_string( fp );
    		if (ch->pcdata->title[0] != '.' && ch->pcdata->title[0] != ','
		&&  ch->pcdata->title[0] != '!' && ch->pcdata->title[0] != '?')
		{
		    sprintf( buf, " %s", ch->pcdata->title );
		    free_pstring( ch->pcdata->title );
		    ch->pcdata->title = palloc_string( buf );
		}
		fMatch = TRUE;
		break;
	    }

		if (!str_cmp(word, "Trophies")) {
			scalps = fread_number(fp);
			ch->pcdata->trophy = new_trophy_data (fread_word(fp));
			placeholder = ch->pcdata->trophy;
			for (i=2;i<=scalps;i++) {
				ch->pcdata->trophy = ch->pcdata->trophy->next = new_trophy_data (fread_word(fp));
				if(!str_cmp(ch->pcdata->trophy->victname,"XYZ")) {	
					free_pstring(ch->pcdata->trophy->victname);
					break;
				}
			}	
			ch->pcdata->trophy = placeholder;
			fMatch = TRUE;
			break;
		}

	    break;

	case 'V':
	    KEY( "Version",     ch->version,		fread_number ( fp ) );
	    KEY( "Vers",	ch->version,		fread_number ( fp ) );
	    KEYV( "Vuln",	lvuln_flags);
	    if ( !str_cmp( word, "Vnum" ) )
	    {
		ch->pIndexData = get_mob_index( fread_number( fp ) );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    KEY( "Wimpy",	ch->wimpy,		fread_number( fp ) );
	    KEY( "Wimp",	ch->wimpy,		fread_number( fp ) );
	    KEYV( "Wizn",	ch->wiznet);
	    break;
	}

	if ( !fMatch )
	{
//	    bug( "Fread_char: no match.", 0 );
	    fread_to_eol( fp );
	}
    }
}

/* load a pet from the forgotten reaches */
void fread_pet( CHAR_DATA *ch, FILE *fp )
{
    char *word;
    CHAR_DATA *pet;
    bool fMatch;
    int lastlogoff = current_time;
    int percent;

    /* first entry had BETTER be the vnum or we barf */
    word = feof(fp) ? (char*)"END" : fread_word(fp);
    if (!str_cmp(word,"Vnum"))
    {
    	int vnum;
    	
    	vnum = fread_number(fp);
    	if (get_mob_index(vnum) == NULL)
	{
    	    bug("Fread_pet: bad vnum %d.",vnum);
	    pet = create_mobile(get_mob_index(MOB_VNUM_FIDO));
	}
    	else
    	    pet = create_mobile(get_mob_index(vnum));
    }
    else
    {
        bug("Fread_pet: no vnum in file.",0);
        pet = create_mobile(get_mob_index(MOB_VNUM_FIDO));
    }

    for ( ; ; )
    {
    	word 	= feof(fp) ? (char*)"END" : fread_word(fp);
    	fMatch = FALSE;
    	
    	switch (UPPER(word[0]))
    	{
    	case '*':
    	    fMatch = TRUE;
    	    fread_to_eol(fp);
    	    break;
    		
    	case 'A':
    	    KEYV( "Act",		pet->act);
    	    KEYV( "AfBy",	pet->affected_by);
    	    KEY( "Alig",	pet->alignment,		fread_number(fp));
    	
    	    if (!str_cmp(word,"ACs"))
    	    {
    	    	int i;
    	    	
    	    	for (i = 0; i < 4; i++)
    	    	    pet->armor[i] = fread_number(fp);
    	    	fMatch = TRUE;
    	    	break;
    	    }
    	
            if (!str_cmp(word,"Affc"))
            {
		CHAR_DATA *wch = NULL;
		char *owner;
		char *afname;
                AFFECT_DATA *paf;
                int sn;

                paf = new_affect();
 		init_affect(paf);
                sn = skill_lookup(fread_word(fp));
                if (sn < 0)
                    bug("Fread_char: unknown skill.",0);
                else
                   paf->type = sn;

		paf->where	= fread_number(fp);
                paf->level      = fread_number(fp);
                paf->duration   = fread_number(fp);
                paf->modifier   = fread_number(fp);
                paf->location   = fread_number(fp);
		fread_flag_new(paf->bitvector, fp);
                paf->aftype  	= fread_number(fp);
		owner = fread_word( fp );
		if(strcmp(owner, "none")) //safe default
			paf->owner = ch;
		for (wch = char_list; wch; wch = wch->next)
		{
			if (!str_cmp(wch->name,owner)) {
				paf->owner = wch;
				break;
			}
		}

		afname = fread_word(fp);
		if (str_cmp(afname,"none")) paf->name = palloc_string(afname);
		paf->next       = pet->affected;
                pet->affected   = paf;
                fMatch          = TRUE;
                break;
            }
    	
    	    if (!str_cmp(word,"AMod"))
    	    {
    	     	int stat;
    	     	
    	     	for (stat = 0; stat < MAX_STATS; stat++)
    	     	    pet->mod_stat[stat] = fread_number(fp);
    	     	fMatch = TRUE;
    	     	break;
    	    }
    	
    	    if (!str_cmp(word,"Attr"))
    	    {
    	         int stat;
    	
    	         for (stat = 0; stat < MAX_STATS; stat++)
    	             pet->perm_stat[stat] = fread_number(fp);
    	         fMatch = TRUE;
    	         break;
    	    }
    	    break;
    	
    	 case 'C':
    	     KEYV( "Comm",	pet->comm);
             KEY( "Cabal",       pet->cabal, cabal_lookup(fread_string(fp)));
    	     break;
    	
    	 case 'D':
    	     KEY( "Dam",	pet->damroll,		fread_number(fp));
    	     KEY( "Desc",	pet->description,	fread_string(fp));
    	     break;
    	
    	 case 'E':
    	     if (!str_cmp(word,"End"))
	     {
		pet->leader = ch;
		pet->master = ch;
		ch->pet = pet;
    		/* adjust hp mana move up  -- here for speed's sake */
    		percent = (current_time - lastlogoff) * 25 / ( 2 * 60 * 60);

    		if (percent > 0 && !IS_AFFECTED(ch,AFF_POISON)
    		&&  !IS_AFFECTED(ch,AFF_PLAGUE))
    		{
		    percent = UMIN(percent,100);
    		    pet->hit	+= (pet->max_hit - pet->hit) * percent / 100;
        	    pet->mana   += (pet->max_mana - pet->mana) * percent / 100;
        	    pet->move   += (pet->max_move - pet->move)* percent / 100;
    		}
    	     	return;
	     }
    	     KEY( "Exp",	pet->exp,		fread_number(fp));
    	     break;
    	
    	 case 'G':
    	     KEY( "Gold",	pet->gold,		fread_number(fp));
    	     break;
    	
    	 case 'H':
    	     KEY( "Hit",	pet->hitroll,		fread_number(fp));
    	
    	     if (!str_cmp(word,"HMV"))
    	     {
    	     	pet->hit	= fread_number(fp);
    	     	pet->max_hit	= fread_number(fp);
    	     	pet->mana	= fread_number(fp);
    	     	pet->max_mana	= fread_number(fp);
    	     	pet->move	= fread_number(fp);
    	     	pet->max_move	= fread_number(fp);
    	     	fMatch = TRUE;
    	     	break;
    	     }
    	     break;
    	
     	case 'L':
    	     KEY( "Levl",	pet->level,		fread_number(fp));
    	     KEY( "LnD",	pet->long_descr,	fread_string(fp));
	     KEY( "LogO",	lastlogoff,		fread_number(fp));
    	     break;
    	
    	case 'N':
    	     KEY( "Name",	pet->name,		fread_string(fp));
    	     break;
    	
    	case 'P':
    	     KEY( "Pos",	pet->position,		fread_number(fp));
    	     break;
    	
	case 'R':
    	    KEY( "Race",	pet->race, race_lookup(fread_string(fp)));
    	    break;
 	
    	case 'S' :
    	    KEY( "Save",	pet->saving_throw,	fread_number(fp));
    	    KEY( "Sex",		pet->sex,		fread_number(fp));
    	    KEY( "ShD",		pet->short_descr,	fread_string(fp));
    	    break;
    	
    	if ( !fMatch )
    	{
    	    bug("Fread_pet: no match.",0);
    	    fread_to_eol(fp);
    	}
    	
    	}
    }
}

extern	OBJ_DATA	*obj_free;

void fread_obj( CHAR_DATA *ch, FILE *fp )
{

    OBJ_DATA *obj;
    char *word;
    int iNest;
    bool fMatch;
    bool fNest;
    bool fVnum;
    bool first;
    bool new_format;  /* to prevent errors */
    bool make_new;    /* update object */

    fVnum = FALSE;
    obj = NULL;
    first = TRUE;  /* used to counter fp offset */
    new_format = FALSE;
    make_new = FALSE;

    word   = feof( fp ) ? (char*)"End" : fread_word( fp );
    if (!str_cmp(word,"Vnum" ))
    {
        int vnum;
	first = FALSE;  /* fp will be in right place */

        vnum = fread_number( fp );
        if (  get_obj_index( vnum )  == NULL )
	{
            bug( "Fread_obj: bad vnum %d.", vnum );
	}
        else
	{
	    obj = create_object(get_obj_index(vnum),-1);
	    obj->pIndexData->limcount -= 1;
  	    new_format = TRUE;
	}
	
    }

    if (obj == NULL)  /* either not found or old style */
    {
    	obj = new_obj();
    	obj->name		= palloc_string( "" );
    	obj->short_descr	= palloc_string( "" );
    	obj->description	= palloc_string( "" );
    }

    fNest		= FALSE;
    fVnum		= TRUE;
    iNest		= 0;

    for ( ; ; )
    {
	if (first)
	    first = FALSE;
	else
	    word   = feof( fp ) ? (char*)"End" : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
            if (!str_cmp(word,"Affc"))
            {
				CHAR_DATA *wch;
				char *owner;
                OBJ_AFFECT_DATA *paf;
                int sn;

                paf = new_affect_obj();
				init_affect_obj(paf);
                sn = skill_lookup(fread_word(fp));
                if (sn < 0)
                    bug("Fread_obj: unknown skill.",0);
                else
                    paf->type = sn;
 	
		paf->where	= fread_number( fp );
                paf->level      = fread_number( fp );
                paf->duration   = fread_number( fp );
                paf->modifier   = fread_number( fp );
                paf->location   = fread_number( fp );
		fread_flag_new(paf->bitvector, fp);
                paf->aftype  	= fread_number( fp );
		owner = fread_word( fp );
		for (wch = char_list; wch; wch = wch->next) {
			if (!str_cmp(wch->name,owner)) {
				paf->owner = wch;
				break;
			}
		}

		if (!str_cmp(ch->name,owner))
			paf->owner = ch;
                paf->next       = obj->affected;
                obj->affected   = paf;
                fMatch          = TRUE;
                break;
            }
	   if (!str_cmp(word,"AddApp"))
	   {
		OBJ_APPLY_DATA *oad = new_apply_data();
		oad->location = fread_number(fp);
		oad->modifier = fread_number(fp);
		oad->type     = fread_number(fp);
		oad->next     = obj->apply;
		obj->apply    = oad;
		fMatch = TRUE;
		break;
           } 
	    break;

	case 'C':
	    KEY( "Cond",	obj->condition,		fread_number( fp ) );
	    KEY( "Cost",	obj->cost,		fread_number( fp ) );
	    break;

	case 'D':
	    KEY( "Description",	obj->description,	fread_string( fp ) );
	    KEY( "Desc",	obj->description,	fread_string( fp ) );
	    break;

	case 'E':

	    KEYV( "ExtraFlags",	obj->extra_flags);
		if (!str_cmp(word, "ExtF")) {
			zero_vector(obj->extra_flags);
			KEYV( "ExtF",	obj->extra_flags);
		}

	    if ( !str_cmp( word, "ExtraDescr" ) || !str_cmp(word,"ExDe")) {
			EXTRA_DESCR_DATA *ed;

			ed = new_extra_descr();

			ed->keyword			= fread_string( fp );
			/* Don't repeat extra descriptions... */
			if ((obj->extra_descr != NULL)
				&& !str_cmp(ed->keyword,obj->extra_descr->keyword)) {
				free_extra_descr(ed);
			} else {
				ed->description		= fread_string( fp );
				ed->next			= obj->extra_descr;
				obj->extra_descr	= ed;
			}
			fMatch = TRUE;
	    }

	    if ( !str_cmp( word, "End" ) )
	    {
		if ( !fNest || ( fVnum && obj->pIndexData == NULL ) )
		{
		    bug( "Fread_obj: incomplete object.", 0 );
		    free_pstring( obj->name        );
		    free_pstring( obj->description );
		    free_pstring( obj->short_descr );
		    obj->next = obj_free;
		    obj_free  = obj;
		    return;
		}
		else
	        {
		    if ( !fVnum )
		    {
		        free_pstring( obj->name        );
		        free_pstring( obj->description );
			free_pstring( obj->short_descr );
			obj->next = obj_free;
			obj_free  = obj;

			obj = create_object( get_obj_index( OBJ_VNUM_DUMMY ), 0 );
		    }

		    if (!new_format)
		    {
		    	obj->next	= object_list;
		    	object_list	= obj;
		    	obj->pIndexData->count++;
		    }

		    if (!obj->pIndexData->new_format
		    && obj->item_type == ITEM_ARMOR
		    &&  obj->value[1] == 0)
		    {
			obj->value[1] = obj->value[0];
			obj->value[2] = obj->value[0];
		    }
		    if (make_new)
		    {
			int wear;
			
			wear = obj->wear_loc;
			extract_obj(obj);
			obj->pIndexData->limcount += 1;		
			obj = create_object(obj->pIndexData,0);
			obj->pIndexData->limcount -= 1;
			obj->wear_loc = wear;
		    }
		    if ( iNest == 0 || rgObjNest[iNest] == NULL )
			obj_to_char( obj, ch );
		    else
			obj_to_obj( obj, rgObjNest[iNest-1] );
		    return;
		}
	    }
	    break;

	case 'I':
	    KEY( "ItemType",	obj->item_type,		fread_number( fp ) );
	    KEY( "Ityp",	obj->item_type,		fread_number( fp ) );
	    break;

	case 'L':
	    KEY( "Level",	obj->level,		fread_number( fp ) );
	    KEY( "Lev",		obj->level,		fread_number( fp ) );
	    break;

	case 'M':
		KEY( "Mat",		obj->material,	fread_string ( fp ) );
		break;
		
	case 'N':
	    KEY( "Name",	obj->name,		fread_string( fp ) );

	    if ( !str_cmp( word, "Nest" ) )
	    {
		iNest = fread_number( fp );
		if ( iNest < 0 || iNest >= MAX_NEST )
		{
		    bug( "Fread_obj: bad nest %d.", iNest );
		}
		else
		{
		    rgObjNest[iNest] = obj;
		    fNest = TRUE;
		}
		fMatch = TRUE;
	    }
	    break;

   	case 'O':
	    KEY( "Owner",	obj->owner,		fread_string( fp));
	    if ( !str_cmp( word,"Oldstyle" ) )
	    {
		if (obj->pIndexData != NULL && obj->pIndexData->new_format)
		    make_new = TRUE;
		fMatch = TRUE;
	    }
	    break;
		
	case 'R':
	    //KEY( "ResF",        obj->restrict_flags,    fread_number( fp ) );
	    break;
	case 'S':
	    KEY( "ShortDescr",	obj->short_descr,	fread_string( fp ) );
	    KEY( "ShD",		obj->short_descr,	fread_string( fp ) );

	    if ( !str_cmp( word, "Spell" ) )
	    {
		int iValue;
		int sn;

		iValue = fread_number( fp );
		sn     = skill_lookup( fread_word( fp ) );
		if ( iValue < 0 || iValue > 3 )
		{
		    bug( "Fread_obj: bad iValue %d.", iValue );
		}
		else if ( sn < 0 )
		{
		    bug( "Fread_obj: unknown skill.", 0 );
		}
		else
		{
		    obj->value[iValue] = sn;
		}
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'T':
	    KEY( "Timer",	obj->timer,		fread_number( fp ) );
	    KEY( "Time",	obj->timer,		fread_number( fp ) );
	    break;

	case 'V':
	    if ( !str_cmp( word, "Values" ) || !str_cmp(word,"Vals"))
	    {
		obj->value[0]	= fread_number( fp );
		obj->value[1]	= fread_number( fp );
		obj->value[2]	= fread_number( fp );
		obj->value[3]	= fread_number( fp );
		if (obj->item_type == ITEM_WEAPON && obj->value[0] == 0)
		   obj->value[0] = obj->pIndexData->value[0];
		fMatch		= TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Val" ) )
	    {
		obj->value[0] 	= fread_number( fp );
	 	obj->value[1]	= fread_number( fp );
	 	obj->value[2] 	= fread_number( fp );
		obj->value[3]	= fread_number( fp );
		obj->value[4]	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Vnum" ) )
	    {
		int vnum;

		vnum = fread_number( fp );
		if ( ( obj->pIndexData = get_obj_index( vnum ) ) == NULL )
		    bug( "Fread_obj: bad vnum %d.", vnum );
		else
		    fVnum = TRUE;
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    KEYV( "WearFlags",	obj->wear_flags);
	    KEYV( "WeaF",	obj->wear_flags);
		KEY( "WLName",	obj->wear_loc_name,	fread_string(fp) );
	    KEY( "WearLoc",	obj->wear_loc,		fread_number( fp ) );
	    KEY( "Wear",	obj->wear_loc,		fread_number( fp ) );
	    KEY( "Weight",	obj->weight,		fread_number( fp ) );
	    KEY( "Wt",		obj->weight,		fread_number( fp ) );
	    break;

	}

	if ( !fMatch )
	{
//	    bug( "Fread_obj: no match.", 0 );
	    fread_to_eol( fp );
	}
    }
}
