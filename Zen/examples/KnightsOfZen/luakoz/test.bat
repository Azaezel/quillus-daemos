@ECHO OFF

:Looper
 
ECHO 1. player stats (also enter stat, ie: str, con, dex, int, wis, char)
ECHO 2. inventory (also enter slot for description)
ECHO 3. inventory equipping (also enter slot to see if you can equip it. current example: 6 5) 
ECHO 4. inventory instance addition test
ECHO 5. location
ECHO 6. skills

set INPUT=
set /P INPUT=Pick test: %=%

lua.exe client\init.lua %INPUT%

pause
goto Looper
