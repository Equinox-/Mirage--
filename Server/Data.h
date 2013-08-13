#ifndef _DATA_H
#define _DATA_H

#include "CIniFile.h"

struct Data
{
	Data()
	{
		CIniFile::Get()->Load("Data.ini");
		port = CIniFile::Get()->GetInt("SERVER_PORT", 1000);
		maxClients = CIniFile::Get()->GetInt("MAX_CONNECTIONS", 15);
		updateTimer = CIniFile::Get()->GetInt("UPDATE_TIMER", 1500);
		connectionUpdateTimer = CIniFile::Get()->GetInt("CONNECTION_UPDATE", 10);
		saveUpdateTimer = CIniFile::Get()->GetInt("SAVE_PLAYERS", 500);
	}

	int port;
	int maxClients;
	int updateTimer;
	int connectionUpdateTimer;
	int saveUpdateTimer;
};

#endif