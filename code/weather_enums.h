#ifndef WEATHER_ENUMS_H
#define WEATHER_ENUMS_H

enum Temperature 
{
	Hot = 0, 
	Warm = 1,
	Cool = 2,
	Cold = 3,
	MaxTemperature = 4 // used for setting the max size of const tables, should always be last
};

enum Windspeed 
{
	Calm = 0,
	Breeze = 1,
	Windy = 2,
	Gale = 3,
	MaxWindspeed = 4 // used for setting the max size of const tables, should always be last
};

enum Climate
{
	None = 0,
	Temperate = 1,
	Desert = 2,
	Tropical = 3,
	Alpine = 4,
	Tundra = 5,
	Subtropical = 6,
	Arid = 7,
	English = 8,
	MaxClimate = 9 // used for setting the max size of const tables, should always be last
};

enum WeatherCondition
{
	Clear = 0,
	PartlyCloudy = 1,
	Overcast = 2,
	Drizzle = 3,
	Downpour = 4,
	ThunderStorm = 5,
	SnowFlurry = 6,
	Blizzard = 7,
	Hail = 8,
	MaxWeatherCondition = 9 // used for setting the max size of const tables, should always be last
};

enum SolarPosition
{
	Dark = 0,
	Sunrise = 1,
	Daylight = 2,
	Sunset = 3
};

enum LunarCyclePosition
{
	NewMoon = 0,
	CrescentWaxing = 1,
	HalfWaxing = 2,
	GibbousWaxing = 3,
	FullMoon = 4,
	GibbousWaning = 5,
	HalfWaning = 6,
	CrescentWaning = 7,
	MaxLunarCycles = 8
};

enum Seasons 
{
	Darkness = 0,
	Birth = 1,
	Light = 2,
	Death = 3,
	SeasonsCount = 4
};

enum LunarSpeed 
{
	Berus = 90,
	Calabren = 150
};

#endif