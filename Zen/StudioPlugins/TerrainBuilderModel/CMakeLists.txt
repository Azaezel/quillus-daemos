cmake_minimum_required (VERSION 2.6) 

project (TerrainBuilderModel)
set ( PROJECT_PREFIX "StudioPlugins/TerrainBuilderModel" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of Zen Engine

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( TerrainBuilderModel_SRC
    Configuration.hpp
    I_TerrainChunkDataCollection.hpp
    I_TerrainChunkDataMap.hpp
    I_TerrainChunkDomainObject.hpp
    I_TerrainFolderDataCollection.hpp
    I_TerrainFolderDataMap.hpp
    I_TerrainFolderDomainObject.hpp
    I_TerrainGroupDataCollection.hpp
    I_TerrainGroupDataMap.hpp
    I_TerrainGroupDomainObject.hpp
    src/I_TerrainChunkDataCollection.cpp
    src/I_TerrainChunkDataMap.cpp
    src/I_TerrainChunkDomainObject.cpp
    src/I_TerrainFolderDataCollection.cpp
    src/I_TerrainFolderDataMap.cpp
    src/I_TerrainFolderDomainObject.cpp
    src/I_TerrainGroupDataCollection.cpp
    src/I_TerrainGroupDataMap.cpp
    src/I_TerrainGroupDomainObject.cpp
    src/TerrainChunkDataCollection.cpp
    src/TerrainChunkDataCollection.hpp
    src/TerrainChunkDataMap.cpp
    src/TerrainChunkDataMap.hpp
    src/TerrainChunkDomainObject.cpp
    src/TerrainChunkDomainObject.hpp
    src/TerrainFolderDataCollection.cpp
    src/TerrainFolderDataCollection.hpp
    src/TerrainFolderDataMap.cpp
    src/TerrainFolderDataMap.hpp
    src/TerrainFolderDomainObject.cpp
    src/TerrainFolderDomainObject.hpp
    src/TerrainGroupDataCollection.cpp
    src/TerrainGroupDataCollection.hpp
    src/TerrainGroupDataMap.cpp
    src/TerrainGroupDataMap.hpp
    src/TerrainGroupDomainObject.cpp
    src/TerrainGroupDomainObject.hpp

    )

add_definitions( -DTERRAINBUILDERMODEL_EXPORTS  ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT}  )

link_directories ( ${DEV_LIB} ${wxWidgets_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS} )

add_library ( TerrainBuilderModel ${LIB_TYPE} ${TerrainBuilderModel_SRC} )

add_dependencies(TerrainBuilderModel ZenCore ZenEngine ZenEnterprise )

target_link_libraries( TerrainBuilderModel WorkbenchCommon ${Boost_LIBRARIES} )

set_target_properties ( TerrainBuilderModel 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( TerrainBuilderModel PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( TerrainBuilderModel PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_TerrainChunkDataCollection.hpp
    I_TerrainChunkDataMap.hpp
    I_TerrainChunkDomainObject.hpp
    I_TerrainFolderDataCollection.hpp
    I_TerrainFolderDataMap.hpp
    I_TerrainFolderDomainObject.hpp
    I_TerrainGroupDataCollection.hpp
    I_TerrainGroupDataMap.hpp
    I_TerrainGroupDomainObject.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	TerrainBuilderModel
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)