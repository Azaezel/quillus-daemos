
nickServ = 
{
	nick = "ZChan"
}

playerCommands["help"] = function (connection, channel, nick, command, words)
  connection.send("PRIVMSG " .. channel .. " :Whaddya think I am, a deckhand?")
end

ircServices["NickServ"] = nickServ;
