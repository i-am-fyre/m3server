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

#include <cassert>
#include <algorithm>
#include <cstdio>

#include <mpq.h>
#include "model.h"
#include "wmo.h"
#include "dbcfile.h"
#include <ExtractorCommon.h>

/**
 * @brief Constructor for the Model class.
 *
 * @param filename The name of the model file.
 */
Model::Model(std::string& filename)
    : filename(filename), vertices(0), indices(0), boundingVertices(nullptr), nIndices(0), ok(false)
{
    memset(&headerClassicTBC, 0, sizeof(headerClassicTBC));
    memset(&headerOthers, 0, sizeof(headerOthers));
}

/**
 * @brief Opens the model file and reads its data.
 *
 * @param failedPaths Set of failed paths.
 * @param iCoreNumber Core number.
 * @return true if the model file is successfully opened, false otherwise.
 */
bool Model::open(StringSet& failedPaths, int iCoreNumber)
{
    HANDLE mpqHandle;
    if (!OpenNewestFile(filename.c_str(), &mpqHandle))
    {
        printf("Error opening model file %s\n", filename.c_str());
        return false;
    }
    MPQFile f(mpqHandle, filename.c_str());

    ok = !f.isEof();

    if (!ok)
    {
        f.close();
        failedPaths.insert(filename);
        return false;
    }

    _unload();

    bool bBoundingTriangles = false;
    uint32 uofsBoundingVertices = 0;
    uint32 uofsBoundingTriangles = 0;
    uint32 unBoundingVertices = 0;
    uint32 unBoundingTriangles = 0;

    // Read headers based on the core number
    if (iCoreNumber == CLIENT_CLASSIC || iCoreNumber == CLIENT_TBC)
    {
        memcpy(&headerClassicTBC, f.getBuffer(), sizeof(ModelHeaderClassicTBC));
        if (headerClassicTBC.nBoundingTriangles > 0)
        {
            bBoundingTriangles = true;
            uofsBoundingVertices = headerClassicTBC.ofsBoundingVertices;
            uofsBoundingTriangles = headerClassicTBC.ofsBoundingTriangles;
            unBoundingVertices = headerClassicTBC.nBoundingVertices;
            unBoundingTriangles = headerClassicTBC.nBoundingTriangles;
        }
    }
    if (iCoreNumber == CLIENT_WOTLK || iCoreNumber == CLIENT_CATA)
    {
        memcpy(&headerOthers, f.getBuffer(), sizeof(ModelHeaderOthers));
        if (headerOthers.nBoundingTriangles > 0)
        {
            bBoundingTriangles = true;
            uofsBoundingVertices = headerOthers.ofsBoundingVertices;
            uofsBoundingTriangles = headerOthers.ofsBoundingTriangles;
            unBoundingVertices = headerOthers.nBoundingVertices;
            unBoundingTriangles = headerOthers.nBoundingTriangles;
        }
    }
    if (bBoundingTriangles)
    {
        boundingVertices = (ModelBoundingVertex*)(f.getBuffer() + uofsBoundingVertices);
        vertices = new Vec3D[unBoundingVertices];

        for (size_t i = 0; i < unBoundingVertices; i++)
        {
            vertices[i] = boundingVertices[i].pos;
        }

        uint16* triangles = (uint16*)(f.getBuffer() + uofsBoundingTriangles);

        nIndices = unBoundingTriangles; // refers to the number of int16's, not the number of triangles
        indices = new uint16[nIndices];
        memcpy(indices, triangles, nIndices * 2);

        f.close();
    }
    else
    {
        //printf("not included %s\n", filename.c_str());
        f.close();
        return false;
    }
    return true;
}

/**
 * @brief Converts the model to a VMAP model.
 *
 * @param outfilename Output filename.
 * @param iCoreNumber Core number.
 * @param szRawVMAPMagic VMAP magic string.
 * @param preciseVectorData Whether to use precise vector data.
 * @param szWorkDirWmo Working directory for WMO files.
 * @return true if the conversion is successful, false otherwise.
 */
bool Model::ConvertToVMAPModel(std::string& outfilename, int iCoreNumber, std::string szRawVMAPMagic, bool preciseVectorData, std::string szWorkDirWmo)
{
    int N[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    FILE* output = fopen(outfilename.c_str(), "wb");
    if (!output)
    {
        printf("Can't create the output file '%s'\n", outfilename.c_str());
        return false;
    }

    fwrite(szRawVMAPMagic.c_str(), 8, 1, output);
    uint32 nVertices = 0;
    if (iCoreNumber == CLIENT_CLASSIC || iCoreNumber == CLIENT_TBC)
    {
        nVertices = headerClassicTBC.nBoundingVertices;
    }
    if (iCoreNumber == CLIENT_WOTLK || iCoreNumber == CLIENT_CATA)
    {
        nVertices = headerOthers.nBoundingVertices;
    }

    fwrite(&nVertices, sizeof(int), 1, output);
    uint32 nofgroups = 1;
    fwrite(&nofgroups, sizeof(uint32), 1, output);
    fwrite(N, 4 * 3, 1, output); // rootwmoid, flags, groupid
    fwrite(N, sizeof(float), 3 * 2, output); //bbox, only needed for WMO currently
    fwrite(N, 4, 1, output); // liquidflags
    fwrite("GRP ", 4, 1, output);
    uint32 branches = 1;
    int wsize;
    wsize = sizeof(branches) + sizeof(uint32) * branches;
    fwrite(&wsize, sizeof(int), 1, output);
    fwrite(&branches, sizeof(branches), 1, output);
    uint32 nIndexes = (uint32) nIndices;
    fwrite(&nIndexes, sizeof(uint32), 1, output);
    fwrite("INDX", 4, 1, output);
    wsize = sizeof(uint32) + sizeof(unsigned short) * nIndexes;
    fwrite(&wsize, sizeof(int), 1, output);
    fwrite(&nIndexes, sizeof(uint32), 1, output);
    if (nIndexes > 0)
    {
        for (uint32 i = 0; i < nIndices; ++i)
        {
            // index[0] -> x, index[1] -> y, index[2] -> z, index[3] -> x ...
            if ((i % 3) - 1 == 0)
            {
                uint16 tmp = indices[i];
                indices[i] = indices[i+1];
                indices[i+1] = tmp;
            }
        }
        fwrite(indices, sizeof(unsigned short), nIndexes, output);
    }
    fwrite("VERT", 4, 1, output);
    wsize = sizeof(int) + sizeof(float) * 3 * nVertices;
    fwrite(&wsize, sizeof(int), 1, output);
    fwrite(&nVertices, sizeof(int), 1, output);
    if (nVertices > 0)
    {
        fwrite(vertices, sizeof(float) * 3, nVertices, output);
    }

    fclose(output);

    return true;
}

/**
 * @brief Constructor for the ModelInstance class.
 *
 * @param f MPQ file.
 * @param ModelInstName Name of the model instance.
 * @param mapID Map ID.
 * @param tileX X coordinate of the tile.
 * @param tileY Y coordinate of the tile.
 * @param pDirfile Directory file.
 * @param coreNumber Core number.
 * @param szWorkDirWmo Working directory for WMO files.
 */
ModelInstance::ModelInstance(MPQFile& f, std::string& ModelInstName, uint32 mapID, uint32 tileX, uint32 tileY, FILE* pDirfile, int coreNumber, std::string szWorkDirWmo)
    : model(nullptr) // Initialize model to nullptr
{
    float ff[3];
    f.read(&id, 4);
    f.read(ff, 12);
    pos = fixCoords(Vec3D(ff[0], ff[1], ff[2]));
    f.read(ff, 12);
    rot = Vec3D(ff[0], ff[1], ff[2]);
    if (coreNumber == CLIENT_TBC || coreNumber == CLIENT_WOTLK)
    {
        uint16 fFlags;      // dummy var
        f.read(&scaleOthers, 2);
        f.read(&fFlags, 2); // unknown but flag 1 is used for biodome in Outland, currently this value is not used
        sc = scaleOthers / 1024.0f; // scale factor - divide by 1024. why not just use a float?
    }
    if (coreNumber == CLIENT_CLASSIC || coreNumber == CLIENT_CATA)
    {
        f.read(&scaleZeroOnly, 4);  // The above three lines introduced a regression bug in Mangos Zero, is Fine for other cores.
        sc = scaleZeroOnly / 1024.0f; // scale factor - divide by 1024. why not just use a float?
    }

    char tempname[512];
    sprintf(tempname, "%s/%s", szWorkDirWmo.c_str(), ModelInstName.c_str());
    FILE* input;
    input = fopen(tempname, "r+b");

    if (!input)
    {
        //printf("ModelInstance::ModelInstance couldn't open %s\n", tempname);
        return;
    }

    fseek(input, 8, SEEK_SET); // get the correct no of vertices
    int nVertices;
    size_t file_read = fread(&nVertices, sizeof(int), 1, input);
    fclose(input);

    if (nVertices == 0 || file_read <= 0)
    {
        return;
    }

    uint16 adtId = 0; // not used for models
    uint32 flags = MOD_M2;
    if (tileX == 65 && tileY == 65)
    {
        flags |= MOD_WORLDSPAWN;
    }
    // write mapID, tileX, tileY, Flags, ID, Pos, Rot, Scale, name
    fwrite(&mapID, sizeof(uint32), 1, pDirfile);
    fwrite(&tileX, sizeof(uint32), 1, pDirfile);
    fwrite(&tileY, sizeof(uint32), 1, pDirfile);
    fwrite(&flags, sizeof(uint32), 1, pDirfile);
    fwrite(&adtId, sizeof(uint16), 1, pDirfile);
    fwrite(&id, sizeof(uint32), 1, pDirfile);
    fwrite(&pos, sizeof(float), 3, pDirfile);
    fwrite(&rot, sizeof(float), 3, pDirfile);
    fwrite(&sc, sizeof(float), 1, pDirfile);
    uint32 nlen = ModelInstName.length();
    fwrite(&nlen, sizeof(uint32), 1, pDirfile);
    fwrite(ModelInstName.c_str(), sizeof(char), nlen, pDirfile);

}

/**
 * @brief Extracts a single model.
 *
 * @param origPath Original path of the model.
 * @param fixedName Fixed name of the model.
 * @param failedPaths Set of failed paths.
 * @param iCoreNumber Core number.
 * @param szRawVMAPMagic VMAP magic string.
 * @param preciseVectorData Whether to use precise vector data.
 * @param szWorkDirWmo Working directory for WMO files.
 * @return true if the model is successfully extracted, false otherwise.
 */
bool ExtractSingleModel(std::string& origPath, std::string& fixedName, std::set<std::string>& failedPaths, int iCoreNumber, std::string szRawVMAPMagic, bool preciseVectorData, std::string szWorkDirWmo)
{
    string ext = GetExtension(origPath);

    // < 3.1.0 ADT MMDX section store filename.mdx filenames for corresponded .m2 file
    if ((ext == "mdx") || (ext=="mdl"))
    {
        // replace .md[l,x] -> .m2
        origPath.erase(origPath.length() - 2, 2);
        origPath.append("2");
    }
    // >= 3.1.0 ADT MMDX section store filename.m2 filenames for corresponded .m2 file
    // nothing do

    fixedName = GetUniformName(origPath);
    std::string output(szWorkDirWmo);                       // Stores output filename
    output += "/";
    output += fixedName;

    if (FileExists(output.c_str()))
    {
        return true;
    }

    Model mdl(origPath);                                    // Possible changed fname
    if (!mdl.open(failedPaths, iCoreNumber))
    {
        return false;
    }

    return mdl.ConvertToVMAPModel(output, iCoreNumber, szRawVMAPMagic, preciseVectorData, szWorkDirWmo);
}

/**
 * @brief Extracts game object models.
 *
 * @param iCoreNumber Core number.
 * @param szRawVMAPMagic VMAP magic string.
 * @param preciseVectorData Whether to use precise vector data.
 * @param szWorkDirWmo Working directory for WMO files.
 */
void ExtractGameobjectModels(int iCoreNumber, std::string szRawVMAPMagic, bool preciseVectorData, std::string szWorkDirWmo)
{
    printf("\n");
    printf("Extracting GameObject models...\n");
    HANDLE dbcHandle;
    if (!OpenNewestFile("DBFilesClient\\GameObjectDisplayInfo.dbc", &dbcHandle))
    {
        printf("Error opening GameObjectDisplayInfo.dbc\n");
        return;
    }
    DBCFile dbc(dbcHandle);
    if (!dbc.open())
    {
        printf("Fatal error: Invalid GameObjectDisplayInfo.dbc file format!\n");
        exit(1);
    }

    std::string basepath = szWorkDirWmo;
    basepath += "/";
    std::string path;
    StringSet failedPaths;

    FILE* model_list = fopen((basepath + "temp_gameobject_models").c_str(), "wb");

    for (DBCFile::Iterator it = dbc.begin(); it != dbc.end(); ++it)
    {
        path = it->getString(1);

        if (path.length() < 4)
        {
            continue;
        }

        string name;

        string ch_ext = GetExtension(path);
        if (ch_ext.empty())
        {
            continue;
        }

        bool result = false;
        if (ch_ext == "wmo")
        {
            name = GetUniformName(path);
            result = ExtractSingleWmo(path, iCoreNumber, szRawVMAPMagic, preciseVectorData, szWorkDirWmo);
        }
        else
        {
            result = ExtractSingleModel(path, name, failedPaths, iCoreNumber, szRawVMAPMagic, preciseVectorData, szWorkDirWmo);
        }

        if (result && FileExists((basepath + name).c_str()))
        {
            uint32 displayId = it->getUInt(0);
            uint32 path_length = name.length();
            fwrite(&displayId, sizeof(uint32), 1, model_list);
            fwrite(&path_length, sizeof(uint32), 1, model_list);
            fwrite(name.c_str(), sizeof(char), path_length, model_list);
        }
    }

    fclose(model_list);

    if (!failedPaths.empty())
    {
        printf("\n Warning: Some models could not be extracted, see below\n");
        for (StringSet::const_iterator itr = failedPaths.begin(); itr != failedPaths.end(); ++itr)
        {
            printf(" Could not find file of model %s\n", itr->c_str());
        }
        printf("\n A few of these warnings are expected to happen, so be not alarmed!\n");
    }

    printf("\n Asset Extraction Complete !\n");
}

