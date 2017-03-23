--
-- Copyright (C) 2016 IIT iCub-facility
-- Authors: Ali Paikan, Tanis Mar
-- CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
--
 
-- loading lua-yarp binding library
require("yarp")

function pm_print(msg) print('['..file_name..'] ', msg) end

PortMonitor.create = function(options)    
    -- setting constrasint 
    --file_name = options:find("file"):asString()
    file_name = "pm_take"   
    pm_print('created')

    rf = yarp.ResourceFinder()
    rf:setDefaultContext("AffordancesProject")
    --rf:configure(arg)
    helper_file = rf:findFile("pm_take_helper.lua")
    if helper_file == "" then
        pm_print("Cannot find pm_take_helper.lua")
        return false 
    end
    dofile(helper_file)
    
    --PortMonitor.setConstraint("not e_taken and e_idle and e_pull_idle and e_ask_idle")

    ar_rpc_io = yarp.Port()
    ar_rpc = yarp.Port()
    o3de_rpc = yarp.Port()
    tmanager_rpc = yarp.Port()
    wlbodey_rpc = yarp.Port()
    ispeak_port = yarp.Port()

    ar_rpc:open("...")
    ar_rpc_io:open("...")
    o3de_rpc:open("...")
    tmanager_rpc:open("...")
    wlbodey_rpc:open("...")
    ispeak_port:open("...")

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

    ret = yarp.NetworkBase_connect(o3de_rpc:getName(), "/objects3DExplorer/rpc:i")
    if ret == false then 	
        pm_print("cannot connect to /objects3DExplorer/rpc:i!")
        return false
    end

    ret = yarp.NetworkBase_connect(tmanager_rpc:getName(), "/tool3DManager/rpc:i")
    if ret == false then 	
        pm_print("cannot connect to /tool3DManager/rpc:i!")
        return false
    end

    ret = yarp.NetworkBase_connect(wlbodey_rpc:getName(), "/wholeBodyDynamics/rpc:i")
    if ret == false then 	
        pm_print("cannot connect to /wholeBodyDynamics/rpc:i!")
        return false
    end

    ret = yarp.NetworkBase_connect(ispeak_port:getName(), "/iSpeak")
    if ret == false then 	
        pm_print("cannot connect to /iSpeak!")
        --return false
    end

    objects = {}            -- for keeping the memory of objects
    prev_cmd_time = yarp.Time_now()
    --PortMonitor.setTrigInterval(1.0)
    AR_CMD = yarp.Bottle()
    cleanTable = false
    tooFarSaid = false

    return true
end


PortMonitor.destroy = function()
    --yarp.NetworkBase_disconnect(ar_rpc:getName(), "/actionsRenderingEngine/rpc")
    --yarp.NetworkBase_disconnect(ar_rpc_io:getName(), "/actionsRenderingEngine/get:io")
    ar_rpc:close()
    ar_rpc_io:close()
    o3de_rpc:close()
    tmanager_rpc:close()
    wlbodey_rpc:close()
    ispeak_port:close()
    PortMonitor.unsetEvent("e_taken")
end


PortMonitor.accept = function(thing)
    AR_CMD:clear()

    --getStatus()
    blobs = thing:asBottle()
    if blobs:size() == 0 then 
        
        return false
 
    end

    local t_now = yarp.Time_now()
    leaky_integrate(blobs, t_now)
    forget_expired_objects()

    if get_stable_objects_count(objects) == 0  then
        -- Chack if the table is clean AND its not moving around.
--[[        if not leftarm_idle or lefthand_holding then 
            if cleanTable == false then
                cleanTable = true
                say("The table is now clean, hurray!")
            end
        end]]--
        return false    
--    else
--        cleanTable = false
    end

    -- slow down the commands to the action rendering port
    if (yarp.Time_now() - prev_cmd_time) < 1.0 then return false end
     
    get_3d_pos(objects)


    status = get_ar_status()
    if status:find("left_arm"):asString() == "idle" then 
        leftarm_idle = true
    else
        leftarm_idle = false
    end

    status = get_ar_holding()
    if status:toString() == "[ack]" then 
        lefthand_holding = true
    else
        lefthand_holding = false
    end

   -- do not do any other action if left arm is busy 
    if not leftarm_idle or lefthand_holding then 
        pm_print("left hand or arm is busy. ignoring objects on the table!")
        print("Status:", leftarm_idle, lefthand_holding, not leftarm_idle or lefthand_holding) 
        return false
    end
 
    --
    -- take the closest object in reachable zone
    --
    local closest_obj = get_object_in_zone(objects, REACHABLE_ZONE_X, REACHABLE_ZONE_Y)
    if closest_obj ~= nil then 
         print("[pm_take] reachable:", closest_obj.x, closest_obj.y, closest_obj.z) 
        --status = get_ar_status()
        --leftarm_idle = (status:find("left_arm"):asString() == "idle")
        --status = get_ar_holding()
        lefthand_holding = (status:toString() == "[ack]")

        AR_CMD:clear()
        AR_CMD:addString("take")
        pos = AR_CMD:addList()
        pos:addDouble(closest_obj.x + XOFFSET)
        pos:addDouble(closest_obj.y + YOFFSET)
        pos:addDouble(closest_obj.z + ZOFFSET)        
        return (leftarm_idle and not lefthand_holding)
    end


    local object_out_of_zone = true

    local btright_obj = get_object_in_zone(objects, BOTTOMRIGHT_ZONE_X, BOTTOMRIGHT_ZONE_Y)
    if btright_obj ~= nil then
        object_out_of_zone = false
        tooFarSaid = false
        print("[pm_take] btright:", btright_obj.x, btright_obj.y, btright_obj.z) 
        local action = "drag_left_hand"
        pm_print("i am doing "..action)
        perform_action(action, btright_obj)
        return false
    end


    local upleft_obj = get_object_in_zone(objects, UPLEFT_ZONE_X, UPLEFT_ZONE_Y)
    if upleft_obj ~= nil then
        object_out_of_zone = false
        tooFarSaid = false
        print("[pm_take] upleft:", upleft_obj.x, upleft_obj.y, upleft_obj.z) 
        local afford = get_tool_affordance()
        pm_print(afford:toString())
        local action = select_action(afford, "UPLEFT")
        if action == nil then
            pm_print("no reply from O3DE")
            print("no reply from O3DE")
            return false
        end
        if action ~= "not_affordable" then
            pm_print("i am doing "..action)
            perform_action(action, upleft_obj)
            return false
        end
    end
    
    local upright_obj = get_object_in_zone(objects, UPRIGHT_ZONE_X, UPRIGHT_ZONE_Y)
    if upright_obj ~= nil then
        object_out_of_zone = false
        tooFarSaid = false
        print("[pm_take] upright:", upright_obj.x, upright_obj.y, upright_obj.z) 
        local afford = get_tool_affordance()
        pm_print(afford:toString())
        local action = select_action(afford, "UPRIGHT") 
        if action == nil then
            pm_print("no reply from O3DE")
            print("no reply from O3DE")
            return false
        end
        if action ~= "not_affordable" then
            pm_print("i am doing "..action)
            perform_action(action, upright_obj)
            return false
        end     
    end
    
    if object_out_of_zone == true then
        if tooFarSaid == false then
            say("Objects are too far!")
            pm_print("Objects are too far!!!")
            tooFarSaid = true
        end
        return false
    end

    pm_print("I need a tool")
    say("I need a tool.")
    ask_tool()
 --[[   if get_object_in_zone(objects, UPRIGHT_ZONE_X, UPRIGHT_ZONE_Y) ~= nil then 
        --say("I need a tool. Give me the shovel, please!")
        --ask_tool("STI3") -- Yellow shovel
        print("Saying: 'I need a tool, Give me the stick, please!'")
        say("I need a tool. Give me the stick, please!")
        ask_tool("STI3") -- Markers stick

        --say("I need a tool. Give me the rake, please!")
        --ask_tool("real/rakeBlue")
    else
        say("I need a tool. Give me the rake, please!")
        print("Saying: 'I need a tool, Give me the rake, please!'")
        ask_tool("RAK2")  -- Blue rake
    end
]]--
    -- go home
    AR_CMD:clear()
    AR_CMD:addString("home")
    AR_CMD:addString("head")
    AR_CMD:addString("arms")    
    return true
end

--
-- update is called when the port receives new data
-- @param thing The Things abstract data type
-- @return Things
PortMonitor.update = function(thing)
    pm_print(AR_CMD:toString())
    reset_wholebody()
    prev_cmd_time = yarp.Time_now()
    th = yarp.Things()
    th:setPortWriter(AR_CMD)
    PortMonitor.setEvent("e_taken")
    return th
end

--PortMonitor.trig = function()
--end

