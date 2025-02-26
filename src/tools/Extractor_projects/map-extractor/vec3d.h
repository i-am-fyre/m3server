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

#ifndef VEC3D_H
#define VEC3D_H

#include <iostream>
#include <cmath>

/**
 * @brief Class representing a 3D vector.
 */
class Vec3D
{
    public:
        float x, y, z; /**< The x, y, and z coordinates of the vector. */

        /**
         * @brief Constructs a Vec3D object with given coordinates.
         *
         * @param x0 The x coordinate.
         * @param y0 The y coordinate.
         * @param z0 The z coordinate.
         */
        Vec3D(float x0 = 0.0f, float y0 = 0.0f, float z0 = 0.0f) : x(x0), y(y0), z(z0) {}

        /**
         * @brief Copy constructor.
         *
         * @param v The vector to copy.
         */
        Vec3D(const Vec3D& v) : x(v.x), y(v.y), z(v.z) {}

        /**
         * @brief Assignment operator.
         *
         * @param v The vector to assign.
         * @return Reference to the assigned vector.
         */
        Vec3D& operator= (const Vec3D& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
            return *this;
        }

        /**
         * @brief Adds two vectors.
         *
         * @param v The vector to add.
         * @return The resulting vector.
         */
        Vec3D operator+ (const Vec3D& v) const
        {
            Vec3D r(x + v.x, y + v.y, z + v.z);
            return r;
        }

        /**
         * @brief Subtracts two vectors.
         *
         * @param v The vector to subtract.
         * @return The resulting vector.
         */
        Vec3D operator- (const Vec3D& v) const
        {
            Vec3D r(x - v.x, y - v.y, z - v.z);
            return r;
        }

        /**
         * @brief Dot product of two vectors.
         *
         * @param v The vector to dot with.
         * @return The dot product.
         */
        float operator* (const Vec3D& v) const
        {
            return x * v.x + y * v.y + z * v.z;
        }

        /**
         * @brief Multiplies the vector by a scalar.
         *
         * @param d The scalar to multiply by.
         * @return The resulting vector.
         */
        Vec3D operator* (float d) const
        {
            Vec3D r(x * d, y * d, z * d);
            return r;
        }

        /**
         * @brief Friend function to multiply a scalar by a vector.
         *
         * @param d The scalar to multiply by.
         * @param v The vector to multiply.
         * @return The resulting vector.
         */
        friend Vec3D operator* (float d, const Vec3D& v)
        {
            return v * d;
        }

        /**
         * @brief Cross product of two vectors.
         *
         * @param v The vector to cross with.
         * @return The resulting vector.
         */
        Vec3D operator% (const Vec3D& v) const
        {
            Vec3D r(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
            return r;
        }

        /**
         * @brief Adds another vector to this vector.
         *
         * @param v The vector to add.
         * @return Reference to the resulting vector.
         */
        Vec3D& operator+= (const Vec3D& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        /**
         * @brief Subtracts another vector from this vector.
         *
         * @param v The vector to subtract.
         * @return Reference to the resulting vector.
         */
        Vec3D& operator-= (const Vec3D& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        /**
         * @brief Multiplies this vector by a scalar.
         *
         * @param d The scalar to multiply by.
         * @return Reference to the resulting vector.
         */
        Vec3D& operator*= (float d)
        {
            x *= d;
            y *= d;
            z *= d;
            return *this;
        }

        /**
         * @brief Calculates the squared length of the vector.
         *
         * @return The squared length.
         */
        float lengthSquared() const
        {
            return x * x + y * y + z * z;
        }

        /**
         * @brief Calculates the length of the vector.
         *
         * @return The length.
         */
        float length() const
        {
            return sqrt(x * x + y * y + z * z);
        }

        /**
         * @brief Normalizes the vector.
         *
         * @return Reference to the normalized vector.
         */
        Vec3D& normalize()
        {
            this->operator*= (1.0f / length());
            return *this;
        }

        /**
         * @brief Returns a normalized copy of the vector.
         *
         * @return The normalized vector.
         */
        Vec3D operator~() const
        {
            Vec3D r(*this);
            r.normalize();
            return r;
        }

        /**
         * @brief Input stream operator for Vec3D.
         *
         * @param in The input stream.
         * @param v The vector to read into.
         * @return Reference to the input stream.
         */
        friend std::istream& operator>>(std::istream& in, Vec3D& v)
        {
            in >> v.x >> v.y >> v.z;
            return in;
        }

        /**
         * @brief Output stream operator for Vec3D.
         *
         * @param out The output stream.
         * @param v The vector to write.
         * @return Reference to the output stream.
         */
        friend std::ostream& operator<<(std::ostream& out, const Vec3D& v)
        {
            out << v.x << " " << v.y << " " << v.z;
            return out;
        }

        /**
         * @brief Conversion operator to float pointer.
         *
         * @return Pointer to the vector's data.
         */
        operator float* ()
        {
            return (float*)this;
        }
};

/**
 * @brief Rotates a point around another point by a given angle.
 *
 * @param x0 The x coordinate of the center point.
 * @param y0 The y coordinate of the center point.
 * @param x The x coordinate of the point to rotate.
 * @param y The y coordinate of the point to rotate.
 * @param angle The angle to rotate by.
 */
inline void rotate(float x0, float y0, float* x, float* y, float angle)
{
    float xa = *x - x0, ya = *y - y0;
    *x = xa * cosf(angle) - ya * sinf(angle) + x0;
    *y = xa * sinf(angle) + ya * cosf(angle) + y0;
}

/**
 * @brief Fixes the coordinates of a vector by swapping its components.
 *
 * @param v The vector to fix.
 * @return The fixed vector.
 */
inline Vec3D fixCoords(const Vec3D& v) { return Vec3D(v.z, v.x, v.y); }

#endif // VEC3D_H

