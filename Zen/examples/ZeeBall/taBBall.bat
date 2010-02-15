@echo off

set Path=%Path%;D:\Zen\dev\LIBXML2\win32

set Path=%Path%;D:\Zen\dev\ogre\ogre_1.6.0\lib;D:\Zen\dev\ogre\ogre_1.6.0\Samples\Common\bin\Debug;D:\Zen\dev\ogre\ogre_1.6.0\lib

set Path=%Path%;D:\Zen\dev\newton\NewtonSDK\sdk\dll

D:\Zen\dev\bin\ZGameLoader_d.exe client.xml ZeeBallClient lua scripts\client\init.lua hosting

pause
