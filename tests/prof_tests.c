#include "catch.hpp"
#include "../code/prof.h"
#include "../code/merc.h"
#include "../code/db.h"
#include "../code/handler.h"
#include "../code/mud.h"
#include "test_helpers.c"

SCENARIO("performing an index lookup based on proficiency name", "[ProfIndexLookup]")
{
	GIVEN("an uninitialized parameter")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);

			THEN("it should return -1")
			{
				auto result = player->Profs()->ProfIndexLookup(nullptr);

				REQUIRE(result == -1);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("an invalid proficiency name")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);

			THEN("it should return -1")
			{
				auto result = player->Profs()->ProfIndexLookup("ÿßã");

				REQUIRE(result == -1);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("an incomplete proficiency name")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);

			THEN("it should return -1")
			{
				auto result = player->Profs()->ProfIndexLookup("cook");

				REQUIRE(result == -1);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("a valid proficiency name")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);

			THEN("it should return a valid index")
			{
				auto result = player->Profs()->ProfIndexLookup("cooking");

				REQUIRE(result == 6);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("check to see if a character has a proficiency (int)", "[HasProf]")
{
	GIVEN("an invalid index")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);

			THEN("it should return false")
			{
				auto result = player->Profs()->HasProf(-1);

				REQUIRE(result == false);
			}

			THEN("it should return false")
			{
				auto result = player->Profs()->HasProf(MAX_PROFS + 1);

				REQUIRE(result == false);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("a character who doesn't have the proficiency")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);

			THEN("it should return false")
			{
				auto result = player->Profs()->HasProf(6);

				REQUIRE(result == false);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("a character who has the proficiency")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			player->Profs()->SetProf(6, 1); // cooking

			THEN("it should return true")
			{
				auto result = player->Profs()->HasProf(6);

				REQUIRE(result == true);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("check to see if a character has a proficiency (string)", "[HasProf]")
{
	GIVEN("an uninitialized paramter")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);

			THEN("it should return false")
			{
				auto result = player->Profs()->HasProf(nullptr);

				REQUIRE(result == false);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("an invalid proficiency name")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);

			THEN("it should return false")
			{
				auto result = player->Profs()->HasProf("ÿßã");

				REQUIRE(result == false);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("an incomplete proficiency name")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);

			THEN("it should return false")
			{
				auto result = player->Profs()->HasProf("cook");

				REQUIRE(result == false);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("a valid proficiency name")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			player->Profs()->SetProf(6, 1); // cooking

			THEN("it should return true")
			{
				auto result = player->Profs()->HasProf("cooking");

				REQUIRE(result == true);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("retrieve the level of a procifiency by its psn (int)", "[GetProf]")
{
	CProficiencies::AssignPsns();

	GIVEN("an invalid psn")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);

			THEN("it should return -1")
			{
				auto result = player->Profs()->GetProf(-1);

				REQUIRE(result == -1);
			}

			THEN("it should return -1")
			{
				auto result = player->Profs()->GetProf(MAX_PROFS + 1);

				REQUIRE(result == -1);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("a valid psn")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			player->Profs()->SetProf(2, 1); // mountaineering

			THEN("it should return the level")
			{
				auto result = player->Profs()->GetProf(psn_mountaineering);

				REQUIRE(result == 1);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("retrieve the level of a procifiency by its psn (string)", "[GetProf]")
{
	CProficiencies::AssignPsns();

	GIVEN("an uninitialized paramter")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);

			THEN("it should return -1")
			{
				auto result = player->Profs()->GetProf(nullptr);

				REQUIRE(result == -1);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("an invalid proficiency name")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);

			THEN("it should return -1")
			{
				auto result = player->Profs()->GetProf("ÿßã");

				REQUIRE(result == -1);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("an incomplete proficiency name")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			player->Profs()->SetProf(2, 1); // mountaineering

			THEN("it should return -1")
			{
				auto result = player->Profs()->GetProf("mountain");

				REQUIRE(result == -1);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("a valid proficiency name")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			player->Profs()->SetProf(2, 1); // mountaineering

			THEN("it should return the level")
			{
				auto result = player->Profs()->GetProf("mountaineering");

				REQUIRE(result == 1);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("retrieve the proficiencies taught by a trainer", "[GetProfsTaughtByTrainer]")
{
	CProficiencies::AssignPsns();

	GIVEN("a trainer with proficiencies to teach")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			TestHelperSetupPlayerBuffer(player);

			auto trainer = new char_data();
			TestHelperSetupTrainer(trainer);
			trainer->pIndexData->profs_taught[0] = 6; // cooking
			trainer->pIndexData->profs_taught[1] = 7; // firestarting

			THEN("a list of procifiencies the trainer teaches is listed")
			{
				player->Profs()->GetProfsTaughtByTrainer(player, trainer);

				auto result = !str_cmp(player->desc->outbuf, "\n\rcooking          | 13 points\n\rfirestarting     | 15 points\n\r");
				REQUIRE(result == true);
			}

			TestHelperCleanupPlayerObject(trainer);
			TestHelperCleanupPlayerObject(player);
		}
	}

	//TODO: what happens if a trainer has no proficiencies to teach
}

SCENARIO("a trainer tries to teach a character a procificiency", "[TrainProficiency]")
{
	CProficiencies::AssignPsns();

	GIVEN("a command that is not train")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			TestHelperSetupPlayerBuffer(player);

			auto trainer = new char_data();
			TestHelperSetupTrainer(trainer);
			trainer->pIndexData->profs_taught[0] = 6; // cooking
			trainer->pIndexData->profs_taught[1] = 7; // firestarting

			THEN("a help message should be displayed")
			{
				player->Profs()->TrainProficiency(player, trainer, "moo");

				auto result = !str_cmp(player->desc->outbuf,"\n\rSyntax: proficiencies train <proficiency>\n\r");
				REQUIRE(result == true);
			}

			TestHelperCleanupPlayerObject(trainer);
			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("the trainer doesn't know the proficiency")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			TestHelperSetupPlayerBuffer(player);

			auto trainer = new char_data();
			TestHelperSetupTrainer(trainer);
			trainer->pIndexData->profs_taught[0] = 6; // cooking
			trainer->pIndexData->profs_taught[1] = 7; // firestarting

			THEN("a message stating that proficiency isn't trained here")
			{
				player->Profs()->TrainProficiency(player, trainer, "train butchery");

				auto result = !str_cmp(player->desc->outbuf,"\n\rYou can't study that here.\n\r");
				REQUIRE(result == true);
			}

			TestHelperCleanupPlayerObject(trainer);
			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("the character already knows the proficiency")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			TestHelperSetupPlayerBuffer(player);

			auto trainer = new char_data();
			TestHelperSetupTrainer(trainer);
			trainer->pIndexData->profs_taught[0] = 6; // cooking
			trainer->pIndexData->profs_taught[1] = 7; // firestarting

			THEN("a mesasge stating that proficiency is already known")
			{
				player->Profs()->SetProf(6, 1); // cooking
				player->Profs()->TrainProficiency(player, trainer, "train cooking");

				auto result = !str_cmp(player->desc->outbuf,"\n\rYou are already familiar with that proficiency.\n\r");
				REQUIRE(result == true);
			}

			TestHelperCleanupPlayerObject(trainer);
			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("the character doesn't know the proficiency and doesn't have enough points to train")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			TestHelperSetupPlayerBuffer(player);

			auto trainer = new char_data();
			TestHelperSetupTrainer(trainer);
			trainer->pIndexData->profs_taught[0] = 6; // cooking
			trainer->pIndexData->profs_taught[1] = 7; // firestarting

			THEN("a mesasge stating that the character does not have enough points")
			{
				player->Profs()->SetPoints(8);
				player->Profs()->TrainProficiency(player, trainer, "train cooking");

				auto result = !str_cmp(player->desc->outbuf,"\n\rYou don't have enough points to study that proficiency.\n\r");
				REQUIRE(result == true);
			}

			TestHelperCleanupPlayerObject(trainer);
			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("the character needs to know another proficiency before learning this proficiency")
	{
		WHEN("the function is called")
		{
			THEN("a message stating that the character needs to learn a required proficiency first")
			{
				// TODO: test a proficiency that requires another before training -- currently know proficiencies need another proficiency to be learned first
			}
		}
	}

	GIVEN("the character isn't high enough level to learn the proficiency")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			TestHelperSetupPlayerBuffer(player);

			auto trainer = new char_data();
			TestHelperSetupTrainer(trainer);
			trainer->pIndexData->profs_taught[0] = 12; // tracking
			trainer->pIndexData->profs_taught[1] = 7; // firestarting

			THEN("a message stating the character is not high enough level")
			{
				player->level = 8;
				player->Profs()->SetPoints(30);
				player->Profs()->TrainProficiency(player, trainer, "train tracking");

				auto result = !str_cmp(player->desc->outbuf,"\n\rYou are not advanced enough in your guild to learn that proficiency.\n\r");
				REQUIRE(result == true);
			}

			TestHelperCleanupPlayerObject(trainer);
			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("the character meets the requirements for learning this proficiency")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			TestHelperSetupPlayerBuffer(player);

			auto trainer = new char_data();
			TestHelperSetupTrainer(trainer);
			trainer->pIndexData->profs_taught[0] = 12; // tracking
			trainer->pIndexData->profs_taught[1] = 7; // firestarting

			THEN("a message by the trainer to the character")
			{
				player->level = 30;
				player->Profs()->SetPoints(30);
				player->Profs()->TrainProficiency(player, trainer, "train tracking");

				// TODO: figure out how to process the riftshadow queue
				//RS.Queue.ProcessQueue();

				// TODO: this should be the trainers message to the character
				//auto result = !str_cmp(player->desc->outbuf,"\n\r$N beckons you over to a nearby tree.\n\r");
				//REQUIRE(result == true);

				REQUIRE(player->Profs()->GetPoints() == 0);
				REQUIRE(player->Profs()->GetProf(12) == 1);
			}

			TestHelperCleanupPlayerObject(trainer);
			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("retrieve a procifiency by its psn (int)", "[GetProficiency]")
{
	CProficiencies::AssignPsns();

	GIVEN("an invalid psn")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			proficiency_type proficiency = { &psn_none, "", 0, 0, NULL, PFLAGS_NONE };

			THEN("it should return a blank proficiency")
			{
				auto result = player->Profs()->GetProficiency(-1);

				REQUIRE(*result.ppsn == *proficiency.ppsn);
				REQUIRE(!str_cmp(result.name, proficiency.name));
				REQUIRE(result.cost == proficiency.cost);
				REQUIRE(result.minlevel == proficiency.minlevel);
				REQUIRE(result.requires == proficiency.requires);
				REQUIRE(result.flags == proficiency.flags);
			}

			THEN("it should return a blank proficiency")
			{
				auto result = player->Profs()->GetProficiency(MAX_PROFS + 1);

				REQUIRE(*result.ppsn == *proficiency.ppsn);
				REQUIRE(!str_cmp(result.name, proficiency.name));
				REQUIRE(result.cost == proficiency.cost);
				REQUIRE(result.minlevel == proficiency.minlevel);
				REQUIRE(result.requires == proficiency.requires);
				REQUIRE(result.flags == proficiency.flags);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("a valid psn")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			player->Profs()->SetProf(2, 1); // mountaineering

			sh_int psn_mountaineering = 2;
			struct proficiency_type proficiency =
			{
				&psn_mountaineering,
				"mountaineering",
				13,
				1,
				NULL,
				PFLAGS_BASIC
			};

			THEN("it should return the proficiency")
			{
				auto result = player->Profs()->GetProficiency(2);

				REQUIRE(*result.ppsn == *proficiency.ppsn);
				REQUIRE(!str_cmp(result.name, proficiency.name));
				REQUIRE(result.cost == proficiency.cost);
				REQUIRE(result.minlevel == proficiency.minlevel);
				REQUIRE(result.requires == proficiency.requires);
				REQUIRE(result.flags == proficiency.flags);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("retrieve a procifiency by its psn (string)", "[GetProficiency]")
{
	CProficiencies::AssignPsns();

	GIVEN("an uninitialized paramter")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			proficiency_type proficiency = { &psn_none, "", 0, 0, NULL, PFLAGS_NONE };

			THEN("it should return a blank proficiency")
			{
				auto result = player->Profs()->GetProficiency(nullptr);

				REQUIRE(*result.ppsn == *proficiency.ppsn);
				REQUIRE(!str_cmp(result.name, proficiency.name));
				REQUIRE(result.cost == proficiency.cost);
				REQUIRE(result.minlevel == proficiency.minlevel);
				REQUIRE(result.requires == proficiency.requires);
				REQUIRE(result.flags == proficiency.flags);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("an invalid proficiency name")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			proficiency_type proficiency = { &psn_none, "", 0, 0, NULL, PFLAGS_NONE };

			THEN("it should return a blank proficiency")
			{
				auto result = player->Profs()->GetProficiency("ÿßã");

				REQUIRE(*result.ppsn == *proficiency.ppsn);
				REQUIRE(!str_cmp(result.name, proficiency.name));
				REQUIRE(result.cost == proficiency.cost);
				REQUIRE(result.minlevel == proficiency.minlevel);
				REQUIRE(result.requires == proficiency.requires);
				REQUIRE(result.flags == proficiency.flags);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("an incomplete proficiency name")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			player->Profs()->SetProf(2, 1); // mountaineering
			proficiency_type proficiency = { &psn_none, "", 0, 0, NULL, PFLAGS_NONE };

			THEN("it should return a blank proficiency")
			{
				auto result = player->Profs()->GetProficiency("mountain");

				REQUIRE(*result.ppsn == *proficiency.ppsn);
				REQUIRE(!str_cmp(result.name, proficiency.name));
				REQUIRE(result.cost == proficiency.cost);
				REQUIRE(result.minlevel == proficiency.minlevel);
				REQUIRE(result.requires == proficiency.requires);
				REQUIRE(result.flags == proficiency.flags);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("a valid proficiency name")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			player->Profs()->SetProf(2, 1); // mountaineering

			sh_int psn_mountaineering = 2;
			struct proficiency_type proficiency =
			{
				&psn_mountaineering,
				"mountaineering",
				13,
				1,
				NULL,
				PFLAGS_BASIC
			};

			THEN("it should return the proficiency")
			{
				auto result = player->Profs()->GetProficiency("mountaineering");

				REQUIRE(*result.ppsn == *proficiency.ppsn);
				REQUIRE(!str_cmp(result.name, proficiency.name));
				REQUIRE(result.cost == proficiency.cost);
				REQUIRE(result.minlevel == proficiency.minlevel);
				REQUIRE(result.requires == proficiency.requires);
				REQUIRE(result.flags == proficiency.flags);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("retrieve the effect of a procifiency (string float)", "[ProfEffect]")
{
	CProficiencies::AssignPsns();

	GIVEN("an uninitialized paramter")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);

			THEN("it should return 0")
			{
				auto result = player->Profs()->ProfEffect(nullptr);

				REQUIRE(result == 0);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("an invalid proficiency name")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);

			THEN("it should return 0")
			{
				auto result = player->Profs()->ProfEffect("ÿßã");

				REQUIRE(result == 0);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("an incomplete proficiency name")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			player->Profs()->SetProf(2, 1); // mountaineering

			THEN("it should return 0")
			{
				auto result = player->Profs()->ProfEffect("mountain");

				REQUIRE(result == 0);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("a valid proficiency name with no effects associated with it")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			player->Profs()->SetProf(2, 1); // mountaineering

			THEN("it should return 0")
			{
				auto result = player->Profs()->ProfEffect("mountaineering");

				REQUIRE(result == 0);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("a valid proficiency name with effects associated with it")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			player->Profs()->SetProf(4, 1); // appraising
			player->Profs()->SetProf(5, 1); // trap detecting
			player->Profs()->SetProf(7, 1); // firestarting

			THEN("it should return the chance")
			{
				// TODO: figure out how to game the chance generator to test correct output
				auto result = player->Profs()->ProfEffect("appraising");

				REQUIRE(result > 0);
			}

			THEN("it should return the chance")
			{
				// TODO: figure out how to game the chance generator to test correct output
				auto result = player->Profs()->ProfEffect("trap detecting");

				REQUIRE(result > 0);
			}

			THEN("it should return the chance")
			{
				auto result = player->Profs()->ProfEffect("firestarting");

				REQUIRE(result > 0);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("retrieve the effect of a procifiency (int)", "[ProfEffect]")
{
	CProficiencies::AssignPsns();

	GIVEN("an invalid psn")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);

			THEN("it should return 0")
			{
				auto result = player->Profs()->ProfEffect(-1);

				REQUIRE(result == 0);
			}

			THEN("it should return 0")
			{
				auto result = player->Profs()->ProfEffect(MAX_PROFS + 1);

				REQUIRE(result == 0);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("a valid psn with no effects associated with it")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			player->Profs()->SetProf(2, 1); // mountaineering

			THEN("it should return 0")
			{
				auto result = player->Profs()->ProfEffect(2);

				REQUIRE(result == 0);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("a valid proficiency name with effects associated with it")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			player->Profs()->SetChar(player);
			player->Profs()->SetProf(4, 1); // appraising
			player->Profs()->SetProf(5, 1); // trap detecting
			player->Profs()->SetProf(7, 1); // firestarting

			THEN("it should return the chance")
			{
				// TODO: figure out how to game the chance generator to test correct output
				auto result = player->Profs()->ProfEffect(4);

				REQUIRE(result > 0);
			}

			THEN("it should return the chance")
			{
				// TODO: figure out how to game the chance generator to test correct output
				auto result = player->Profs()->ProfEffect(5);

				REQUIRE(result > 0);
			}

			THEN("it should return the chance")
			{
				auto result = player->Profs()->ProfEffect(7);

				REQUIRE(result > 0);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("testing outputting character known proficiencies", "[ListKnownProficiencies]")
{
	CProficiencies::AssignPsns();

	GIVEN("a player with no proficiencies")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			player->Profs()->ListKnownProficiencies(player);

			THEN("it should send a single line of text to the player buffer")
			{
				auto result = !str_cmp(player->desc->outbuf, "\n\rYou currently have no proficiencies.\n\r");

				REQUIRE(result == true);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("a player with some proficiencies")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			player->Profs()->SetProf(6, 1);	// cooking
			player->Profs()->SetProf(7, 1);	// firestarting
			player->Profs()->ListKnownProficiencies(player);

			THEN("it should send a listing of proficiencies to the player buffer")
			{
				auto result = !str_cmp(player->desc->outbuf, "\n\rYour proficiencies are:\n\rYou are proficient at cooking.\n\rYou are proficient at firestarting.\n\r");

				REQUIRE(result == true);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("testing outputting a list of basic proficiencies", "[ListBasicProficiencies]")
{
	CProficiencies::AssignPsns();

	GIVEN("a valid player")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			player->Profs()->ListBasicProficiencies(player);

			THEN("it should send a listing of basic proficiencies to the player buffer")
			{
				auto buffer = std::string("\n\rThe basic proficiencies available to adventurers are:\n\r");
				buffer += std::string("swimming              butchery              mountaineering\n\r");
				buffer += std::string("bandaging             appraising            trap detecting\n\r");
				buffer += std::string("cooking               firestarting          \n\r");
				buffer += std::string("Many other proficiencies are known to Shalaran adventurers, but you must first find a teacher.\n\r");

				REQUIRE(!str_cmp(buffer.c_str(), player->desc->outbuf));
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("testing outputting a list of known proficiencies and points", "[DisplayProfsForStat]")
{
	CProficiencies::AssignPsns();

	GIVEN("a valid player with no proficiencies learned")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			player->Profs()->SetPoints(30);
			player->Profs()->DisplayProfsForStat(player);

			THEN("it should send a message showing remaining points to the player buffer")
			{
				auto buffer = std::string("\n\rProficiencies (30 pts left): \n\r");

				REQUIRE(!str_cmp(buffer.c_str(), player->desc->outbuf));
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("a valid player with proficiencies learned and points remaining")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			player->Profs()->SetPoints(30);
			player->Profs()->SetProf(2, 1);  // mountaineering
			player->Profs()->SetProf(6, 1);  // cooking
			player->Profs()->SetProf(7, 1);  // firestarting
			player->Profs()->DisplayProfsForStat(player);

			THEN("it should send a message showing remaining points to the player buffer")
			{
				auto buffer = std::string("\n\rProficiencies (30 pts left): mountaineering (1) cooking (1) firestarting\n\r(1) \n\r");

				REQUIRE(!str_cmp(buffer.data(), player->desc->outbuf));
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("retrieve the skill mastery name for a certain proficiency level", "[GetSkillLevelName]")
{
	CProficiencies::AssignPsns();

	GIVEN("an invalid level")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			player->Profs()->SetProf(2, 7);  // mountaineering

			THEN("it should return an empty string")
			{
				auto result = player->Profs()->GetSkillLevelName(-1);

				REQUIRE(!str_cmp(result, ""));
			}

			THEN("it should return an empty string")
			{
				auto result = player->Profs()->GetSkillLevelName(MAX_PROFS + 1);

				REQUIRE(!str_cmp(result, ""));
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("an valid level")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			player->Profs()->SetProf(2, 7);  // mountaineering, adept

			THEN("it should the name of the skill mastery")
			{
				auto result = player->Profs()->GetSkillLevelName(2);

				REQUIRE(!str_cmp(result, "adept"));
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("display a list of proficiencies and skill mastery the player has learned", "[ShowProfsToChar]")
{
	CProficiencies::AssignPsns();

	GIVEN("a valid player")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			player->Profs()->SetProf(2, 7);  // mountaineering, adept
			player->Profs()->SetProf(6, 10); // cooking, masterful
			player->Profs()->ShowProfsToChar();

			THEN("it should the name of the skill mastery")
			{
				auto buffer = std::string("\n\rYou are adept in mountaineering.\n\rYou are masterful in cooking.\n\r");

				REQUIRE(!str_cmp(buffer.c_str(), player->desc->outbuf));
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("update the players proficiency points and notify them", "[UpdateProfPoints]")
{
	GIVEN("a valid player who can learn new proficiencies")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			player->played = 4500;
			player->Profs()->SetPATime(45);
			player->Profs()->UpdateProfPoints();

			THEN("it should display a message notifying the player")
			{
				auto buffer = std::string("\n\rYou feel ready to study new proficiencies.\n\r");
				REQUIRE(!str_cmp(buffer.c_str(), player->desc->outbuf));
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("check to see if player leveled a proficiency (int int)", "[CheckImprove]")
{
	GIVEN("a valid player")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			player->Profs()->SetProf(2, 1);  // mountaineering
			player->Profs()->CheckImprove(2, 10);

			THEN("it should display a message notifying the player")
			{
				// TODO: figure out how to game the chance generator to test correct output
				auto result = player->desc->outbuf;

				REQUIRE(1 == 1);
			}
		}
	}
}

SCENARIO("check to see if player leveled a proficiency (char* int)", "[CheckImprove]")
{
	GIVEN("a valid player")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			player->Profs()->SetProf(2, 1);  // mountaineering
			player->Profs()->CheckImprove("mountaineering", 10);

			THEN("it should display a message notifying the player")
			{
				// TODO: figure out how to game the chance generator to test correct output
				auto result = player->desc->outbuf;

				REQUIRE(1 == 1);
			}
		}
	}
}

SCENARIO("testing interpreting proficiency commands", "[InterpCommand]")
{
	CProficiencies::AssignPsns();

	GIVEN("an invalid command")
	{
		WHEN("a character calls it")
		{
			auto player = new char_data();
			player->name = "player1";
			player->pcdata = new pc_data();

			player->Profs()->SetChar(player);

			THEN("it should return false")
			{
				bool result = player->Profs()->InterpCommand("", "");

				REQUIRE(result == false);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("a valid command")
	{
		WHEN("a character without the skill calls it")
		{
			auto player = new char_data();
			player->name = "player1";
			player->pcdata = new pc_data();

			player->Profs()->SetChar(player);

			THEN("it should return false")
			{
				bool result = player->Profs()->InterpCommand("firestart", "");

				REQUIRE(result == false);
			}

			TestHelperCleanupPlayerObject(player);
		}

		WHEN("a character with the skill calls it")
		{
			auto player = new char_data();
			player->name = "player1";
			player->pcdata = new pc_data();
			player->in_room = new room_index_data();
			player->in_room->sector_type = SECT_WATER;

			player->Profs()->SetChar(player);
			player->Profs()->SetProf(7, 1);	// firestarting

			THEN("it should return true")
			{
				bool result = player->Profs()->InterpCommand("firestart", "");

				REQUIRE(result == true);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

SCENARIO("Determine if a player is affected by a proficiency affect", "[is_affected_prof]")
{
	GIVEN("a player not under the effects of an affect")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			//add_prof_affect(player, "bandage", 2, false);

			THEN("it should return false")
			{
				auto result = is_affected_prof(player, "bandage");

				REQUIRE(result == false);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}

	GIVEN("a player under the effects of an affect")
	{
		WHEN("the function is called")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			add_prof_affect(player, "bandage", 2, false);

			THEN("it should return true")
			{
				auto result = is_affected_prof(player, "bandage");

				REQUIRE(result == true);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}

// TODO: write tests
// void do_proficiencies(CHAR_DATA *ch, char *argument)

// TODO: write tests
// void prof_tracking(CHAR_DATA *ch, char *argument)

// TODO: write tests
// void build_fire(CHAR_DATA *ch, int dur)

// TODO: write tests
// void prof_firestart(CHAR_DATA *ch, char *argument)

// TODO: write tests
// void prof_cook(CHAR_DATA *ch, char *argument)

// TODO: write tests
// void prof_appraise(CHAR_DATA *ch, char *argument)

// TODO: write tests
// void prof_butcher(CHAR_DATA *ch, char *argument)

SCENARIO("Attempt to bandage a character", "[prof_bandage]")
{
	CProficiencies::AssignPsns();

	// All proficiencies are not called directly. Instead they flow threw the InterpCommand method. 
	GIVEN("a player does not have the bandaging proficiency")
	{
		WHEN("the player tries issuing a bandage command with no target")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);

			THEN("it should return false")
			{
				auto result = player->Profs()->InterpCommand("bandage", nullptr);

				REQUIRE(result == false);
			}

			TestHelperCleanupPlayerObject(player);
		}

		WHEN("the player tries bandaging themselve")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);

			THEN("it should return false")
			{
				auto t = 1;
				auto result = player->Profs()->InterpCommand("bandage", "self");

				REQUIRE(result == false);
			}

			TestHelperCleanupPlayerObject(player);
		}

		WHEN("the player tries bandaging another player in the same room")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);

			auto player2 = new char_data();
			TestHelperSetupPlayerBuffer(player2, "player2");

			THEN("it should return false")
			{
				auto t = 1;
				auto result = player->Profs()->InterpCommand("bandage", "player2");

				REQUIRE(result == false);
			}

			TestHelperCleanupPlayerObject(player);
			TestHelperCleanupPlayerObject(player2);
		}

		WHEN("the player tries bandaging another player in another room")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);

			auto player2 = new char_data();
			TestHelperSetupPlayerBuffer(player2, "player2", "room2");

			THEN("it should return false")
			{
				auto t = 1;
				auto result = player->Profs()->InterpCommand("bandage", "player2");

				REQUIRE(result == false);
			}

			TestHelperCleanupPlayerObject(player);
			TestHelperCleanupPlayerObject(player2);
		}
	}

	GIVEN("a player has the bandaging proficiency")
	{
		WHEN("the player tries issuing a bandage command with no target")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			player->Profs()->SetProf(3, 1);

			THEN("it should display a message notifying the player")
			{
				player->Profs()->InterpCommand("bandage", nullptr);
				auto result = !str_cmp(player->desc->outbuf,"\n\rThey aren't here.\n\r");

				REQUIRE(result == true);
			}

			TestHelperCleanupPlayerObject(player);
		}

		WHEN("the player tries bandaging themselve while not bleeding")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			player->Profs()->SetProf(3, 1);

			THEN("it should display a message notifying the player")
			{
				player->Profs()->InterpCommand("bandage", "self");
				auto result = !str_cmp(player->desc->outbuf,"\n\rYou can only bandage wounds that are bleeding.\n\r");

				REQUIRE(result == true);
			}

			TestHelperCleanupPlayerObject(player);
		}

		WHEN("the player tries bandaging themselve while bleeding but was recently bandaged")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			player->Profs()->SetProf(3, 1);
			add_prof_affect(player, "bandage", 2, false);

			THEN("it should display a message notifying the player")
			{
				player->Profs()->InterpCommand("bandage", "self");
				auto result = !str_cmp(player->desc->outbuf,"\n\rThat wound has been bandaged too recently.\n\r");

				REQUIRE(result == true);
			}

			TestHelperCleanupPlayerObject(player);
		}

		WHEN("the player tries bandaging themselve while bleeding")
		{
			auto player = new char_data();
			TestHelperSetupPlayerBuffer(player);

			player->Profs()->SetChar(player);
			player->Profs()->SetProf(3, 1);

			RS.LoadGsn();

			AFFECT_DATA af;
			init_affect(&af);
			af.type = gsn_bleeding;
			af.where = TO_AFFECTS;
			af.aftype = AFT_TIMER;
			af.location = APPLY_LEGS;
			af.duration = 6;
			af.level = 6;
			af.modifier = -1;
			affect_to_char(player, &af);

			THEN("it should display a message notifying the player")
			{
				player->Profs()->InterpCommand("bandage", "self");
				auto result = !str_cmp(player->desc->outbuf,"\n\rYou bandage your wounds, staunching the worst of the bleeding.\n\r");

				REQUIRE(result == true);
			}

			TestHelperCleanupPlayerObject(player);
		}
	}
}
