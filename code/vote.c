#include <utility>
#include "merc.h"
#include "vote.h"
#include "rift.h"
#include "db.h"
#include "comm.h"
#include "lookup.h"
#include "interp.h"
#include "devextra.h"
#include "newmem.h"
#include "utility.h"
#include "./repositories/voterepository.h"
#include "./include/spdlog/fmt/bundled/format.h"
#include "./include/spdlog/fmt/bundled/printf.h"

bool sort_votes(std::string hold[], int *holdi, int cabal)
{
	int j, m;
	bool added = false;
	int tempi;

	auto hold_size = 200; //std::size(hold);
	auto holdi_size = 200; //std::size(holdi);
	// TODO: make hold and holdi into a std::map<std::string, int>
	// NOTE: calculations assume both arrays are the same length
	if(hold_size != holdi_size)
		return false;

	auto votes = VoteRepository(RS.DbRift);
	auto rows = votes.FindByCabal(cabal);

	if (rows.empty())
		return false;

	for (const auto &vote : rows)
	{
		added = false;

		for (j = 0; j < hold_size; j++)
		{
			if (hold[j].empty())
				break;

			if (!str_cmp(hold[j].c_str(), vote.vote_for.c_str()))
			{
				added = true;
				holdi[j]++;
				break;
			}
		}

		if (!added)
		{
			hold[j] = vote.vote_for;
			holdi[j] = 1;
		}
	}

	// TODO: pre-existing off-by-one: the inner bound should be `m < hold_size - j - 1`.
	// As written, j==0 lets m reach hold_size-1, so `hold[m + 1]`/`holdi[m + 1]` below
	// touch index hold_size (one past the end). Guarded-unreachable in practice -- the
	// `!holdi[m]` check continues first unless all 200 slots hold distinct candidates,
	// which a cabal poll never produces -- so left as-is here. Fix belongs with the
	// std::map refactor the TODO above calls for, not this DB migration.
	for (j = 0; j < hold_size ; j++)
	{
		for (m = 0; m < hold_size - j; m++)
		{
			if (!holdi[m] || hold[m].empty())
				continue;

			if (holdi[m] < holdi[m + 1])
			{
				tempi = holdi[m];
				holdi[m] = holdi[m + 1];
				holdi[m + 1] = tempi;
				std::swap(hold[m], hold[m + 1]);
			}
		}
	}

	return true;
}

void do_listvotes(CHAR_DATA *ch, char *argument)
{
	char buf[MSL], arg1[MSL], time[MSL], arg2[MSL];
	std::string hold[200];
	int holdi[200];
	int count = 0, cabal, j;
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

		auto votes = VoteRepository(RS.DbRift);
		auto rows = votes.FindByCabal(cabal);

		if (!rows.empty())
		{
			count = 0;

			for (const auto &vote : rows)
			{
				count++;
				test = (time_t)vote.time;

				strftime(time, 200, "%m-%d-%Y %H:%M:%S", localtime(&test));
				auto buffer = fmt::format("\t{}: {} votes for {} (at {} from {})\n\r", std::to_string(count), vote.voter, vote.vote_for, time, vote.host); //TODO: change the rest of the sprintf calls to format
				send_to_char(buffer.c_str(), ch);
			}
		}
		else
		{
			send_to_char("No valid votes to list.\n\r", ch);
		}
	}
	else
	{
		// TODO: make hold and holdi into a std::map<char*, int>
		// NOTE: hold and holdi are the same size.
		auto hold_size = std::size(hold);
		for (j = 0; j < hold_size; j++)
		{
			hold[j].clear();
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
			if (hold[j].empty())
				continue;

			count++;
			sprintf(buf, "\t%d: %s has %d votes.\n\r", count, hold[j].c_str(), holdi[j]);
			send_to_char(buf, ch);
		}
	}
}

void do_vote(CHAR_DATA *ch, char *argument)
{
	char arg1[MSL], *word;
	int cabal = 0;
	FILE *fp;
	CHAR_DATA *victim;
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

	auto votes = VoteRepository(RS.DbRift);
	auto existing = votes.FindByVoter(ch->true_name);

	if (!existing.empty())
	{
		auto buffer = fmt::format("You have already voted for {} during this election.\n\r", existing.front().vote_for);
		send_to_char(buffer.c_str(), ch);
	}
	else
	{
		Vote vote;
		vote.voter = ch->true_name;
		vote.vote_for = arg1;
		vote.cabal = ch->cabal;
		vote.time = current_time;
		vote.host = ch->pcdata->host ? ch->pcdata->host : ch->desc->host;

		votes.Add(vote);

		auto buffer = fmt::format("You have placed your vote for {}.\n\r", (!str_cmp(arg1, ch->true_name)) ? "yourself" : arg1);
		send_to_char(buffer.c_str(), ch);
	}
}
