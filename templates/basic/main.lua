-- Description: The main.lua file is the first (and only) Lua
-- script file that is loaded by the engine. It is the job of
-- the main.lua file to direct the engine as to what exactly
-- it should be doing.

require "server.lua"
require "client.lua"

-- Parameter: filename
-- The path to the Lua script to re-execute.
function exec(filename)
    package.loaded[filename] = nil
    return require(filename)
end

-- Parameter: filename
-- The path to the Lua script to reload.
function reload(filename)
    return exec(filename)
end

-- Parameter: parameters
-- A table passed in by the engine representing the startup
-- parameters of this given game instance.
-- Parameter: commandLine
-- A table passed in by the engine that contains command line
-- information passed into the engine itself. This is literally
-- just a pass-through of argv in the internal coding.
-- Description: This method is called when the engine is ready to
-- begin running the game. This behaves as the Lua-script equivalent
-- of the engine's main method in the internal coding.
function main(parameters, commandLine)
    -- Print the executable command line information
    print("main.lua: Running with the following command line parameters: ")

    output = "  "
    for key, parameter in pairs(parameters) do
        output = output .. " " .. parameter
    end

    print(output)
end