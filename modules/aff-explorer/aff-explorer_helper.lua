
--
-- exploring helper functions
--

--/---------------------------------------------------------------------/
function find(object_list, item)
    for key, value in pairs(object_list) do
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
        if find(object_list, {u=c_x, v=c_y}) == nil then 
            table.insert(object_list, {u=c_x, v=c_y, w=0, t=t_now, x=0, y=0, z=0})
        end
    end

    for key, value in pairs(object_list) do
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
    for i=#object_list,1,-1 do
        if object_list[i].w < 0.0 then table.remove(object_list, i) end
    end
end


function get_stable_objects_count(object_list)
    local count = 0
    for i=1,#object_list do        
        if object_list[i].w > 1.0 then 
            count = count + 1 
        end
    end
    return count
end

--/---------------------------------------------------------------------/
function get_3d_pos(object_list)
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("get")
    cmd:addString("s2c")
    for i=1,#object_list do
        local obj = cmd:addList()
        obj:addDouble(object_list[i].u)
        obj:addDouble(object_list[i].v)
    end
    ar_rpc_io:write(cmd, rep)

    for i=0,rep:size()-1 do
        bt = rep:get(i):asList()
        object_list[i+1].x = bt:get(0):asDouble()
        object_list[i+1].y = bt:get(1):asDouble()
        object_list[i+1].z = bt:get(2):asDouble()+0.04
    end
end

--/---------------------------------------------------------------------/
function update_objects(blobs)
    local t_now = yarp.Time_now()
    leaky_integrate(blobs, t_now)
    forget_expired_objects()

    if get_stable_objects_count(object_list) == 0  then return false end
    
    -- slow down the commands to the action rendering port
    if (yarp.Time_now() - t0) < 1.0 then 
        return false 
    else
        t0 = yarp.Time_now()
    end

    -- get the 3D position
    get_3d_pos(object_list)
    return true
end


--/---------------------------------------------------------------------/
function perform_action(action, object)
    if action == "drag_down" then
        local cmd = yarp.Bottle()    
        local rep = yarp.Bottle()
        cmd:addString("drag3D")
        if SIM then
            cmd:addDouble(object.x - 0.04)
            cmd:addDouble(object.y)
            cmd:addDouble(object.z)
        else
            cmd:addDouble(object.x - 0.10)
            cmd:addDouble(object.y)
            cmd:addDouble(object.z + 0.00)
        end
        cmd:addDouble(270)
        cmd:addDouble(math.abs(object.x -(CENTER_X + 0.07)))
        print(cmd:toString())
        tmanager_rpc:write(cmd, rep)
        return
    end
    if action == "drag_left" then
        local cmd = yarp.Bottle()    
        local rep = yarp.Bottle()
        cmd:addString("drag3D")
        if SIM then
            cmd:addDouble(object.x - 0.04)
            cmd:addDouble(object.y + 0.04)
            cmd:addDouble(object.z + 0.01)
        else
            cmd:addDouble(object.x - 0.05)
            cmd:addDouble(object.y + 0.07)
            cmd:addDouble(object.z + 0.01)
        end
        cmd:addDouble(180)
        cmd:addDouble(math.abs(object.y - (CENTER_Y - 0.10)))
        print(cmd:toString())
        tmanager_rpc:write(cmd, rep)
        return
    end
    if action == "drag_right" then
        local cmd = yarp.Bottle()    
        local rep = yarp.Bottle()
        cmd:addString("drag3D")
        if SIM then
            cmd:addDouble(object.x - 0.04)
            cmd:addDouble(object.y - 0.04)
            cmd:addDouble(object.z + 0.01)
        else
            cmd:addDouble(object.x - 0.06)
            cmd:addDouble(object.y - 0.02)
            cmd:addDouble(object.z + 0.01)
        end
        cmd:addDouble(0)
        cmd:addDouble(math.abs(object.y - (CENTER_Y + 0.10)))
        print(cmd:toString())
        tmanager_rpc:write(cmd, rep)
        return
    end
    if action == "drag_down_right" then
        local cmd = yarp.Bottle()    
        local rep = yarp.Bottle()
        cmd:addString("drag3D")
        if SIM then
            cmd:addDouble(object.x - 0.04)
            cmd:addDouble(object.y - 0.04)
            cmd:addDouble(object.z + 0.01)
        else
            cmd:addDouble(object.x - 0.06)
            cmd:addDouble(object.y - 0.02)
            cmd:addDouble(object.z + 0.01)
        end
        cmd:addDouble(315)
        local a = math.abs(object.y - (CENTER_Y + 0.10))
        local b = math.abs(object.x - (CENTER_X + 0.05))
        cmd:addDouble(math.sqrt(a*a + b*b))
        print(cmd:toString())
        tmanager_rpc:write(cmd, rep)
        return
    end
    if action == "drag_up" then
        local cmd = yarp.Bottle()    
        local rep = yarp.Bottle()
        cmd:addString("drag3D")
        if SIM then 
            cmd:addDouble(object.x + 0.04)
            cmd:addDouble(object.y)
            cmd:addDouble(object.z)
        else
            cmd:addDouble(object.x + 0.04)
            cmd:addDouble(object.y)
            cmd:addDouble(object.z)
        end
        cmd:addDouble(90)
        cmd:addDouble(math.abs(object.x -(CENTER_X - 0.07)))        
        print(cmd:toString())
        tmanager_rpc:write(cmd, rep)
        return
    end
    if action == "drag_up_left" then
        local cmd = yarp.Bottle()    
        local rep = yarp.Bottle()
        cmd:addString("drag3D")
        if SIM then
            cmd:addDouble(object.x + 0.04)
            cmd:addDouble(object.y + 0.04)
            cmd:addDouble(object.z)
        else
            cmd:addDouble(object.x + 0.03)
            cmd:addDouble(object.y + 0.02)
            cmd:addDouble(object.z)
        end
        cmd:addDouble(135)
        local a = math.abs(object.y - (CENTER_Y - 0.07))
        local b = math.abs(object.x - (CENTER_X - 0.07))
        cmd:addDouble(math.sqrt(a*a + b*b))
        print(cmd:toString())
        tmanager_rpc:write(cmd, rep)
        return
    end
--[[
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
]]--
end


--/---------------------------------------------------------------------/
function localize_object()
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("objLoc")
    print("Cmd: ", cmd:toString())
    tmanager_rpc:write(cmd, rep)
    print("Rep: ", rep:toString())

    local obj = {}
    obj.x = rep:get(0):asList():get(0):asDouble()
    obj.y = rep:get(0):asList():get(1):asDouble()
    obj.z = rep:get(0):asList():get(2):asDouble()
    print("Object located at x: ", obj.x, ", y: ", obj.y, ", z: ", obj.z)
    return obj
end

--/---------------------------------------------------------------------/
function get_objects_in_zone(object_list, ZONE_X, ZONE_Y)
    closest_id = nil
    for i=1,#object_list do
        if object_list[i].w > 1.0 and
           object_list[i].x <= ZONE_X.max and
           object_list[i].x >= ZONE_X.min and
           object_list[i].y <= ZONE_Y.max and
           object_list[i].y >= ZONE_Y.min then
            if closest_id == nil or object_list[closest_id].x < object_list[i].x then
                closest_id = i
            end 
        end
    end
    if closest_id == nil then return nil end
    return object_list[closest_id]
end


--/---------------------------------------------------------------------/
function get_object_zone(object)

    if object.y > MAX_Y or object.y < MIN_Y or object.x> MAX_X or object.x < MIN_X then
        return "OUT"
    end

    if object.y > CENTER_Y then
        if object.x > CENTER_X then
            return "BOTTOMRIGHT"  
        end
        if object.x < CENTER_X then
            return "UPRIGHT"
        end
    else
        if object.x > CENTER_X then
            return "BOTTOMLEFT"  
        end
        if object.x < CENTER_X then
            return "UPLEFT"
        end
    end
end


--/---------------------------------------------------------------------/
function explore(obj)

    local zone = get_object_zone(obj)
    local act = "no_act"
    -- based on the positon, choose action at random among the 2 possible on each zone
    if SIM then
        if zone == "OUT" or zone == "BOTTOMLEFT"  then
            local cmd = yarp.Bottle()    
            local rep = yarp.Bottle()
            cmd:addString("compEff")
            tmanager_rpc:write(cmd, rep)
            print("Objects out of limits, reseted!!")
            return -1
        end        
    else
        if zone == "OUT" or zone == "BOTTOMLEFT"  then
            say("Object out of reach!")
            print("Objects out of limits!!")
            return -1
        end
    end

    if zone == "UPLEFT" then
        if (math.random() >= 0.5) then 
            act = "drag_right"
        else            
            act = "drag_down_right"
        end
    end
    if zone == "UPRIGHT" then
        if (math.random() >= 0.5) then 
            act = "drag_left"
        else            
            act = "drag_down"
        end
    end
    if zone == "BOTTOMRIGHT" then
        if (math.random() >= 0.5) then 
            act = "drag_up"
        else            
            act ="drag_up_left"
        end
    end

    print("[object] in zone ", zone ,":", obj.x, obj.y, obj.z)

    return  ACTION_LIST[act]       -- return the action index (0 index, for c++)
end

function go_home()   
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:clear()
    cmd:addString("home")
    cmd:addString("head")
    cmd:addString("arms")  
    ar_rpc_io:write(cmd, rep)
end

--/---------------------------------------------------------------------/
function comp_effect(obj, obj_prev, bin_flag)    
    local eff
    --if binF == true, effect is binary (success/fail), where success is if the object changed its ZONE
    if bin_flag == true then
        local zone = get_object_zone(obj)
        local zone_prev = get_object_zone(obj_prev)
        if zone == zone_prev then
            eff = 0                 -- same zone: no success
        else
            eff = 1                 -- different zone: success
        end
    else

    --if binF == false, effect is euclidean, measured as the 2D distance displaced in plane XY
        eff = math.sqrt((obj.x-obj_prev.x)*(obj.x-obj_prev.x) + (obj.y-obj_prev.y)*(obj.y-obj_prev.y)) 
    end

    return eff
end

function save_effect(act_i, eff)
    local bot = yarp.Bottle()    
    bot:clear()
    bot:addInt(act_i)
    bot:addDouble(eff)
    
    print("Sending: acteff_bot ", bot:toString())
    port_acteff:write(bot)
end


function aff_generator(click)
    if click == 0 then                  -- return generated act_i
        return math.random(-1,5)
    else
        return math.random(0, 1)        -- return generated eff
   end
end

--/---------------------------------------------------------------------/
function select_tool(tool_list, tool_i)
    num_tools = #tool_list
    print("Selecting among ", num_tools , " tools")
    if tool_i > num_tools then
        print("Tool index exceeds the number of available tools ")
        return " "
    end
    if tool_i == 0 then     -- in this case select a random one
        tool_i = math.random(num_tools)
    end
    return tool_list[tool_i]
end

--/---------------------------------------------------------------------/
function deg2ori(deg)
    if (deg > 45.0) and (deg < 135.0) then        -- oriented left
        ori = "left"
    elseif ((deg < 45.0) and (deg > -45.0)) then  -- oriented front
        ori = "frnt";
    elseif (deg > -135.0) and (deg < -45.0) then  -- oriented right
        ori = "rght";
    else 
        return "outx";
    end
    return ori
end

--/---------------------------------------------------------------------/
function ask_for_tool(tool_name)

    say("Give me the " .. tool_name)

    tool_file = "real/" .. tool_name
    print("Tool file: ", tool_file)

    print("grasp the tool")
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("graspTool")
    print("Sending", cmd:toString())
    tmanager_rpc:write(cmd, rep)
    print("Reply", rep:get(0):asString())
    local reply = rep:get(0):asString()
    if reply ~= "ok" then  return "invalid"   end 
 
  
    print("set the tool name")
    cmd:clear()
    rep:clear()
    cmd:addString("setToolName")
    cmd:addString(tool_file)
    print("Sending", cmd:toString())
    tmanager_rpc:write(cmd, rep)
    print("Reply", rep:get(0):asString())    
    local reply = rep:get(0):asString()
    if reply ~= "ok" then return "invalid"   end 
 
    print("find the tool pose")
    cmd:clear()
    rep:clear()    
    cmd:addString("findPose")
    print("Sending", cmd:toString())
    tmanager_rpc:write(cmd, rep)
    print("Reply", rep:toString())
    local reply = rep:get(0):asString()
    if reply ~= "ok" then  
        say("I could not find the pose" )
        return "invalid"   
    end 

    print("get orientation")
    cmd:clear()
    rep:clear()
    cmd:addString("getOri")
    print("Sending", cmd:toString())
    o3de_rpc:write(cmd, rep) 
    print("Reply", rep:toString())
    local reply = rep:get(0):asString()
    if reply ~= "ack" then  return "invalid"   end 
 

    local deg = rep:get(1):asDouble()
    print("Deg", deg)
    local pose = deg2ori(deg)
    if pose == "out" then
        return "invalid"       
    end 

    say("I have the tool in pose " .. pose)

    tool_pose = tool_name .. "_" .. pose

    print("Deg: ", deg, " -> Pose", pose)
    print("Tool", tool_name , " oriented ", pose, " -> tool-pose " , tool_pose)

    return tool_pose
end

-- like ask_for_tool but for simulation
function load_tool(tool_name)

    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()

    print("load the tool")
    tool_file = "sim/" .. tool_name
    local deg =  math.random(-1, 1)
    deg = 80 *deg; -- (-80, 0, 80)
    cmd:addString("getToolByPose")
    cmd:addString(tool_name)
    cmd:addDouble(deg)
    tmanager_rpc:write(cmd, rep)

    print("set the tool pose")
    cmd:clear()
    rep:clear()
    cmd:addString("setPoseParam")
    cmd:addDouble(deg)
    tmanager_rpc:write(cmd, rep) 

    local pose = deg2ori(deg)
    if pose == "out" then
        return "invalid"       
    end 

    tool_pose = tool_name .. "_" .. pose

    print("Deg: ", deg, " -> Pose", pose)
    print("Tool", tool_name , " oriented ", pose, " -> tool-pose " , tool_pose)

    return tool_pose
end

--/---------------------------------------------------------------------/
function set_tool_label(tool_pose)
    --print("Sending label to affCollector")
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("setlabel")
    cmd:addString(tool_pose)
    --print("Sending: ", cmd:toString())
    affcollect_rpc:write(cmd, rep)
    --print("Received:", rep:toString())
    print("label ", tool_pose, "set in affCollector")
    return true
end

--/---------------------------------------------------------------------/
function save_affordances()
    --print("Sending label to affCollector")
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("savetofile")
    affcollect_rpc:write(cmd, rep)
    print("Affordance knowledge saved to file")
    return true
end


function find_key( list, value )
  for k,v in pairs(list) do
    if v==value then return k end
  end
  return nil
end


