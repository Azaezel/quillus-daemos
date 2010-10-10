cmake_minimum_required (VERSION 2.6) 

project (WorldCommon)
set ( PROJECT_PREFIX "Worlds/WorldCommon" )

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


	
set ( WorldCommon_SRC
    I_BehaviorManager.hpp
    I_WorldService.hpp
    I_WorldSession.hpp
    I_ZoneId.hpp
    src/I_BehaviorManager.cpp
    src/BehaviorManager.cpp
    src/BehaviorManager.hpp
    src/I_ZoneId.cpp
    src/GravityBehavior.hpp
    I_WorldEventQueue.hpp
    Configuration.hpp

    )

add_definitions( -DWORLDCOMMON_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( WorldCommon ${LIB_TYPE} ${WorldCommon_SRC} )

add_dependencies(WorldCommon ZenCore ZenEngine ZenEnterprise)

target_link_libraries(WorldCommon ZenCore ZenEngine ZenEnterprise ${Boost_LIBRARIES} )

set_target_properties ( WorldCommon
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( WorldCommon PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( WorldCommon PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    I_BehaviorManager.hpp
    I_WorldService.hpp
    I_WorldSession.hpp
    I_ZoneId.hpp
    I_WorldEventQueue.hpp
    Configuration.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	WorldCommon
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)