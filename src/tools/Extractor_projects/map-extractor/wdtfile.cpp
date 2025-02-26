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

#include <cstdio>
#include "wdtfile.h"
#include <ExtractorCommon.h>

/**
 * @brief Constructor for the WDTFile class.
 *
 * @param handle Handle to the file.
 * @param file_name Name of the file.
 * @param file_name1 Another name of the file.
 */
WDTFile::WDTFile(HANDLE handle, char* file_name, char* file_name1)
    : WDT(handle, file_name), gWmoInstansName(nullptr), gnWMO(0), nMaps(0)
{
    // Assign the second file name to the filename member variable
    filename.assign(file_name1);

    // Initialize the mapAreaInfo array with NULL pointers
    for (int i = 0; i < MAP_TILE_SIZE * MAP_TILE_SIZE; i++)
    {
        mapAreaInfo[i] = NULL;
    }
}

/**
 * @brief Initializes the WDT file.
 *
 * @param map_id ID of the map.
 * @param mapID Map ID.
 * @param szWorkDirWmo Working directory for WMO files.
 * @return true if initialization is successful, false otherwise.
 */
bool WDTFile::init(char* map_id, unsigned int mapID, std::string szWorkDirWmo)
{
    // Check if the WDT file has reached the end
    if (WDT.isEof())
    {
        //printf("Can't find WDT file.\n");
        return false;
    }

    char fourcc[5];
    uint32 size;

    // Create the directory name for WMO files
    std::string dirname = std::string(szWorkDirWmo) + "/dir_bin";
    FILE* dirfile;
    dirfile = fopen(dirname.c_str(), "ab");
    if (!dirfile)
    {
        printf("Can't open dirfile!'%s'\n", dirname.c_str());
        return false;
    }

    // Read the WDT file until the end
    while (!WDT.isEof())
    {
        WDT.read(fourcc, 4);
        WDT.read(&size, 4);

        // Flip the fourcc characters
        flipcc(fourcc);
        fourcc[4] = 0;

        size_t nextpos = WDT.getPos() + size;

        // Process the "MAIN" chunk
        if (!strcmp(fourcc, "MAIN"))
        {
            // Area Info
            if (size != (MAP_TILE_SIZE * MAP_TILE_SIZE * sizeof(SMAreaInfo)))
            {
                printf("The size for the ADT Map Tile is not the expected one. This file looks corrupted: %s", filename.c_str());
                return false;
            }

            // Read area information for each map tile
            for (int i = 0; i < MAP_TILE_SIZE * MAP_TILE_SIZE; i++)
            {
                SMAreaInfo* info = new SMAreaInfo();
                WDT.read(&(info->flags), 4);
                WDT.read(&(info->asyncId), 4);
                mapAreaInfo[i] = info;
            }
        }
        // Process the "MWMO" chunk
        if (!strcmp(fourcc, "MWMO"))
        {
            // Global map objects
            if (size)
            {
                char* buf = new char[size];
                WDT.read(buf, size);
                char* p = buf;
                int q = 0;
                gWmoInstansName = new string[size];
                while (p < buf + size)
                {
                    string path(p);
                    gWmoInstansName[q++] = GetUniformName(path);
                    p = p + strlen(p) + 1;
                }
                delete[] buf;
            }
        }
        // Process the "MODF" chunk
        else if (!strcmp(fourcc, "MODF"))
        {
            // Global WMO instance data
            if (size)
            {
                gnWMO = (int)size / 64;
                std::string gWMO_mapname;
                std::string fake_mapname;
                fake_mapname = "65 65 ";
                //gWMO_mapname = fake_mapname + filename;
                gWMO_mapname = fake_mapname + std::string(map_id);
                for (int i = 0; i < gnWMO; ++i)
                {
                    int id;
                    WDT.read(&id, 4);
                    WMOInstance inst(WDT, gWmoInstansName[id], mapID, 65, 65, dirfile, szWorkDirWmo);
                }
                delete[] gWmoInstansName;
            }
        }
        // Move to the next position in the WDT file
        WDT.seek((int)nextpos);
    }

    fclose(dirfile);
    return true;
}

/**
 * @brief Destructor for the WDTFile class.
 */
WDTFile::~WDTFile(void)
{
    // Close the WDT file
    WDT.close();

    // Delete the area information for each map tile
    for (int i = 0; i < MAP_TILE_SIZE * MAP_TILE_SIZE; i++)
    {
        delete mapAreaInfo[i];
    }
}

/**
 * @brief Checks if the terrain has an ADT file.
 *
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return true if the terrain has an ADT file, false otherwise.
 */
bool WDTFile::hasTerrain(int x, int y)
{
    return (mapAreaInfo[x * MAP_TILE_SIZE + y]->flags & TERRAIN_HAS_ADT);
}

/**
 * @brief Gets the ADT file for the specified coordinates.
 *
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return A pointer to the ADTFile object.
 */
ADTFile* WDTFile::GetMap(int x, int y)
{
    // Check if the coordinates are within bounds and if the terrain has an ADT file
    if (!(x >= 0 && y >= 0 && x < 64 && y < 64) || !hasTerrain(y, x))
    {
        return NULL;
    }

    char name[512];

    // Format the ADT file name
    sprintf(name, "World\\Maps\\%s\\%s_%d_%d.adt", filename.c_str(), filename.c_str(), x, y);
    return new ADTFile(name);
}
