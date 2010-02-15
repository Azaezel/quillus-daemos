cmake_minimum_required (VERSION 2.4) 

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Newer CMake Find system for locating Boost includes and libraries

SET( Boost_USE_MULTITHREAD On )
SET( Boost_USE_STATIC_LIBS Off )

Find_Package( Boost REQUIRED )

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# We want this to be a shared lib (.so, .dylib, or .dll)
SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Look for libraries in frameworks last. Should help find the 
#  correct (Zen version) of libScripting.
if( APPLE )
    SET( CMAKE_FIND_FRAMEWORK LAST )
endif( APPLE )

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
project ( ZenEvent )
set ( Rendering_SRC
    src/BoundingVolumeManager.cpp 
    src/I_AttachableObject.cpp
    src/I_AxisAlignedBoundingBox.cpp
    src/I_BoundingSphere.cpp
    src/I_BoundingVolume.cpp
    src/I_BoundingVolumeManager.cpp
    src/I_BoundingVolumeService.cpp
    src/I_BoundingVolumeServiceFactory.cpp
    src/I_BoundingVolumeTree.cpp
    src/I_Camera.cpp
    src/I_Canvas.cpp
    src/I_Context.cpp
    src/I_Light.cpp
    src/I_Plane.cpp
    src/I_Renderable.cpp
    src/I_RenderingCanvas.cpp
    src/I_RenderingManager.cpp
    src/I_RenderingService.cpp
    src/I_RenderingServiceFactory.cpp
    src/I_SceneManager.cpp
    src/I_SceneNode.cpp
    src/I_SceneService.cpp
    src/I_SceneServiceFactory.cpp
    src/I_View.cpp
    src/RenderingManager.cpp
    src/SceneManager.cpp
    src/I_RenderableResource.cpp
    )

find_file ( SYNCH_H synch.h ENV )
if ( SYNCH_H )
    add_definitions( -DTHREADMODEL_SOLARIS )
endif( SYNCH_H )

find_file ( PTHREAD_H pthread.h ENV )
if ( PTHREAD_H )
    add_definitions( -DTHREADMODEL_POSIX )

    set ( CPPFLAGS
        ${CPPFLAGS}
        -pthread
        )

    set ( LDFLAGS
        -pthread
        )
endif ( PTHREAD_H )

find_file ( WINDOWS_H windows.h ENV )
if ( WINDOWS_H )
    add_definitions( -DTHREADMODEL_WIN32 )
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

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ../../.. )

add_library ( Rendering ${LIB_TYPE} ${Rendering_SRC} )

TARGET_LINK_LIBRARIES( Rendering Resource )

