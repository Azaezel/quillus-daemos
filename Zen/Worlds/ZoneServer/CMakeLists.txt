cmake_minimum_required (VERSION 2.6) 

project (ZoneServer)
set ( PROJECT_PREFIX "Worlds/ZoneServer" )

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


	
set ( ZoneServer_SRC
    Configuration.hpp
    I_ZonePortalScopator.hpp
    src/I_ZonePortalScopator.cpp

    )

add_definitions( -DZONESERVER_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( ZoneServer ${LIB_TYPE} ${ZoneServer_SRC} )

add_dependencies(ZoneServer ZenCore ZenEngine ZenEnterprise WorldCommon ZoneCommon)

target_link_libraries(ZoneServer ZenCore ZenEngine ZenEnterprise WorldCommon ZoneCommon ${Boost_LIBRARIES} )

set_target_properties ( ZoneServer
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZoneServer PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZoneServer PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_ZonePortalScopator.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ZoneServer
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)