cmake_minimum_required (VERSION 2.6) 

project (ZNewton)
set ( PROJECT_PREFIX "plugins/ZNewton" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED system )

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include ZNewton source file

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( ZNewton_SRC
    ZNewtonModule.hpp
    src/PhysicsServiceFactory.hpp
    src/PhysicsResourceService.cpp
    src/PhysicsServiceFactory.cpp
    src/PhysicsService.cpp
    src/CollisionShape.hpp
    src/ZNewtonModule.cpp
    src/PhysicsZone.hpp
    src/PhysicsActor.hpp
    src/PhysicsActor.cpp
    src/ZNewtonPlugin.cpp
    src/PhysicsMaterial.hpp
    src/PhysicsJoint.hpp
    src/ResourceServiceFactory.cpp
    src/PhysicsJoint.cpp
    src/CollisionShape.cpp
    src/ZNewtonPlugin.hpp
    src/ResourceServiceFactory.hpp
    src/PhysicsResourceService.hpp
    src/PhysicsResource.hpp
    src/PhysicsMaterial.cpp
    src/PhysicsService.hpp
    src/PhysicsResource.cpp
    src/PhysicsZone.cpp
    Configuration.hpp

    )

add_definitions( -DZNEWTON_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${NEWTON_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( ZNewton ${LIB_TYPE} ${ZNewton_SRC} )

add_dependencies( ZNewton ZenCore ZenEngine )

target_link_libraries( ZNewton ZenCore ZenEngine ${NEWTON_LIBRARIES} )

set_target_properties ( ZNewton 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZNewton PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZNewton PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    ZNewtonModule.hpp
    plugin.xml
    Configuration.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ZNewton
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)