@echo off

set PATH=%PATH%;C:\dev\tools\svn
set DESTINATION=C:\dev\stage

: -----------------------------------
:ZenStudio

cd %DESTINATION%

mkdir bin

xcopy /D /Y /EXCLUDE:C:\dev\Zen\Setup\Package\debug.txt C:\dev\bin\*.dll bin
xcopy /D /Y /EXCLUDE:C:\dev\Zen\Setup\Package\debug.txt C:\dev\bin\*.exe bin
xcopy /D /Y C:\dev\bin\Microsoft.VC80.CRT bin\Microsoft.VC80.CRT

rem Export some things to tmpstage and then copy them.
cd ..
rmdir /s /q tmpstage
mkdir tmpstage
cd tmpstage

svn export svn://www.indiezen.org/zoss/plugins/trunk Zen/plugins
svn export svn://www.indiezen.org/zoss/StudioPlugins/trunk Zen/StudioPlugins
svn export svn://www.indiezen.org/zoss/Studio/trunk/WorkbenchServer Zen/Studio/WorkbenchServer
svn export svn://www.indiezen.org/towerd/trunk towerd

xcopy /D /Y /S *.xml %DESTINATION%
xcopy /D /Y /S *.exsd %DESTINATION%

cd %DESTINATION%
mkdir workbench.template

xcopy /D /Y /S C:\dev\workbench.template workbench.template

svn export svn://www.indiezen.org/zoss/Studio/trunk/icons bin/icons

xcopy /D /Y C:\dev\Zen\Setup\Package\zpkg-config.ini 
xcopy /D /Y /S "C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\Remote Debugger\x86" "Remote Debugger"

cd %DESTINATION%

mkdir towerd
xcopy /D /Y /S C:\dev\Zen\Setup\Package\towerd towerd

mkdir config\dev\config

cd config\dev

xcopy C:\dev\Zen\Setup\Package\config\* config
