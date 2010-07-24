#!/usr/bin/lua
package.path=package.path..";./?.lua;"

require "irc"
require "playerCommands"
require "bot"

require "quests.questEngine"
require "quests.ventin"
require "quests.barkeep"

--Custom Commands for Ginger
require "gingerCommands"

runBot("irc.indiezen.org", 16667, "Ginger", "#test")
