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

#ifndef WMO_H
#define WMO_H

#define TILESIZE (533.33333f)
#define CHUNKSIZE ((TILESIZE) / 16.0f)

#include <string>
#include <set>
#include "vec3d.h"
#include <mpq.h>
#include <loadlib.h>

// MOPY flags
#define WMO_MATERIAL_NOCAMCOLLIDE    0x01
#define WMO_MATERIAL_DETAIL          0x02
#define WMO_MATERIAL_NO_COLLISION    0x04
#define WMO_MATERIAL_HINT            0x08
#define WMO_MATERIAL_RENDER          0x10
#define WMO_MATERIAL_COLLIDE_HIT     0x20
#define WMO_MATERIAL_WALL_SURFACE    0x40

/**
 * @brief Represents the root of a WMO (World Map Object) file.
 */
class WMORoot
{
    public:
        uint32 nTextures, nGroups, nP, nLights, nModels, nDoodads, nDoodadSets, RootWMOID, liquidType; /**< Various counts and IDs related to the WMO. */
        unsigned int col; /**< Color or other attribute. */
        float bbcorn1[3]; /**< Bounding box corner 1. */
        float bbcorn2[3]; /**< Bounding box corner 2. */

        /**
         * @brief Constructs a WMORoot object.
         *
         * @param filename The name of the WMO file.
         */
        WMORoot(std::string& filename);
        /**
         * @brief Destroys the WMORoot object.
         */
        ~WMORoot();

        /**
         * @brief Opens the WMO file.
         *
         * @return True if the file was successfully opened, false otherwise.
         */
        bool open();
        /**
         * @brief Converts the WMO root to VMAP format.
         *
         * @param output The output file.
         * @param szRawVMAPMagic The VMAP magic string.
         * @return True if the conversion was successful, false otherwise.
         */
        bool ConvertToVMAPRootWmo(FILE* output, std::string szRawVMAPMagic);
    private:
        std::string filename; /**< The name of the WMO file. */
};

/**
 * @brief Represents the header of a WMO liquid.
 */
struct WMOLiquidHeader
{
    int xverts, yverts, xtiles, ytiles; /**< Number of vertices and tiles in x and y directions. */
    float pos_x; /**< X position. */
    float pos_y; /**< Y position. */
    float pos_z; /**< Z position. */
    short type; /**< Type of liquid. */
};

/**
 * @brief Represents a vertex in a WMO liquid.
 */
struct WMOLiquidVert
{
    uint16 unk1; /**< Unknown attribute 1. */
    uint16 unk2; /**< Unknown attribute 2. */
    float height; /**< Height of the vertex. */
};

/**
 * @brief Represents a group within a WMO file.
 */
class WMOGroup
{
    public:
        // MOGP
        int groupName, descGroupName, mogpFlags; /**< Group name, description, and flags. */
        float bbcorn1[3]; /**< Bounding box corner 1. */
        float bbcorn2[3]; /**< Bounding box corner 2. */
        uint16 moprIdx; /**< MOPR index. */
        uint16 moprNItems; /**< Number of MOPR items. */
        uint16 nBatchA; /**< Number of batch A items. */
        uint16 nBatchB; /**< Number of batch B items. */
        uint32 nBatchC, fogIdx, liquidType, groupWMOID; /**< Number of batch C items, fog index, liquid type, and group WMO ID. */

        int mopy_size, moba_size; /**< Sizes of MOPY and MOBA. */
        int LiquEx_size; /**< Size of LiquEx. */
        unsigned int nVertices; /**< Number of vertices when loaded. */
        int nTriangles; /**< Number of triangles when loaded. */
        char* MOPY; /**< MOPY data. */
        uint16* MOVI; /**< MOVI data. */
        uint16* MoviEx; /**< MoviEx data. */
        float* MOVT; /**< MOVT data. */
        uint16* MOBA; /**< MOBA data. */
        int* MobaEx; /**< MobaEx data. */
        WMOLiquidHeader* hlq; /**< WMO liquid header. */
        WMOLiquidVert* LiquEx; /**< WMO liquid vertices. */
        char* LiquBytes; /**< Liquid bytes. */
        uint32 liquflags; /**< Liquid flags. */

        /**
         * @brief Constructs a WMOGroup object.
         *
         * @param filename The name of the WMO group file.
         */
        WMOGroup(std::string& filename);
        /**
         * @brief Destroys the WMOGroup object.
         */
        ~WMOGroup();

        /**
         * @brief Opens the WMO group file.
         *
         * @return True if the file was successfully opened, false otherwise.
         */
        bool open();
        /**
         * @brief Converts the WMO group to VMAP format.
         *
         * @param output The output file.
         * @param rootWMO The root WMO object.
         * @param pPreciseVectorData Whether to use precise vector data.
         * @param iCoreNumber The core number.
         * @return The result of the conversion.
         */
        int ConvertToVMAPGroupWmo(FILE* output, WMORoot* rootWMO, bool pPreciseVectorData, int iCoreNumber);

    private:
        std::string filename; /**< The name of the WMO group file. */
};

/**
 * @brief Represents an instance of a WMO.
 */
class WMOInstance
{
        static std::set<int> ids; /**< Set of IDs. */
    public:
        std::string MapName; /**< Name of the map. */
        int currx; /**< Current x coordinate. */
        int curry; /**< Current y coordinate. */
        WMOGroup* wmo; /**< Pointer to the WMO group. */
        Vec3D pos; /**< Position vector. */
        Vec3D pos2, pos3, rot; /**< Additional position and rotation vectors. */
        uint32 indx, id, d2, d3; /**< Index, ID, and additional data. */
        int doodadset; /**< Doodad set. */
        std::string szWorkDirWmo; /**< Working directory for WMO. */

        /**
         * @brief Constructs a WMOInstance object.
         *
         * @param f The MPQ file.
         * @param WmoInstName The name of the WMO instance.
         * @param mapID The map ID.
         * @param tileX The x coordinate of the tile.
         * @param tileY The y coordinate of the tile.
         * @param pDirfile The directory file.
         * @param szWorkDirWmo The working directory for WMO.
         */
        WMOInstance(MPQFile& f, std::string& WmoInstName, uint32 mapID, uint32 tileX, uint32 tileY, FILE* pDirfile, std::string szWorkDirWmo);

        /**
         * @brief Resets the WMO instance.
         */
        static void reset();
};

/**
 * @brief Extracts a single WMO file.
 *
 * @param fname The name of the WMO file.
 * @param iCoreNumber The core number.
 * @param szRawVMAPMagic The VMAP magic string.
 * @param preciseVectorData Whether to use precise vector data.
 * @param szWorkDirWmo The working directory for WMO.
 * @return True if the extraction was successful, false otherwise.
 */
bool ExtractSingleWmo(std::string& fname, int iCoreNumber, std::string szRawVMAPMagic, bool preciseVectorData, std::string szWorkDirWmo);

/**
 * @brief Extracts all WMO files.
 *
 * @param iCoreNumber The core number.
 * @param szRawVMAPMagic The VMAP magic string.
 * @param preciseVectorData Whether to use precise vector data.
 * @param szWorkDirWmo The working directory for WMO.
 * @return True if the extraction was successful, false otherwise.
 */
bool ExtractWmo(int iCoreNumber, std::string szRawVMAPMagic, bool preciseVectorData, std::string szWorkDirWmo);

#endif // WMO_H
