cmake_minimum_required (VERSION 2.6) 

project (SessionCommon)
set ( PROJECT_PREFIX "Community/SessionCommon" )

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


	
set ( SessionCommon_SRC
    Configuration.hpp
    I_Attribute.hpp
    I_Session.hpp
    I_SessionService.hpp
    src/I_Attribute.cpp
    src/I_Session.cpp
    src/I_SessionService.cpp

    )

add_definitions( -DSESSIONCOMMON_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( SessionCommon ${LIB_TYPE} ${SessionCommon_SRC} )

add_dependencies(SessionCommon ZenCore ZenEngine ZenEnterprise)

target_link_libraries(SessionCommon ZenCore ZenEngine ZenEnterprise ${Boost_LIBRARIES} )

set_target_properties ( SessionCommon
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( SessionCommon PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( SessionCommon PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_Attribute.hpp
    I_Session.hpp
    I_SessionService.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	SessionCommon
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)