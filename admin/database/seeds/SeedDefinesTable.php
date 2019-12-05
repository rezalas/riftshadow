<?php

use Illuminate\Database\Seeder;

class SeedDefinesTable extends Seeder
{
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run()
	{
		DB::connection('rift_core')->table('defines')->insert([
			[
				'name' => 'strength',
				'abbrev' => 'str',
				'define' => 'STAT_STR',
				'category' => 'stats',
				'value' => 0
			],
			[
				'name' => 'intelligence',
				'abbrev' => 'int',
				'define' => 'STAT_INT',
				'category' => 'stats',
				'value' => 1
			],
			[
				'name' => 'wisdom',
				'abbrev' => 'wis',
				'define' => 'STAT_WIS',
				'category' => 'stats',
				'value' => 2
			],
			[
				'name' => 'dexterity',
				'abbrev' => 'dex',
				'define' => 'STAT_DEX',
				'category' => 'stats',
				'value' => 3
			],
			[
				'name' => 'constitution',
				'abbrev' => 'con',
				'define' => 'STAT_CON',
				'category' => 'stats',
				'value' => 4
			],
			[
				'name' => 'casts',
				'abbrev' => 'casts',
				'define' => 'CLASS_CASTER',
				'category' => 'magical_class',
				'value' => 1
			],
			[
				'name' => 'communes',
				'abbrev' => 'communes',
				'define' => 'CLASS_COMMUNER',
				'category' => 'magical_class',
				'value' => 2
			],
			[
				'name' => 'none',
				'abbrev' => 'none',
				'define' => 'CLASS_NEITHER',
				'category' => 'magical_class',
				'value' => 0
			],
			[
				'name' => 'open',
				'abbrev' => 'open',
				'define' => 'RACE_OPEN',
				'category' => 'race_status',
				'value' => 1
			],
			[
				'name' => 'closed',
				'abbrev' => 'closed',
				'define' => 'RACE_CLOSED',
				'category' => 'race_status',
				'value' => 0
			],
			[
				'name' => 'any',
				'abbrev' => 'any',
				'define' => 'ALIGN_ANY',
				'category' => 'align_allowed',
				'value' => 0
			],
			[
				'name' => 'good neutral',
				'abbrev' => 'good neutral',
				'define' => 'ALIGN_GN',
				'category' => 'align_allowed',
				'value' => 1
			],
			[
				'name' => 'neutral evil',
				'abbrev' => 'neutral evil',
				'define' => 'ALIGN_NE',
				'category' => 'align_allowed',
				'value' => 2
			],
			[
				'name' => 'good evil',
				'abbrev' => 'good evil',
				'define' => 'ALIGN_GE',
				'category' => 'align_allowed',
				'value' => 3
			],
			[
				'name' => 'good',
				'abbrev' => 'good',
				'define' => 'ALIGN_G',
				'category' => 'align_allowed',
				'value' => 4
			],
			[
				'name' => 'neutral',
				'abbrev' => 'neutral',
				'define' => 'ALIGN_N',
				'category' => 'align_allowed',
				'value' => 5
			],
			[
				'name' => 'evil',
				'abbrev' => 'evil',
				'define' => 'ALIGN_E',
				'category' => 'align_allowed',
				'value' => 6
			],
			[
				'name' => 'tiny',
				'abbrev' => 'tiny',
				'define' => 'SIZE_TINY',
				'category' => 'size',
				'value' => 0
			],
			[
				'name' => 'small',
				'abbrev' => 'small',
				'define' => 'SIZE_SMALL',
				'category' => 'size',
				'value' => 1
			],
			[
				'name' => 'medium',
				'abbrev' => 'medium',
				'define' => 'SIZE_MEDIUM',
				'category' => 'size',
				'value' => 2
			],
			[
				'name' => 'large',
				'abbrev' => 'large',
				'define' => 'SIZE_LARGE',
				'category' => 'size',
				'value' => 3
			],
			[
				'name' => 'huge',
				'abbrev' => 'huge',
				'define' => 'SIZE_HUGE',
				'category' => 'size',
				'value' => 4
			],
			[
				'name' => 'giant',
				'abbrev' => 'giant',
				'define' => 'SIZE_GIANT',
				'category' => 'size',
				'value' => 5
			],
			[
				'name' => 'immense',
				'abbrev' => 'immense',
				'define' => 'SIZE_IMMENSE',
				'category' => 'size',
				'value' => 6
			],
			[
				'name' => 'open',
				'abbrev' => 'open',
				'define' => 'CLASS_OPEN',
				'category' => 'class_status',
				'value' => 1
			],
			[
				'name' => 'closed',
				'abbrev' => 'closed',
				'define' => 'CLASS_CLOSED',
				'category' => 'class_status',
				'value' => 0
			],
			[
				'name' => 'normal',
				'abbrev' => 'normal',
				'define' => 'ARE_NORMAL',
				'category' => 'area_type',
				'value' => 0
			],
			[
				'name' => 'road_river',
				'abbrev' => 'road_river',
				'define' => 'ARE_ROAD_RIVER',
				'category' => 'area_type',
				'value' => 1
			],
			[
				'name' => 'cabal',
				'abbrev' => 'cabal',
				'define' => 'ARE_CABAL',
				'category' => 'area_type',
				'value' => 2
			],
			[
				'name' => 'quest',
				'abbrev' => 'quest',
				'define' => 'ARE_QUEST',
				'category' => 'area_type',
				'value' => 3
			],
			[
				'name' => 'city',
				'abbrev' => 'city',
				'define' => 'ARE_CITY',
				'category' => 'area_type',
				'value' => 4
			],
			[
				'name' => 'unopened',
				'abbrev' => 'unopened',
				'define' => 'ARE_UNOPENED',
				'category' => 'area_type',
				'value' => 5
			],
			[
				'name' => 'shrine',
				'abbrev' => 'shrine',
				'define' => 'ARE_SHRINE',
				'category' => 'area_type',
				'value' => 6
			],
			[
				'name' => 'none',
				'abbrev' => 'none',
				'define' => 'CLIMATE_NONE',
				'category' => 'climate_type',
				'value' => 0
			],
			[
				'name' => 'temperate',
				'abbrev' => 'temperate',
				'define' => 'CLIMATE_TEMPERATE',
				'category' => 'climate_type',
				'value' => 1
			],
			[
				'name' => 'desert',
				'abbrev' => 'desert',
				'define' => 'CLIMATE_DESERT',
				'category' => 'climate_type',
				'value' => 2
			],
			[
				'name' => 'tropical',
				'abbrev' => 'tropical',
				'define' => 'CLIMATE_TROPICAL',
				'category' => 'climate_type',
				'value' => 3
			],
			[
				'name' => 'alpine',
				'abbrev' => 'alpine',
				'define' => 'CLIMATE_ALPINE',
				'category' => 'climate_type',
				'value' => 4
			],
			[
				'name' => 'tundra',
				'abbrev' => 'tundra',
				'define' => 'CLIMATE_TUNDRA',
				'category' => 'climate_type',
				'value' => 5
			],
			[
				'name' => 'subtropical',
				'abbrev' => 'subtropical',
				'define' => 'CLIMATE_SUBTROPICAL',
				'category' => 'climate_type',
				'value' => 6
			],
			[
				'name' => 'arid',
				'abbrev' => 'arid',
				'define' => 'CLIMATE_ARID',
				'category' => 'climate_type',
				'value' => 7
			],
			[
				'name' => 'north',
				'abbrev' => 'n',
				'define' => 'DIR_NORTH',
				'category' => 'directions',
				'value' => 0
			],
			[
				'name' => 'east',
				'abbrev' => 'e',
				'define' => 'DIR_EAST',
				'category' => 'directions',
				'value' => 1
			],
			[
				'name' => 'south',
				'abbrev' => 's',
				'define' => 'DIR_SOUTH',
				'category' => 'directions',
				'value' => 2
			],
			[
				'name' => 'west',
				'abbrev' => 'w',
				'define' => 'DIR_WEST',
				'category' => 'directions',
				'value' => 3
			],
			[
				'name' => 'up',
				'abbrev' => 'u',
				'define' => 'DIR_UP',
				'category' => 'directions',
				'value' => 4
			],
			[
				'name' => 'down',
				'abbrev' => 'd',
				'define' => 'DIR_DOWN',
				'category' => 'directions',
				'value' => 5
			] 
		]);
	}
}
