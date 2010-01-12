#!/bin/bash
rm -rf ~/stage/Zen
mkdir ~/stage/towerd

## Export the main source code
SVNDIRS="build Core Engine Enterprise Spaces examples Scripts Starter Studio StudioPlugins tests tutorials"
BRANCH="trunk"

mkdir ~/stage/Zen
cd ~/stage/Zen

for DIR in $SVNDIRS
do
    svn export svn://www.indiezen.org/zoss/$DIR/$BRANCH/ $DIR
done

## Community export
svn export svn://www.indiezen.org/community/$BRANCH Community

## Worlds export
svn export svn://www.indiezen.org/worlds/$BRANCH Worlds

## Plugins export
PLUGINS="Framework ZBasicCommandLineParser ZBasicConsole ZBoostNetworking ZFMOD ZInput ZLua ZMicroPather ZMySQL ZNewton ZOgre ZOpenAL ZODE ZCrazyEddie ZPhysX ZPostgres ZPython ZSQLite ZSky ZTerrain ZWebServices"

mkdir ~/stage/Zen/plugins
cd ~/stage/Zen/plugins
for PLUGIN in $PLUGINS
do
    svn export svn://www.indiezen.org/zoss/plugins/$BRANCH/$PLUGIN $PLUGIN
done

## CyberTD export
svn export svn://www.indiezen.org/towerd/$BRANCH ~/stage/towerd

## Staged items
#cd ~/stage/Zen/
#wget http://www.indiezen.org/wiki/raw-attachment/wiki/zoss/Compiling/config.zip
#unzip config.zip
#rm -rf config.zip

