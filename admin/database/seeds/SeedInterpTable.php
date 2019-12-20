<?php

use Illuminate\Database\Seeder;

class SeedInterpTable extends Seeder
{
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run()
	{
		DB::database('rift_core')
			->table('interp_table')
			->insert([
				[
					'command_name' => 'north',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_north',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 2
				],
				[
					'command_name' => 'east',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_east',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 2
				],
				[
					'command_name' => 'south',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_south',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 2
				],
				[
					'command_name' => 'west',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_west',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 2
				],
				[
					'command_name' => 'up',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_up',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 2
				],
				[
					'command_name' => 'down',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_down',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 2
				],
				[
					'command_name' => 'affects',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_affects',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'at',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_at',
					'minimum_level' => 54,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'cast',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_cast',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'call',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_call',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'commune',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_commune',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'rest',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_rest',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'rune',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_rune',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'raffects',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_raffects',
					'minimum_level' => 54,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'buy',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_buy',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'channels',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_channels',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'exits',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_exits',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'get',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_get',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'goto',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_goto',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'group',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_group',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'balance',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_balance',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'deposit',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_deposit',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'ratings',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_ratings',
					'minimum_level' => 55,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'hit',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_hit',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'look',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_look',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'lore',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_lore',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'cleader',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_leader',
					'minimum_level' => 58,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'inventory',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_inventory',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'induct',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_induct',
					'minimum_level' => 0,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'kill',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_kill',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'cb',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_cb',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => '/',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_recall',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'order',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_order',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'practice',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_practice',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'pray',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_pray',
					'minimum_level' => 0,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'sit',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_sit',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'sockets',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_sockets',
					'minimum_level' => 54,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'extitle',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_extitle',
					'minimum_level' => 55,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'multicheck',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_multicheck',
					'minimum_level' => 54,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'stand',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_stand',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'tell',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_tell',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'unlock',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_unlock',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'wield',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_wear',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'withdraw',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_withdraw',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'wizhelp',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_wizhelp',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'request',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_request',
					'minimum_level' => 0,
					'log_level' => 1,
					'visibility' => 1
				],
				[
					'command_name' => 'demand',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_demand',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'areas',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_areas',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'bug',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_bug',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'changes',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_changes',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'commands',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_commands',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'compare',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_compare',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'consider',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_consider',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'count',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_count',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'creds',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_credits',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'credits',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_diku',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'equipment',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_equipment',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'examine',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_examine',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'glance',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_glance',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'oldhelp',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_oldhelp',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'help',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_help',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'idea',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_idea',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'info',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_groups',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'motd',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_motd',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'news',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_news',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'powers',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_powers',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'supplications',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_supps',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'read',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_look',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'report',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_report',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'rules',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_rules',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'score',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_score',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'skills',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_skills',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'socials',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_socials',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'show',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_show',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'spells',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_spells',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'story',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_story',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'time',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_time',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'typo',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_typo',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'vote',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_vote',
					'minimum_level' => 0,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'weather',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_weather',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'who',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_who',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'whois',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_whois',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'wizlist',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_wizlist',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'worth',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_worth',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'alias',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_alias',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'autolist',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_autolist',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'autoabort',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_autoabort',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'autoassist',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_autoassist',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'autoexit',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_autoexit',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'autogold',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_autogold',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'autoloot',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_autoloot',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'autosac',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_autosac',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'autosplit',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_autosplit',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'brief',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_brief',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'chess',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_chess',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'color',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_color',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'combine',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_combine',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'compact',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_compact',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'description',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_description',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'delete',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_delete',
					'minimum_level' => 0,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'nofollow',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_nofollow',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'nosummon',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_nosummon',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'outfit',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_outfit',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'password',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_password',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 0
				],
				[
					'command_name' => 'prompt',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_prompt',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'role',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_role',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'scroll',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_scroll',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'specialize',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_specialize',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'title',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_title',
					'minimum_level' => 55,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'unalias',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_unalias',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'wimpy',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_wimpy',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'deaf',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_deaf',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'emote',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_emote',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'pmote',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_pmote',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => ',',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_emote',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'gtell',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_gtell',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => ';',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_gtell',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'builder',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_builder',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => '.',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_builder',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'immtalk',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_immtalk',
					'minimum_level' => 53,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'note',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_note',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'noreply',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_noreply',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'newbie',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_newbie',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'reply',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_reply',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'replay',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_replay',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'say',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_say',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => '\'',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_say',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'sing',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_sing',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'whisper',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_whisper',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => '[',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_whisper',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'unread',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_unread',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'yell',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_yell',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'brandish',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_brandish',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'close',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_close',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'drink',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_drink',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'drop',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_drop',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'embalm',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_embalm',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'eat',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_eat',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'envenom',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_envenom',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'fill',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_fill',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'give',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_give',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'heal',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_heal',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'hold',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_wear',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'list',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_list',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'lock',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_lock',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'open',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_open',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'pick',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_pick',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'pour',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_pour',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'put',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_put',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'quaff',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_quaff',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'quiet',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_quiet',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'recite',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_recite',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'remove',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_remove',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'roll',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_roll',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'doorbash',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_door_bash',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'headbutt',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_headbutt',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'silentmovement',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_silent_movement',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'gore',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_gore',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'sell',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_sell',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'take',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_get',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'sacrifice',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_sacrifice',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'junk',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_sacrifice',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'butcher',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_butcher',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'skin',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_skin',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'value',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_value',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'wear',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_wear',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'listvotes',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_listvotes',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'zap',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_zap',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'vanish',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_vanish',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'aura',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_aura_of_sustenance',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'wake',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_wake',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'style',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_style',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'cleave',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_cleave',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'herb',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_herb',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'records',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_records',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'ambush',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_ambush',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'pugil',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_pugil',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'lash',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_lash',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'shieldcleave',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_shield_cleave',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'backstab',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_backstab',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'knife',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_knife',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'bash',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_bash',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'throw',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_throw',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'strangle',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_strangle',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'blindness',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_blindness_dust',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'poison',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_poison_dust',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'warcry',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_warcry',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'endure',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_endure',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'nerve',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_nerve',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'tail',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_tail',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'blackjack',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_blackjack',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'berserk',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_berserk',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'circle',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_circle_stab',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'darksight',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_darksight',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'detect',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_detect_hidden',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'dirt',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_dirt',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'disarm',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_disarm',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'disengage',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_disengage',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'flee',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_flee',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'kick',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_kick',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'murder',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_murder',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'rescue',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_rescue',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'trip',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_trip',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'defend',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_defend',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'assassinate',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_assassinate',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'forage',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_forage',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'protection',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_protection_heat_cold',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'intimidate',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_intimidate',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'bind',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_bind',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'unbind',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_unbind',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'charge',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_charge',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'offhand',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_offhand',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'enter',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_enter',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 2
				],
				[
					'command_name' => 'storytell',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_storytell',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 2
				],
				[
					'command_name' => 'entrap',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_entrap',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'drive',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_drive',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'dash',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_dash',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'howl',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_howl',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'hobble',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_hobble',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'cripplingblow',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_crippling_blow',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'gouge',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_gouge',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'bleed',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_bleed',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'posture',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_posture',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'unbalance',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_unbalance',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'drain',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_drain',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'concuss',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_concuss',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'retreat',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_retreat',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'overhead',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_overhead',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'uppercut',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_uppercut',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'disrupt',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_disrupt',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'dart',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_dart',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'hurl',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_hurl',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'extract',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_extract',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'exchange',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_exchange',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'shieldbash',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_shieldbash',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'brace',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_brace',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'shatter',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_shatter',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'whirlwind',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_whirlwind',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'leadership',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_leadership',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'uncoil',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_uncoil',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'entwine',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_entwine',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'pull',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_pull',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'assess',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_assess',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'exploit',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_exploit',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'outflank',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_outflank',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'impale',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_impale',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'valiantcharge',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_valiant_charge',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'turnundead',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_turn_undead',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'strikeofvirtue',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_strike_of_virtue',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'groupretreat',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_group_retreat',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'flip',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_flip',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'follow',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_follow',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'creep',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_creep',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'scan',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_scan',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'gain',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_gain',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'groups',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_groups',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'acute',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_acute_vision',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'barkskin',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_barkskin',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'animal',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_animal_call',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'enlist',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_enlist',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'camouflage',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_camouflage',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'camp',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_camp',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'hide',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_hide',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'laying',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_laying_hands',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'quit',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_quit',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'release',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_release',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'recall',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_recall',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'tame',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_tame',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'find water',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_find_water',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'fw',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_fw',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 3
				],
				[
					'command_name' => 'rent',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_rent',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'save',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_save',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'sleep',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_sleep',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'sneak',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_sneak',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'split',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_split',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'steal',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_steal',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 0
				],
				[
					'command_name' => 'plant',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_plant',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 0
				],
				[
					'command_name' => 'gag',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_gag',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 0
				],
				[
					'command_name' => 'ungag',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_ungag',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 0
				],
				[
					'command_name' => 'drag',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_drag',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 0
				],
				[
					'command_name' => 'sign',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_sign',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 0
				],
				[
					'command_name' => 'slash',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_slash',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 0
				],
				[
					'command_name' => 'tripwire',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_tripwire',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 0
				],
				[
					'command_name' => 'stash',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_stash',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 0
				],
				[
					'command_name' => 'disguise',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_disguise',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 1
				],
				[
					'command_name' => 'undisguise',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_undisguise',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 1
				],
				[
					'command_name' => 'shadowcloak',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_shadow_cloak',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 1
				],
				[
					'command_name' => 'search',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_search',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 0
				],
				[
					'command_name' => 'counterfeit',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_counterfeit',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 0
				],
				[
					'command_name' => 'strip',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_strip',
					'minimum_level' => 0,
					'log_level' => 2,
					'visibility' => 0
				],
				[
					'command_name' => 'train',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_train',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'visible',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_visible',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'where',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_where',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'cloak',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_cloak',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'hometown',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_hometown',
					'minimum_level' => 0,
					'log_level' => 1,
					'visibility' => 1
				],
				[
					'command_name' => 'invoke',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_invoke',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'snare',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_snare',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'bearcall',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_bear_call',
					'minimum_level' => 0,
					'log_level' => 1,
					'visibility' => 2
				],
				[
					'command_name' => 'bounty',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_bounty',
					'minimum_level' => 0,
					'log_level' => 1,
					'visibility' => 2
				],
				[
					'command_name' => 'topbounties',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_topbounties',
					'minimum_level' => 55,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'offer',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_offer',
					'minimum_level' => 0,
					'log_level' => 1,
					'visibility' => 2
				],
				[
					'command_name' => 'listoffer',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_listoffer',
					'minimum_level' => 53,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'taunt',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_taunt',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'consume',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_consume',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'breathofmephisto',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_breath_mephisto',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'touch',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_touch',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'exile',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_exile',
					'minimum_level' => 0,
					'log_level' => 1,
					'visibility' => 1
				],
				[
					'command_name' => 'trustgroup',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_trustgroup',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'trustcabal',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_trustcabal',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'trustall',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_trustall',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'advance',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_advance',
					'minimum_level' => 60,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'alist',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_alist',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'dump',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_dump',
					'minimum_level' => 60,
					'log_level' => 1,
					'visibility' => 2
				],
				[
					'command_name' => 'immtrust',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_immtrust',
					'minimum_level' => 60,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'violate',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_violate',
					'minimum_level' => 60,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'allow',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_allow',
					'minimum_level' => 58,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'ban',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_ban',
					'minimum_level' => 58,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'rinfo',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_rinfo',
					'minimum_level' => 54,
					'log_level' => 2,
					'visibility' => 1
				],
				[
					'command_name' => 'deny',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_deny',
					'minimum_level' => 58,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'disconn',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_disconnect',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'flag',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_flag',
					'minimum_level' => 57,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'freeze',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_freeze',
					'minimum_level' => 57,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'lag',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_lag',
					'minimum_level' => 60,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'global',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_global',
					'minimum_level' => 54,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'smite',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_smite',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'permban',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_permban',
					'minimum_level' => 59,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'iprotect',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_protect',
					'minimum_level' => 59,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'reboot',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_reboot',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'set',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_set',
					'minimum_level' => 58,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'wizlock',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_wizlock',
					'minimum_level' => 58,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'classes',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_classes',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'force',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_force',
					'minimum_level' => 53,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'load',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_load',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'newlock',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_newlock',
					'minimum_level' => 58,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'nochann',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_nochannels',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'noemote',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_noemote',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'noshout',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_noshout',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'notell',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_notell',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'pecho',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_pecho',
					'minimum_level' => 53,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'pardon',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_pardon',
					'minimum_level' => 57,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'purge',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_purge',
					'minimum_level' => 56,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'restore',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_restore',
					'minimum_level' => 57,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'slay',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_slay',
					'minimum_level' => 57,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'teleport',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_transfer',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'transfer',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_transfer',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'poofin',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_bamfin',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'addapply',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_addapply',
					'minimum_level' => 58,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'maxlimits',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_max_limits',
					'minimum_level' => 57,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'poofout',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_bamfout',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'freset',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_force_reset',
					'minimum_level' => 51,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'gecho',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_gecho',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'astrip',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_astrip',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'tattoo',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_brands',
					'minimum_level' => 53,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'untattoo',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_unbrands',
					'minimum_level' => 53,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'holylight',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_holylight',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'incognito',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_incognito',
					'minimum_level' => 54,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'invis',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_invis',
					'minimum_level' => 53,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'log',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_log',
					'minimum_level' => 59,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'memory',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_memory',
					'minimum_level' => 55,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'mwhere',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_mwhere',
					'minimum_level' => 55,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'owhere',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_owhere',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'peace',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_peace',
					'minimum_level' => 54,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'praclist',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_praclist',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'ghost',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_ghost',
					'minimum_level' => 54,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'echo',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_recho',
					'minimum_level' => 54,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'return',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_return',
					'minimum_level' => 53,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'snoop',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_snoop',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'stat',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_stat',
					'minimum_level' => 53,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'string',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_string',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'switch',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_switch',
					'minimum_level' => 53,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'wizinvis',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_invis',
					'minimum_level' => 54,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'vnum',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_vnum',
					'minimum_level' => 53,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'vstat',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_vstat',
					'minimum_level' => 53,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'zecho',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_zecho',
					'minimum_level' => 54,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'clone',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_clone',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'wiznet',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_wiznet',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'imotd',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_imotd',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => ':',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_immtalk',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 2
				],
				[
					'command_name' => 'rename',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_rename',
					'minimum_level' => 57,
					'log_level' => 1,
					'visibility' => 2
				],
				[
					'command_name' => 'smote',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_smote',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'prefix',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_prefix',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'bandage',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_bandage',
					'minimum_level' => 0,
					'log_level' => 0,
					'visibility' => 1
				],
				[
					'command_name' => 'xlook',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_xlook',
					'minimum_level' => 53,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'affrem',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_affrem',
					'minimum_level' => 58,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'divine',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_divine_intervention',
					'minimum_level' => 58,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'ccb',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_ccb',
					'minimum_level' => 53,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'heroimm',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_heroimm',
					'minimum_level' => 58,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'history',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_history',
					'minimum_level' => 53,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'finger',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_finger',
					'minimum_level' => 57,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'otrack',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_otrack',
					'minimum_level' => 54,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'ltrack',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_ltrack',
					'minimum_level' => 60,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'sitetrack',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_sitetrack',
					'minimum_level' => 60,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'afk',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_afk',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'affto',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_affto',
					'minimum_level' => 58,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'crumble',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_crumble',
					'minimum_level' => 53,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'empower',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_empower',
					'minimum_level' => 53,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'pktrack',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_pktrack',
					'minimum_level' => 53,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'ctrack',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_ctrack',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'pload',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_pload',
					'minimum_level' => 60,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'listhelp',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_listhelp',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'addhelp',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_addhelp',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'delhelp',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_delhelp',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'modhelp',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_modhelp',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'damage',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_damage',
					'minimum_level' => 57,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'forcetick',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_forcetick',
					'minimum_level' => 60,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'demograph',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_demo',
					'minimum_level' => 53,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'deathmsg',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_deathmessage',
					'minimum_level' => 52,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'rastrip',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_rastrip',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'oastrip',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_oastrip',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'aastrip',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_aastrip',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'damdice',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_damdice',
					'minimum_level' => 52,
					'log_level' => 2,
					'visibility' => 0
				],
				[
					'command_name' => 'rngtest',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_rngtest',
					'minimum_level' => 52,
					'log_level' => 2,
					'visibility' => 0
				],
				[
					'command_name' => 'clean',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_clean',
					'minimum_level' => 60,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'gold',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_gold',
					'minimum_level' => 54,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'favor',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_devilfavor',
					'minimum_level' => 56,
					'log_level' => 1,
					'visibility' => 2
				],
				[
					'command_name' => 'clearfavors',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_clearfavors',
					'minimum_level' => 58,
					'log_level' => 1,
					'visibility' => 2
				],
				[
					'command_name' => 'gsnlist',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_gsnlist',
					'minimum_level' => 58,
					'log_level' => 1,
					'visibility' => 2
				],
				[
					'command_name' => 'clearquests',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_clearquests',
					'minimum_level' => 58,
					'log_level' => 1,
					'visibility' => 2
				],
				[
					'command_name' => 'ccl',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_ccl',
					'minimum_level' => 58,
					'log_level' => 2,
					'visibility' => 2
				],
				[
					'command_name' => 'antiidle',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_antiidle',
					'minimum_level' => 52,
					'log_level' => 2,
					'visibility' => 2
				],
				[
					'command_name' => 'noskills',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_noskills',
					'minimum_level' => 53,
					'log_level' => 2,
					'visibility' => 2
				],
				[
					'command_name' => 'givexp',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_givexp',
					'minimum_level' => 55,
					'log_level' => 1,
					'visibility' => 2
				],
				[
					'command_name' => 'buglist',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_buglist',
					'minimum_level' => 52,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'questmod',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_memtest',
					'minimum_level' => 60,
					'log_level' => 2,
					'visibility' => 0
				],
				[
					'command_name' => 'constdump',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_constdump',
					'minimum_level' => 59,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'setpath',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_setpath',
					'minimum_level' => 53,
					'log_level' => 1,
					'visibility' => 0
				],
				[
					'command_name' => 'racedump',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_racedump',
					'minimum_level' => 58,
					'log_level' => 0,
					'visibility' => 0
				],
				[
					'command_name' => 'interpdump',
					'immediate_eval' => 0,
					'do_fun_name' => 'do_interpdump',
					'minimum_level' => 58,
					'log_level' => 0,
					'visibility' => 0
				]
			]);
	}
}
