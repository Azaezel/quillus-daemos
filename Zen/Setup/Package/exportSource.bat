@echo off

set PATH=%PATH%;C:\dev\tools\svn
set DESTINATION=C:\dev\stage\ZOSS_src\dev
set ROOTSVN=svn://www.indiezen.org/zoss
set BRANCH=trunk

:echo About to remove %DESTINATION%
:pause
rmdir /s /q %DESTINATION%
mkdir %DESTINATION%


cd %DESTINATION%
mkdir Zen
cd Zen

svn export %ROOTSVN%/build/%BRANCH% build
svn export %ROOTSVN%/Core/%BRANCH% Core
svn export %ROOTSVN%/Engine/%BRANCH% Engine
svn export %ROOTSVN%/Enterprise/%BRANCH% Enterprise
svn export %ROOTSVN%/examples/%BRANCH% examples
svn export %ROOTSVN%/plugins/%BRANCH% plugins
svn export %ROOTSVN%/Scripts/%BRANCH% Scripts
:svn export %ROOTSVN%/Shaders/%BRANCH% Shaders
svn export %ROOTSVN%/Starter/%BRANCH% Starter
svn export %ROOTSVN%/Studio/%BRANCH% Studio
svn export %ROOTSVN%/StudioPlugins/%BRANCH% StudioPlugins
svn export %ROOTSVN%/tutorials/%BRANCH% tutorials

svn export svn://www.indiezen.org/worlds/%BRANCH% Worlds
svn export svn://www.indiezen.org/community/%BRANCH% Community

cd ..

svn export svn://www.indiezen.org/towerd/%BRANCH% towerd

pause
