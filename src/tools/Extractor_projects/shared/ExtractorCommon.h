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

#ifndef EXTRACTOR_COMMON_H
#define EXTRACTOR_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "loadlib.h"
#include <vector>
#include <md5.h>
#include <list>
#include "../vmap-extractor/adtfile.h"

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
/**
 * @brief
 *
 */
typedef std::set<std::string> StringSet;
//std::set<std::string> StringSet;

/**
 * @brief Test if the specified file exists in the building directory
 *
 * @param file
 * @return bool
 */
bool FileExists(const char* file);

/**
 * @brief Get "uniform" name for a path (a uniform name has the format <md5hash>-<filename>.<ext>)
 *
 * @param path
 * @return string
 */
std::string GetUniformName(std::string& path);

/**
 * @brief Get extension for a file
 *
 * @param file
 * @return extension, if found, or empty string if not
 */
std::string GetExtension(std::string& file);

bool scan_patches(char* scanmatch, std::vector<std::string>& pArchiveNames);

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

/**
 * @brief
 *
 */
enum ModelFlags
{
    MOD_M2 = 1,
    MOD_WORLDSPAWN = 1 << 1,
    MOD_HAS_BOUND = 1 << 2
};

/**
 * @brief
 *
 */
typedef struct
{
    float x; /**< TODO */
    float y; /**< TODO */
    float z; /**< TODO */
} svec;

/**
 * @brief
 *
 */
typedef struct
{
    float v9[16 * 8 + 1][16 * 8 + 1]; /**< TODO */
    float v8[16 * 8][16 * 8]; /**< TODO */
} Cell;

/**
 * @brief
 *
 */
typedef struct
{
    double v9[9][9]; /**< TODO */
    double v8[8][8]; /**< TODO */
    uint16 area_id; /**< TODO */
    //Liquid *lq;
    float waterlevel[9][9]; /**< TODO */
    uint8 flag; /**< TODO */
} chunk;

/**
 * @brief
 *
 */
typedef struct
{
    chunk ch[16][16]; /**< TODO */
} mcell;

/**
 * @brief
 *
 */
struct MapChunkHeader
{
    uint32 flags; /**< TODO */
    uint32 ix; /**< TODO */
    uint32 iy; /**< TODO */
    uint32 nLayers; /**< TODO */
    uint32 nDoodadRefs; /**< TODO */
    uint32 ofsHeight; /**< TODO */
    uint32 ofsNormal; /**< TODO */
    uint32 ofsLayer; /**< TODO */
    uint32 ofsRefs; /**< TODO */
    uint32 ofsAlpha; /**< TODO */
    uint32 sizeAlpha; /**< TODO */
    uint32 ofsShadow; /**< TODO */
    uint32 sizeShadow; /**< TODO */
    uint32 areaid; /**< TODO */
    uint32 nMapObjRefs; /**< TODO */
    uint32 holes; /**< TODO */
    uint16 s1; /**< TODO */
    uint16 s2; /**< TODO */
    uint32 d1; /**< TODO */
    uint32 d2; /**< TODO */
    uint32 d3; /**< TODO */
    uint32 predTex; /**< TODO */
    uint32 nEffectDoodad; /**< TODO */
    uint32 ofsSndEmitters; /**< TODO */
    uint32 nSndEmitters; /**< TODO */
    uint32 ofsLiquid; /**< TODO */
    uint32 sizeLiquid; /**< TODO */
    float  zpos; /**< TODO */
    float  xpos; /**< TODO */
    float  ypos; /**< TODO */
    uint32 textureId; /**< TODO */
    uint32 props; /**< TODO */
    uint32 effectId; /**< TODO */
};

/**
 * @brief
 *
 */
struct vec
{
    double x; /**< TODO */
    double y; /**< TODO */
    double z; /**< TODO */
};

/**
 * @brief
 *
 */
struct triangle
{
    vec v[3]; /**< TODO */
};


#endif