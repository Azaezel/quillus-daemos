cmake_minimum_required (VERSION 2.6) 

project (BaseCommon)
set ( PROJECT_PREFIX "Starter/Base/BaseCommon" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include BaseCommon source file

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( BaseCommon_SRC
    Configuration.hpp
    I_BaseAction.hpp
    I_BaseGame.hpp
    I_BaseGameObject.hpp
    src/Action.cpp
    src/Action.hpp
    src/ActionMap.cpp
    src/ActionMap.hpp
    src/BaseGame.cpp
    src/BaseGame.hpp
    src/BehavioredGameObject.cpp
    src/BehavioredGameObject.hpp
    src/BehaviorService.cpp
    src/BehaviorService.hpp
    src/GameGroup.cpp
    src/GameGroup.hpp
    src/GameObject.cpp
    src/GameObject.hpp
    src/GameObjectBehaviors.cpp
    src/GameObjectBehaviors.hpp
    src/I_BaseAction.cpp
    src/I_BaseGame.cpp
    src/I_BaseGameObject.cpp

    )

add_definitions( -DBASECOMMON_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( BaseCommon ${LIB_TYPE} ${BaseCommon_SRC} )

add_dependencies( BaseCommon ZenCore ZenEngine )

target_link_libraries( BaseCommon ZenCore ZenEngine )

set_target_properties ( BaseCommon 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( BaseCommon PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( BaseCommon PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_BaseAction.hpp
    I_BaseGame.hpp
    I_BaseGameObject.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	BaseCommon
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)