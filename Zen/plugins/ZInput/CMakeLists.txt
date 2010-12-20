cmake_minimum_required (VERSION 2.6) 

project (ZInput)
set ( PROJECT_PREFIX "plugins/ZInput" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include ZInput source file

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( ZInput_SRC
    Configuration.hpp
    src/InputService.cpp
    src/InputService.hpp
    src/InputServiceFactory.cpp
    src/InputServiceFactory.hpp
    src/KeyEvent.cpp
    src/KeyEvent.hpp
    src/KeyModifierState.cpp
    src/KeyModifierState.hpp
    src/KeyState.cpp
    src/KeyState.hpp
    src/MouseClickEvent.cpp
    src/MouseClickEvent.hpp
    src/MouseMoveEvent.cpp
    src/MouseMoveEvent.hpp
    src/VectorEvent.cpp
    src/VectorEvent.hpp
    src/ZInputModule.cpp
    src/ZInputPlugin.cpp
    src/ZInputPlugin.hpp
    ZInputModule.hpp

    )

add_definitions( -DZINPUT_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${OIS_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( ZInput ${LIB_TYPE} ${ZInput_SRC} )

add_dependencies( ZInput ZenCore ZenEngine )

target_link_libraries( ZInput ZenCore ZenEngine ${OIS_LIBRARIES} )

set_target_properties ( ZInput 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZInput PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZInput PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    plugin.xml
    ZInputModule.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ZInput
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)