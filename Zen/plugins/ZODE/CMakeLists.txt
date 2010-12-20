cmake_minimum_required (VERSION 2.6) 

project (ZODE)

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED system )

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include ZODE source file

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( ZODE_SRC
    Configuration.hpp
    src/CollisionShape.cpp
    src/CollisionShape.hpp
    src/HeightfieldCollisionShape.cpp
    src/HeightfieldCollisionShape.hpp
    src/HeightfieldResource.cpp
    src/HeightfieldResource.hpp
    src/MeshCollisionShape.cpp
    src/MeshCollisionShape.hpp
    src/PhysicsActor.cpp
    src/PhysicsActor.hpp
    src/PhysicsJoint.cpp
    src/PhysicsJoint.hpp
    src/PhysicsMaterial.cpp
    src/PhysicsMaterial.hpp
    src/PhysicsResource.cpp
    src/PhysicsResource.hpp
    src/PhysicsResourceService.cpp
    src/PhysicsResourceService.hpp
    src/PhysicsService.cpp
    src/PhysicsService.hpp
    src/PhysicsServiceFactory.cpp
    src/PhysicsServiceFactory.hpp
    src/PhysicsZone.cpp
    src/PhysicsZone.hpp
    src/ResourceServiceFactory.cpp
    src/ResourceServiceFactory.hpp
    src/ZODEPlugin.cpp

    )

add_definitions( -DZODE_EXPORTS ${Zen_DEFINITIONS} ${OPENDE_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${OPENDE_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( ZODE ${LIB_TYPE} ${ZODE_SRC} )

add_dependencies( ZODE ZenCore ZenEngine )

target_link_libraries( ZODE ZenCore ZenEngine ${OPENDE_LIBRARIES} )

set_target_properties ( ZODE 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZODE PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZODE PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)

