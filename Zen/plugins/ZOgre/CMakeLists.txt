cmake_minimum_required (VERSION 2.6) 

project (ZOgre)
set ( PROJECT_PREFIX "plugins/ZOgre" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include ZOgre source file

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( ZOgre_SRC
    I_OgreLight.hpp
    src/RenderingServiceFactory.cpp
    src/SceneNode.cpp
    src/NullLight.cpp
    src/SceneService.hpp
    src/AxisAlignedBoundingBoxTree.hpp
    src/AxisAlignedBoundingBoxSplitter.hpp
    src/RenderingService.cpp
    src/AnimationState.hpp
    src/RenderingService.hpp
    src/RenderingServiceFactory.hpp
    src/Water.hpp
    src/Camera.cpp
    src/I_OgreSceneService.cpp
    src/WaterServiceFactory.hpp
    src/BoundingVolumeServiceFactory.hpp
    src/NullLight.hpp
    src/BoundingSphere.hpp
    src/AttachableObject.hpp
    src/RenderingCanvas.hpp
    src/AttachableObject.cpp
    src/ParticleSystem.hpp
    src/RenderingCanvas.cpp
    src/RenderingView.hpp
    src/SceneServiceFactory.cpp
    src/SceneNode.hpp
    src/Ogre.hpp
    src/ResourceServiceFactory.cpp
    src/SceneServiceFactory.hpp
    src/ResourceEntity.cpp
    src/Camera.hpp
    src/SceneService.cpp
    src/NullContext.hpp
    src/AxisAlignedBoundingBox.hpp
    src/ZOgreModule.cpp
    src/BoundingSphere.cpp
    src/ResourceService.cpp
    src/SkyService.hpp
    src/ParticleSystem.cpp
    src/Sky.hpp
    src/ZOgrePlugin.hpp
    src/ResourceServiceFactory.hpp
    src/BoundingVolumeService.cpp
    src/AnimationState.cpp
    src/RenderingView.cpp
    src/BoundingVolumeService.hpp
    src/WaterService.hpp
    src/Plane.cpp
    src/I_OgreView.cpp
    src/SkyServiceFactory.hpp
    src/AxisAlignedBoundingBox.cpp
    src/AxisAlignedBoundingBoxTreeNode.hpp
    src/ResourceService.hpp
    src/Water.cpp
    src/BoundingVolumeServiceFactory.cpp
    src/ZOgrePlugin.cpp
    src/AxisAlignedBoundingBoxSplitter.cpp
    src/NullCamera.cpp
    src/NullCamera.hpp
    src/AxisAlignedBoundingBoxTreeNode.cpp
    src/I_OgreRenderingCanvas.cpp
    src/I_OgreLight.cpp
    src/WaterService.cpp
    src/ResourceEntity.hpp
    src/WaterServiceFactory.cpp
    src/Plane.hpp
    I_OgreRenderingCanvas.hpp
    ZOgreModule.hpp
    I_OgreSceneService.hpp
    I_OgreView.hpp
    Configuration.hpp

    )

add_definitions( -DZOGRE_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${OGRE_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( ZOgre ${LIB_TYPE} ${ZOgre_SRC} )

add_dependencies( ZOgre ZenCore ZenEngine )

target_link_libraries( ZOgre ZenCore ZenEngine ${OGRE_LIBRARIES} )

set_target_properties ( ZOgre 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZOgre PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZOgre PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    plugin.xml
    I_OgreLight.hpp
    I_OgreRenderingCanvas.hpp
    ZOgreModule.hpp
    I_OgreSceneService.hpp
    I_OgreView.hpp
    Configuration.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ZOgre
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)