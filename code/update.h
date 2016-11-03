/*
 * Local functions
 */

int hit_gain    args( ( CHAR_DATA *ch ) );
int mana_gain   args( ( CHAR_DATA *ch ) );
int move_gain   args( ( CHAR_DATA *ch ) );
void    mobile_update   args( ( void ) );
void    time_update args( ( void ) );
void    weather_update args ( ( void ) );
void	berus_update args ( ( void ) );
void	calabren_update args ( ( void ) );
void    char_update args( ( void ) );
void    rune_update args( ( void ) );
void    obj_update  args( ( void ) );
void    aggr_update args( ( void ) );
void	gold_update args( ( void ) );
void	affect_update args ( ( void ) );
void    age_update  args( ( void ) );
void    ayell_update    args( ( void ) );
void    upfile_update   args( ( void ) );

void    room_update     args( ( void ) );
void    room_affect_update     args( ( void ) );
void    iprog_pulse_update args(( bool isTick ));
void    save_votes  args( ( void ) );
void    save_demos  args( ( void ) );
bool    do_mob_cast args((CHAR_DATA *ch));
/* used for saving */
int reverse_d args((int dir));
int get_grav_distance args( ( OBJ_DATA *obj ) );
ROOM_INDEX_DATA *get_random_exit args((ROOM_INDEX_DATA *room));
void extract_rune args((RUNE_DATA *rune));
