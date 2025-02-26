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

#ifndef MODELHEADERS_H
#define MODELHEADERS_H

#pragma pack(push,1)

/**
 * @brief Struct representing the header of a model for Classic and TBC versions.
 */
struct ModelHeaderClassicTBC
{
    char id[4]; /**< Model ID. */
    uint8 version[4]; /**< Model version. */
    uint32 nameLength; /**< Length of the model name. */
    uint32 nameOfs; /**< Offset to the model name. */
    uint32 type; /**< Model type. */
    uint32 nGlobalSequences; /**< Number of global sequences. */
    uint32 ofsGlobalSequences; /**< Offset to global sequences. */
    uint32 nAnimations; /**< Number of animations. */
    uint32 ofsAnimations; /**< Offset to animations. */
    uint32 nAnimationLookup; /**< Number of animation lookups. */
    uint32 ofsAnimationLookup; /**< Offset to animation lookups. */
    uint32 nD; /**< Number of D (unknown). */
    uint32 ofsD; /**< Offset to D (unknown). */
    uint32 nBones; /**< Number of bones. */
    uint32 ofsBones; /**< Offset to bones. */
    uint32 nKeyBoneLookup; /**< Number of key bone lookups. */
    uint32 ofsKeyBoneLookup; /**< Offset to key bone lookups. */
    uint32 nVertices; /**< Number of vertices. */
    uint32 ofsVertices; /**< Offset to vertices. */
    uint32 nViews; /**< Number of views. */
    uint32 ofsViews; /**< Offset to views. */
    uint32 nColors; /**< Number of colors. */
    uint32 ofsColors; /**< Offset to colors. */
    uint32 nTextures; /**< Number of textures. */
    uint32 ofsTextures; /**< Offset to textures. */
    uint32 nTransparency; /**< Number of transparency entries. */
    uint32 ofsTransparency; /**< Offset to transparency entries. */
    uint32 nI; /**< Number of I (unknown). */
    uint32 ofsI; /**< Offset to I (unknown). */
    uint32 nTextureanimations; /**< Number of texture animations. */
    uint32 ofsTextureanimations; /**< Offset to texture animations. */
    uint32 nTexReplace; /**< Number of texture replacements. */
    uint32 ofsTexReplace; /**< Offset to texture replacements. */
    uint32 nRenderFlags; /**< Number of render flags. */
    uint32 ofsRenderFlags; /**< Offset to render flags. */
    uint32 nBoneLookupTable; /**< Number of bone lookup table entries. */
    uint32 ofsBoneLookupTable; /**< Offset to bone lookup table. */
    uint32 nTexLookup; /**< Number of texture lookups. */
    uint32 ofsTexLookup; /**< Offset to texture lookups. */
    uint32 nTexUnits; /**< Number of texture units. */
    uint32 ofsTexUnits; /**< Offset to texture units. */
    uint32 nTransLookup; /**< Number of transparency lookups. */
    uint32 ofsTransLookup; /**< Offset to transparency lookups. */
    uint32 nTexAnimLookup; /**< Number of texture animation lookups. */
    uint32 ofsTexAnimLookup; /**< Offset to texture animation lookups. */
    float floats[14]; /**< Array of floats (unknown purpose). */
    uint32 nBoundingTriangles; /**< Number of bounding triangles. */
    uint32 ofsBoundingTriangles; /**< Offset to bounding triangles. */
    uint32 nBoundingVertices; /**< Number of bounding vertices. */
    uint32 ofsBoundingVertices; /**< Offset to bounding vertices. */
    uint32 nBoundingNormals; /**< Number of bounding normals. */
    uint32 ofsBoundingNormals; /**< Offset to bounding normals. */
    uint32 nAttachments; /**< Number of attachments. */
    uint32 ofsAttachments; /**< Offset to attachments. */
    uint32 nAttachLookup; /**< Number of attachment lookups. */
    uint32 ofsAttachLookup; /**< Offset to attachment lookups. */
    uint32 nAttachments_2; /**< Number of secondary attachments. */
    uint32 ofsAttachments_2; /**< Offset to secondary attachments. */
    uint32 nLights; /**< Number of lights. */
    uint32 ofsLights; /**< Offset to lights. */
    uint32 nCameras; /**< Number of cameras. */
    uint32 ofsCameras; /**< Offset to cameras. */
    uint32 nCameraLookup; /**< Number of camera lookups. */
    uint32 ofsCameraLookup; /**< Offset to camera lookups. */
    uint32 nRibbonEmitters; /**< Number of ribbon emitters. */
    uint32 ofsRibbonEmitters; /**< Offset to ribbon emitters. */
    uint32 nParticleEmitters; /**< Number of particle emitters. */
    uint32 ofsParticleEmitters; /**< Offset to particle emitters. */
};

/**
 * @brief Struct representing the header of a model for other versions.
 */
struct ModelHeaderOthers
{
    char id[4]; /**< Model ID. */
    uint8 version[4]; /**< Model version. */
    uint32 nameLength; /**< Length of the model name. */
    uint32 nameOfs; /**< Offset to the model name. */
    uint32 type; /**< Model type. */
    uint32 nGlobalSequences; /**< Number of global sequences. */
    uint32 ofsGlobalSequences; /**< Offset to global sequences. */
    uint32 nAnimations; /**< Number of animations. */
    uint32 ofsAnimations; /**< Offset to animations. */
    uint32 nAnimationLookup; /**< Number of animation lookups. */
    uint32 ofsAnimationLookup; /**< Offset to animation lookups. */
    uint32 nBones; /**< Number of bones. */
    uint32 ofsBones; /**< Offset to bones. */
    uint32 nKeyBoneLookup; /**< Number of key bone lookups. */
    uint32 ofsKeyBoneLookup; /**< Offset to key bone lookups. */
    uint32 nVertices; /**< Number of vertices. */
    uint32 ofsVertices; /**< Offset to vertices. */
    uint32 nViews; /**< Number of views. */
    uint32 nColors; /**< Number of colors. */
    uint32 ofsColors; /**< Offset to colors. */
    uint32 nTextures; /**< Number of textures. */
    uint32 ofsTextures; /**< Offset to textures. */
    uint32 nTransparency; /**< Number of transparency entries. */
    uint32 ofsTransparency; /**< Offset to transparency entries. */
    uint32 nTextureanimations; /**< Number of texture animations. */
    uint32 ofsTextureanimations; /**< Offset to texture animations. */
    uint32 nTexReplace; /**< Number of texture replacements. */
    uint32 ofsTexReplace; /**< Offset to texture replacements. */
    uint32 nRenderFlags; /**< Number of render flags. */
    uint32 ofsRenderFlags; /**< Offset to render flags. */
    uint32 nBoneLookupTable; /**< Number of bone lookup table entries. */
    uint32 ofsBoneLookupTable; /**< Offset to bone lookup table. */
    uint32 nTexLookup; /**< Number of texture lookups. */
    uint32 ofsTexLookup; /**< Offset to texture lookups. */
    uint32 nTexUnits; /**< Number of texture units. */
    uint32 ofsTexUnits; /**< Offset to texture units. */
    uint32 nTransLookup; /**< Number of transparency lookups. */
    uint32 ofsTransLookup; /**< Offset to transparency lookups. */
    uint32 nTexAnimLookup; /**< Number of texture animation lookups. */
    uint32 ofsTexAnimLookup; /**< Offset to texture animation lookups. */
    float floats[14]; /**< Array of floats (unknown purpose). */
    uint32 nBoundingTriangles; /**< Number of bounding triangles. */
    uint32 ofsBoundingTriangles; /**< Offset to bounding triangles. */
    uint32 nBoundingVertices; /**< Number of bounding vertices. */
    uint32 ofsBoundingVertices; /**< Offset to bounding vertices. */
    uint32 nBoundingNormals; /**< Number of bounding normals. */
    uint32 ofsBoundingNormals; /**< Offset to bounding normals. */
    uint32 nAttachments; /**< Number of attachments. */
    uint32 ofsAttachments; /**< Offset to attachments. */
    uint32 nAttachLookup; /**< Number of attachment lookups. */
    uint32 ofsAttachLookup; /**< Offset to attachment lookups. */
    uint32 nAttachments_2; /**< Number of secondary attachments. */
    uint32 ofsAttachments_2; /**< Offset to secondary attachments. */
    uint32 nLights; /**< Number of lights. */
    uint32 ofsLights; /**< Offset to lights. */
    uint32 nCameras; /**< Number of cameras. */
    uint32 ofsCameras; /**< Offset to cameras. */
    uint32 nCameraLookup; /**< Number of camera lookups. */
    uint32 ofsCameraLookup; /**< Offset to camera lookups. */
    uint32 nRibbonEmitters; /**< Number of ribbon emitters. */
    uint32 ofsRibbonEmitters; /**< Offset to ribbon emitters. */
    uint32 nParticleEmitters; /**< Number of particle emitters. */
    uint32 ofsParticleEmitters; /**< Offset to particle emitters. */
};

/**
 * @brief Struct representing a bounding vertex in a model.
 */
struct ModelBoundingVertex
{
    Vec3D pos; /**< Position of the bounding vertex. */
};

#pragma pack(pop)
#endif // MODELHEADERS_H
