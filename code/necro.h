/*
 * Stores all the functions for necromancer spells and handling of blood
 * charges, urns, and so forth.  Set up by Eladrian, 3/5/01.
 */

OBJ_DATA *find_urn args((CHAR_DATA *ch));
bool drain_urn args((CHAR_DATA *ch, int charges));
bool check_bond args((CHAR_DATA *ch, CHAR_DATA *mob));
bool check_zombie_summon args((CHAR_DATA *ch));
void power_urn args((CHAR_DATA *ch, int charges));
void animate_two args((CHAR_DATA *ch, OBJ_DATA *corpse));
void animate_three args((CHAR_DATA *ch, OBJ_DATA *corpse));
void animate_four args((CHAR_DATA *ch, OBJ_DATA *corpse));
void make_urn args((CHAR_DATA *ch, OBJ_DATA *corpse));
void visceral_two args((CHAR_DATA *ch));
void visceral_three args((CHAR_DATA *ch));
void visceral_four args((CHAR_DATA *ch));
void ritual_two args((CHAR_DATA *ch, CHAR_DATA *victim));
void ritual_three args((CHAR_DATA *ch, CHAR_DATA *victim));
void ritual_four args((CHAR_DATA *ch, CHAR_DATA *victim));
void flesh_two args((CHAR_DATA *ch, CHAR_DATA *victim));
void flesh_three args((CHAR_DATA *ch, CHAR_DATA *victim));
void flesh_four args((CHAR_DATA *ch, CHAR_DATA *victim));
void soul_add args((CHAR_DATA *ch, int souls));
DECLARE_IPROG_FUN_GREET(greet_prog_corpse_explode);
