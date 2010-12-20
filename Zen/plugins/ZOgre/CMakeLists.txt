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
    Configuration.hpp
    I_OgreLight.hpp
    I_OgreRenderingCanvas.hpp
    I_OgreSceneService.hpp
    I_OgreView.hpp
    src/AnimationState.cpp
    src/AnimationState.hpp
    src/AttachableObject.cpp
    src/AttachableObject.hpp
    src/AxisAlignedBoundingBox.cpp
    src/AxisAlignedBoundingBox.hpp
    src/AxisAlignedBoundingBoxSplitter.cpp
    src/AxisAlignedBoundingBoxSplitter.hpp
    src/AxisAlignedBoundingBoxTree.hpp
    src/AxisAlignedBoundingBoxTreeNode.cpp
    src/AxisAlignedBoundingBoxTreeNode.hpp
    src/BoundingSphere.cpp
    src/BoundingSphere.hpp
    src/BoundingVolumeService.cpp
    src/BoundingVolumeService.hpp
    src/BoundingVolumeServiceFactory.cpp
    src/BoundingVolumeServiceFactory.hpp
    src/Camera.cpp
    src/Camera.hpp
    src/I_OgreLight.cpp
    src/I_OgreRenderingCanvas.cpp
    src/I_OgreSceneService.cpp
    src/I_OgreView.cpp
    src/Mesh.cpp
    src/Mesh.hpp
    src/NullCamera.cpp
    src/NullCamera.hpp
    src/NullContext.hpp
    src/NullLight.cpp
    src/NullLight.hpp
    src/Ogre.hpp
    src/ParticleSystem.cpp
    src/ParticleSystem.hpp
    src/Plane.cpp
    src/Plane.hpp
    src/RenderingCanvas.cpp
    src/RenderingCanvas.hpp
    src/RenderingService.cpp
    src/RenderingService.hpp
    src/RenderingServiceFactory.cpp
    src/RenderingServiceFactory.hpp
    src/RenderingView.cpp
    src/RenderingView.hpp
    src/ResourceEntity.cpp
    src/ResourceEntity.hpp
    src/ResourceService.cpp
    src/ResourceService.hpp
    src/ResourceServiceFactory.cpp
    src/ResourceServiceFactory.hpp
    src/SceneNode.cpp
    src/SceneNode.hpp
    src/SceneService.cpp
    src/SceneService.hpp
    src/SceneServiceFactory.cpp
    src/SceneServiceFactory.hpp
    src/Sky.hpp
    src/SkyService.hpp
    src/SkyServiceFactory.hpp
    src/Water.cpp
    src/Water.hpp
    src/WaterService.cpp
    src/WaterService.hpp
    src/WaterServiceFactory.cpp
    src/WaterServiceFactory.hpp
    src/ZOgreModule.cpp
    src/ZOgrePlugin.cpp
    src/ZOgrePlugin.hpp
    ZOgreModule.hpp

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
    Configuration.hpp
    I_OgreLight.hpp
    I_OgreRenderingCanvas.hpp
    I_OgreSceneService.hpp
    I_OgreView.hpp
    plugin.xml
    ZOgreModule.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ZOgre
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)