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

#include "dbcfile.h"
#undef min
#undef max
#include <mpq.h>

#include <cstdio>

/**
 * @brief Constructor that initializes a DBCFile object with a filename.
 *
 * @param filename The name of the DBC file.
 */
DBCFile::DBCFile(const std::string& filename):
    filename(filename),
    fileHandle(NULL),
    recordSize(0),
    recordCount(0),
    fieldCount(0),
    stringSize(0),
    data(0),
    stringTable(0)
{
}

/**
 * @brief Constructor that initializes a DBCFile object with a file handle.
 *
 * @param file The handle to the DBC file.
 */
DBCFile::DBCFile(HANDLE file)
    : fileHandle(file),
      data(0),
      recordSize(0),
      recordCount(0),
      fieldCount(0),
      stringSize(0),
      stringTable(0)
{
}

/**
 * @brief Opens the DBC file and reads its header and data.
 *
 * @return true if the file was successfully opened and read, false otherwise.
 */
bool DBCFile::open()
{
    unsigned char header[4];
    unsigned int na, nb, es, ss;

    // Read the magic header
    if (!SFileReadFile(fileHandle, header, 4, NULL, NULL))
    {
        SFileCloseFile(fileHandle);
        printf("Could not read header in DBCFile %s. err=%u\n", filename.c_str(), GetLastError());
        return false;
    }

    // Check if the header matches "WDBC"
    if (header[0] != 'W' || header[1] != 'D' || header[2] != 'B' || header[3] != 'C')
    {
        SFileCloseFile(fileHandle);
        printf("The header in DBCFile %s did not match. err=%u\n", filename.c_str(), GetLastError());
        return false;
    }

    // Read the number of records
    if (!SFileReadFile(fileHandle, &na, 4, NULL, NULL))
    {
        SFileCloseFile(fileHandle);
        printf("Could not read number of records from DBCFile %s. err=%u\n", filename.c_str(), GetLastError());
        return false;
    }

    // Read the number of fields
    if (!SFileReadFile(fileHandle, &nb, 4, NULL, NULL))
    {
        SFileCloseFile(fileHandle);
        printf("Could not read number of fields from DBCFile %s. err=%u\n", filename.c_str(), GetLastError());
        return false;
    }

    // Read the size of a record
    if (!SFileReadFile(fileHandle, &es, 4, NULL, NULL))
    {
        SFileCloseFile(fileHandle);
        printf("Could not read record size from DBCFile %s. err=%u\n", filename.c_str(), GetLastError());
        return false;
    }

    // Read the string block size
    if (!SFileReadFile(fileHandle, &ss, 4, NULL, NULL))
    {
        SFileCloseFile(fileHandle);
        printf("Could not read string block size from DBCFile %s. err=%u\n", filename.c_str(), GetLastError());
        return false;
    }

    // Set the class members
    recordSize = es;
    recordCount = na;
    fieldCount = nb;
    stringSize = ss;

    // Check if the field count matches the record size
    if (fieldCount * 4 != recordSize)
    {
        SFileCloseFile(fileHandle);
        printf("Field count and record size in DBCFile %s do not match.\n", filename.c_str());
        return false;
    }

    // Allocate memory for the data and string table
    data = new unsigned char[recordSize * recordCount + stringSize];
    stringTable = data + recordSize * recordCount;

    size_t data_size = recordSize * recordCount + stringSize;

    // Read the data and string table
    if (!SFileReadFile(fileHandle, data, data_size, NULL, NULL))
    {
        SFileCloseFile(fileHandle);
        printf("DBCFile %s did not contain expected amount of data for records.\n", filename.c_str());
        return false;
    }

    SFileCloseFile(fileHandle);
    return true;
}

/**
 * @brief Destructor that cleans up the allocated memory.
 */
DBCFile::~DBCFile()
{
    delete [] data;
}

/**
 * @brief Gets a record by its ID.
 *
 * @param id The ID of the record.
 * @return The record.
 */
DBCFile::Record DBCFile::getRecord(size_t id)
{
    assert(data);
    return Record(*this, data + id * recordSize);
}

/**
 * @brief Gets the maximum ID of the records.
 *
 * @return The maximum ID.
 */
size_t DBCFile::getMaxId()
{
    assert(data);

    size_t maxId = 0;
    for (size_t i = 0; i < getRecordCount(); ++i)
    {
        if (maxId < getRecord(i).getUInt(0))
        {
            maxId = getRecord(i).getUInt(0);
        }
    }
    return maxId;
}

/**
 * @brief Gets an iterator to the beginning of the records.
 *
 * @return The iterator.
 */
DBCFile::Iterator DBCFile::begin()
{
    assert(data);
    return Iterator(*this, data);
}

/**
 * @brief Gets an iterator to the end of the records.
 *
 * @return The iterator.
 */
DBCFile::Iterator DBCFile::end()
{
    assert(data);
    return Iterator(*this, stringTable);
}
