--
-- Copyright (C) 2012 IITRBCS
-- Authors: Ali Paikan
-- CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
--
 
-- loading lua-yarp binding library
require("yarp")

    bot = yarp.Bottle()

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
    --pm_print("get_ar_status : " .. rep:toString())
    return rep    
end

-- Ask graspChecker whether the hand is full or empty
function check_grasp()    
    -- Call graspChekcer module to ask classifier to check if its full or empty
    local cmdGC = yarp.Bottle()    
    local repGC = yarp.Bottle()
    cmdGC:addString("check")
    gc_rpc:write(cmdGC, repGC)
    --pm_print("check_grasp  " .. repGC:toString())

    return repGC  
end

function get_ar_closed()
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("get")
    cmd:addString("holding")
    ar_rpc_io:write(cmd, rep)
    --pm_print("get_ar_closed :" .. rep:toString())
    return rep
end


PortMonitor.create = function(options)    
    -- setting constrasint 
    --file_name = options:find("file"):asString()
    file_name = "pm_drop_clas"   
    pm_print('created')

    --PortMonitor.setConstraint("e_taken")

    mod_cmd = false
    observing_hand = false
    AR_CMD = yarp.Bottle()

    ar_rpc_io = yarp.Port()
    ar_rpc = yarp.Port()
    gc_rpc = yarp.Port()
    ar_rpc:open("...")
    ar_rpc_io:open("...")
    gc_rpc:open("...")

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

    ret = yarp.NetworkBase_connect(gc_rpc:getName(), "/graspChecker/rpc:i")
    if ret == false then 	
        pm_print("cannot connect to /graspChecker/rpc:i")
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
    pm_print(" Accept " )
    -- e_taken is set when wither the object is in "reachable position", or there is no object and the robot is in home position. 
    -- slow down the commands to the action rendering port
    if (yarp.Time_now() - prev_cmd_time) < 10.0 then return false end


    pm_print(" About to get status " )
    --Check first that the arm is idle before doing anything else
    status = get_ar_status()
    pm_print("ARE get status returned: " .. status:toString())
    leftarm_idle = (status:find("left_arm"):asString() == "idle")
    if leftarm_idle ==false then
        pm_print("arm is idle ", status)
        return false
    end

    -- Check that the hand is closed (otherwise it can be in home position)
    status = get_ar_closed()
    pm_print("ARE get holding returned: " .. status:toString())
    lefthand_closed = (status:toString() == "[ack]")
    if lefthand_closed == false then
        pm_print("hand is open, robot is home ", status)
        return false
    end

    -- If the hand is closed (not on HOME), look at it
    if observing_hand == false then
        AR_CMD:clear()
        AR_CMD:addString("observe")
        observing_hand = true
        mod_cmd = true
        return true    
    end

    -- if all conditions are met and the robot is observing the hand:
    -- check whether we have an object in the hand or not
    observing_hand = false
    status = check_grasp()
    pm_print("graspChecker returned: " .. status:toString())
    lefthand_holding = (status:toString() == "[ok]")

    -- if the hand is empty, go home
    if lefthand_holding == false then
        AR_CMD:clear()
        AR_CMD:addString("home")
        AR_CMD:addString("head")
        AR_CMD:addString("arms")  
        mod_cmd = true 
        return true

    --if the hand is full, accept drop-away command
    else
       return true
    end
end

--
-- update is called when the port receives new data
-- @param thing The Things abstract data type
-- @return Things
PortMonitor.update = function(thing)

    prev_cmd_time = yarp.Time_now()

    pm_print("Time: " .. prev_cmd_time)
    -- in order to observe hand, we modify the ARE command
    --[[if mod_cmd == true then
        th = yarp.Things()
        th:setPortWriter(AR_CMD)
        mod_cmd = false
        
        pm_print("Thing: " .. AR_CMD:toString())        
        return th
    end
        ]]--
--[[
    PortMonitor.unsetEvent("e_taken")
    pm_print("dropping away")
    return thing
        ]]--

    th = yarp.Things()
    bot:clear()
    bot:addString("observe")
    th:setPortWriter(bot)
    return th
end


