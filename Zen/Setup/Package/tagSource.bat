@echo off

set PATH=%PATH%;C:\dev\tools\svn
set ROOTSVN=svn://www.indiezen.org/zoss
set VERSION=RELEASE_0_6_1
set BRANCH=tags/%VERSION%

echo Tagging %VERSION%
pause 

svn copy %ROOTSVN%/build/trunk %ROOTSVN%/build/%BRANCH% -m "Tagging release %VERSION%"
svn copy %ROOTSVN%/Core/trunk %ROOTSVN%/Core/%BRANCH% -m "Tagging release %VERSION%"
svn copy %ROOTSVN%/Engine/trunk %ROOTSVN%/Engine/%BRANCH% -m "Tagging release %VERSION%"
svn copy %ROOTSVN%/Enterprise/trunk %ROOTSVN%/Enterprise/%BRANCH% -m "Tagging release %VERSION%"
svn copy %ROOTSVN%/examples/trunk %ROOTSVN%/examples/%BRANCH% -m "Tagging release %VERSION%"
svn copy %ROOTSVN%/plugins/trunk %ROOTSVN%/plugins/%BRANCH% -m "Tagging release %VERSION%"
svn copy %ROOTSVN%/Scripts/trunk %ROOTSVN%/Scripts/%BRANCH% -m "Tagging release %VERSION%"
:svn copy %ROOTSVN%/Shaders/trunk %ROOTSVN%/Shaders/%BRANCH% -m "Tagging release %VERSION%"
svn copy %ROOTSVN%/Starter/trunk %ROOTSVN%/Starter/%BRANCH% -m "Tagging release %VERSION%"
svn copy %ROOTSVN%/Studio/trunk %ROOTSVN%/Studio/%BRANCH% -m "Tagging release %VERSION%"
svn copy %ROOTSVN%/StudioPlugins/trunk %ROOTSVN%/StudioPlugins/%BRANCH% -m "Tagging release %VERSION%"
svn copy %ROOTSVN%/tutorials/trunk %ROOTSVN%/tutorials/%BRANCH% -m "Tagging release %VERSION%"

svn copy svn://www.indiezen.org/worlds/trunk svn://www.indiezen.org/worlds/%BRANCH% -m "Tagging release %VERSION%"
svn copy svn://www.indiezen.org/community/trunk svn://www.indiezen.org/community/%BRANCH% -m "Tagging release %VERSION%"
svn copy svn://www.indiezen.org/towerd/trunk svn://www.indiezen.org/towerd/%BRANCH% -m "Tagging release %VERSION%"
