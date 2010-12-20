cmake_minimum_required (VERSION 2.6) 

project (WorldBuilderCommon)
set ( PROJECT_PREFIX "StudioPlugins/WorldBuilderCommon" )

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


	
set ( WorldBuilderCommon_SRC
    Configuration.hpp
    I_Project.hpp
    I_ProjectService.hpp
    I_WorldBuilderService.hpp
    I_Zone.hpp
    I_ZoneFolder.hpp
    I_ZoneModel.hpp
    src/I_Project.cpp
    src/I_ProjectService.cpp
    src/I_WorldBuilderService.cpp
    src/I_Zone.cpp
    src/I_ZoneFolder.cpp
    src/I_ZoneModel.cpp

    )

add_definitions( -DWorldBuilderCommon_EXPORTS -DWORLDBUILDERCOMMON_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( WorldBuilderCommon ${LIB_TYPE} ${WorldBuilderCommon_SRC} )

add_dependencies(WorldBuilderCommon ZenCore ZenEnterprise WorkbenchCommon ${Boost_LIBRARIES} )

target_link_libraries( WorldBuilderCommon )

set_target_properties ( WorldBuilderCommon 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( WorldBuilderCommon PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( WorldBuilderCommon PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_Project.hpp
    I_ProjectService.hpp
    I_WorldBuilderService.hpp
    I_Zone.hpp
    I_ZoneFolder.hpp
    I_ZoneModel.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	WorldBuilderCommon
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)