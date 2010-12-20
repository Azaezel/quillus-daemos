cmake_minimum_required (VERSION 2.6) 

project (ZenEngine)
set ( PROJECT_PREFIX "Engine" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of Zen Engine

source_group(
    Cinematics\\
	REGULAR_EXPRESSION 
	Cinematics/*.*
	)

source_group(
    Cinematics\\src\\
	REGULAR_EXPRESSION 
	Cinematics/src/*.*
	)

source_group(
    Client\\
	REGULAR_EXPRESSION 
	Client/*.*
	)

source_group(
    Client\\src\\
	REGULAR_EXPRESSION 
	Client/src/*.*
	)

source_group(
    Core\\
	REGULAR_EXPRESSION 
	Core/*.*
	)

source_group(
    Core\\src\\
	REGULAR_EXPRESSION 
	Core/src/*.*
	)

source_group(
    Input\\
	REGULAR_EXPRESSION 
	Input/*.*
	)

source_group(
    Input\\src\\
	REGULAR_EXPRESSION 
	Input/src/*.*
	)

source_group(
    Move\\
	REGULAR_EXPRESSION 
	Move/*.*
	)

source_group(
    Move\\src\\
	REGULAR_EXPRESSION 
	Move/src/*.*
	)

source_group(
    Navigation\\
	REGULAR_EXPRESSION 
	Navigation/*.*
	)

source_group(
    Navigation\\src\\
	REGULAR_EXPRESSION 
	Navigation/src/*.*
	)

source_group(
    Physics\\
	REGULAR_EXPRESSION 
	Physics/*.*
	)

source_group(
    Physics\\src\\
	REGULAR_EXPRESSION 
	Physics/src/*.*
	)

source_group(
    Rendering\\
	REGULAR_EXPRESSION 
	Rendering/*.*
	)

source_group(
    Rendering\\src\\
	REGULAR_EXPRESSION 
	Rendering/src/*.*
	)

source_group(
    Resource\\
	REGULAR_EXPRESSION 
	Resource/*.*
	)

source_group(
    Resource\\src\\
	REGULAR_EXPRESSION 
	Resource/src/*.*
	)

source_group(
    Rules\\
	REGULAR_EXPRESSION 
	Rules/*.*
	)

source_group(
    Rules\\src\\
	REGULAR_EXPRESSION 
	Rules/src/*.*
	)

source_group(
    Server\\
	REGULAR_EXPRESSION 
	Server/*.*
	)

source_group(
    Server\\src\\
	REGULAR_EXPRESSION 
	Server/src/*.*
	)

source_group(
    Sound\\
	REGULAR_EXPRESSION 
	Sound/*.*
	)

source_group(
    Sound\\src\\
	REGULAR_EXPRESSION 
	Sound/src/*.*
	)

source_group(
    Widgets\\
	REGULAR_EXPRESSION 
	Widgets/*.*
	)

source_group(
    Widgets\\src\\
	REGULAR_EXPRESSION 
	Widgets/src/*.*
	)

source_group(
    World\\
	REGULAR_EXPRESSION 
	World/*.*
	)

source_group(
    World\\src\\
	REGULAR_EXPRESSION 
	World/src/*.*
	)


	
set ( ZenEngine_SRC
    Cinematics/Configuration.hpp
    Cinematics/I_Choreographer.hpp
    Cinematics/I_CinematicManager.hpp
    Cinematics/I_CinematicSequence.hpp
    Cinematics/I_CinematicService.hpp
    Cinematics/I_CinematicServiceFactory.hpp
    Cinematics/I_CinematicStep.hpp
    Cinematics/I_CinematicStepData.hpp
    Cinematics/I_Timer.hpp
    Cinematics/src/CinematicManager.cpp
    Cinematics/src/CinematicManager.hpp
    Cinematics/src/I_Choreographer.cpp
    Cinematics/src/I_CinematicManager.cpp
    Cinematics/src/I_CinematicSequence.cpp
    Cinematics/src/I_CinematicService.cpp
    Cinematics/src/I_CinematicServiceFactory.cpp
    Cinematics/src/I_CinematicStep.cpp
    Cinematics/src/I_CinematicStepData.cpp
    Cinematics/src/I_Timer.cpp
    Client/Configuration.hpp
    Client/I_ConnectionToServer.hpp
    Client/I_GameClient.hpp
    Client/I_GameClientFactory.hpp
    Client/I_GameClientManager.hpp
    Client/I_GameConsoleManager.hpp
    Client/I_GameConsoleService.hpp
    Client/I_SplashScreen.hpp
    Client/I_SplashScreenSequence.hpp
    Client/src/GameClientManager.cpp
    Client/src/GameClientManager.hpp
    Client/src/I_ConnectionToServer.cpp
    Client/src/I_GameClient.cpp
    Client/src/I_GameClientFactory.cpp
    Client/src/I_GameClientManager.cpp
    Client/src/I_SplashScreen.cpp
    Client/src/I_SplashScreenSequence.cpp
    Core/Configuration.hpp
    Core/I_Action.hpp
    Core/I_ActionFactory.hpp
    Core/I_ActionMap.hpp
    Core/I_BehaviorService.hpp
    Core/I_GameGroup.hpp
    Core/I_GameGroupData.hpp
    Core/I_GameObject.hpp
    Core/I_GameObjectBehaviors.hpp
    Core/I_GameObjectComposition.hpp
    Core/I_GameObjectData.hpp
    Core/I_GameObjectType.hpp
    Core/I_GameObjectTypeFactory.hpp
    Core/src/I_Action.cpp
    Core/src/I_ActionFactory.cpp
    Core/src/I_ActionMap.cpp
    Core/src/I_BehaviorService.cpp
    Core/src/I_GameGroup.cpp
    Core/src/I_GameObject.cpp
    Core/src/I_GameObjectBehaviors.cpp
    Input/ButtonIDs.hpp
    Input/Configuration.hpp
    Input/I_ButtonEvent.hpp
    Input/I_InputEvent.hpp
    Input/I_InputMapService.hpp
    Input/I_InputService.hpp
    Input/I_InputServiceFactory.hpp
    Input/I_InputServiceManager.hpp
    Input/I_KeyEvent.hpp
    Input/I_KeyMap.hpp
    Input/I_KeyModifierState.hpp
    Input/I_KeyPublisher.hpp
    Input/I_KeyState.hpp
    Input/I_MouseClickEvent.hpp
    Input/I_MouseMoveEvent.hpp
    Input/I_MousePublisher.hpp
    Input/I_ScalarEvent.hpp
    Input/I_VectorEvent.hpp
    Input/KeyCodes.hpp
    Input/src/InputMapService.cpp
    Input/src/InputMapService.hpp
    Input/src/InputServiceManager.cpp
    Input/src/InputServiceManager.hpp
    Input/src/I_ButtonEvent.cpp
    Input/src/I_InputEvent.cpp
    Input/src/I_InputMapService.cpp
    Input/src/I_InputService.cpp
    Input/src/I_InputServiceFactory.cpp
    Input/src/I_InputServiceManager.cpp
    Input/src/I_KeyEvent.cpp
    Input/src/I_KeyMap.cpp
    Input/src/I_KeyModifierState.cpp
    Input/src/I_KeyPublisher.cpp
    Input/src/I_KeyState.cpp
    Input/src/I_MouseClickEvent.cpp
    Input/src/I_MouseMoveEvent.cpp
    Input/src/I_MousePublisher.cpp
    Input/src/I_ScalarEvent.cpp
    Input/src/I_VectorEvent.cpp
    Input/src/KeyMap.cpp
    Input/src/KeyMap.hpp
    Input/src/KeyModifierState.cpp
    Input/src/KeyModifierState.hpp
    Input/src/KeyState.cpp
    Input/src/KeyState.hpp
    Move/Configuration.hpp
    Move/I_Controllable.hpp
    Move/I_ControllableFactory.hpp
    Move/I_Move.hpp
    Move/I_MoveManager.hpp
    Move/I_MovePool.hpp
    Move/I_MoveRequest.hpp
    Move/I_MoveRequestFactory.hpp
    Move/I_MoveService.hpp
    Move/I_MoveServiceFactory.hpp
    Move/src/I_Controllable.cpp
    Move/src/I_ControllableFactory.cpp
    Move/src/I_Move.cpp
    Move/src/I_MoveManager.cpp
    Move/src/I_MovePool.cpp
    Move/src/I_MoveRequest.cpp
    Move/src/I_MoveRequestFactory.cpp
    Move/src/I_MoveService.cpp
    Move/src/I_MoveServiceFactory.cpp
    Move/src/MoveManager.hpp
    Navigation/Configuration.hpp
    Navigation/I_Graph.hpp
    Navigation/I_NavigationManager.hpp
    Navigation/I_NavigationService.hpp
    Navigation/I_NavigationServiceFactory.hpp
    Navigation/I_Node.hpp
    Navigation/I_NodeComparator.hpp
    Navigation/I_NodeIterator.hpp
    Navigation/I_Path.hpp
    Navigation/src/I_Graph.cpp
    Navigation/src/I_NavigationManager.cpp
    Navigation/src/I_NavigationService.cpp
    Navigation/src/I_NavigationServiceFactory.cpp
    Navigation/src/I_Node.cpp
    Navigation/src/I_NodeComparator.cpp
    Navigation/src/I_NodeIterator.cpp
    Navigation/src/I_Path.cpp
    Navigation/src/NavigationManager.cpp
    Navigation/src/NavigationManager.hpp
    Physics/Configuration.hpp
    Physics/I_CollisionService.hpp
    Physics/I_CollisionShape.hpp
    Physics/I_HeightfieldShape.hpp
    Physics/I_MeshCollisionShape.hpp
    Physics/I_PhysicsActor.hpp
    Physics/I_PhysicsFrame.hpp
    Physics/I_PhysicsFrameListener.hpp
    Physics/I_PhysicsGroup.hpp
    Physics/I_PhysicsJoint.hpp
    Physics/I_PhysicsManager.hpp
    Physics/I_PhysicsMaterial.hpp
    Physics/I_PhysicsService.hpp
    Physics/I_PhysicsServiceFactory.hpp
    Physics/I_PhysicsZone.hpp
    Physics/src/I_CollisionService.cpp
    Physics/src/I_CollisionShape.cpp
    Physics/src/I_MeshCollisionShape.cpp
    Physics/src/I_PhysicsActor.cpp
    Physics/src/I_PhysicsFrame.cpp
    Physics/src/I_PhysicsFrameListener.cpp
    Physics/src/I_PhysicsGroup.cpp
    Physics/src/I_PhysicsJoint.cpp
    Physics/src/I_PhysicsManager.cpp
    Physics/src/I_PhysicsMaterial.cpp
    Physics/src/I_PhysicsService.cpp
    Physics/src/I_PhysicsServiceFactory.cpp
    Physics/src/I_PhysicsZone.cpp
    Physics/src/PhysicsManager.cpp
    Physics/src/PhysicsManager.hpp
    Rendering/Color.hpp
    Rendering/Configuration.hpp
    Rendering/I_AnimationState.hpp
    Rendering/I_AttachableObject.hpp
    Rendering/I_AxisAlignedBoundingBox.hpp
    Rendering/I_BoundingSphere.hpp
    Rendering/I_BoundingVolume.hpp
    Rendering/I_BoundingVolumeManager.hpp
    Rendering/I_BoundingVolumeService.hpp
    Rendering/I_BoundingVolumeServiceFactory.hpp
    Rendering/I_BoundingVolumeTree.hpp
    Rendering/I_Camera.hpp
    Rendering/I_Canvas.hpp
    Rendering/I_Context.hpp
    Rendering/I_Light.hpp
    Rendering/I_Mesh.hpp
    Rendering/I_ParticleSystem.hpp
    Rendering/I_PickingCanvas.hpp
    Rendering/I_Plane.hpp
    Rendering/I_Renderable.hpp
    Rendering/I_RenderableResource.hpp
    Rendering/I_RenderingCanvas.hpp
    Rendering/I_RenderingManager.hpp
    Rendering/I_RenderingService.hpp
    Rendering/I_RenderingServiceFactory.hpp
    Rendering/I_SceneGraph.hpp
    Rendering/I_SceneManager.hpp
    Rendering/I_SceneNode.hpp
    Rendering/I_SceneService.hpp
    Rendering/I_SceneServiceFactory.hpp
    Rendering/I_TextureManager.hpp
    Rendering/I_View.hpp
    Rendering/src/BoundingVolumeManager.cpp
    Rendering/src/BoundingVolumeManager.hpp
    Rendering/src/Color.cpp
    Rendering/src/I_AnimationState.cpp
    Rendering/src/I_AttachableObject.cpp
    Rendering/src/I_AxisAlignedBoundingBox.cpp
    Rendering/src/I_BoundingSphere.cpp
    Rendering/src/I_BoundingVolume.cpp
    Rendering/src/I_BoundingVolumeManager.cpp
    Rendering/src/I_BoundingVolumeService.cpp
    Rendering/src/I_BoundingVolumeServiceFactory.cpp
    Rendering/src/I_BoundingVolumeTree.cpp
    Rendering/src/I_Camera.cpp
    Rendering/src/I_Canvas.cpp
    Rendering/src/I_Context.cpp
    Rendering/src/I_Light.cpp
    Rendering/src/I_Mesh.cpp
    Rendering/src/I_ParticleSystem.cpp
    Rendering/src/I_Plane.cpp
    Rendering/src/I_Renderable.cpp
    Rendering/src/I_RenderableResource.cpp
    Rendering/src/I_RenderingCanvas.cpp
    Rendering/src/I_RenderingManager.cpp
    Rendering/src/I_RenderingService.cpp
    Rendering/src/I_RenderingServiceFactory.cpp
    Rendering/src/I_SceneGraph.cpp
    Rendering/src/I_SceneManager.cpp
    Rendering/src/I_SceneNode.cpp
    Rendering/src/I_SceneService.cpp
    Rendering/src/I_SceneServiceFactory.cpp
    Rendering/src/I_View.cpp
    Rendering/src/RenderingManager.cpp
    Rendering/src/RenderingManager.hpp
    Rendering/src/SceneManager.cpp
    Rendering/src/SceneManager.hpp
    Resource/Configuration.hpp
    Resource/I_CollisionResource.hpp
    Resource/I_CompoundResource.hpp
    Resource/I_Exporter.hpp
    Resource/I_ExporterFactory.hpp
    Resource/I_Importer.hpp
    Resource/I_ImporterFactory.hpp
    Resource/I_Resource.hpp
    Resource/I_ResourceFactory.hpp
    Resource/I_ResourceManager.hpp
    Resource/I_ResourceService.hpp
    Resource/I_ResourceServiceFactory.hpp
    Resource/I_ResourceStream.hpp
    Resource/I_ResourceStreamFactory.hpp
    Resource/src/CompoundResource.cpp
    Resource/src/CompoundResource.hpp
    Resource/src/FileStream.cpp
    Resource/src/FileStream.hpp
    Resource/src/I_CollisionResource.cpp
    Resource/src/I_CompoundResource.cpp
    Resource/src/I_Exporter.cpp
    Resource/src/I_ExporterFactory.cpp
    Resource/src/I_Importer.cpp
    Resource/src/I_ImporterFactory.cpp
    Resource/src/I_Resource.cpp
    Resource/src/I_ResourceFactory.cpp
    Resource/src/I_ResourceManager.cpp
    Resource/src/I_ResourceService.cpp
    Resource/src/I_ResourceServiceFactory.cpp
    Resource/src/I_ResourceStream.cpp
    Resource/src/I_ResourceStreamFactory.cpp
    Resource/src/ResourceManager.cpp
    Resource/src/ResourceManager.hpp
    Rules/Configuration.hpp
    Rules/I_Event.hpp
    Rules/I_EventRegistry.hpp
    Rules/I_GameRule.hpp
    Rules/I_GameRuleFactory.hpp
    Rules/I_RuleDataService.hpp
    Rules/I_RuleDataServiceFactory.hpp
    Rules/I_RuleManager.hpp
    Rules/I_RuleService.hpp
    Rules/I_RuleServiceFactory.hpp
    Rules/I_State.hpp
    Rules/I_StateMachine.hpp
    Rules/I_Transition.hpp
    Rules/src/Event.cpp
    Rules/src/Event.hpp
    Rules/src/EventRegistry.cpp
    Rules/src/EventRegistry.hpp
    Rules/src/I_Event.cpp
    Rules/src/I_EventRegistry.cpp
    Rules/src/I_GameRule.cpp
    Rules/src/I_GameRuleFactory.cpp
    Rules/src/I_RuleDataService.cpp
    Rules/src/I_RuleDataServiceFactory.cpp
    Rules/src/I_RuleManager.cpp
    Rules/src/I_RuleService.cpp
    Rules/src/I_RuleServiceFactory.cpp
    Rules/src/I_State.cpp
    Rules/src/I_StateMachine.cpp
    Rules/src/I_Transition.cpp
    Rules/src/RuleManager.cpp
    Rules/src/RuleManager.hpp
    Rules/src/StateMachine.hpp
    Server/Configuration.hpp
    Server/I_CommandHandler.hpp
    Server/I_CommandLineParseEvent.hpp
    Server/I_CommandLineParserManager.hpp
    Server/I_CommandLineParserService.hpp
    Server/I_CommandLineParserServiceFactory.hpp
    Server/I_ConnectionToClient.hpp
    Server/I_ConsoleInputEvent.hpp
    Server/I_ConsoleManager.hpp
    Server/I_ConsoleService.hpp
    Server/I_ConsoleServiceFactory.hpp
    Server/I_ConsoleStream.hpp
    Server/I_GameServer.hpp
    Server/I_ServerManager.hpp
    Server/I_ServerService.hpp
    Server/I_ServerServiceFactory.hpp
    Server/src/CommandLineParserManager.cpp
    Server/src/CommandLineParserManager.hpp
    Server/src/ConsoleManager.cpp
    Server/src/ConsoleManager.hpp
    Server/src/I_CommandHandler.cpp
    Server/src/I_CommandLineParseEvent.cpp
    Server/src/I_CommandLineParserManager.cpp
    Server/src/I_CommandLineParserService.cpp
    Server/src/I_CommandLineParserServiceFactory.cpp
    Server/src/I_ConsoleInputEvent.cpp
    Server/src/I_ConsoleManager.cpp
    Server/src/I_ConsoleService.cpp
    Server/src/I_ConsoleServiceFactory.cpp
    Server/src/I_ConsoleStream.cpp
    Server/src/I_ServerService.cpp
    Server/src/I_ServerServiceFactory.cpp
    Sound/Configuration.hpp
    Sound/I_Audible.hpp
    Sound/I_SoundManager.hpp
    Sound/I_SoundResource.hpp
    Sound/I_SoundService.hpp
    Sound/I_SoundServiceFactory.hpp
    Sound/I_SoundSource.hpp
    Sound/src/I_Audible.cpp
    Sound/src/I_SoundManager.cpp
    Sound/src/I_SoundResource.cpp
    Sound/src/I_SoundService.cpp
    Sound/src/I_SoundServiceFactory.cpp
    Sound/src/I_SoundSource.cpp
    Sound/src/SoundManager.cpp
    Sound/src/SoundManager.hpp
    Widgets/Configuration.hpp
    Widgets/I_FontService.hpp
    Widgets/I_StyleService.hpp
    Widgets/I_Widget.hpp
    Widgets/I_WidgetEvent.hpp
    Widgets/I_WidgetManager.hpp
    Widgets/I_WidgetProperty.hpp
    Widgets/I_WidgetRenderer.hpp
    Widgets/I_WidgetService.hpp
    Widgets/I_WidgetServiceFactory.hpp
    Widgets/I_WidgetStyle.hpp
    Widgets/src/I_FontService.cpp
    Widgets/src/I_Widget.cpp
    Widgets/src/I_WidgetEvent.cpp
    Widgets/src/I_WidgetManager.cpp
    Widgets/src/I_WidgetProperty.cpp
    Widgets/src/I_WidgetRenderer.cpp
    Widgets/src/I_WidgetService.cpp
    Widgets/src/I_WidgetServiceFactory.cpp
    Widgets/src/WidgetManager.cpp
    Widgets/src/WidgetManager.hpp
    World/Configuration.hpp
    World/I_Cloud.hpp
    World/I_CloudParticle.hpp
    World/I_CloudService.hpp
    World/I_CloudServiceFactory.hpp
    World/I_Sky.hpp
    World/I_SkyService.hpp
    World/I_SkyServiceFactory.hpp
    World/I_Terrain.hpp
    World/I_TerrainChunk.hpp
    World/I_TerrainGroup.hpp
    World/I_TerrainHeightfield.hpp
    World/I_TerrainService.hpp
    World/I_TerrainServiceFactory.hpp
    World/I_Water.hpp
    World/I_WaterService.hpp
    World/I_WaterServiceFactory.hpp
    World/I_WorldManager.hpp
    World/I_WorldService.hpp
    World/I_WorldServiceFactory.hpp
    World/src/I_Cloud.cpp
    World/src/I_CloudParticle.cpp
    World/src/I_CloudService.cpp
    World/src/I_CloudServiceFactory.cpp
    World/src/I_Sky.cpp
    World/src/I_SkyService.cpp
    World/src/I_SkyServiceFactory.cpp
    World/src/I_Terrain.cpp
    World/src/I_TerrainChunk.cpp
    World/src/I_TerrainGroup.cpp
    World/src/I_TerrainHeightfield.cpp
    World/src/I_TerrainService.cpp
    World/src/I_TerrainServiceFactory.cpp
    World/src/I_Water.cpp
    World/src/I_WaterService.cpp
    World/src/I_WaterServiceFactory.cpp
    World/src/I_WorldManager.cpp
    World/src/I_WorldService.cpp
    World/src/I_WorldServiceFactory.cpp
    World/src/WorldManager.cpp
    World/src/WorldManager.hpp

    )

add_definitions( -DZENENGINE_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${LIBXML2_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( ZenEngine ${LIB_TYPE} ${ZenEngine_SRC} )

add_dependencies(ZenEngine ZenCore)

target_link_libraries( ZenEngine ZenCore)

set_target_properties ( ZenEngine 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZenEngine PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZenEngine PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Cinematics/Configuration.hpp
    Cinematics/I_Choreographer.hpp
    Cinematics/I_CinematicManager.hpp
    Cinematics/I_CinematicSequence.hpp
    Cinematics/I_CinematicService.hpp
    Cinematics/I_CinematicServiceFactory.hpp
    Cinematics/I_CinematicStep.hpp
    Cinematics/I_CinematicStepData.hpp
    Cinematics/I_Timer.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Cinematics/
)

install ( FILES 
    Client/Configuration.hpp
    Client/I_ConnectionToServer.hpp
    Client/I_GameClient.hpp
    Client/I_GameClientFactory.hpp
    Client/I_GameClientManager.hpp
    Client/I_GameConsoleManager.hpp
    Client/I_GameConsoleService.hpp
    Client/I_SplashScreen.hpp
    Client/I_SplashScreenSequence.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Client/
)

install ( FILES 
    Core/Configuration.hpp
    Core/I_Action.hpp
    Core/I_ActionFactory.hpp
    Core/I_ActionMap.hpp
    Core/I_BehaviorService.hpp
    Core/I_GameGroup.hpp
    Core/I_GameGroupData.hpp
    Core/I_GameObject.hpp
    Core/I_GameObjectBehaviors.hpp
    Core/I_GameObjectComposition.hpp
    Core/I_GameObjectData.hpp
    Core/I_GameObjectType.hpp
    Core/I_GameObjectTypeFactory.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Core/
)

install ( FILES 
    Input/ButtonIDs.hpp
    Input/Configuration.hpp
    Input/I_ButtonEvent.hpp
    Input/I_InputEvent.hpp
    Input/I_InputMapService.hpp
    Input/I_InputService.hpp
    Input/I_InputServiceFactory.hpp
    Input/I_InputServiceManager.hpp
    Input/I_KeyEvent.hpp
    Input/I_KeyMap.hpp
    Input/I_KeyModifierState.hpp
    Input/I_KeyPublisher.hpp
    Input/I_KeyState.hpp
    Input/I_MouseClickEvent.hpp
    Input/I_MouseMoveEvent.hpp
    Input/I_MousePublisher.hpp
    Input/I_ScalarEvent.hpp
    Input/I_VectorEvent.hpp
    Input/KeyCodes.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Input/
)

install ( FILES 
    Move/Configuration.hpp
    Move/I_Controllable.hpp
    Move/I_ControllableFactory.hpp
    Move/I_Move.hpp
    Move/I_MoveManager.hpp
    Move/I_MovePool.hpp
    Move/I_MoveRequest.hpp
    Move/I_MoveRequestFactory.hpp
    Move/I_MoveService.hpp
    Move/I_MoveServiceFactory.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Move/
)

install ( FILES 
    Navigation/Configuration.hpp
    Navigation/I_Graph.hpp
    Navigation/I_NavigationManager.hpp
    Navigation/I_NavigationService.hpp
    Navigation/I_NavigationServiceFactory.hpp
    Navigation/I_Node.hpp
    Navigation/I_NodeComparator.hpp
    Navigation/I_NodeIterator.hpp
    Navigation/I_Path.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Navigation/
)

install ( FILES 
    Physics/Configuration.hpp
    Physics/I_CollisionService.hpp
    Physics/I_CollisionShape.hpp
    Physics/I_HeightfieldShape.hpp
    Physics/I_MeshCollisionShape.hpp
    Physics/I_PhysicsActor.hpp
    Physics/I_PhysicsFrame.hpp
    Physics/I_PhysicsFrameListener.hpp
    Physics/I_PhysicsGroup.hpp
    Physics/I_PhysicsJoint.hpp
    Physics/I_PhysicsManager.hpp
    Physics/I_PhysicsMaterial.hpp
    Physics/I_PhysicsService.hpp
    Physics/I_PhysicsServiceFactory.hpp
    Physics/I_PhysicsZone.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Physics/
)

install ( FILES 
    Rendering/Color.hpp
    Rendering/Configuration.hpp
    Rendering/I_AnimationState.hpp
    Rendering/I_AttachableObject.hpp
    Rendering/I_AxisAlignedBoundingBox.hpp
    Rendering/I_BoundingSphere.hpp
    Rendering/I_BoundingVolume.hpp
    Rendering/I_BoundingVolumeManager.hpp
    Rendering/I_BoundingVolumeService.hpp
    Rendering/I_BoundingVolumeServiceFactory.hpp
    Rendering/I_BoundingVolumeTree.hpp
    Rendering/I_Camera.hpp
    Rendering/I_Canvas.hpp
    Rendering/I_Context.hpp
    Rendering/I_Light.hpp
    Rendering/I_Mesh.hpp
    Rendering/I_ParticleSystem.hpp
    Rendering/I_PickingCanvas.hpp
    Rendering/I_Plane.hpp
    Rendering/I_Renderable.hpp
    Rendering/I_RenderableResource.hpp
    Rendering/I_RenderingCanvas.hpp
    Rendering/I_RenderingManager.hpp
    Rendering/I_RenderingService.hpp
    Rendering/I_RenderingServiceFactory.hpp
    Rendering/I_SceneGraph.hpp
    Rendering/I_SceneManager.hpp
    Rendering/I_SceneNode.hpp
    Rendering/I_SceneService.hpp
    Rendering/I_SceneServiceFactory.hpp
    Rendering/I_TextureManager.hpp
    Rendering/I_View.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Rendering/
)

install ( FILES 
    Resource/Configuration.hpp
    Resource/I_CollisionResource.hpp
    Resource/I_CompoundResource.hpp
    Resource/I_Exporter.hpp
    Resource/I_ExporterFactory.hpp
    Resource/I_Importer.hpp
    Resource/I_ImporterFactory.hpp
    Resource/I_Resource.hpp
    Resource/I_ResourceFactory.hpp
    Resource/I_ResourceManager.hpp
    Resource/I_ResourceService.hpp
    Resource/I_ResourceServiceFactory.hpp
    Resource/I_ResourceStream.hpp
    Resource/I_ResourceStreamFactory.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Resource/
)

install ( FILES 
    Rules/Configuration.hpp
    Rules/I_Event.hpp
    Rules/I_EventRegistry.hpp
    Rules/I_GameRule.hpp
    Rules/I_GameRuleFactory.hpp
    Rules/I_RuleDataService.hpp
    Rules/I_RuleDataServiceFactory.hpp
    Rules/I_RuleManager.hpp
    Rules/I_RuleService.hpp
    Rules/I_RuleServiceFactory.hpp
    Rules/I_State.hpp
    Rules/I_StateMachine.hpp
    Rules/I_Transition.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Rules/
)

install ( FILES 
    Server/Configuration.hpp
    Server/I_CommandHandler.hpp
    Server/I_CommandLineParseEvent.hpp
    Server/I_CommandLineParserManager.hpp
    Server/I_CommandLineParserService.hpp
    Server/I_CommandLineParserServiceFactory.hpp
    Server/I_ConnectionToClient.hpp
    Server/I_ConsoleInputEvent.hpp
    Server/I_ConsoleManager.hpp
    Server/I_ConsoleService.hpp
    Server/I_ConsoleServiceFactory.hpp
    Server/I_ConsoleStream.hpp
    Server/I_GameServer.hpp
    Server/I_ServerManager.hpp
    Server/I_ServerService.hpp
    Server/I_ServerServiceFactory.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Server/
)

install ( FILES 
    Sound/Configuration.hpp
    Sound/I_Audible.hpp
    Sound/I_SoundManager.hpp
    Sound/I_SoundResource.hpp
    Sound/I_SoundService.hpp
    Sound/I_SoundServiceFactory.hpp
    Sound/I_SoundSource.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Sound/
)

install ( FILES 
    Widgets/Configuration.hpp
    Widgets/I_FontService.hpp
    Widgets/I_StyleService.hpp
    Widgets/I_Widget.hpp
    Widgets/I_WidgetEvent.hpp
    Widgets/I_WidgetManager.hpp
    Widgets/I_WidgetProperty.hpp
    Widgets/I_WidgetRenderer.hpp
    Widgets/I_WidgetService.hpp
    Widgets/I_WidgetServiceFactory.hpp
    Widgets/I_WidgetStyle.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Widgets/
)

install ( FILES 
    World/Configuration.hpp
    World/I_Cloud.hpp
    World/I_CloudParticle.hpp
    World/I_CloudService.hpp
    World/I_CloudServiceFactory.hpp
    World/I_Sky.hpp
    World/I_SkyService.hpp
    World/I_SkyServiceFactory.hpp
    World/I_Terrain.hpp
    World/I_TerrainChunk.hpp
    World/I_TerrainGroup.hpp
    World/I_TerrainHeightfield.hpp
    World/I_TerrainService.hpp
    World/I_TerrainServiceFactory.hpp
    World/I_Water.hpp
    World/I_WaterService.hpp
    World/I_WaterServiceFactory.hpp
    World/I_WorldManager.hpp
    World/I_WorldService.hpp
    World/I_WorldServiceFactory.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/World/
)


install ( TARGETS
	ZenEngine
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)