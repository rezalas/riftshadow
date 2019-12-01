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
                'define' => 'STAT_STR'
            ],
            [
                'name' => 'intelligence',
                'abbrev' => 'int',
                'define' => 'STAT_INT'
            ],
            [
                'name' => 'wisdom',
                'abbrev' => 'wis',
                'define' => 'STAT_WIS'
            ],
            [
                'name' => 'dexterity',
                'abbrev' => 'dex',
                'define' => 'STAT_DEX'
            ],
            [
                'name' => 'constitution',
                'abbrev' => 'con',
                'define' => 'STAT_CON'
            ],
            [
                'name' => 'casts',
                'abbrev' => 'casts',
                'define' => 'CLASS_CASTER'
            ],
            [
                'name' => 'communes',
                'abbrev' => 'communes',
                'define' => 'CLASS_COMMUNER'
            ],
            [
                'name' => 'none',
                'abbrev' => 'none',
                'define' => 'CLASS_NEITHER'
            ],
            [
                'name' => 'open',
                'abbrev' => 'open',
                'define' => 'RACE_OPEN'
            ],
            [
                'name' => 'closed',
                'abbrev' => 'closed',
                'define' => 'RACE_CLOSED'
            ],
            [
                'name' => 'any',
                'abbrev' => 'any',
                'define' => 'ALIGN_ANY'
            ],
            [
                'name' => 'good neutral',
                'abbrev' => 'good neutral',
                'define' => 'ALIGN_GN'
            ],
            [
                'name' => 'neutral evil',
                'abbrev' => 'neutral evil',
                'define' => 'ALIGN_NE'
            ],
            [
                'name' => 'good evil',
                'abbrev' => 'good evil',
                'define' => 'ALIGN_GE'
            ],
            [
                'name' => 'good',
                'abbrev' => 'good',
                'define' => 'ALIGN_G'
            ],
            [
                'name' => 'neutral',
                'abbrev' => 'neutral',
                'define' => 'ALIGN_N'
            ],
            [
                'name' => 'evil',
                'abbrev' => 'evil',
                'define' => 'ALIGN_E'
            ],
            [
                'name' => 'tiny',
                'abbrev' => 'tiny',
                'define' => 'SIZE_TINY'
            ],
            [
                'name' => 'small',
                'abbrev' => 'small',
                'define' => 'SIZE_SMALL'
            ],
            [
                'name' => 'medium',
                'abbrev' => 'medium',
                'define' => 'SIZE_MEDIUM'
            ],
            [
                'name' => 'large',
                'abbrev' => 'large',
                'define' => 'SIZE_LARGE'
            ],
            [
                'name' => 'huge',
                'abbrev' => 'huge',
                'define' => 'SIZE_HUGE'
            ],
            [
                'name' => 'giant',
                'abbrev' => 'giant',
                'define' => 'SIZE_GIANT'
            ],
            [
                'name' => 'immense',
                'abbrev' => 'immense',
                'define' => 'SIZE_IMMENSE'
            ],
            [
                'name' => 'open',
                'abbrev' => 'open',
                'define' => 'CLASS_OPEN'
            ],
            [
                'name' => 'closed',
                'abbrev' => 'closed',
                'define' => 'CLASS_CLOSED'
            ],
            [
                'name' => 'normal',
                'abbrev' => 'normal',
                'define' => 'ARE_NORMAL'
            ],
            [
                'name' => 'road_river',
                'abbrev' => 'road_river',
                'define' => 'ARE_ROAD_RIVER'
            ],
            [
                'name' => 'cabal',
                'abbrev' => 'cabal',
                'define' => 'ARE_CABAL'
            ],
            [
                'name' => 'quest',
                'abbrev' => 'quest',
                'define' => 'ARE_QUEST'
            ],
            [
                'name' => 'city',
                'abbrev' => 'city',
                'define' => 'ARE_CITY'
            ],
            [
                'name' => 'unopened',
                'abbrev' => 'unopened',
                'define' => 'ARE_UNOPENED'
            ],
            [
                'name' => 'shrine',
                'abbrev' => 'shrine',
                'define' => 'ARE_SHRINE'
            ],
            [
                'name' => 'none',
                'abbrev' => 'none',
                'define' => 'CLIMATE_NONE'
            ],
            [
                'name' => 'temperate',
                'abbrev' => 'temperate',
                'define' => 'CLIMATE_TEMPERATE'
            ],
            [
                'name' => 'desert',
                'abbrev' => 'desert',
                'define' => 'CLIMATE_DESERT'
            ],
            [
                'name' => 'tropical',
                'abbrev' => 'tropical',
                'define' => 'CLIMATE_TROPICAL'
            ],
            [
                'name' => 'alpine',
                'abbrev' => 'alpine',
                'define' => 'CLIMATE_ALPINE'
            ],
            [
                'name' => 'tundra',
                'abbrev' => 'tundra',
                'define' => 'CLIMATE_TUNDRA'
            ],
            [
                'name' => 'subtropical',
                'abbrev' => 'subtropical',
                'define' => 'CLIMATE_SUBTROPICAL'
            ],
            [
                'name' => 'arid',
                'abbrev' => 'arid',
                'define' => 'CLIMATE_ARID'
            ],
            [
                'name' => 'north',
                'abbrev' => 'n',
                'define' => 'DIR_NORTH'
            ],
            [
                'name' => 'east',
                'abbrev' => 'e',
                'define' => 'DIR_EAST'
            ],
            [
                'name' => 'south',
                'abbrev' => 's',
                'define' => 'DIR_SOUTH'
            ],
            [
                'name' => 'west',
                'abbrev' => 'w',
                'define' => 'DIR_WEST'
            ],
            [
                'name' => 'up',
                'abbrev' => 'u',
                'define' => 'DIR_UP'
            ],
            [
                'name' => 'down',
                'abbrev' => 'd',
                'define' => 'DIR_DOWN'
            ] 
        ]);
    }
}
