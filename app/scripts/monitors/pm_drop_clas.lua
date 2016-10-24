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

-- Ask himrep whether the hand is full or empty
function get_hand_holding()
    
    --  look at hand
    local cmdARE = yarp.Bottle()    
    local repARE = yarp.Bottle()
    cmdARE:addString("observe")
    ar_rpc:write(cmdARE, repARE)


    -- Cal lclassifier to check if its full or empty
    local img = yarp.Bottle()
    img = img_in:read()
    himrep_img:write(img)

    local bb_bot = yarp.Bottle()
    local bb = yarp.Bottle()

    bb_bot = dispblob_bb:read(true);
    bb = bb_bot:get(0):asList();
    local tlx = bb:get(0):asDouble()
    local tly = bb:get(1):asDouble()
    local brx = bb:get(2):asDouble()
    local bry = bb:get(3):asDouble()

    print("[trainObserve] got bounding Box is ", tlx,", ", tly, ", ", brx,", ", bry)

    local cmdHR = yarp.Bottle()    
    local repHR = yarp.Bottle()
    cmdHR:addString("train")
    local options = yarp.Bottle()
    options  = cmdHR:addList():addList()
    options:add(bb_bot:get(0))
    
    himrep_rpc:write(cmdHR,repHR)
    return repHR    
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

    img_in = yarp.Port()
    dispblob_bb = yarp.Port()
    img_in:open("...")
    dispblob_bb:open("...")

    himrep_img = yarp.Port()
    himrep_rpc = yarp.Port()    
    himrep_img:open("...")
    himrep_rpc:open("...")


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

    ret = yarp.NetworkBase_connect(img_in:getName(), "/icub/camcalib/left/out")
    if ret == false then 	
        pm_print("cannot connect to /icub/camcalib/left/out")
        return false
    end
    ret = yarp.NetworkBase_connect(dispblob_bb:getName(), "/dispBlobber/roi/left:o")
    if ret == false then 	
        pm_print("cannot connect to /dispBlobber/roi/left:o")
        return false
    end

    ret = yarp.NetworkBase_connect(himrep_img:getName(), "/himrepClassifierHand/img:i")
    if ret == false then 	
        pm_print("cannot connect to /himrepClassifierHand/img:i")
        return false
    end
    ret = yarp.NetworkBase_connect(dispblob_bb:getName(), "/himrepClassifierHand/rpc")
    if ret == false then 	
        pm_print("cannot connect to /himrepClassifierHand/rpc")
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
    status = get_hand_holding()
    lefthand_holding = (status:toString() == "full")

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


