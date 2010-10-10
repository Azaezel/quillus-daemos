cmake_minimum_required (VERSION 2.6) 

project (ZMicroPather)
set ( PROJECT_PREFIX "plugins/ZMicroPather" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include ZMicroPather source file

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( ZMicroPather_SRC
    src/Graph.hpp
    src/ZMicroPatherPlugin.cpp
    src/Graph.cpp
    src/NavigationService.hpp
    src/NavigationService.cpp
    src/Path.hpp
    src/micropather.cpp
    src/Path.cpp
    src/NavigationServiceFactory.cpp
    src/NavigationServiceFactory.hpp
    src/NodeIterator.cpp
    src/NodeIterator.hpp
    src/micropather.hpp

    )

add_definitions( -DZMICROPATHER_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( ZMicroPather ${LIB_TYPE} ${ZMicroPather_SRC} )

add_dependencies( ZMicroPather ZenCore ZenEngine )

target_link_libraries( ZMicroPather ZenCore ZenEngine )

set_target_properties ( ZMicroPather 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZMicroPather PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZMicroPather PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ZMicroPather
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)