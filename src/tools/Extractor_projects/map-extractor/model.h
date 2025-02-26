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

#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <loadlib.h>
#include "vec3d.h"
#include "modelheaders.h"
#include "wmo.h"

/**
 * @brief
 *
 */
class Model
{
    public:
        ModelHeaderClassicTBC headerClassicTBC; /**< TODO */
        ModelHeaderOthers headerOthers; /**< TODO */
        ModelBoundingVertex* boundingVertices; /**< TODO */
        Vec3D* vertices; /**< TODO */
        uint16* indices; /**< TODO */
        size_t nIndices; /**< TODO */

        /**
         * @brief
         *
         * @param failedPaths
         * @return bool
         */
        bool open(std::set<std::string>& failedPaths, int iCoreNumber);
        /**
         * @brief
         *
         * @param outfilename
         * @return bool
         */
        bool ConvertToVMAPModel(std::string& outfilename, int iCoreNumber, std::string szRawVMAPMagic, bool preciseVectorData, std::string szWorkDirWmo);

        bool ok; /**< TODO */

        /**
         * @brief
         *
         * @param filename
         */
        Model(std::string& filename);
        /**
         * @brief
         *
         */
        ~Model() {_unload();}

    private:
        /**
         * @brief
         *
         */
        void _unload()
        {
            delete[] vertices;
            delete[] indices;
            vertices = NULL;
            indices = NULL;
        }
        std::string filename; /**< TODO */
};

/**
 * @brief Represents an instance of a model in the game world.
 */
class ModelInstance
{
    public:
        Model* model = nullptr; /**< Pointer to the model associated with this instance. */

        uint32 id = 0; /**< Unique identifier for the model instance. */
        Vec3D pos, rot; /**< Position and rotation of the model instance in the game world. */
        unsigned int d1 = 0; /**< Unknown data field. */
        float w = 0.0f, sc = 0.0f; /**< Width and scale of the model instance. */
        unsigned int scaleZeroOnly = 0; /**< Scale value used only for zero scale. */
        //unsigned int scale; // This line introduced a regression bug in Mangos Zero, is Fine for other cores.
        uint16 scaleOthers = 0; /**< Scale value for other cores. */
        std::string szWorkDirWmo; /**< Working directory for WMO files. */

        /**
         * @brief Default constructor for ModelInstance.
         */
        ModelInstance() : d1(0) {}

        /**
         * @brief Constructs a ModelInstance from an MPQ file.
         *
         * @param f Reference to the MPQ file.
         * @param ModelInstName Name of the model instance.
         * @param mapID ID of the map.
         * @param tileX X coordinate of the tile.
         * @param tileY Y coordinate of the tile.
         * @param pDirfile Pointer to the directory file.
         * @param iCoreNumber Core number.
         * @param szWorkDirWmo Working directory for WMO files.
         */
        ModelInstance(MPQFile& f, std::string& ModelInstName, uint32 mapID, uint32 tileX, uint32 tileY, FILE* pDirfile, int iCoreNumber, std::string szWorkDirWmo);
};

/**
 * @brief
 *
 * @param origPath original path of the model, cleaned with fixnamen and fixname2
 * @param fixedName will store the translated name (if changed)
 * @param failedPaths Set to collect errors
 * @return bool
 */
bool ExtractSingleModel(std::string& origPath, std::string& fixedName, std::set<std::string>& failedPaths, int iCoreNumber, std::string szRawVMAPMagic, bool preciseVectorData, std::string szWorkDirWmo);

/**
 * @brief
 *
 */
void ExtractGameobjectModels(int iCoreNumber, std::string szRawVMAPMagic, bool preciseVectorData, std::string szWorkDirWmo);

#endif
