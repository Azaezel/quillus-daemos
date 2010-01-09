#Defines is where all C++ calls are converted into readable Python variables
#Check out www.indiezen.org/wiki/wiki/KoZ/Scripts for the full complete list of available arguments.


from ResourceManager import *
from Rendering import *
import GameClient

#Allows the addition of a resource location with the following arguments
#(path, resource container type, resource container, recursive)
addResourceLocation = gameClient().getGUIResourceService().addResourceLocation


