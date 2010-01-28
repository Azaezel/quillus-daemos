from ResourceManager import *
from Rendering import *
import GameClient

#Establish the root skin directory
if MySkin:
    addResourceLocation("~/resources/ui/skins/MySkin", "FileSystem", "KoZ", False)
elif MySkin2:
    addResourceLocation("~/resources/ui/skins/MySkin2", "FileSystem", "KoZ", False)
else:
    addResourceLocation("~/resources/ui/skins/Default", "FileSystem", "KoZ", False)

#Establish the icon directory for skill icons, etc.
addResourceLocation("~/resources/ui/skins/MySkin", "FileSystem", "KoZ", False)


    


