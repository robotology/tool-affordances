--
-- Copyright (C) 2012 IITRBCS
-- Authors: Ali Paikan
-- CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
--
 
-- loading lua-yarp binding library
require("yarp")

--
-- helper functions
--
function pm_print(msg) print('['..file_name..'] ', msg) end

function get_ar_status()
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("get")
    cmd:addString("status")
    ar_rpc:write(cmd, rep)
    return rep    
end

function get_ar_holding()
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("get")
    cmd:addString("holding")
    ar_rpc_io:write(cmd, rep)
    return rep
end


PortMonitor.create = function(options)    
    -- setting constrasint 
    --file_name = options:find("file"):asString()
    file_name = "pm_drop"   
    pm_print('created')

    PortMonitor.setConstraint("e_taken")

    ar_rpc_io = yarp.Port()
    ar_rpc = yarp.Port()
    ar_rpc:open("...")
    ar_rpc_io:open("...")

    ret = yarp.NetworkBase_connect(ar_rpc:getName(), "/actionsRenderingEngine/rpc")
    if ret == false then
        pm_print("cannot connect to /actionsRenderingEngine/rpc!")
        return false
    end

    ret = yarp.NetworkBase_connect(ar_rpc_io:getName(), "/actionsRenderingEngine/get:io")
    if ret == false then 	
        pm_print("cannot connect to /actionsRenderingEngine/get:io!")
        return false
    end
    prev_cmd_time = yarp.Time_now()
    return true
end


PortMonitor.destroy = function()
    yarp.NetworkBase_disconnect(ar_rpc:getName(), "/actionsRenderingEngine/rpc")
    yarp.NetworkBase_disconnect(ar_rpc_io:getName(), "/actionsRenderingEngine/get:io")
    ar_rpc:close()
    ar_rpc_io:close()
    PortMonitor.unsetEvent("e_taken")
end

PortMonitor.accept = function(thing)

    -- slow down the commands to the action rendering port
    if (yarp.Time_now() - prev_cmd_time) < 1.0 then return false end
    status = get_ar_status()
    leftarm_idle = (status:find("left_arm"):asString() == "idle")
    status = get_ar_holding()
    lefthand_holding = (status:toString() == "[ack]")

    return (leftarm_idle and lefthand_holding)
end

--
-- update is called when the port receives new data
-- @param thing The Things abstract data type
-- @return Things
PortMonitor.update = function(thing)
    PortMonitor.unsetEvent("e_taken")
    pm_print("dropping away")
    return thing
end


