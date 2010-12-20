cmake_minimum_required (VERSION 2.6) 

project (WorldBuilderServer)
set ( PROJECT_PREFIX "StudioPlugins/WorldBuilderServer" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )
Find_Package(wxWidgets REQUIRED)

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of Zen Engine

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( WorldBuilderServer_SRC
    src/ExplorerNodeFactory.cpp
    src/ExplorerNodeFactory.hpp
    src/Project.cpp
    src/Project.hpp
    src/ProjectService.cpp
    src/ProjectService.hpp
    src/ProjectServiceFactory.cpp
    src/ProjectServiceFactory.hpp
    src/WorldBuilderPlugin.cpp
    src/Zone.cpp
    src/Zone.hpp
    src/ZoneFolder.cpp
    src/ZoneFolder.hpp
    src/ZoneModel.cpp
    src/ZoneModel.hpp

    )

add_definitions( -DWORLDBUILDERSERVER_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT}  )

link_directories ( ${DEV_LIB} ${wxWidgets_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS} )

add_library ( WorldBuilderServer ${LIB_TYPE} ${WorldBuilderServer_SRC} )

add_dependencies(WorldBuilderServer ZenCore ZenEngine ZenEnterprise WorkbenchCommon WorkbenchModel WorldBuilderCommon WorldBuilderModel  )

target_link_libraries( WorldBuilderServer ZenCore ZenEngine ZenEnterprise WorkbenchCommon WorkbenchModel WorldBuilderCommon WorldBuilderModel ${Boost_LIBRARIES} )

set_target_properties ( WorldBuilderServer 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( WorldBuilderServer PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( WorldBuilderServer PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	WorldBuilderServer
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
