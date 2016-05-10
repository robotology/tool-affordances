--
-- Copyright (C) 2012 IITRBCS
-- Authors: Ali Paikan
-- CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
--
 
OBJECT_MEMORY   = 0.5       -- seconds   
SENSITIVITY     = 0.8       -- 80 percent
XOFFSET         = 0.0       -- m
YOFFSET         = -0.02       -- m
ZOFFSET         = 0.0       -- m

REACHABLE_ZONE_X  = {min=-0.38, max=-0.10}    -- meter
REACHABLE_ZONE_Y  = {min=-0.40, max=0.0}    -- meter

UPRIGHRT_ZONE_X = {min=-0.60, max=-0.38}
UPRIGHRT_ZONE_Y = {min=0.0, max=0.40}

UPLEFT_ZONE_X = {min=-0.60, max=-0.38}
UPLEFT_ZONE_Y = {min=-0.40, max=0.0}

BOTOMMRIGHRT_ZONE_X = {min=-0.38, max=-0.10}
BOTOMMRIGHRT_ZONE_Y = {min=0.0, max=0.40}

--
-- helper functions
--

function find(objects, item)
    for key, value in pairs(objects) do
        if overlap(item, value) == true then return key end
    end
    return nil
end

function overlap(object1, object2)
    dist = math.sqrt(math.pow(object1.u-object2.u, 2) + math.pow(object1.v-object2.v, 2))
    if dist < 5.0 then return true end
    return false
end

function leaky_integrate(blobs, t_now)
    -- update object list
    for i=0,blobs:size()-1 do
        obj = blobs:get(i):asList()
        c_x = (obj:get(2):asDouble() + obj:get(0):asDouble()) / 2.0
        c_y = (obj:get(3):asDouble() + obj:get(1):asDouble()) / 2.0
        if find(objects, {u=c_x, v=c_y}) == nil then 
            table.insert(objects, {u=c_x, v=c_y, w=0, t=t_now, x=0, y=0, z=0})
        end
    end

    for key, value in pairs(objects) do
        is_overlaped = false
        for i=0,blobs:size()-1 do
            obj = blobs:get(i):asList()
            c_x = (obj:get(2):asDouble() + obj:get(0):asDouble()) / 2.0 
            c_y = (obj:get(3):asDouble() + obj:get(1):asDouble()) / 2.0
            if overlap(value, {u=c_x, v=c_y}) == true then 
                is_overlaped = true
                break;
            end          
        end
        if is_overlaped == true then
            -- with 30fps after 2s it reaches the threshold 1.0
            value.w = value.w + 0.03
            if value.w > (2.0-SENSITIVITY) then value.w = 2.0-SENSITIVITY end
            value.t = t_now
        else
            value.w = value.w - (t_now-value.t)/OBJECT_MEMORY
        end
    end
end

function forget_expired_objects()
    for i=#objects,1,-1 do
        if objects[i].w < 0.0 then table.remove(objects, i) end
    end
end

function get_stable_objects_count(objects)
    local count = 0
    for i=1,#objects do        
        if objects[i].w > 1.0 then 
            count = count + 1 
        end
    end
    return count
end

function get_3d_pos(objects)
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("get")
    cmd:addString("s2c")
    for i=1,#objects do
        local obj = cmd:addList()
        obj:addDouble(objects[i].u)
        obj:addDouble(objects[i].v)
    end
    ar_rpc_io:write(cmd, rep)
    for i=0,rep:size()-1 do
        bt = rep:get(i):asList()
        objects[i+1].x = bt:get(0):asDouble()
        objects[i+1].y = bt:get(1):asDouble()
        objects[i+1].z = bt:get(2):asDouble()+0.04
    end
end


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

function say(msg)
    local cmd = yarp.Bottle()    
    cmd:addString(msg)
    ispeak_port:write(cmd)
end


function get_object_in_zone(objects, ZONE_X, ZONE_Y)
    closest_id = nil
    for i=1,#objects do
        if objects[i].w > 1.0 and
           objects[i].x <= ZONE_X.max and
           objects[i].x >= ZONE_X.min and
           objects[i].y <= ZONE_Y.max and
           objects[i].y >= ZONE_Y.min then
            if closest_id == nil or objects[closest_id].x < objects[i].x then
                closest_id = i
            end 
        end
    end
    if closest_id == nil then return nil end
    return objects[closest_id]
end


function get_tool_affordance() 
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("getAffordance")
    o3de_rpc:write(cmd, rep)
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

function perform_action(action, object)
    if action == "drag_down" then
        local cmd = yarp.Bottle()    
        local rep = yarp.Bottle()
        cmd:addString("drag3D")
        cmd:addDouble(object.x - 0.10)
        cmd:addDouble(object.y)
        cmd:addDouble(object.z + 0.00)
        cmd:addDouble(270)
        cmd:addDouble(math.abs(object.x -(BOTOMMRIGHRT_ZONE_X.min + 0.05)))
        tmanager_rpc:write(cmd, rep)
        return
    end
    if action == "drag_left_hand" then
        local cmd = yarp.Bottle()    
        local rep = yarp.Bottle()
        cmd:addString("drag3D")
        cmd:addDouble(object.x - 0.05)
        cmd:addDouble(object.y + 0.07)
        cmd:addDouble(object.z + 0.0)
        cmd:addDouble(180)
        cmd:addDouble(math.abs(object.y - (REACHABLE_ZONE_Y.max - 0.10)))
        cmd:addDouble(0.0)
        cmd:addInt(0)
        pm_print(cmd:toString())
        tmanager_rpc:write(cmd, rep)
        return
    end
    if action == "drag_left" then
        local cmd = yarp.Bottle()    
        local rep = yarp.Bottle()
        cmd:addString("drag3D")
        cmd:addDouble(object.x - 0.05)
        cmd:addDouble(object.y + 0.07)
        cmd:addDouble(object.z + 0.01)
        cmd:addDouble(180)
        cmd:addDouble(math.abs(object.y - (REACHABLE_ZONE_Y.max - 0.10)))
        pm_print(cmd:toString())
        tmanager_rpc:write(cmd, rep)
        return
    end
    if action == "drag_diagr" then
        local cmd = yarp.Bottle()    
        local rep = yarp.Bottle()
        cmd:addString("drag3D")
        cmd:addDouble(object.x - 0.06)
        cmd:addDouble(object.y - 0.02)
        cmd:addDouble(object.z + 0.01)
        cmd:addDouble(315)
        local a = math.abs(object.y - (REACHABLE_ZONE_Y.max - 0.10))
        local b = math.abs(object.x - (BOTOMMRIGHRT_ZONE_X.min + 0.05))
        cmd:addDouble(math.sqrt(a*a + b*b))
        pm_print(cmd:toString())
        tmanager_rpc:write(cmd, rep)
        return
    end

end

function ask_tool(tool_name)
    pm_print("grasp the tool")
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("graspTool")
    tmanager_rpc:write(cmd, rep)
  
    pm_print("set the tool name")
    cmd:clear()
    rep:clear()
    cmd:addString("setToolName")
    cmd:addString(tool_name)
    tmanager_rpc:write(cmd, rep)
 
    pm_print("find the tool pose")
    cmd:clear()
    rep:clear()
    cmd:addString("findPose")
    tmanager_rpc:write(cmd, rep)
    
end





