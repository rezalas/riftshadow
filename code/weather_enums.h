#ifndef WEATHER_ENUMS_H
#define WEATHER_ENUMS_H

// TEMP_HOT					0
// TEMP_WARM					1
// TEMP_COOL					2
// TEMP_COLD					3
// MAX_TEMP					4

enum Temperature 
{
	Hot, 
	Warm,
	Cool,
	Cold,
	MaxTemperature // used for setting the max size of const tables, should always be last
};

// WIND_CALM					0
// WIND_BREEZE					1
// WIND_WINDY					2
// WIND_GALE					3
// MAX_WIND					4

enum Windspeed 
{
	Calm,
	Breeze,
	Windy,
	Gale,
	MaxWindspeed // used for setting the max size of const tables, should always be last
};

// #define CLIMATE_NONE				0
// #define CLIMATE_TEMPERATE			1
// #define CLIMATE_DESERT				2
// #define CLIMATE_TROPICAL			3
// #define CLIMATE_ALPINE				4
// #define CLIMATE_TUNDRA				5
// #define CLIMATE_SUBTROPICAL			6
// #define CLIMATE_ARID				7
// #define CLIMATE_ENGLISH				8
// #define MAX_CLIMATE					9

enum Climate
{
	None,
	Temperate,
	Desert,
	Tropical,
	Alpine,
	Tundra,
	Subtropical,
	Arid,
	English,
	MaxClimate // used for setting the max size of const tables, should always be last
};

// #define SKY_CLEAR					0
// #define SKY_PCLOUDY					1
// #define SKY_OVERCAST				2
// #define SKY_DRIZZLE					3
// #define SKY_DOWNPOUR				4
// #define SKY_TSTORM					5
// #define SKY_FLURRY					6
// #define SKY_BLIZZARD				7
// #define SKY_HAIL					8
// #define MAX_SKY						9

enum WeatherCondition
{
	Clear,
	PartlyCloudy,
	Overcast,
	Drizzle,
	Downpour,
	ThunderStorm,
	SnowFlurry,
	Blizzard,
	Hail,
	MaxWeatherCondition // used for setting the max size of const tables, should always be last
};

#endif