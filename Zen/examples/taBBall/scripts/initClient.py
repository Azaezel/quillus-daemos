
from ResourceManager import *
from Rendering import *
import GameClient

print "Python Scripting Loaded."

## Don't hardcode the base... instead get the "gameRoot" environment variable

resourceService.addResourceLocation("C:\\dev\\Zen\\examples\\taBBall\\resources",
                                                    "FileSystem", "Pong", True)

print "Added resource location"

## This is done in C++ for now

## Create the Left Wall
##wall = resourceService.loadResource("cube.mesh", "WallLeft")
##wall.setMaterialName("wall")
##wall.setNormaliseNormals(True)
##node = sceneService.createChildNode("WallLeft")
##node.attachResource(wall)
##node.setPosition(-95.0, 0.0, 0.0)
##node.setScale(0.05, 1.45, 0.1)
