#!/usr/bin/lua 

-- Copyright: (C) 2011 Robotics, Brain and Cognitive Sciences - Italian Institute of Technology (IIT)
-- Author: Ali Paikan <ali.paikan@iit.it>
-- Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


-- LUA_CPATH should have the path to yarp-lua binding library (i.e. yarp.so, yarp.dll) 
require("yarp")

-- initialize yarp network
yarp.Network()


-- call static member in this way
-- t = yarp.Time_now()

-- create and open sender/receiver ports
sender = yarp.Port()
sender:open("...")

-- connect sender to receiver
if not yarp.NetworkBase_connect(sender:getName(), "/objects3DExplorer/rpc:i") then
	print("cannot connect to the port!")
end


function get_tool_affordance() 
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("getAffordance")
    sender:write(cmd, rep)
    return rep
end

function reset_wholebody()
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addInt(0)
    wlbodey_rpc:write(cmd, rep)
end


function select_action(reply, zone_name) 
   tool = reply:get(0):asString()
   if tool == "no_aff" then 
        print(reply:toString())
        return  "not_affordable" 
    end
   affordance = reply:get(1):asDict()
   --print(affordance:toString())
   if zone_name == "UPRIGHT" then	
        if affordance:check("drag_down") == true then return "drag_down" end
        if affordance:check("drag_left") == true then return "drag_left" end
        -- if affordance:check("drag_diag_left") == true then return "drag_diagl" end
    elseif zone_name == "UPLEFT" then        
        if affordance:check("drag_diag_right") == true then return "drag_diagr" end
    end
    return "not_affordable"
end


afford = select_action(get_tool_affordance(), "UPRIGHT")

print("Selected action:", afford)


-- disconnect sender from receiver
yarp.NetworkBase_disconnect(sender:getName(), "/objects3DExplorer/rpc:i")

-- close the ports
sender:close()

-- Deinitialize yarp network
yarp.Network_fini()

