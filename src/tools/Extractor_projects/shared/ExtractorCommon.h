/**
 * MaNGOS is a full featured server for World of Warcraft, supporting
 * the following clients: 1.12.x, 2.4.3, 3.3.5a, 4.3.4a and 5.4.8
 *
 * Copyright (C) 2005-2025 MaNGOS <https://www.getmangos.eu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * World of Warcraft, and all World of Warcraft or Warcraft art, images,
 * and lore are copyrighted by Blizzard Entertainment, Inc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "loadlib.h"
#include <vector>

/// all of the updates for this version of WoW
uint32 const Builds[] = {16016, 16048, 16057, 16309, 16357, 16516, 16650, 16844, 16965, 17116, 17266, 17325, 17345, 17538, 17645, 17688, 17898, 18273};
static uint32 CONF_TargetBuild = 18273; /// last actual update file under the Data folder

static char const* Locales[] = { "enGB", "enUS", "deDE", "esES", "frFR", "koKR", "zhCN", "zhTW", "enCN", "enTW", "esMX", "ruRU", "itIT" };
constexpr auto LOCALES_COUNT = 13;
FILE* openWoWExe(char const* path = NULL);
int getBuildNumber(char const* path = NULL);
int getCoreNumber();
int getCoreNumberFromBuild(int iBuildNumber);
std::vector<std::string> getMPQListForCore(int coreNumber);
void showBanner(const std::string& title, int iCoreNumber);
void showWebsiteBanner();
void setMapMagicVersion(int iCoreNumber, char* magic);
void setVMapMagicVersion(int iCoreNumber, char* magic);
void setMMapMagicVersion(int iCoreNumber, char* magic);
bool CreateDir(const std::string& sPath);
bool ClientFileExists(const char* sFileName);
bool isTransportMap(int mapID);
bool shouldSkipMap(int mapID, bool m_skipContinents, bool m_skipJunkMaps, bool m_skipBattlegrounds);

/// Enumerated Core Numbers
enum CoreNumber
{
    CLIENT_CLASSIC = 0,
    CLIENT_TBC     = 1,
    CLIENT_WOTLK   = 2,
    CLIENT_CATA    = 3,
    CLIENT_MOP     = 4,
    CLIENT_WOD     = 5,
    CLIENT_LEGION  = 6
};

//std::vector<std::string> getMPQListForCore(int client);
//
//
