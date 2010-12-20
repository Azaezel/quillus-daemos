cmake_minimum_required (VERSION 2.6) 

project (ZOgreTerrain)

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED system serialization thread)
Find_Package(OGRE REQUIRED)

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include ZTerrain source file

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( ZOgreTerrain_SRC
    Configuration.hpp
    src/Terrain.cpp
    src/Terrain.hpp
    src/TerrainChunk.cpp
    src/TerrainChunk.hpp
    src/TerrainGroup.cpp
    src/TerrainGroup.hpp
    src/TerrainHeightfield.cpp
    src/TerrainHeightfield.hpp
    src/TerrainService.cpp
    src/TerrainService.hpp
    src/TerrainServiceFactory.cpp
    src/TerrainServiceFactory.hpp
    src/ZOgreTerrainModule.cpp
    src/ZOgreTerrainPlugin.cpp
    src/ZOgreTerrainPlugin.hpp
    ZOgreTerrainModule.hpp

    )

add_definitions( -DZOGRETERRAIN_EXPORTS -DBOOST_ALL_DYN_LINK)

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${OGRE_INCLUDE_DIRS} ${OGRE_Paging_INCLUDE_DIRS} ${OGRE_Terrain_INCLUDE_DIRS} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS})

add_library ( ZOgreTerrain ${LIB_TYPE} ${ZOgreTerrain_SRC} )

add_dependencies( ZOgreTerrain ZenCore ZenEngine ZOgre )

target_link_libraries( ZOgreTerrain ZenCore ZenEngine ZOgre ${OGRE_LIBRARIES} ${OGRE_Paging_LIBRARIES} ${OGRE_Terrain_LIBRARIES})

set_target_properties ( ZOgreTerrain 
	PROPERTIES
	DEBUG_POSTFIX "_d"
)

if (MSVC_IDE)
	# hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
	set_target_properties ( ZOgreTerrain PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZOgreTerrain PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)

