// Dev's awesome databased helpfiles. Yay.
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "merc.h"
#include "help.h"
#include "db.h"
#include "magic.h"
#include "lookup.h"
#include "tables.h"
#include "interp.h"
#include "comm.h"
#include "act_comm.h"
#include "devextra.h"
#include "newmem.h"
#include "utility.h"
#include "./repositories/helprepository.h"

int can_see_help(CHAR_DATA *ch, const Help &help, bool fOnlyResult)
{
	int sn = skill_lookup(help.skill.c_str());

	if (!str_cmp(help.skill.c_str(), "olc"))
	{
		if (ch->pcdata->security > 0)
			return true;
		else
			return false;
	}

	if (help.minlevel > ch->level && ch->level > 0)
		return false;

	if (help.minlevel == 51 && (!str_cmp(help.skill.c_str(), "heroimm") && !is_heroimm(ch)))
		return false;

	if (sn > 1 && get_skill(ch, sn) < 1)
	{
		send_to_char("Viewing that helpfile requires a skill you do not have.\n\r", ch);
		return false;
	}

	return true;
}

void show_helpfile(CHAR_DATA *ch, const Help &help)
{
	char buf[MSL];
	sprintf(buf, "%s\n\r%s\n\r", help.title.c_str(), help.helpdata.c_str());
	send_to_char(buf, ch);
}

void modhelp_end_fun(CHAR_DATA *ch, char *argument)
{
	auto helpfiles = HelpRepository(RS.DbRift);
	int updated = helpfiles.Update(ch->pcdata->helpid, argument);

	char results[MSL];
	sprintf(results, "%i helpfiles updated.\n\r", updated);
	send_to_char(results, ch);
}
void do_modhelp(CHAR_DATA *ch, char *argument)
{
	if (!str_cmp(argument, "") || !is_number(argument))
	{
		send_to_char("Syntax: modhelp <id>\n\r", ch);
		return;
	}

	int id = atoi(argument);

	auto helpfiles = HelpRepository(RS.DbRift);
	auto results = helpfiles.FindById(id);

	if (results.empty())
	{
		send_to_char("Error accessing that id number.\n\r", ch);
		return;
	}

	ch->pcdata->helpid = id;
	ch->pcdata->entered_text = palloc_string(results[0].helpdata.c_str());
	enter_text(ch, modhelp_end_fun);
}

void do_help(CHAR_DATA *ch, char *argument)
{
	char buf[MSL];

	if (!str_cmp(argument, ""))
	{
		do_help(ch, "topics");
		return;
	}
	else
	{
		if ((!is_alphanum(argument) && !have_space(argument)) || have_schar(argument))
		{
			do_help(ch, "topics");
			return;
		}
	}

	auto helpfiles = HelpRepository(RS.DbRift);

	std::vector<Help> results = is_number(argument)
		? helpfiles.FindById(atoi(argument))
		: helpfiles.FindByTitle(argument);

	if (results.empty())
	{
		send_to_char("No matching helpfiles found.\n\r", ch);
	}
	else if (results.size() == 1)
	{
		if (!can_see_help(ch, results[0], true))
			send_to_char("No matching helpfiles found.\n\r", ch);
		else
			show_helpfile(ch, results[0]);
	}
	else
	{
		auto exact = helpfiles.FindByQuotedOrExactTitle(argument);

		if (!exact.empty())
		{
			if (!can_see_help(ch, exact[0], false))
				send_to_char("No matching helpfiles found.\n\r", ch);
			else
				show_helpfile(ch, exact[0]);
		}
		else
		{
			send_to_char("Multiple helpfiles matched your request:\n\r", ch);

			for (const Help &help : results)
			{
				if (!can_see_help(ch, help, false))
					continue;

				sprintf(buf, "%-5d %s\n\r", help.id, help.title.c_str());
				send_to_char(buf, ch);
			}
		}
	}
}

void do_delhelp(CHAR_DATA *ch, char *argument)
{
	if (!str_cmp(argument, ""))
	{
		send_to_char("Syntax: delhelp <id #>\n\r", ch);
		send_to_char("        Deletes the helpfile with the given ID number.\n\r", ch);
		return;
	}

	if (!is_number(argument))
	{
		send_to_char("Argument must be a number.\n\r", ch);
		return;
	}

	int id = atoi(argument);

	auto helpfiles = HelpRepository(RS.DbRift);
	int deleted = helpfiles.Remove(id);

	char buf[MSL];
	sprintf(buf, "%i helpfiles deleted.\n\r", deleted);
	send_to_char(buf, ch);
}

void addhelp_end_fun(CHAR_DATA *ch, char *argument)
{
	send_to_char("Help text modified.\n\r", ch);
	send_to_char("Use addhelp to specify the rest of your helpfiles settings and add it.\n\r", ch);
}

void do_addhelp(CHAR_DATA *ch, char *argument)
{
	char buf[MSL * 3], title[MSL], skill[MSL], arg[MSL], *ttitle;
	int minlevel;

	if (!str_cmp(argument, ""))
	{
		send_to_char("Syntax: addhelp edit\n\r", ch);
		send_to_char("Syntax: addhelp 'help title' 'skill required' minimumlevel\n\r", ch);
		send_to_char("Example: addhelp 'HELP NEW FORMAT' 'none' 0\n\r", ch);
		send_to_char("Any words in the title will be keywords, case insensitive, that the help command searches for.\n\r", ch);
		send_to_char("The skill required is the name of a skill you must have at 1% or greater to view the help.\n\r", ch);
		send_to_char("It should only be used for cabal power helpfiles and other semi-secret helpfiles and should be 'none' otherwise.\n\r", ch);
		send_to_char("Minimum level is the minimum level required to view this helpfile, and should have a 0 if it's not\n\r", ch);
		send_to_char("an immortal or level dependant helpfile.\n\r", ch);
		send_to_char("Addhelp edit will allow you to change and add the actual text of your helpfile, and must be done first.\n\r", ch);
		return;
	}

	if (!str_cmp(argument, "edit"))
	{
		enter_text(ch, addhelp_end_fun);
		return;
	}

	if (!ch->pcdata->entered_text || ch->pcdata->entered_text[0] == '\0' || !str_cmp(ch->pcdata->entered_text, ""))
	{
		send_to_char("You haven't specified the contents of your helpfile.\n\r", ch);
		return;
	}

	argument = one_argument(argument, title);
	argument = one_argument(argument, skill);
	argument = one_argument(argument, arg);

	ttitle = palloc_string(upstring(title));
	minlevel = atoi(arg);

	Help help;
	help.title = ttitle;
	help.skill = skill;
	help.minlevel = minlevel;
	help.helpdata = ch->pcdata->entered_text;

	auto helpfiles = HelpRepository(RS.DbRift);
	auto added = helpfiles.Add(help);

	if (added)
		sprintf(buf, "Help file added:\n\rTitle: %s\n\rSkill Required: %s\n\rMinimum Level: %d\n\rHelp Text: %s\n\r", ttitle, skill, minlevel, ch->pcdata->entered_text);
	else
		sprintf(buf, "Failed to add helpfile:\n\rTitle: %s\n\rSkill Required: %s\n\rMinimum Level: %d\n\rHelp Text: %s\n\r", ttitle, skill, minlevel, ch->pcdata->entered_text);

	send_to_char(buf, ch);
}

void do_listhelp(CHAR_DATA *ch, char *argument)
{
	char buf[MSL], arg1[MSL];

	if (!str_cmp(argument, ""))
	{
		send_to_char("Syntax: listhelp all\n\r", ch);
		send_to_char("        listhelp <field> <value>\n\r", ch);
		send_to_char("        Valid fields are: id, title, skill, minlevel, helpdata\n\r", ch);
		send_to_char("        You can search for substrings by entering them normally.\n\r", ch);
		return;
	}

	argument = one_argument(argument, arg1);

	auto helpfiles = HelpRepository(RS.DbRift);

	std::vector<Help> results = !str_cmp(arg1, "ALL")
		? helpfiles.FindAll()
		: helpfiles.FindByFieldRegex(arg1, argument);

	for (const Help &help : results)
	{
		sprintf(buf, "Help ID: %d\n\rHelp Title: %s\n\rRequired Skill: %s\n\rMinimum Level: %d\n\r", help.id, help.title.c_str(), help.skill.c_str(), help.minlevel);
		send_to_char(buf, ch);
		sprintf(buf, "Help Text:\n\r%s\n\r", help.helpdata.c_str());
		send_to_char(buf, ch);
	}
}

