-- Description: This method is called when the game client
-- successfully connects to a remote server.
function GameClient:onConnected()
    print("Client: Connected")
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