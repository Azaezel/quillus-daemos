#!/usr/bin/lua
package.path=package.path..";./?.lua;"

print(package.path)

require "bot"

require "quests.questEngine"
require "quests.ventin"
require "quests.barkeep"

runBot("irc.indiezen.org", 16667, "Rhapsody", "#test")
