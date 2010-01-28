@echo off

set PATH=%PATH%;C:\dev\tools\svn
set DESTINATION=C:\dev\stage

echo About to re-create %DESTINATION%
pause

mkdir %DESTINATION%
cd %DESTINATION%

:rmdir /s /q dependencies

mkdir dependencies
cd dependencies

: -----------------------------------
:Boost MSVC8

mkdir msvc8
cd msvc8

rem This is two "Boost" because the top layer is actually "C:\dev" or "C:\IndieZen"

mkdir Boost\dev\Boost\include
cd Boost\dev
mkdir bin
mkdir lib

xcopy /D /Y /S C:\dev\Boost\include Boost\include

rem Release libraries
xcopy /D /Y /S C:\dev\Boost\lib\*-mt-1_35.lib lib
rem Don't do the dll's (are there any?)
rem xcopy /D /Y /S C:\dev\Boost\lib\*-mt-1_35.dll bin

rem Debug libraries
xcopy /D /Y /S C:\dev\Boost\lib\*-mt-gd-1_35.lib lib

cd ..


: -----------------------------------
:msvc8 dependencies

cd %DESTINATION%\dependencies\msvc8

: -----------------------------------
:libxml2

mkdir libxml2\dev
cd libxml2\dev

mkdir libxml2\include\libxml
mkdir lib
mkdir bin

xcopy /D /Y C:\dev\libxml2\include\libxml libxml2\include\libxml

xcopy /D /Y C:\dev\bin\libxml2.dll bin
xcopy /D /Y C:\dev\bin\libxml2.lib lib

xcopy /D /Y C:\dev\bin\libxml2_d.dll bin
xcopy /D /Y C:\dev\bin\libxml2_d.lib lib

cd ..

: -----------------------------------
:OGRE 

cd %DESTINATION%\dependencies\msvc8

mkdir ogre\dev
cd ogre\dev

mkdir ogre\OgreMain\include
mkdir ogre\Mac\Ogre
mkdir ogre\Dependencies\include
mkdir lib
mkdir bin

xcopy /D /S /Y C:\dev\ogre\OgreMain\include ogre\OgreMain\include
xcopy /D /S /Y C:\dev\ogre\Mac\Ogre\include ogre\Mac\Ogre
xcopy /D /S /Y C:\dev\ogre\Dependencies\include ogre\Dependencies\include

rem These are only the debug dll's
xcopy /D /Y C:\dev\ogre\lib\*_d.dll bin
xcopy /D /Y C:\dev\ogre\lib\*.lib lib

xcopy /D /Y C:\dev\ogre\Dependencies\lib\Release\*.lib lib
xcopy /D /Y C:\dev\ogre\Dependencies\lib\Debug\*.lib lib

cd ..


: -----------------------------------
: Newton 

cd %DESTINATION%\dependencies\msvc8

mkdir Newton\dev
cd Newton\dev

mkdir NewtonSDK\sdk\
xcopy /D /Y C:\dev\NewtonSDK\sdk\*.h NewtonSDK\sdk

mkdir bin
mkdir lib

xcopy /D /Y C:\dev\NewtonSDK\sdk\dll\*.dll bin
xcopy /D /Y C:\dev\NewtonSDK\sdk\dll\*.lib lib


: -----------------------------------
: Lua 
: Lua is not in msvc8

cd %DESTINATION%\dependencies

mkdir Lua
cd Lua

mkdir include
xcopy /D /Y C:\dev\Lua\include include

: -----------------------------------
: Lua Release bin


mkdir lua
mkdir clibs
mkdir lib

xcopy /D /Y C:\dev\Lua\*.dll
xcopy /D /Y C:\dev\Lua\*.exe
xcopy /D /Y C:\dev\Lua\LICENSE.txt
xcopy /D /Y C:\dev\Lua\*.wlua
xcopy /D /Y C:\dev\Lua\lib\*.lib lib

xcopy /D /Y /S C:\dev\Lua\clibs clibs
xcopy /D /Y /S C:\dev\Lua\lua lua

cd ..

: -----------------------------------
:SVN

mkdir %DESTINATION%\dependencies\svn\tools\svn
xcopy /D /Y /S C:\dev\tools\svn %DESTINATION%\dependencies\svn\tools\svn

