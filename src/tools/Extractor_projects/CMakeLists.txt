# MaNGOS is a full featured server for World of Warcraft, supporting
# the following clients: 1.12.x, 2.4.3, 3.3.5a, 4.3.4a and 5.4.8
#
# Copyright (C) 2005-2025 MaNGOS <https://www.getmangos.eu>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

# Used for install targets
set(TOOLS_DIR "tools")
set(EXTRACTOR_BINARIES_DIR "${CMAKE_SOURCE_DIR}/src/tools/Extractor_Binaries")

set(SHARED_SRCS
    shared/ExtractorCommon.cpp
    shared/ExtractorCommon.h
)

#=======================================================#
#map-extractor
#=======================================================#
add_executable(map-extractor
    map-extractor/System.cpp
    map-extractor/assembler.cpp
    map-extractor/dbcfile.cpp
    map-extractor/dbcfile.h
    map-extractor/adtfile.cpp
    map-extractor/adtfile.h
    map-extractor/wdtfile.cpp
    map-extractor/wdtfile.h
    map-extractor/wmo.cpp
    map-extractor/wmo.h
    map-extractor/model.cpp
    map-extractor/model.h
    map-extractor/modelheaders.h
    map-extractor/vec3d.h
    ../../game/vmap/TileAssembler.h
    ../../game/vmap/TileAssembler.cpp
    ${SHARED_SRCS}
    $<$<BOOL:${WIN32}>:map-extractor/map-extractor.rc>
)

target_include_directories(map-extractor
    PUBLIC
        shared
        vmap2
        map-extractor
)

target_link_libraries(map-extractor
    PUBLIC
    loadlib
    shared
    RecastNavigation::Recast
    Threads::Threads
    g3dlite
    RecastNavigation::Detour
    vmap2

#endif()
)

install(
    TARGETS map-extractor
    DESTINATION "${BIN_DIR}/${TOOLS_DIR}"
)

if(WIN32 AND MSVC)
    install(
        FILES $<TARGET_PDB_FILE:map-extractor>
        DESTINATION ${BIN_DIR}/${TOOLS_DIR}
        OPTIONAL)
endif()

if(NOT BUILD_MANGOSD)
if(BUILD_TOOLS)
add_library(vmap2 STATIC
    ../../game/vmap/BIH.cpp
    ../../game/vmap/VMapManager2.cpp
    ../../game/vmap/MapTree.cpp
    ../../game/vmap/TileAssembler.cpp
    ../../game/vmap/WorldModel.cpp
    ../../game/vmap/ModelInstance.cpp
    ../../game/vmap/BIH.h
    ../../game/vmap/VMapManager2.h
    ../../game/vmap/MapTree.h
    ../../game/vmap/TileAssembler.h
    ../../game/vmap/WorldModel.h
    ../../game/vmap/ModelInstance.h
)

target_include_directories(vmap2
    PUBLIC
        ../../game/vmap
        ../../game/WorldHandlers
)

target_compile_definitions(vmap2
    PUBLIC
        MMAP_GENERATOR
        NO_CORE_FUNCS
)
target_link_libraries(vmap2
    PUBLIC
        shared
        g3dlite
        RecastNavigation::Detour
)
endif()
endif()

#=======================================================#
#mmap-extractor
#=======================================================#
add_executable(mmap-extractor
    Movemap-Generator/generator.cpp
    Movemap-Generator/IntermediateValues.cpp
    Movemap-Generator/IntermediateValues.h
    Movemap-Generator/MangosMap.h
    Movemap-Generator/MapBuilder.cpp
    Movemap-Generator/MapBuilder.h
    Movemap-Generator/MMapCommon.h
    Movemap-Generator/TerrainBuilder.cpp
    Movemap-Generator/TerrainBuilder.h
    Movemap-Generator/TileMsgBlock.h
    Movemap-Generator/TileThreadPool.cpp
    Movemap-Generator/TileThreadPool.h
    Movemap-Generator/VMapExtensions.cpp
    shared/ExtractorCommon.cpp
    shared/ExtractorCommon.h
    $<$<BOOL:${WIN32}>:Movemap-Generator/Movemap-Generator.rc>
)

target_include_directories(mmap-extractor
    PUBLIC
        shared
        Movemap-Generator
)

target_link_libraries(mmap-extractor
    PUBLIC
        loadlib
        vmap2
        shared
        RecastNavigation::Recast
        Threads::Threads
        ${OPENSSL_LIBRARIES}
#if (defined(OPENSSL_VERSION_MAJOR) && OPENSSL_VERSION_MAJOR >= 3)
        OpenSSL::Crypto
#endif()


)

install(
    TARGETS mmap-extractor
    DESTINATION ${BIN_DIR}/${TOOLS_DIR}
)

install (
    FILES "${EXTRACTOR_BINARIES_DIR}/MoveMapGen.sh"
         DESTINATION ${BIN_DIR}/${TOOLS_DIR}
)
install (
    FILES "${EXTRACTOR_BINARIES_DIR}/offmesh.txt"
         DESTINATION ${BIN_DIR}/${TOOLS_DIR}
)

install (
    FILES "${EXTRACTOR_BINARIES_DIR}/mmap_excluded.txt"
         DESTINATION ${BIN_DIR}/${TOOLS_DIR}
)

if(WIN32 AND MSVC)
    install(
        FILES $<TARGET_PDB_FILE:mmap-extractor>
        DESTINATION ${BIN_DIR}/${TOOLS_DIR}
        OPTIONAL
)
endif()
