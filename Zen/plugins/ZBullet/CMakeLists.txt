cmake_minimum_required (VERSION 2.6) 

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Newer CMake Find system for locating Boost includes and libraries

SET( Boost_USE_MULTITHREAD On )
SET( Boost_USE_STATIC_LIBS Off )

Find_Package( Boost REQUIRED system )

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# By default, we're not going to use HydraX for water

set ( USE_HYDRAX false
    CACHE BOOL
    "Do you want to enable HydraX, a water addon for Ogre?")
if ( USE_HYDRAX )
    add_definitions( -DUSE_HYDRAX )
endif ( USE_HYDRAX )

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# We want this to be a shared lib (.so, .dylib, or .dll)
SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-

project ( ZNewton )
set ( ZNewton_SRC
    ./src/PhysicsJoint.cpp
    ./src/PhysicsMaterial.cpp
    ./src/PhysicsResource.cpp
    ./src/PhysicsResourceService.cpp
    ./src/PhysicsService.cpp
    ./src/PhysicsServiceFactory.cpp
    ./src/PhysicsShape.cpp
    ./src/PhysicsWorld.cpp
    ./src/ResourceServiceFactory.cpp
    ./src/ZNewtonModule.cpp
    ./src/ZNewtonPlugin.cpp
    )

find_file ( SYNCH_H synch.h ENV )
if ( SYNCH_H )
    add_definitions( -DTHREADMODEL_SOLARIS )
endif( SYNCH_H )

find_file ( PTHREAD_H pthread.h ENV )
if ( PTHREAD_H )
    add_definitions( -DTHREADMODEL_POSIX )
endif ( PTHREAD_H )

find_file ( WINDOWS_H windows.h ENV )
if ( WINDOWS_H )
    add_definitions( -DTHREADMODEL_WIN32 )
    set ( CPPFLAGS
        ${CPPFLAGS}
        -pthread
        )

    set ( LDFLAGS
        -pthread
        )
endif ( WINDOWS_H )

find_file ( THREAD_H thread.h ENV )
if ( SYNCH_H AND THREAD_H )
    add_definitions( -DHOST_SOLARIS )
endif ( SYNCH_H AND THREAD_H )

find_file ( UNISTD_H unistd.h ENV )
if ( UNISTD_H )
    add_definitions( -DHOST_POSIX )
endif ( UNISTD_H )

find_file ( WINDOWS_H windows.h ENV )
if ( WINDOWS_H )
    add_definitions( -DHOST_WIN32 )
endif ( WINDOWS_H )

find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )

find_path ( NEWTON_INCLUDE_DIR Newton.h ENV )
find_library ( NEWTON_LIBRARY Newton ${NEWTON_INCLUDE_DIR} ~/Library )

find_library( ZEN_THREADING_LIBRARY Threading ${CMAKE_BINARY_DIR}/../Core/Threading ${INDIEZEN_PARENT}/Zen/Core/Threading )
if( NOT ZEN_THREADING_LIBRARY )
    message( "Could not find the path to libThreading! Please set ZEN_THREADING_LIBRARY to the proper value." )
elseif( ZEN_THREADING_LIBRARY )
    message( "-- Found libThreading at ${ZEN_THREADING_LIBRARY}" )
endif( NOT ZEN_THREADING_LIBRARY )

find_library( ZEN_PLUGINS_LIBRARY Plugins ${CMAKE_BINARY_DIR}/../Core/Plugins ${INDIEZEN_PARENT}/Zen/Core/Plugins )
if( NOT ZEN_PLUGINS_LIBRARY )
    message( "Could not find the path to libPlugins! Please set ZEN_PLUGINS_LIBRARY to the proper value." )
elseif( ZEN_PLUGINS_LIBRARY )
    message( "-- Found libPlugins at ${ZEN_PLUGINS_LIBRARY}" )
endif( NOT ZEN_PLUGINS_LIBRARY )

find_library( ZEN_SCRIPTING_LIBRARY Scripting ${CMAKE_BINARY_DIR}/../Core/Scripting ${INDIEZEN_PARENT}/Zen/Core/Scripting )
if( NOT ZEN_SCRIPTING_LIBRARY )
    message( "Could not find the path to libScripting! Please set ZEN_SCRIPTING_LIBRARY to the proper value." )
elseif( ZEN_SCRIPTING_LIBRARY )
    message( "-- Found libScripting at ${ZEN_SCRIPTING_LIBRARY}" )
endif( NOT ZEN_SCRIPTING_LIBRARY )

find_library( ZEN_MATH_LIBRARY Math ${CMAKE_BINARY_DIR}/../Core/Math ${INDIEZEN_PARENT}/Zen/Core/Math )
if( NOT ZEN_MATH_LIBRARY )
    message( "Could not find the path to libMath! Please set ZEN_MATH_LIBRARY to the proper value." )
elseif( ZEN_MATH_LIBRARY )
    message( "-- Found libMath at ${ZEN_MATH_LIBRARY}" )
endif( NOT ZEN_MATH_LIBRARY )

find_library( ZEN_PHYSICS_LIBRARY Physics ${CMAKE_BINARY_DIR}/../Engine/Physics ${INDIEZEN_PARENT}/Zen/Engine/Physics )
if( NOT ZEN_PHYSICS_LIBRARY )
    message( "Could not find the path to libPhysics! Please set ZEN_PHYSICS_LIBRARY to the proper value." )
elseif( ZEN_PHYSICS_LIBRARY )
    message( "-- Found libPhysics at ${ZEN_PHYSICS_LIBRARY}" )
endif( NOT ZEN_PHYSICS_LIBRARY )

find_library( ZEN_RESOURCE_LIBRARY Resource ${CMAKE_BINARY_DIR}/../Engine/Resource ${INDIEZEN_PARENT}/Zen/Engine/Resource )
if( NOT ZEN_RESOURCE_LIBRARY )
    message( "Could not find the path to libResource! Please set ZEN_RESOURCE_LIBRARY to the proper value." )
elseif( ZEN_RESOURCE_LIBRARY )
    message( "-- Found libResource at ${ZEN_RESOURCE_LIBRARY}" )
endif( NOT ZEN_RESOURCE_LIBRARY )

find_library( ZEN_CAMERA_LIBRARY Camera ${CMAKE_BINARY_DIR}/../Engine/Camera ${INDIEZEN_PARENT}/Zen/Engine/Camera )
if( NOT ZEN_CAMERA_LIBRARY )
    message( "Could not find the path to libCamera! Please set ZEN_CAMERA_LIBRARY to the proper value." )
elseif( ZEN_CAMERA_LIBRARY )
    message( "-- Found libCamera at ${ZEN_CAMERA_LIBRARY}" )
endif( NOT ZEN_CAMERA_LIBRARY )

find_library( ZEN_WORLD_LIBRARY World ${CMAKE_BINARY_DIR}/../Engine/World ${INDIEZEN_PARENT}/Zen/Engine/World )
if( NOT ZEN_WORLD_LIBRARY )
    message( "Could not find the path to libWorld! Please set ZEN_WORLD_LIBRARY to the proper value." )
elseif( ZEN_WORLD_LIBRARY )
    message( "-- Found libWorld at ${ZEN_WORLD_LIBRARY}" )
endif( NOT ZEN_WORLD_LIBRARY )

set ( ZNEWTON_INCLUDES
    ${Boost_INCLUDE_DIR}
    ${INDIEZEN_PARENT}
    ${NEWTON_INCLUDE_DIR}
    )

set ( ZNEWTON_LINK_LIBRARIES
    ${ZEN_THREADING_LIBRARY}
    ${ZEN_MATH_LIBRARY}
    ${ZEN_PLUGINS_LIBRARY}
    ${ZEN_SCRIPTING_LIBRARY}
    ${ZEN_RESOURCE_LIBRARY}
    ${ZEN_PHYSICS_LIBRARY}
    ${NEWTON_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    )

include_directories ( ${ZNEWTON_INCLUDES}  )

link_directories( ${LINK_DIRECTORIES} ${INDIEZEN_PARENT}/Core )

add_library ( ZNewton ${LIB_TYPE} ${ZNewton_SRC} )

TARGET_LINK_LIBRARIES( ZNewton ${ZNEWTON_LINK_LIBRARIES} )
