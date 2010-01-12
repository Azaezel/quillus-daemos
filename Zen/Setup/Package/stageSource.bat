@echo off

set PATH=%PATH%;C:\dev\tools\svn
set DESTINATION=C:\dev\stage\ZOSS_headers\dev
set SOURCE=C:\dev\stage\ZOSS_src\dev

mkdir %DESTINATION%


xcopy /D /Y /S %SOURCE%\*.xml %DESTINATION%
xcopy /D /Y /S %SOURCE%\*.exsd %DESTINATION%

xcopy /D /Y /S %SOURCE%\towerd %DESTINATION%\towerd
xcopy /D /Y /S %SOURCE%\Zen\examples %DESTINATION%\Zen\examples
xcopy /D /Y /S %SOURCE%\Zen\tutorials %DESTINATION%\Zen\tutorials
xcopy /D /Y /S %SOURCE%\Zen\Scripts %DESTINATION%\Zen\Scripts

Rem stage examples and tutorials
cd C:\dev\stage\

mkdir towerd
cd towerd

mkdir resources
mkdir scripts
mkdir standins
mkdir SourceArt

xcopy /D /Y /S %SOURCE%\towerd\resources resources
xcopy /D /Y /S %SOURCE%\towerd\scripts scripts
xcopy /D /Y /S %SOURCE%\towerd\standins standins
xcopy /D /Y /S %SOURCE%\towerd\SourceArt SourceArt

cd C:\dev\stage\Zen\
mkdir examples\taBBall
cd examples\taBBall

mkdir scripts
mkdir resources

xcopy /D /Y /S %SOURCE%\Zen\examples\taBBall\scripts scripts
xcopy /D /Y /S %SOURCE%\Zen\examples\taBBall\resources resources

