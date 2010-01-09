
from ResourceManager import *
from Rendering import *
import GameClient

print "Python Scripting Loaded."

## Don't hardcode the base... instead get the "gameRoot" environment variable

gameClient.getRenderingResourceService().addResourceLocation("~/resources",
                                                    "FileSystem", "KoZ", True)

print "Added resource location"

## TODO Execute Client.py (TR - I Don't know how todo this... do you?)
