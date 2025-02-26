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

#ifndef DBCFILE_H
#define DBCFILE_H

#include <cassert>
#include <string>
#include "StormLib.h"

/**
 * @brief Class representing a DBC (DataBase Client) file.
 */
class DBCFile
{
    public:
        /**
         * @brief Constructs a DBCFile object with the given filename.
         *
         * @param filename The name of the DBC file.
         */
        DBCFile(const std::string& filename);
        /**
         * @brief Constructs a DBCFile object with the given file handle.
         *
         * @param file The handle to the DBC file.
         */
        DBCFile(HANDLE file);
        /**
         * @brief Destroys the DBCFile object.
         */
        ~DBCFile();

        /**
         * @brief Opens the DBC file. It must be opened before it can be used.
         *
         * @return True if the file was successfully opened, false otherwise.
         */
        bool open();

        /**
         * @brief Class representing exceptions related to DBC files.
         */
        class Exception
        {
            public:
                /**
                 * @brief Constructs an Exception object with the given message.
                 *
                 * @param message The exception message.
                 */
                Exception(const std::string& message): message(message) { }
                /**
                 * @brief Destroys the Exception object.
                 */
                virtual ~Exception() { }
                /**
                 * @brief Gets the exception message.
                 *
                 * @return The exception message.
                 */
                const std::string& getMessage() { return message; }
            private:
                std::string message; /**< The exception message. */
        };

        /**
         * @brief Class representing a "not found" exception.
         */
        class NotFound: public Exception
        {
            public:
                /**
                 * @brief Constructs a NotFound exception.
                 */
                NotFound(): Exception("Key was not found") { }
        };

        class Iterator;

        /**
         * @brief Class representing a record in the DBC file.
         */
        class Record
        {
            public:
                /**
                 * @brief Assignment operator for Record.
                 *
                 * @param r The record to assign.
                 * @return Reference to the assigned record.
                 */
                Record& operator= (const Record& r)
                {
                    file = r.file;
                    offset = r.offset;
                    return *this;
                }

                /**
                 * @brief Gets a float value from the specified field.
                 *
                 * @param field The field index.
                 * @return The float value.
                 */
                float getFloat(size_t field) const
                {
                    assert(field < file.fieldCount);
                    return *reinterpret_cast<float*>(offset + (field * 4));
                }

                /**
                 * @brief Gets an unsigned int value from the specified field.
                 *
                 * @param field The field index.
                 * @return The unsigned int value.
                 */
                unsigned int getUInt(size_t field) const
                {
                    assert(field < file.fieldCount);
                    return *reinterpret_cast<unsigned int*>(offset + (field * 4));
                }

                /**
                 * @brief Gets an int value from the specified field.
                 *
                 * @param field The field index.
                 * @return The int value.
                 */
                int getInt(size_t field) const
                {
                    assert(field < file.fieldCount);
                    return *reinterpret_cast<int*>(offset + (field * 4));
                }

                /**
                 * @brief Gets a byte value from the specified offset.
                 *
                 * @param ofs The offset.
                 * @return The byte value.
                 */
                unsigned char getByte(size_t ofs) const
                {
                    assert(ofs < file.recordSize);
                    return *reinterpret_cast<unsigned char*>(offset + ofs);
                }

                /**
                 * @brief Gets a string value from the specified field.
                 *
                 * @param field The field index.
                 * @return The string value.
                 */
                const char* getString(size_t field) const
                {
                    assert(field < file.fieldCount);
                    size_t stringOffset = getUInt(field);
                    assert(stringOffset < file.stringSize);
                    return reinterpret_cast<char*>(file.stringTable + stringOffset);
                }
            private:
                /**
                 * @brief Constructs a Record object.
                 *
                 * @param file The DBC file.
                 * @param offset The offset to the record.
                 */
                Record(DBCFile& file, unsigned char* offset): file(file), offset(offset) {}
                DBCFile& file; /**< Reference to the DBC file. */
                unsigned char* offset; /**< Offset to the record. */

                friend class DBCFile;
                friend class DBCFile::Iterator;
        };

        /**
         * @brief Class representing an iterator that iterates over records in the DBC file.
         */
        class Iterator
        {
            public:
                /**
                 * @brief Constructs an Iterator object.
                 *
                 * @param file The DBC file.
                 * @param offset The offset to the record.
                 */
                Iterator(DBCFile& file, unsigned char* offset): record(file, offset) {}

                /**
                 * @brief Advances the iterator (prefix only).
                 *
                 * @return Reference to the advanced iterator.
                 */
                Iterator& operator++()
                {
                    record.offset += record.file.recordSize;
                    return *this;
                }

                /**
                 * @brief Dereferences the iterator to get the current record.
                 *
                 * @return Reference to the current record.
                 */
                Record const& operator*() const { return record; }

                /**
                 * @brief Dereferences the iterator to get the current record.
                 *
                 * @return Pointer to the current record.
                 */
                const Record* operator->() const
                {
                    return &record;
                }
                /**
                 * @brief Compares two iterators for equality.
                 *
                 * @param b The iterator to compare with.
                 * @return True if the iterators are equal, false otherwise.
                 */
                bool operator==(const Iterator& b) const
                {
                    return record.offset == b.record.offset;
                }
                /**
                 * @brief Compares two iterators for inequality.
                 *
                 * @param b The iterator to compare with.
                 * @return True if the iterators are not equal, false otherwise.
                 */
                bool operator!=(const Iterator& b) const
                {
                    return record.offset != b.record.offset;
                }
            private:
                Record record; /**< The current record. */
        };

        /**
         * @brief Gets a record by its ID.
         *
         * @param id The record ID.
         * @return The record.
         */
        Record getRecord(size_t id);

        /**
         * @brief Gets an iterator to the beginning of the records.
         *
         * @return The iterator.
         */
        Iterator begin();

        /**
         * @brief Gets an iterator to the end of the records.
         *
         * @return The iterator.
         */
        Iterator end();

        /**
         * @brief Gets the number of records in the DBC file.
         *
         * @return The number of records.
         */
        size_t getRecordCount() const { return recordCount; }

        /**
         * @brief Gets the number of fields in each record.
         *
         * @return The number of fields.
         */
        size_t getFieldCount() const { return fieldCount; }

        /**
         * @brief Gets the maximum ID of the records.
         *
         * @return The maximum ID.
         */
        size_t getMaxId();
    private:
        std::string filename; /**< The name of the DBC file. */
        HANDLE fileHandle; /**< The handle to the DBC file. */
        size_t recordSize; /**< The size of each record. */
        size_t recordCount; /**< The number of records. */
        size_t fieldCount; /**< The number of fields in each record. */
        size_t stringSize; /**< The size of the string table. */
        unsigned char* data; /**< Pointer to the data. */
        unsigned char* stringTable; /**< Pointer to the string table. */
};

#endif // DBCFILE_H
