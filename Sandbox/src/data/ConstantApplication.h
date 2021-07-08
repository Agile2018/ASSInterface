#pragma once

struct ConstantApplication {
	static const inline char* TYPES_PERSON[3] = { "Visitors", "Observed", "Employees" };	
	static const inline char* DESTINATION_PLACES[2] = { "Office 1", "Office 2" };
	static const inline char* QUANTITY_CHANNELS[4] = { "Channel 1", "Channel 2", 
		"Channel 3", "Channel 4" };
	static const inline char* TYPES_CONFIGURATION[4] = { "face", "track", "enroll", "entry" };

	enum class ConfigurationType
	{
		None = -1,
		Face = 0,
		Track, Enroll, Entry, Global
	};
};

