#include "../code/merc.h"
#include "../code/prof.h"

void TestHelperSetupPlayerBuffer(CHAR_DATA *player, char *name = "player1")
{
	player->name = name;
	player->pcdata = new pc_data();
	player->desc = new descriptor_data();
	player->desc->outbuf = new char[2];
	player->desc->outtop = 0;
	player->desc->fcommand = false;
	player->desc->outsize = 2;
}

void TestHelperSetupTrainer(CHAR_DATA *trainer, char *name = "trainer1")
{
	trainer->name = name;
	trainer->pIndexData = new mob_index_data();
	memset(trainer->pIndexData->profs_taught, -1, sizeof(sh_int) * MAX_PROFS_TAUGHT_BY_MOB);
}

void TestHelperCleanupPlayerObject(CHAR_DATA *player)
{
	if (player == nullptr)
		return;

	if (player->desc != nullptr)
	{
		if (player->desc->outbuf != nullptr)
			delete[] player->desc->outbuf;
		
		delete player->desc;
	}

	if(player->pIndexData != nullptr)
	{
		delete player->pIndexData;
	}

	if (player->pcdata != nullptr)
		delete player->pcdata;

	if (player->in_room != nullptr)
		delete player->in_room;

	delete player;
}