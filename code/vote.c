#include "vote.h"
#include "rift.h"
#include "db.h"
#include "comm.h"
#include "lookup.h"
#include "interp.h"
#include "devextra.h"
#include "newmem.h"
#include "utility.h"
#include "./include/fmt/format.h"
#include "./include/fmt/printf.h"

bool sort_votes(char *hold[], int *holdi, int cabal)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char buf[MSL];
	int j, m, i;
	bool added = false;
	bool retVal = true;
	char *temp;
	int tempi;

	auto hold_size = 200; //std::size(hold);
	auto holdi_size = 200; //std::size(holdi);
	// TODO: make hold and holdi into a std::map<char*, int>
	// NOTE: calculations assume both arrays are the same length
	if(hold_size != holdi_size)
		return false;

	sprintf(buf, "SELECT vote_for FROM votes WHERE cabal=%d", cabal);

	res = one_query_res(buf);
	i = mysql_num_rows(res);

	if (i > 0)
	{
		while ((row = mysql_fetch_row(res)) != nullptr)
		{
			added = false;

			for (j = 0; j < hold_size; j++)
			{
				if (hold[j] == nullptr)
					break;

				if (!str_cmp(hold[j], row[0]))
				{
					added = true;
					holdi[j]++;
					break;
				}
			}

			if (!added)
			{
				hold[j] = row[0];
				holdi[j] = 1;
			}
		}

		for (j = 0; j < hold_size ; j++)
		{
			for (m = 0; m < hold_size - j; m++)
			{
				if (!holdi[m] || hold[m] == nullptr)
					continue;

				if (holdi[m] < holdi[m + 1])
				{
					tempi = holdi[m];
					holdi[m] = holdi[m + 1];
					holdi[m + 1] = tempi;
					temp = hold[m];
					hold[m] = hold[m + 1];
					hold[m + 1] = temp;
				}
			}
		}
	}
	else
	{
		retVal = false;
	}

	mysql_free_result(res);
	return retVal;
}

void do_listvotes(CHAR_DATA *ch, char *argument)
{
	char buf[MSL], arg1[MSL], time[MSL], arg2[MSL];
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *hold[200];
	int holdi[200];
	int count = 0, cabal, i, j;
	time_t test;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (str_cmp(arg2, "sort"))
	{
		if (arg1[0] == '\0' || (cabal = cabal_lookup(arg1)) == CABAL_NONE)
		{
			send_to_char("Syntax:  listvotes <cabal> [<sort>]\n\r", ch);
			return;
		}

		sprintf(buf, "SELECT * FROM votes WHERE cabal=%d", cabal);
		res = one_query_res(buf);
		i = mysql_num_rows(res);

		if (i > 0)
		{
			count = 0;

			while ((row = mysql_fetch_row(res)) != nullptr)
			{
				count++;
				test = (time_t)atol(row[3]);

				strftime(time, 200, "%m-%d-%Y %H:%M:%S", localtime(&test));
				auto buffer = fmt::format("\t{}: {} votes for {} (at {} from {})\n\r", std::to_string(count), row[0], row[1], time, row[4]); //TODO: change the rest of the sprintf calls to format
				send_to_char(buffer.c_str(), ch);
			}
		}
		else
		{
			send_to_char("No valid votes to list.\n\r", ch);
			mysql_free_result(res);
		}
	}
	else
	{
		// TODO: make hold and holdi into a std::map<char*, int>
		// NOTE: hold and holdi are the same size.
		auto hold_size = std::size(hold);
		for (j = 0; j < hold_size; j++)
		{
			hold[j] = nullptr;
			holdi[j] = 0;
		}

		if (arg1[0] == '\0' || (cabal = cabal_lookup(arg1)) == CABAL_NONE)
		{
			send_to_char("Syntax:  listvotes <cabal> [<sort>]\n\r", ch);
			return;
		}

		if (!sort_votes(hold, holdi, cabal))
		{
			send_to_char("No valid votes to list.\n\r", ch);
			return;
		}

		if (cabal == CABAL_GUILD)
			send_to_char("Common Guild voting polls:\n\r", ch);

		// NOTE: hold and holdi are the same size.
		for (j = 0; j < hold_size; j++)
		{
			if (hold[j] == nullptr)
				continue;

			count++;
			sprintf(buf, "\t%d: %s has %d votes.\n\r", count, hold[j], holdi[j]);
			send_to_char(buf, ch);
		}
	}
}

void do_vote(CHAR_DATA *ch, char *argument)
{
	char arg1[MSL], buf[MSL], *word;
	char *escape;
	int cabal = 0, cvote = 0;
	FILE *fp;
	CHAR_DATA *victim;
	MYSQL_RES *res;
	MYSQL_ROW row;
	bool end= false, fMatch= false;

	if (is_npc(ch))
		return;

	if (!str_cmp(argument, ""))
	{
		send_to_char("vote <person>:  Place your vote for the selected player, who does not have to be playing.\n\r", ch);
		return;
	}

	argument = one_argument(argument, arg1);

	arg1[0] = UPPER(arg1[0]);

	auto dir = fmt::format("{}/{}{}", RIFT_PLAYER_DIR, arg1, ".plr"); //TODO: change the rest of the sprintf calls to format

	victim = get_char_world(ch, arg1);

	if (victim != nullptr)
	{
		cabal = victim->cabal;
	}
	else
	{
		fp = fopen(dir.c_str(), "r");

		if (fp == nullptr)
		{
			send_to_char("There is no such person.\n\r", ch);
			return;
		}
		else
		{
			for (;;)
			{
				word = feof(fp) ? (char *)"End" : fread_word(fp);
				fMatch= false;

				switch (UPPER(word[0]))
				{
					case '*':
						fMatch = true;
						fread_to_eol(fp);
						break;
					case 'C':
						KEY("Cabal", cabal, cabal_lookup(fread_string(fp)))
						break;
					case 'E':
						if (!str_cmp(word, "End"))
							end = true;

						break;
				}

				if (!fMatch)
					fread_to_eol(fp);

				if (end)
					break;
			}
		}

		fclose(fp);
	}

	if (!ch->cabal || ch->cabal != CABAL_GUILD)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (cabal != ch->cabal)
	{
		send_to_char("You can only vote for members of The Common Guild.\n\r", ch);
		return;
	}

	sprintf(buf, "SELECT COUNT(voter) FROM votes WHERE voter='%s'", ch->true_name);
	res = one_query_res(buf);
	row = mysql_fetch_row(res);

	cvote = atoi(row[0]);
	escape = ch->pcdata->host
		? escape_string(ch->pcdata->host)
		: escape_string(ch->desc->host);

	if (cvote > 0)
	{
		mysql_free_result(res);
		sprintf(buf, "SELECT vote_for FROM votes WHERE voter='%s'", ch->true_name);
		res = one_query_res(buf);

		if (res)
		{
			row = mysql_fetch_row(res);
			sprintf(buf, "You have already voted for %s during this election.\n\r", row[0]);
			send_to_char(buf, ch);
		}
		else
		{
			mysql_free_result(res);
			return;
		}
	}
	else
	{
		auto buffer = fmt::sprintf("INSERT INTO votes VALUES('%s','%s', %d, %ld, '%s')", ch->true_name, arg1, ch->cabal, current_time, escape); //TODO: change the rest of the sprintf calls to format
		one_query(buffer.data());

		buffer= fmt::format("You have placed your vote for {}.\n\r", (!str_cmp(arg1, ch->true_name)) ? "yourself" : arg1);
		send_to_char(buffer.c_str(), ch);
	}

	mysql_free_result(res);
}
