/* Proficiencies.  -Cal */
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "db.h"
#include "lookup.h"
#include "tables.h"
#include "comm.h"
#include "devextra.h"
#include "interp.h"
#include "update.h"
#include "newmem.h"
#include "act_info.h"
#include "./include/spdlog/fmt/bundled/format.h"
#include "./include/spdlog/fmt/bundled/printf.h"


//don't forget to increment max_prof in prof.h and clean make if your new proficiency goes over MAX_PROF
//or there are SIGSEGVs and axes to the face in your future

//TOFIX: THE CHECK TO PROF IMPROVEMENT % IS WAY OFF
#define POINTS_PER_HOUR			5

#define OBJ_VNUM_MEAT_CHUNKS	75
#define OBJ_VNUM_CAMPFIRE		76

const std::vector<prof_level_type> CProficiencies::prof_level_table =
{
	// name,				level
	{ "incapable",			-1 	},
	{ "proficient",			 1	},
	{ "very proficient",	 4	},
	{ "adept",				 7	},
	{ "masterful",			10	}
};
/* note: profs can be referred to either by psn or by profname, use psn in the rare cases where it's checked very often
        otherwise use psn_none and refer by profname */

const std::vector<proficiency_type> CProficiencies::prof_table =
{
	// ppsn, 				name,						cost,	mnlevel,	requires, 		flags
	{ &psn_swimming, 		"swimming",					18,		 1, 		nullptr,		PFLAGS_BASIC 	},
	{ &psn_none, 			"butchery",					18,		 1, 		nullptr,		PFLAGS_BASIC 	},
	{ &psn_mountaineering,	"mountaineering",			13,		 1,			nullptr,		PFLAGS_BASIC	},
	{ &psn_none,			"bandaging",				18,		 1,			nullptr,		PFLAGS_BASIC	},
	{ &psn_none,			"appraising",				15,		 1,			nullptr,		PFLAGS_BASIC	},
	{ &psn_none,			"trap detecting",			20,		 1,			nullptr,		PFLAGS_BASIC	},
	{ &psn_none,			"cooking",					13,		 1,			nullptr,		PFLAGS_BASIC	},
	{ &psn_none,			"firestarting", 			15,		 1,			nullptr,		PFLAGS_BASIC	},
	{ &psn_none,			"wildlore",					10,		 1,			nullptr,		PFLAGS_NONE		},
	{ &psn_none,			"wilderness subsistence",	26,		 1,			nullptr,		PFLAGS_NONE		},
	{ &psn_none,			"ancient languages",		25,		30,			nullptr,		PFLAGS_NONE		},
	{ &psn_none,			"forgotten lore",			20,		30,			nullptr,		PFLAGS_NONE		},
	{ &psn_none,			"tracking",					30,		30,			nullptr,		PFLAGS_NONE		}
};

const std::vector<prof_cmd_type> CProficiencies::prof_cmd_table =
{
	// name,		cmd,			requires
	{ "butcher",	prof_butcher,	"butchery"		},
	{ "bandage",	prof_bandage,	"bandaging"		},
	{ "appraise",	prof_appraise,	"appraising"	},
	{ "cook",		prof_cook,		"cooking"		},
	{ "firestart",	prof_firestart, "firestarting"	},
	{ "track",		prof_tracking,	"tracking"		}
};

//last entry in each one should be null
const struct proficiency_msg CProficiencies::prof_msg_table [] =
{
	// swimming
	{
		{ 
			"$N describes basic swimming strokes.", 
			"$N shares a few anecdotes about Shalaran rivers and oceans.", 
			"$N makes you practice breathing in the most efficient manner.",
			nullptr
		}
	},
	// butchery
	{
		{
			"$N grumbles and drags a dead calf out of the back room.", 
			"Taking time to point out the 'good parts', $N hacks up the corpse.",
			"By the time $E's done, chunks of fresh, edible meat litter the floor around the corpse.", 
			nullptr
		}
	},
	// mountaineering
	{
		{
			"$N begins to speak about the common mistakes inexperienced travelers make when traversing mountains.",
			"$N describes how to avoid treacherous spots that could tire legs and make travel more difficult.",
			nullptr
		}
	},
	// bandaging
	{
		{
			"$N takes out a roll of bandages and a tourniquet, explaining where to apply pressure.",
			"$N describes how to treat the various types of bleeding you might encounter.",
			"$N wraps a few practice bandages as you watch.",
			nullptr
		}
	},
	// appraising
	{
		{
			"$N pulls out a gem and holds it up to the light, pointing out tiny flaws and irregularities.",
			"$N shows you basic methods of estimating value based on an items condition and rarity.",
			"$N describes how to best convince a shopkeeper that their price is too high based on your knowledge.",
			nullptr
		}
	},
	// trap detecting
	{
		{
			"$N describes the most common types of traps you will encounter in your travels.",
			"$N shows you how to recognize trap triggers before you stumble into them.",
			"$N makes you practice rolling and ducking to avoid traps after you've triggered them.",
			nullptr
		}
	},
	// cooking
	{
		{
			"$N mutters and tells you to watch, tossing some raw meat into a pan and frying it.",
			"$N makes you practice cooking common foods, requiring you to prove basic competence before moving on.",
			"$N nods slightly as you finish cooking a chicken.",
			nullptr
		}
	},
	// firestarting
	{
		{
			"$N shows you how to gather kindling and small twigs.",
			"$N demonstrates the best way to arrange the wood for the fire.",
			"$N makes you rub sticks together until you can reliably produce a spark.",
			nullptr
		}
	},
	// wildlore
	{
		{ 
			"Blah", nullptr
		}
	},
	// wilderness subsistence
	{
		{
			"Blah2",
			nullptr
		}
	},
	// ancient languages
	{
		{
			"$N hobbles slowly over to the bookshelf, returning with numerous old, dusty books.", 
			"$N drops the books on a table with a thud, motioning for you to open the one before you.",
			"$N goes on to explain the intricacies of certain languages, demonstrating pronunciation, reading, and writing.", 
			nullptr
		}
	},
	// forgotten lore
	{
		{
			"$N takes out some ancient artifacts, showing you common identifiers to recognize.",
			"$N hands you a musty old tome, suggesting you memorize the contents for future reference.",
			"You spend a few hours studying the index.",
			nullptr
		}
	},
	// tracking
	{
		{
			"$N beckons you over to a nearby tree.", 
			"As $E points out small details you would not have noticed on your own, you see something has passed by recently.", 
			"$E tells you of other signs to look for, and you feel more adept at tracking.",
			nullptr
		}
	}
};

CProficiencies prof_none; 
short psn_none;
short psn_swimming;
short psn_mountaineering;

CProficiencies::CProficiencies()
{
	ch = 0;
	points = 0;

	memset(profs, -1, sizeof(int) * MAX_PROFS);
}

CProficiencies::~CProficiencies()
{

}

/// Assigns each proficiency in the proficiency table with their associated psn.
void CProficiencies::AssignPsns()
{
	auto prof_table_size = std::size(prof_table);
	for (auto i = 0; i < prof_table_size; i++)
	{
		*(prof_table[i].ppsn) = i;
	}
}

/// Queries the proficiency table for the index of the specific proficiency.
/// @param profname: The name of the proficiency to query.
/// @returns The table index of the given proficiency. (Default: -1)
short CProficiencies::ProfIndexLookup(const char *profname)
{
	if (profname == nullptr)
		return -1;

	auto prof_table_size = std::size(prof_table);
	for (int i = 0; i < prof_table_size; i++)
	{
		if (!str_cmp(profname, prof_table[i].name))
			return i;
	}

	return -1;
}

/// Checks whether a specific psn is correlated with a proficiency.
/// @param psn: The psn of the proficiency to check.
/// @returns true if the given psn has a correlated proficiency; otherwise false.
bool CProficiencies::HasProf(int psn)
{
	auto profs_size = std::size(profs);
	if (psn < 0 || psn > profs_size - 1)
		return false;

	return profs[psn] > -1;
}

/// Checks whether a specific name is correlated with a proficiency.
/// @param profname: The name of the proficiency to check.
/// @returns true if the given name has a correlated proficiency; otherwise false.
bool CProficiencies::HasProf(char *profname)
{
	auto index = ProfIndexLookup(profname);
	auto hasProf = index != -1
		? HasProf(index)
		: false;

	return hasProf;
}

/// Retrieves the level of proficiency in the proficiency table by the specific psn.
/// @param psn: The psn used to retrieve the level.
/// @returns The level of the retrieved proficiency. (Default: -1)
int CProficiencies::GetProf(int psn)
{
	auto profs_size = std::size(profs);
	if (psn < 0 || psn > profs_size - 1)
		return -1;

	auto prof = profs[psn];
	return prof;
}

/// Retrieves the level of proficiency in the proficiency table by the specific name.
/// @param profname: The name used to retrieve the level.
/// @returns The level of the retrieved proficiency. (Default: -1)
int CProficiencies::GetProf(char *profname)
{
	auto index = ProfIndexLookup(profname);
	auto getProf = index != -1
		? GetProf(index)
		: -1;
	
	return getProf;
}

/// Sends a list of proficiencies taught by specific trainer to the specific character.
/// @param ch: The character to whom to send the list.
/// @param trainer: The name of the trainer teaching the proficiencies.
void CProficiencies::GetProfsTaughtByTrainer(char_data* ch, char_data* trainer)
{
	if (ch == nullptr)
	{
		RS.Bug("CProficiencies::GetProfsTaughtByTrainer: Charater is null.");
		return;
	}
	
	if (trainer == nullptr)
	{
		send_to_char("Invalid trainer", ch);
		return;
	}

	act("You may learn the following proficiencies from $N:", ch, 0, trainer, TO_CHAR);

	auto prof_table_size = std::size(prof_table);
	auto profs_taught_size = std::size(trainer->pIndexData->profs_taught);
	for (auto i = 0; i < prof_table_size; i++)
	{
		for (auto j = 0; j < profs_taught_size; j++)
		{
			if (trainer->pIndexData->profs_taught[j] == i)
			{
				auto buffer = fmt::sprintf("%-16s | %d points\n\r", prof_table[i].name, prof_table[i].cost);
				send_to_char(buffer.c_str(), ch);
			}
		}
	}
}

/// Trains a specific character in a specific proficiency taught by a specific trainer.
/// @param ch: The character to train.
/// @param trainer: The trainer whom trains the character.
/// @param argument: The proficiency being trained.
void CProficiencies::TrainProficiency(char_data* ch, char_data* trainer, char* argument)
{
	if (ch == nullptr)
	{
		RS.Bug("CProficiencies::TrainProficiency: Charater is null.");
		return;
	}
	
	if (trainer == nullptr)
	{
		send_to_char("Invalid trainer", ch);
		return;
	}

	char arg1[MSL];
	argument = one_argument(argument, arg1);

	if (str_cmp(arg1, "train"))
	{
		send_to_char("Syntax: proficiencies train <proficiency>\n\r", ch);
		return;
	}

	auto prof = ProfIndexLookup(argument);

	int i;
	auto found = false;
	auto profs_taught_size = std::size(trainer->pIndexData->profs_taught);
	for (i = 0; i < profs_taught_size; i++)
	{
		if (trainer->pIndexData->profs_taught[i] == prof)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		send_to_char("You can't study that here.\n\r", ch);
		return;
	}

	if (ch->Profs()->GetProf(prof) > 0)
	{
		send_to_char("You are already familiar with that proficiency.\n\r", ch);
		return;
	}

	auto proficiency = prof_table[prof];
	if (proficiency.cost > ch->Profs()->GetPoints())
	{
		send_to_char("You don't have enough points to study that proficiency.\n\r", ch);
		return;
	}

	auto prof_requires = ProfIndexLookup(proficiency.requires);
	if (prof_requires != -1 && ch->Profs()->GetProf(prof_requires) < 0)
	{
		send_to_char("That proficiency requires understanding of a proficiency you do not possess.\n\r", ch);
		return;
	}

	if (proficiency.minlevel > ch->level)
	{
		send_to_char("You are not advanced enough in your guild to learn that proficiency.\n\r", ch);
		return;
	}

	act("You ask $N to teach you about $t.", ch, proficiency.name, trainer, TO_CHAR);

	auto messages = prof_msg_table[prof];
	for (i = 0; messages.learning_msgs[i] != nullptr && i < 5; i++)
	{
		if (i == 4 || !messages.learning_msgs[i + 1])
		{
			auto buffer = fmt::format("{}You are now proficient at {}.{}\n\r",
				get_char_color(ch, "yellow"),
				proficiency.name,
				END_COLOR(ch));

			char *tptr = talloc_string(buffer.c_str()); 
			RS.Queue.AddToQueue((i + 1) * 2, 2, send_to_char, tptr, ch);
		}

		RS.Queue.AddToQueue((i + 1) * 2, 5, act, prof_msg_table[prof].learning_msgs[i], ch, 0, trainer, TO_CHAR);
	}

	ch->Profs()->DeductPoints(proficiency.cost);
	ch->Profs()->SetProf(prof, 1);

	WAIT_STATE(ch, (i + 1) * 2);
}

/// Retrieves a proficiency by a specific psn.
/// @param psn: The psn of the proficiency to retrieve.
/// @returns The proficiency with the given psn. (Default: uninitialized proficiency)
proficiency_type CProficiencies::GetProficiency(int psn)
{
	proficiency_type result = { &psn_none, "", 0, 0, nullptr, PFLAGS_NONE };

	auto prof_table_size = std::size(prof_table);
	if (psn < 0 || psn > prof_table_size - 1)
		return result;

	return prof_table[psn];
}

/// Retrieves a proficiency by a specific name.
/// @param profname: The name of the proficiency to retrieve.
/// @returns The proficiency with the given name. (Default: uninitialized proficiency)
proficiency_type CProficiencies::GetProficiency(char* profname)
{
	proficiency_type result = { &psn_none, "", 0, 0, nullptr, PFLAGS_NONE };
	if(profname == nullptr)
		return result;

	auto it = std::find_if(prof_table.begin(), prof_table.end(), [profname] (auto prof) {
		return prof.name != nullptr && !str_cmp(prof.name, profname);
	});

	if (it == prof_table.end())
		return result;
	
	auto idx = it - prof_table.begin();
	return prof_table[idx];
}

/// TBD
/// @param profname: TBD
/// @param nArg: TBD
/// @returns TBD
float CProficiencies::ProfEffect(char *profname, float nArg)
{
	int pindex = ProfIndexLookup(profname);
	if (pindex == -1)
		return 0;

	if (!str_cmp(profname, "appraising"))
	{
		CheckImprove(profname, 50);
		return 100 - ((profs[pindex] * 2) + 10); // 12% off at 1, 30% at 10
	}

	if (!str_cmp(profname, "trap detecting"))
	{
		//returned value needs to be >= than quality of the trap to detect its presence
		//if you have a skill of 1 and a trap of 5, you need a rnd >=4 (40% chance)
		//if you have a skill of 1 and a trap of 9, you need a rnd =8 (10% chance)
		//if you have a skill of 1 and a trap of 1, you need a rnd >=0 (80% chance)
		//that's for detection in adjacent room, to avoid the trap as it fires the chance is detect / 2
		return number_range(-2, 8) + profs[pindex];
	}

	if (!str_cmp(profname, "firestarting"))
	{
		return (nArg + 25) / 25; // multiplied by gain to get hp gain
	}

	return 0;
}

/// TBD
/// @param psn: TBD
/// @returns TBD
float CProficiencies::ProfEffect(int psn)
{
	auto prof_table_size = std::size(prof_table);
	if (psn < 0 || psn > prof_table_size - 1)
		return 0;

	return ProfEffect(prof_table[psn].name);
}

/// Sends a list of known proficiencies to the specific character.
/// @param player: The character to whom to send the list.
void CProficiencies::ListKnownProficiencies(char_data* player)
{
	if (player == nullptr)
	{
		RS.Bug("CProficiencies::ListKnownProficiencies: Player is null.");
		return;
	}

	std::vector<std::string> responseList;

	auto limit = std::size(profs);
	for (int i = 0; i < limit ; i++)
	{
		if(profs[i] < 0)
			continue;
		
		char* profName = ch->Profs()->GetProficiency(i).name;
		auto profStr = fmt::format("You are {} at {}.\n\r", ch->Profs()->GetSkillLevelName(i), profName);
		responseList.push_back(profStr);
	}

	if (responseList.empty())
	{
		send_to_char("You currently have no proficiencies.\n\r", player);
	}
	else
	{
		send_to_char("Your proficiencies are:\n\r", player);
		for (auto response : responseList)
		{
			send_to_char(response.c_str(), player);
		}
	}	
}

/// Sends a list of basic proficiencies to the specific character.
/// @param player: The character to whom to send the list.
void CProficiencies::ListBasicProficiencies(char_data* player)
{
	if (player == nullptr)
	{
		RS.Bug("CProficiencies::ListBasicProficiencies: Player is null.");
		return;
	}

	char buf[MSL];

	send_to_char("The basic proficiencies available to adventurers are:\n\r", player);

	auto prof_table_size = std::size(prof_table);
	for (auto i = 0; i < prof_table_size; i++)
	{
		if (prof_table[i].flags & PFLAGS_BASIC)
		{
			sprintf(buf, "%-20s  ", prof_table[i].name);

			if ((i + 1) % 3 == 0)
			{
				int slen = strlen(buf), j;
				for (j = 1; buf[slen - j] == ' '; j++) { }
				buf[slen - j + 1] = '\0';
			}

			send_to_char(buf, ch);

			if ((i + 1) % 3 == 0)
				send_to_char("\n\r", player);
		}
	}

	send_to_char("\n\rMany other proficiencies are known to Shalaran adventurers, but you must first find a teacher.\n\r", player);
}

/// Assigns a specific character to these proficiencies.
/// @param nch: The character to whom these proficiencies are assigned.
void CProficiencies::SetChar(char_data *nch)
{
	ch = nch;
}

/// Assigns a specific level to a proficiency marked at the specific index.
/// @param profindex: The index of the proficiency to assign.
/// @param proflevel: The level of the proficiency to assign.
void CProficiencies::SetProf(int profindex, int proflevel)
{
	auto profs_size = std::size(profs);
	if (profindex < 0 || profindex > profs_size - 1)
	{
		RS.Bug("CProficiencies::SetProf : profindex out of bounds [%d]", profindex);
		return;
	}

	profs[profindex] = proflevel;

	return;
}

/// Writes a list of proficiencies and their assigned points to a specific file handle.
/// @param vfp: The file handle used to write the proficiency list.
/// @note The calling method is expected to handle the opening / closing of the file handle.
void CProficiencies::WriteProfs(void *vfp)
{
	FILE *fp = static_cast<FILE *>(vfp);
	fprintf(fp,"ProfPoints %d %ld\n", points, pawardedtime);

	auto prof_table_size = std::size(prof_table);
	for (int i = 0; i < prof_table_size; i++)
	{
		if (profs[i] > -1)
			fprintf(fp, "Prof %s~ %d\n", prof_table[i].name, profs[i]);
	}
}

/// Sends a list of proficiencies and the number of points assigned to them to the specific character.
/// @param imm: The character to whom to send the list.
void CProficiencies::DisplayProfsForStat(CHAR_DATA *imm)
{
	// auto points = GetPoints();
	// auto buffer = fmt::format("Proficiencies ({} pts left): \n\r", points);

	// auto prof_table_size = std::size(prof_table);
	// for (auto i = 0; i < prof_table_size; i++)
	// {
	// 	if (profs[i] == -1)
	// 		continue;

	// 	buffer += fmt::format("{} ({}) \n\r", prof_table[i].name, profs[i]);
	// }

	// buffer = std::string(format_string(buffer.data()));
	// send_to_char(buffer.c_str(), imm);

	// TODO: the above code causes a double free error, so using the old code for now
	// boo format_string(char*) boo!!!

	char buf[MSL], buf2[MSL];
	int i;
	sprintf(buf, "Proficiencies (%d pts left): ", GetPoints());
	for(i = 0; prof_table[i].name != nullptr; i++)
	{
		if(profs[i] == -1)
			continue;
		sprintf(buf2,"%s (%d) ", prof_table[i].name, profs[i]);
		strcat(buf, buf2);
	}
	char *fbuf = palloc_string(buf);
	fbuf = format_string(fbuf);
	send_to_char(fbuf, imm);
	free_pstring(fbuf);
}

/// Retrieves the skill mastery name based on the proficiency level.
/// @param ind: The level of the proficiency.
/// @returns The name of the skill mastery that correlates with the specific level.
const char* CProficiencies::GetSkillLevelName(int ind)
{
	auto profs_size = std::size(profs);
	if(ind < 0 || ind > profs_size - 1)
	{
		RS.Bug("CProficiencies::GetSkillLevelName : index out of bounds [%d]", ind);
		return "";
	}

	auto slevel = profs[ind];
	auto prof_level_table_size = std::size(prof_level_table);
	for (int i = prof_level_table_size - 1; i >= 0; i--)
	{
		if (prof_level_table[i].level <= slevel)
			return prof_level_table[i].name;
	}

	return prof_level_table[0].name;
}

/// Sends a list of proficiencies and at what skill mastery the character has learned.
void CProficiencies::ShowProfsToChar()
{
	auto prof_table_size = std::size(prof_table);
	for (auto i = 0; i < prof_table_size; i++)
	{
		if (profs[i] > -1)
		{
			auto buffer = fmt::format("You are {} in {}.\n\r", GetSkillLevelName(i), prof_table[i].name);
			send_to_char(buffer.c_str(), ch);
		}
	}
}

/// Assigns a specific number of points that a character can use to learn proficiencies.
/// @param newpoints: The number of points to assign to the character.
void CProficiencies::SetPoints(int newpoints)
{
	points = newpoints;
}

/// Reduces the number of points that a character can use to learn proficiencies by a specific amount.
/// @param dpoints: The number of points to deduct from the character.
void CProficiencies::DeductPoints(int dpoints)
{
	points -= dpoints;
}

/// Retrieves the number of points that the character can use on proficiencies.
int CProficiencies::GetPoints()
{
	return points;
}

/// Assigns the time that the character can learn new proficiency points.
/// @param newtime: The time used to set the internal time.
void CProficiencies::SetPATime(time_t newtime)
{
	pawardedtime = newtime;
}

/// Updates the number of points a character can use to learn proficiencies.
void CProficiencies::UpdateProfPoints()
{
	if (ch->played + (current_time - ch->logon) < pawardedtime + 3600)
		return;

	auto buffer = fmt::format("{}You feel ready to study new proficiencies.{}\n\r", 
		get_char_color(ch,"yellow"), 
		END_COLOR(ch));

	if (pawardedtime == 0)
		send_to_char("See HELP PROFICIENCY for more information.\n\r", ch);

	points += POINTS_PER_HOUR;

	send_to_char(buffer.c_str(), ch);

	pawardedtime = ch->played + (current_time - ch->logon);
}

/// Gives chance to level a specific proficiency.
/// @param pindex: The index of the proficiency to level
/// @param chance: The chance to level a specific proficiency.
void CProficiencies::CheckImprove(int pindex, int chance) //chance is out of 10000
{
	if (profs[pindex] >= 10 || profs[pindex] < 1)
		return;

	if (number_percent() > 10)
		return;

	if (number_percent() > 10)
		return;

	if (profs[pindex] > 4)
	{
		if(number_percent() > (6 * profs[pindex]))
			return;
	}

	if (number_percent() > chance)
		return;

	profs[pindex]++;

	auto buffer = fmt::format("{}You have {} {}.{}\n\r", 
		get_char_color(ch, "green"), 
		profs[pindex] == 10 ? "mastered" : "improved at",
		prof_table[pindex].name, 
		END_COLOR(ch));
	send_to_char(buffer.c_str(), ch);

	gain_exp(ch, 250);
}

/// Gives chance to level a specific proficiency.
/// @param profname: The name of the proficiency to level
/// @param chance: The chance to level a specific proficiency.
void CProficiencies::CheckImprove(char *profname, int chance)
{
	return CheckImprove(ProfIndexLookup(profname), chance);
}

/// Executes a specific command with a specific argument.
/// @param command: The name of the command to execute.
/// @param argument: The argument given to the command.
/// @returns true if the command executes; otherwise false.
bool CProficiencies::InterpCommand(char *command, char *argument)
{
	if (strlen(command) == 0)
		return false;

	auto prof_cmd_table_size = std::size(prof_cmd_table);
	for (auto i = 0; i < prof_cmd_table_size; i++)
	{
		auto cmd = prof_cmd_table[i];
		auto foundPos = std::string(cmd.name).find(command);
		if (foundPos == std::string::npos)
			continue;
		
		auto pindex = ProfIndexLookup(cmd.requires);
		if (cmd.requires == "none" || pindex == -1 || ch->Profs()->HasProf(pindex))
		{
			(cmd.cmd) (ch, argument);
			return true;
		}
	}
			
	return false;
}

/// Applies an affect from a proficiency to a specific character.
/// @param ch: The character to whom to apply the affect.
/// @param name: The name of the affect.
/// @param duration: The duration of the affect.
/// @param fInvis: true if the affect is visible; otherwise false. (Default: true)
void add_prof_affect(CHAR_DATA *ch, char *name, int duration, bool fInvis = true)
{
	AFFECT_DATA af;
	init_affect(&af); 
	af.type = gsn_timer;
	af.name = palloc_string(name);
	af.duration = duration;
	af.aftype == fInvis ? AFT_INVIS : AFT_SKILL;
	affect_to_char(ch, &af);
}

/// Determines if a specific character is affected by a specific proficiency affliction.
/// @param ch: The character to check for the affect.
/// @param prof: The name of the affect.
/// @returns true if the character is affected by the specific proficiency affliction.
bool is_affected_prof(CHAR_DATA *ch, char *prof)
{
	for (auto paf = ch->affected; paf; paf = paf->next)
	{
		if (paf->type == gsn_timer && paf->name && !str_cmp(paf->name, prof))
			return true;
	}

	return false;
}

/// TBD
/// @param ch: TBD
/// @param argument: TBD
void do_proficiencies(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *trainer = nullptr;
	bool bBreak= false;

	if (IS_NPC(ch))
		return;

	for (auto iterator = ch->in_room->people; iterator && !bBreak; iterator = iterator->next_in_room)
	{
		if (!IS_NPC(iterator))
			continue;

		for (int i = 0; i < MAX_PROFS_TAUGHT_BY_MOB && !bBreak; i++)
		{
			if (iterator->pIndexData->profs_taught[i] > -1)
			{
				bBreak = true;
				trainer = iterator;
				break;
			}
		}
	}

	if (!trainer && str_cmp(argument, "basic"))
	{
		ch->Profs()->ListKnownProficiencies(ch);
		return;
	}
	else if (!str_cmp(argument, "basic"))
	{
		ch->Profs()->ListBasicProficiencies(ch);
		return;
	}

	if (!trainer)
		return;

	if (argument[0] != '\0')
	{
		ch->Profs()->TrainProficiency(ch, trainer, argument);
		return;
	}

	ch->Profs()->GetProfsTaughtByTrainer(ch, trainer);
}

/// Perform the tracking proficiency.
/// @param ch: The character who is attempting to track.
/// @param argument: The target of the tracking.
void prof_tracking(CHAR_DATA *ch, char *argument)
{
	if (ch == nullptr)
	{
		RS.Bug("prof_bandage: ch is nullptr");
		return;
	}

	if (argument == nullptr)
	{
		send_to_char("Track who?\n\r",ch);
		return;
	}

	auto victim = get_char_world(ch, argument);
	if (!victim || victim == ch || IS_NPC(victim))
	{
		send_to_char("Track who?\n\r",ch);
		return;
	}

	if (is_affected_prof(ch, "tracking"))
	{
		send_to_char("You cannot attempt to track them again yet.\n\r",ch);
		return;
	}

	auto sect = ch->in_room->sector_type;
	if (!IS_GROUND(ch->in_room) || sect == SECT_CITY || sect == SECT_INSIDE || sect == SECT_BURNING || sect == SECT_ROAD)
	{
		send_to_char("Even if they had been here, there would be no suitable tracks left for you to follow.\n\r", ch);
		return;
	}

	act("$n begins to poke and prod at the ground, clearly searching for something.", ch, 0, 0, TO_ROOM);
	auto buffer = std::string("You were unable to find any sign of $N here.");

	char *direction = nullptr;
	for (auto i = 0; i < MAX_TRACKS; i++)
	{
		if (!ch->in_room->tracks[i])
			break;

		if (ch->in_room->tracks[i]->prey != victim || ch->in_room->tracks[i]->flying || ch->in_room->tracks[i]->sneaking)
			continue;

		auto &ttime = ch->in_room->tracks[i]->time;
		if (abs(ttime.day - time_info.day) > 1 || (ttime.month != time_info.month && ttime.day != 30))
			continue;

		if ((ttime.day == time_info.day && abs(time_info.hour - ttime.hour) > 3) ||
		    (ttime.day == time_info.day - 1 && ttime.hour >= 20 && time_info.hour <= 3))
		{
			buffer = std::string("You can tell $N has been here, but the tracks have deteriorated too much to tell which way $E left.");
			break;
		}

		int diruse = (number_percent() > (35 - (ch->Profs()->GetProf("tracking")) * 2))
			? ch->in_room->tracks[i]->direction
			: number_range(0, MAX_DIR - 1);

		direction = flag_name_lookup(diruse, direction_table);

		buffer = std::string("From the pattern of tracks here, you suspect $N left $t.");
	}

	act(buffer.c_str(), ch, direction, victim, TO_CHAR);
	add_prof_affect(ch, "tracking", (std::max(0, 5 - ch->Profs()->GetProf("tracking"))));

	ch->Profs()->CheckImprove("tracking", 150);
}

/// Build a fire using the firestarting proficiency.
/// @param ch: The character who is attempting to start a fire.
/// @param dur: The duration of that the fire remains active.
void build_fire(CHAR_DATA *ch, int dur)
{
	if (number_percent() > 60 + ch->Profs()->GetProf("firestarting") * 3)
	{
		act("$n's fire dies out despite $s best attempts to kindle it.", ch, 0, 0, TO_ROOM);
		act("Your fire dies out despite your best attempts to kindle it.", ch, 0, 0, TO_CHAR);
		ch->Profs()->CheckImprove("firestarting", 300);
		return;
	}

	auto buffer = fmt::format("{}The spark is caught by some dried brush, and it quickly erupts into a blazing campfire!{}",
		get_char_color(ch, "lightred"),
		END_COLOR(ch));
	act(buffer.c_str(), ch, 0, 0, TO_ALL);

	auto fire = create_object(get_obj_index(OBJ_VNUM_CAMPFIRE), ch->level);
	fire->value[0] = ch->Profs()->GetProf("firestarting") + 1;
	fire->timer = dur;
	obj_to_room(fire, ch->in_room);
	add_prof_affect(ch, "firestarting", dur);
	ch->Profs()->CheckImprove("firestarting", 250);
}

/// Perform the firestarting proficiency.
/// @param ch: The character who is attempting to start a fire.
/// @param argument: (Not used).
void prof_firestart(CHAR_DATA *ch, char *argument)
{
	if (is_affected_prof(ch, "firestarting"))
	{
		send_to_char("You cannot start another fire yet.\n\r", ch);
		return;
	}

	auto sect = ch->in_room->sector_type;
	if (sect == SECT_WATER || sect == SECT_UNDERWATER || sect == SECT_VERTICAL || sect == SECT_AIR || sect == SECT_CITY || sect == SECT_INSIDE)
	{
		send_to_char("You can't build a fire here.\n\r",ch);
		return;
	}

	for (auto fire = ch->in_room->contents; fire; fire = fire->next_content)
	{
		if (fire->item_type == ITEM_CAMPFIRE)
		{
			send_to_char("There is already a fire burning here.\n\r", ch);
			return;
		}
	}

	if (ch->move < ch->level)
	{
		send_to_char("You are too tired to gather wood.\n\r", ch);
		return;
	}

	auto dur = 12;
	act("You begin to build a campfire, gathering sticks and twigs from your surroundings.", ch, 0, 0, TO_CHAR);
	act("$n begins to build a campfire, gathering sticks and twigs from $s surroundings.", ch, 0, 0, TO_ROOM);
	ch->move -= ch->level;

	RS.Queue.AddToQueue(1, 2, send_to_char, "You rub two sticks together, trying to produce a flame.\n\r", ch);
	RS.Queue.AddToQueue(1, 5, act, "$n rubs two sticks together, trying to produce a flame.", ch, 0, 0, TO_ROOM);
	RS.Queue.AddToQueue(3, 2, send_to_char, "The sticks begin to smoke, and soon you produce a spark.\n\r", ch);
	RS.Queue.AddToQueue(3, 5, act, "The sticks begin to smoke, and soon $n produces a spark.", ch, 0, 0, TO_ROOM);
	RS.Queue.AddToQueue(5, 2, build_fire, ch, dur);
	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

/// Perform the cooking proficiency.
/// @param ch: The character who is attempting to cook.
/// @param argument: The food to cook.
void prof_cook(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *cooked;

	if (argument[0] == '\0' || !(cooked = get_obj_carry(ch, argument, ch)))
	{
		send_to_char("Syntax: cook <food>", ch);
		return;
	}
}

/// Perform the appraising proficiency.
/// @param ch: The character who is attempting to appraise.
/// @param argument: The object to appraise.
void prof_appraise(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *obj;
	if (argument[0] == '\0' || !(obj = get_obj_carry(ch, argument, ch)))
	{
		send_to_char("You aren't carrying that.\n\r", ch);
		return;
	}

	float mcost = (10 - ch->Profs()->GetProf("appraising")) * number_percent() / 1200;
	float tcost = number_percent() > 50
		? obj->cost + (mcost * obj->cost)
		: std::max((float)0, obj->cost - (mcost * obj->cost));

	auto buffer = fmt::format("You estimate the value of $p to be approximately {} gold.", (int)tcost);
	act(buffer.c_str(), ch, obj, 0, TO_CHAR);
	ch->Profs()->CheckImprove("appraising", 80);

	WAIT_STATE(ch, PULSE_VIOLENCE);
}

/// Perform the butchering proficiency.
/// @param ch: The character who is attempting to butcher.
/// @param argument: The object to butcher.
void prof_butcher(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *obj;

	if (argument[0] == '\0' || !(obj = get_obj_here(ch, argument)))
	{
		send_to_char("You don't see that here.\n\r",ch);
		return;
	}

	if (obj->item_type != ITEM_CORPSE_NPC)
	{
		send_to_char("You can't butcher that.\n\r", ch);
		return;
	}

	if (obj->contains)
	{
		send_to_char("That corpse is too full to be gutted.\n\r", ch);
		return;
	}

	act("$n butchers $p, carefully gutting it, extracting the edible portions and slicing them into rough chunks.", ch, obj, 0, TO_ROOM);
	act("You butcher $p, carefully gutting it, extracting the edible portions and slicing them into rough chunks.", ch, obj, 0, TO_CHAR);

	extract_obj(obj);

	int nummeat = std::max(1, (ch->Profs()->GetProf("butchery") + 2) / 3);
	act("After the butchering process, you are left with $i pieces of meat.", ch, &nummeat, 0, TO_CHAR);

	for (auto i = 0; i < nummeat; i++)
	{
		auto meat = create_object(get_obj_index(OBJ_VNUM_MEAT_CHUNKS), ch->level);
		obj_to_char(meat, ch);
		meat->value[0] += ch->Profs()->GetProf("butchery");
		meat->value[1] += ch->Profs()->GetProf("butchery");
		meat->timer = 12 + ch->Profs()->GetProf("butchery") * 2;
	}

	ch->Profs()->CheckImprove("butchery", 100);

	WAIT_STATE(ch, PULSE_VIOLENCE);
}

/// Perform the bandage proficiency.
/// @param ch: The character who is attempting to bandage.
/// @param argument: The character to bandage.
void prof_bandage(CHAR_DATA *ch, char *argument)
{
	if (ch == nullptr)
	{
		RS.Bug("prof_bandage: ch is nullptr");
		return;
	}

	auto victim = argument == nullptr || argument[0] == '\0'
		? get_char_room(ch, "self")
		: get_char_room(ch, argument);

	if (!victim)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	auto baf = affect_find(victim->affected, gsn_bleeding);
	if (!baf)
	{
		send_to_char("You can only bandage wounds that are bleeding.\n\r", ch);
		return;
	}

	if (is_affected_prof(ch, "bandage"))
	{
		send_to_char("That wound has been bandaged too recently.\n\r", ch);
		return;
	}

	//if(baf->level)
	//	baf->level /= 2;

	add_prof_affect(ch, "bandage", 2, false);

	if (victim == ch)
	{
		send_to_char("You bandage your wounds, staunching the worst of the bleeding.\n\r", ch);
		act("$n bandages $s wounds, staunching the worst of $s bleeding.", ch, 0, 0, TO_ROOM);
	}
	else
	{
		act("You bandage $N's wounds, staunching the worst of $S bleeding.", ch, 0, victim, TO_CHAR);
		act("$n bandages your wounds, staunching the worst of the bleeding.", ch, 0, victim, TO_VICT);
		act("$n bandages $N's wounds, staunching the worst of $S bleeding.", ch, 0, victim, TO_NOTVICT);
	}

	float hadd = (std::max(ch->Profs()->GetProf("bandaging") * 0.4, (double)1) * victim->level) + victim->hit;
	victim->hit = std::min((float)victim->max_hit, hadd);
	ch->Profs()->CheckImprove("bandaging", 1000);

	WAIT_STATE(ch, PULSE_VIOLENCE);
}
