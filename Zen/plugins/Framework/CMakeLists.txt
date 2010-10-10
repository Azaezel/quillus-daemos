cmake_minimum_required (VERSION 2.6)

project(Framework)
set ( PROJECT_PREFIX "plugins/Framework" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)


install ( FILES 
    schema/input.exsd
    schema/rendering.exsd
    schema/widgets.exsd
    schema/commandlineparser.exsd
    schema/camera.exsd
    schema/sky.exsd
    schema/terrain.exsd
    schema/network.exsd
    schema/sound.exsd
    schema/physics.exsd
    schema/navigation.exsd
    schema/console.exsd
    schema/importer.exsd
    schema/gameclient.exsd
    schema/scripting.exsd
    schema/world.exsd
    schema/boundingvolume.exsd
    schema/address.exsd
    schema/resource.exsd
    schema/scene.exsd
    schema/water.exsd
    schema/database.exsd
    schema/protocol.exsd
    schema/application.exsd
    schema/databaseschema.exsd
    schema/cloud.exsd
    DESTINATION
    Zen/${PROJECT_PREFIX}/schema/
)

install ( FILES 
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)

