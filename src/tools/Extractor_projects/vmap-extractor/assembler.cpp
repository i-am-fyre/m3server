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

#include "TileAssembler.h"
#include <string>

/**
 * AssembleVMAP - Assembles VMAP files from source to destination.
 *
 * @param src The source directory containing the VMAP files.
 * @param dest The destination directory where the assembled VMAP files will be stored.
 * @param szMagic A string representing the magic number for the VMAP files (currently unused).
 * @return true if the assembly was successful, false otherwise.
 */
bool AssembleVMAP(std::string src, std::string dest, std::string szMagic)
{
    bool success = true;

    // Create a new TileAssembler object with the source and destination directories.
    VMAP::TileAssembler* ta = new VMAP::TileAssembler(src, dest);

    // Convert the world data to VMAP format.
    // The szMagic parameter is currently not used in the convertWorld2 function.
    if (!ta->convertWorld2())
    {
        success = false; // Set success to false if the conversion fails.
    }

    // Clean up the TileAssembler object.
    delete ta;
    return success; // Return the success status.
}
