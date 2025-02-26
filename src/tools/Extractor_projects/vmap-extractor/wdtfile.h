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

#ifndef VMAP_WDTFILE_H
#define VMAP_WDTFILE_H

#include <string>
#include <mpq.h>
#include "wmo.h"
#include "adtfile.h"

/**
 * @brief Enum representing terrain flags.
 */
enum TerrainFlags {
    TERRAIN_HAS_ADT = 0x01 /**< Indicates that the terrain has an ADT file. */
};

/**
 * @brief Struct representing area information.
 */
struct SMAreaInfo     // -> CMapAreaTableEntry
{
    uint32_t flags; /**< Flags for the area. */
    uint32_t asyncId; /**< Asynchronous ID, only set during runtime. */
};

/**
 * @brief Class representing a WDT (World Map Definition) file.
 */
class WDTFile
{
    public:
        /**
         * @brief Constructs a WDTFile object.
         *
         * @param handle The handle to the MPQ file.
         * @param file_name The name of the WDT file.
         * @param file_name1 An additional file name.
         */
        WDTFile(HANDLE handle, char* file_name, char* file_name1);
        /**
         * @brief Destroys the WDTFile object.
         */
        ~WDTFile(void);
        /**
         * @brief Initializes the WDT file.
         *
         * @param map_id The ID of the map.
         * @param mapID The map ID.
         * @param szWorkDirWmo The working directory for WMO files.
         * @return True if the initialization was successful, false otherwise.
         */
        bool init(char* map_id, unsigned int mapID, std::string szWorkDirWmo);

        /**
         * @brief Checks if the terrain has an ADT file.
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         * @return True if the terrain has an ADT file, false otherwise.
         */
        bool hasTerrain(int x, int y);

        std::string* gWmoInstansName; /**< Name of the WMO instance. */
        int gnWMO, nMaps; /**< Number of WMOs and maps. */

        /**
         * @brief Gets the ADT file for the specified coordinates.
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         * @return A pointer to the ADTFile object.
         */
        ADTFile* GetMap(int x, int y);

    private:
        MPQFile WDT; /**< The MPQ file for the WDT. */
        static const int MAP_TILE_SIZE = 64; /**< The size of the map tile. */
        SMAreaInfo* mapAreaInfo[MAP_TILE_SIZE * MAP_TILE_SIZE]; /**< Array of area information for the map tiles. */
        std::string filename; /**< The name of the WDT file. */
};

#endif // VMAP_WDTFILE_H
