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
                'category' => 'stats'
            ],
            [
                'name' => 'intelligence',
                'abbrev' => 'int',
                'define' => 'STAT_INT',
                'category' => 'stats'
            ],
            [
                'name' => 'wisdom',
                'abbrev' => 'wis',
                'define' => 'STAT_WIS',
                'category' => 'stats'
            ],
            [
                'name' => 'dexterity',
                'abbrev' => 'dex',
                'define' => 'STAT_DEX',
                'category' => 'stats'
            ],
            [
                'name' => 'constitution',
                'abbrev' => 'con',
                'define' => 'STAT_CON',
                'category' => 'stats'
            ],
            [
                'name' => 'casts',
                'abbrev' => 'casts',
                'define' => 'CLASS_CASTER',
                'category' => 'magical_class'
            ],
            [
                'name' => 'communes',
                'abbrev' => 'communes',
                'define' => 'CLASS_COMMUNER',
                'category' => 'magical_class'
            ],
            [
                'name' => 'none',
                'abbrev' => 'none',
                'define' => 'CLASS_NEITHER',
                'category' => 'magical_class'
            ],
            [
                'name' => 'open',
                'abbrev' => 'open',
                'define' => 'RACE_OPEN',
                'category' => 'race_status'
            ],
            [
                'name' => 'closed',
                'abbrev' => 'closed',
                'define' => 'RACE_CLOSED',
                'category' => 'race_status'
            ],
            [
                'name' => 'any',
                'abbrev' => 'any',
                'define' => 'ALIGN_ANY',
                'category' => 'align_allowed'
            ],
            [
                'name' => 'good neutral',
                'abbrev' => 'good neutral',
                'define' => 'ALIGN_GN',
                'category' => 'align_allowed'
            ],
            [
                'name' => 'neutral evil',
                'abbrev' => 'neutral evil',
                'define' => 'ALIGN_NE',
                'category' => 'align_allowed'
            ],
            [
                'name' => 'good evil',
                'abbrev' => 'good evil',
                'define' => 'ALIGN_GE',
                'category' => 'align_allowed'
            ],
            [
                'name' => 'good',
                'abbrev' => 'good',
                'define' => 'ALIGN_G',
                'category' => 'align_allowed'
            ],
            [
                'name' => 'neutral',
                'abbrev' => 'neutral',
                'define' => 'ALIGN_N',
                'category' => 'align_allowed'
            ],
            [
                'name' => 'evil',
                'abbrev' => 'evil',
                'define' => 'ALIGN_E',
                'category' => 'align_allowed'
            ],
            [
                'name' => 'tiny',
                'abbrev' => 'tiny',
                'define' => 'SIZE_TINY',
                'category' => 'size'
            ],
            [
                'name' => 'small',
                'abbrev' => 'small',
                'define' => 'SIZE_SMALL',
                'category' => 'size'
            ],
            [
                'name' => 'medium',
                'abbrev' => 'medium',
                'define' => 'SIZE_MEDIUM',
                'category' => 'size'
            ],
            [
                'name' => 'large',
                'abbrev' => 'large',
                'define' => 'SIZE_LARGE',
                'category' => 'size'
            ],
            [
                'name' => 'huge',
                'abbrev' => 'huge',
                'define' => 'SIZE_HUGE',
                'category' => 'size'
            ],
            [
                'name' => 'giant',
                'abbrev' => 'giant',
                'define' => 'SIZE_GIANT',
                'category' => 'size'
            ],
            [
                'name' => 'immense',
                'abbrev' => 'immense',
                'define' => 'SIZE_IMMENSE',
                'category' => 'size'
            ],
            [
                'name' => 'open',
                'abbrev' => 'open',
                'define' => 'CLASS_OPEN',
                'category' => 'class_status'
            ],
            [
                'name' => 'closed',
                'abbrev' => 'closed',
                'define' => 'CLASS_CLOSED',
                'category' => 'class_status'
            ],
            [
                'name' => 'normal',
                'abbrev' => 'normal',
                'define' => 'ARE_NORMAL',
                'category' => 'area_type'
            ],
            [
                'name' => 'road_river',
                'abbrev' => 'road_river',
                'define' => 'ARE_ROAD_RIVER',
                'category' => 'area_type'
            ],
            [
                'name' => 'cabal',
                'abbrev' => 'cabal',
                'define' => 'ARE_CABAL',
                'category' => 'area_type'
            ],
            [
                'name' => 'quest',
                'abbrev' => 'quest',
                'define' => 'ARE_QUEST',
                'category' => 'area_type'
            ],
            [
                'name' => 'city',
                'abbrev' => 'city',
                'define' => 'ARE_CITY',
                'category' => 'area_type'
            ],
            [
                'name' => 'unopened',
                'abbrev' => 'unopened',
                'define' => 'ARE_UNOPENED',
                'category' => 'area_type'
            ],
            [
                'name' => 'shrine',
                'abbrev' => 'shrine',
                'define' => 'ARE_SHRINE',
                'category' => 'area_type'
            ],
            [
                'name' => 'none',
                'abbrev' => 'none',
                'define' => 'CLIMATE_NONE',
                'category' => 'climate_type'
            ],
            [
                'name' => 'temperate',
                'abbrev' => 'temperate',
                'define' => 'CLIMATE_TEMPERATE',
                'category' => 'climate_type'
            ],
            [
                'name' => 'desert',
                'abbrev' => 'desert',
                'define' => 'CLIMATE_DESERT',
                'category' => 'climate_type'
            ],
            [
                'name' => 'tropical',
                'abbrev' => 'tropical',
                'define' => 'CLIMATE_TROPICAL',
                'category' => 'climate_type'
            ],
            [
                'name' => 'alpine',
                'abbrev' => 'alpine',
                'define' => 'CLIMATE_ALPINE',
                'category' => 'climate_type'
            ],
            [
                'name' => 'tundra',
                'abbrev' => 'tundra',
                'define' => 'CLIMATE_TUNDRA',
                'category' => 'climate_type'
            ],
            [
                'name' => 'subtropical',
                'abbrev' => 'subtropical',
                'define' => 'CLIMATE_SUBTROPICAL',
                'category' => 'climate_type'
            ],
            [
                'name' => 'arid',
                'abbrev' => 'arid',
                'define' => 'CLIMATE_ARID',
                'category' => 'climate_type'
            ],
            [
                'name' => 'north',
                'abbrev' => 'n',
                'define' => 'DIR_NORTH',
                'category' => 'directions'
            ],
            [
                'name' => 'east',
                'abbrev' => 'e',
                'define' => 'DIR_EAST',
                'category' => 'directions'
            ],
            [
                'name' => 'south',
                'abbrev' => 's',
                'define' => 'DIR_SOUTH',
                'category' => 'directions'
            ],
            [
                'name' => 'west',
                'abbrev' => 'w',
                'define' => 'DIR_WEST',
                'category' => 'directions'
            ],
            [
                'name' => 'up',
                'abbrev' => 'u',
                'define' => 'DIR_UP',
                'category' => 'directions'
            ],
            [
                'name' => 'down',
                'abbrev' => 'd',
                'define' => 'DIR_DOWN',
                'category' => 'directions'
            ] 
        ]);
    }
}
