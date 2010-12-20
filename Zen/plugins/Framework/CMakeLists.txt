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
    schema/address.exsd
    schema/application.exsd
    schema/boundingvolume.exsd
    schema/camera.exsd
    schema/cloud.exsd
    schema/commandlineparser.exsd
    schema/console.exsd
    schema/database.exsd
    schema/databaseschema.exsd
    schema/gameclient.exsd
    schema/importer.exsd
    schema/input.exsd
    schema/navigation.exsd
    schema/network.exsd
    schema/physics.exsd
    schema/protocol.exsd
    schema/rendering.exsd
    schema/resource.exsd
    schema/scene.exsd
    schema/scripting.exsd
    schema/sky.exsd
    schema/sound.exsd
    schema/terrain.exsd
    schema/water.exsd
    schema/widgets.exsd
    schema/world.exsd
    DESTINATION
    Zen/${PROJECT_PREFIX}/schema/
)

install ( FILES 
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)

