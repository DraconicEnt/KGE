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
