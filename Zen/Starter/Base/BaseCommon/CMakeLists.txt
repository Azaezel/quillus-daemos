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
    I_BaseGame.hpp
    src/Action.hpp
    src/I_BaseGame.cpp
    src/BaseGame.cpp
    src/BehavioredGameObject.cpp
    src/BehavioredGameObject.hpp
    src/BehaviorService.hpp
    src/BaseGame.hpp
    src/ActionMap.cpp
    src/Action.cpp
    src/GameGroup.hpp
    src/I_BaseAction.cpp
    src/GameGroup.cpp
    src/GameObject.hpp
    src/GameObjectBehaviors.cpp
    src/BehaviorService.cpp
    src/ActionMap.hpp
    src/I_BaseGameObject.cpp
    src/GameObjectBehaviors.hpp
    src/GameObject.cpp
    I_BaseGameObject.hpp
    I_BaseAction.hpp
    Configuration.hpp

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
    I_BaseGame.hpp
    I_BaseGameObject.hpp
    I_BaseAction.hpp
    Configuration.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	BaseCommon
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)