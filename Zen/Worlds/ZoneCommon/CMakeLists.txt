cmake_minimum_required (VERSION 2.6) 

project (ZoneCommon)
set ( PROJECT_PREFIX "Worlds/ZoneCommon" )

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


	
set ( ZoneCommon_SRC
    Configuration.hpp
    I_ZoneControllable.hpp
    I_ZoneGroup.hpp
    I_ZoneObject.hpp
    I_ZoneObjectListener.hpp
    I_ZonePortal.hpp
    I_ZonePortalListener.hpp
    I_ZoneService.hpp
    I_ZoneSession.hpp
    src/I_ZoneControllable.cpp
    src/I_ZoneObject.cpp
    src/I_ZoneService.cpp
    src/I_ZoneSession.cpp

    )

add_definitions( -DZONECOMMON_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( ZoneCommon ${LIB_TYPE} ${ZoneCommon_SRC} )

add_dependencies(ZoneCommon ZenCore ZenEngine ZenEnterprise WorldCommon ZenObjectModel)

target_link_libraries(ZoneCommon ZenCore ZenEngine ZenEnterprise WorldCommon ZenObjectModel ${Boost_LIBRARIES} )

set_target_properties ( ZoneCommon
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZoneCommon PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZoneCommon PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_ZoneControllable.hpp
    I_ZoneGroup.hpp
    I_ZoneObject.hpp
    I_ZoneObjectListener.hpp
    I_ZonePortal.hpp
    I_ZonePortalListener.hpp
    I_ZoneService.hpp
    I_ZoneSession.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ZoneCommon
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)