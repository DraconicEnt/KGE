-- Description: The main.lua file is the first (and only) Lua
-- script file that is loaded by the engine. It is the job of
-- the main.lua file to direct the engine as to what exactly
-- it should be doing.

-- Description: This method is called when the game client
-- successfully connects to a remote server.
function GameClient:onConnected()
end

-- Description: This method is called when the client disconnects
-- from a remote host for any reason.
function GameClient:onDisconnected()
end

-- Description: This method is called for game clients
-- when a connection against some remote host has failed
-- for any reason.
function GameClient:onConnectFailed()
end

-- Description: This method is called for game servers
-- when it should perform a resource cleanup. For dedicated
-- servers this means that a total process kill is in order.
-- For listen servers, we generally only want to clean up
-- resources associated with running the server only.
function GameServer:onShutdown()
end

-- Description: This method is called when a client connects
-- to the server.
function GameServer:onClientConnected()
end

-- Description: This method is called when a client disconnects
-- from the game server.
function GameServer:onClientDisconnected()
end

-- Parameter: parameters
-- A table passed in by the engine that contains command line
-- information passed into the engine itself. This is literally
-- just a pass-through of argv in the internal coding.
-- Description: This method is called when the engine is ready to
-- begin running the game. This is behaves as the Lua-script equivalent
-- of the engine's main method in the internal coding.
function main(parameters)
	-- Print the executable command line information
	print("main.lua: Running with the following command line parameters: ")

	output = "	"
	for key, parameter in pairs(parameters) do
		output = output .. " " .. parameter
	end

	print(output)
end
