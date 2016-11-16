
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

function leaky_integrate(object_list, blobs, t_now)
    -- update object list
    for i=0,blobs:size()-1 do
        obj = blobs:get(i):asList()
        c_x = (obj:get(2):asDouble() + obj:get(0):asDouble()) / 2.0
        c_y = (obj:get(3):asDouble() + obj:get(1):asDouble()) / 2.0
        if find(object_list, {u=c_x, v=c_y}) == nil then 
            table.insert(object_list, {u=c_x, v=c_y, w=0, t=t_now, x=0, y=0, z=0, task = "none"})
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

function forget_expired_objects(object_list)
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
    print(" Getting 3D position of " .. #object_list .. " objects")
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("get")
    cmd:addString("s2c")
    for i=1,#object_list do
        print("Object " .. i .." at 2D " .. object_list[i].u .. "," .. object_list[i].v  )
        local obj = cmd:addList()
        obj:addDouble(object_list[i].u)
        obj:addDouble(object_list[i].v)
    end
    ar_rpc_io:write(cmd, rep)
    print("rep = " .. rep:toString())        
    
    for i=0,rep:size()-1 do
        bt = rep:get(i):asList()  
        -- write in i+1 because we are moving values from a C vector (0-indexed) to a LUA vector (1-indexed)   
        if bt then
            object_list[i+1].x = bt:get(0):asDouble()
            object_list[i+1].y = bt:get(1):asDouble()
            object_list[i+1].z = bt:get(2):asDouble()+0.04
            print("Object " ..  i+1 .. " at 3D " .. object_list[i+1].x .. "," .. object_list[i+1].y .. "," .. object_list[i+1].z )
        else
            object_list[i+1].x = rep:get(0):asDouble()
            object_list[i+1].y = rep:get(1):asDouble()
            object_list[i+1].z = rep:get(2):asDouble()+0.04
            print("Object " .. i+1 .. " at 3D " .. object_list[i+1].x .. "," .. object_list[i+1].y .. "," .. object_list[i+1].z )
            return true        
        end
    end
    return true
end


--/---------------------------------------------------------------------/
-- Updates tasks on global variable object_list
function update_tasks(object_list)
    for i=1,#object_list do
        local obj = object_list[i]
        local zone = get_object_zone(obj)
        local task = select_task(obj)                
        object_list[i].task = task
    end
    return true
end

--/---------------------------------------------------------------------/
function update_objects(object_list, blobs)
    local t_now = yarp.Time_now()
    leaky_integrate(object_list, blobs, t_now)
    forget_expired_objects(object_list)

    if get_stable_objects_count(object_list) == 0  then return false end
    
    -- slow down the commands to the action rendering port
    if (yarp.Time_now() - t0) < 1.0 then 
        return false 
    else
        t0 = yarp.Time_now()
    end
    
    print("object_list")
    -- get the 3D position
    get_3d_pos(object_list)
    update_tasks(object_list)
    return true
end


--/---------------------------------------------------------------------/
function target_object(object_list)

    -- Prioritize objects according to their location
    -- First check if there are any reachable objects
    local obj
    obj = get_object_in_zone(object_list, REACHABLE_ZONE_X, REACHABLE_ZONE_Y)
    if obj ~= nil then 
         print("Reachable object selected:" .. obj.x .. obj.y .. obj.z) 
         return obj
    end

    -- Second, objects pushable with hand
    obj = get_object_in_zone(object_list, BOTTOMRIGHT_ZONE_X, BOTTOMRIGHT_ZONE_Y)
    if obj ~= nil then
         print("Pushable by hand object seletected:" .. obj.x .. obj.y .. obj.z) 
         return obj
    end

    -- Third, pullable with tool from right side
    obj = get_object_in_zone(object_list, UPRIGHT_ZONE_X, UPRIGHT_ZONE_Y)
    if obj ~= nil then
         print("Object on up right zone selected:" .. obj.x .. obj.y .. obj.z) 
         return obj
    end

    -- Third, pullable with tool from left side
    obj = get_object_in_zone(object_list, UPLEFT_ZONE_X, UPLEFT_ZONE_Y)
    if obj ~= nil then
         print("Object on up left zone selected" .. obj.x .. obj.y .. obj.z) 
         return obj
    end

    print("Objects are too far!!") 
    return nil
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
function select_task(obj)

    local zone = get_object_zone(obj)
    local act = "no_act"
    -- based on the positon, choose action at random among the 2 possible on each zone
    if zone == "OUT" then
        say("Object out of reach!")
        print("Objects out of limits!!")
        return nil
    end

    if zone == "BOTTOMLEFT" then
        say("Object reachable!")
        print("Objects reachable!!")
        act = "take_hand"
        return 
    end
    if zone == "BOTTOMRIGHT" then
        say("Object bottomright!")
        print("Object bottomright!!")
        act ="drag_left_hand"
    end
    if zone == "UPLEFT" then
        say("Object upleft!")
        print("Object upleft!!")
        act = "drag_down_right"
        end
    end
    if zone == "UPRIGHT" then
        say("Object upright!")
        print("Object upright!!")
        act = "drag_down"
        end
    end

    print("[object] in zone ".. zone .. ":" .. obj.x .. obj.y .. obj.z .. " requires action " , act)

    return  act
end

--/---------------------------------------------------------------------/
-- Call affCollector
function check_affordance(task, thres) 
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("getAffs")
    affcollect_rpc:write(cmd, rep)
    print(reply:toString())

    tool = reply:get(0):asString()    
    if tool == "no_aff" then         
        return false 
    end 

    local affordance = reply:get(1):asDict()
    local aff_value = affordance:find(task) -- find success rate of the given affordance for active tool
    if aff_value:isNull() then
        return false
    end

    aff_rate = aff_value:asDouble()
    print("Returned aff_rate for task" .. task .. " is " .. aff_rate)
    if (aff_rate < thres)then
        return false
    end

    return true
end

--/---------------------------------------------------------------------/
function select_tool(task)
    -- get index of desired task
    local act_i = find_key(TOOL_ACTIONS, task)
    print("task ".. task .. " has index ".. act_i)

    -- Call affCollector to return the best tool's label for a given task
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("selectTool")
    cmd:addInt(act_i-1)      -- Transform to C++ 0-indexing
    affcollect_rpc:write(cmd, rep)

    local tool = rep:get(0):asString()
    print("Tool selected is " .. tool)

    return tool
end


function set_act_labels(TOOL_ACTIONS)
    -- Call affCollector to return the best tool's label for a given task
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("setactlabels")
    local act_labels = cmd:addList()
    for i=1,#TOOL_ACTIONS do
        act_labels:addString(TOOL_ACTIONS[i])
    end
    affcollect_rpc:write(cmd, rep)
    return true
end



--/---------------------------------------------------------------------/
function ask_for_tool(tool_name)

    speak("Give me the " .. tool_name)

    tool_file = "real/" .. tool_name
    print("Tool file: " .. tool_file)

    print("grasp the tool")
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("graspTool")
    tmanager_rpc:write(cmd, rep)
    print("--Reply" .. rep:get(0):asString())
    local tool_loaded = rep:get(0):asString()
    if tool_loaded == "not_loaded" then  return "invalid"   end

    speak("I have the tool " .. tool_loaded)
 
    print("find the tool pose")
    cmd:clear()
    rep:clear()    
    cmd:addString("findPose")
    print("--Sending".. cmd:toString())
    tmanager_rpc:write(cmd, rep)
    print("Reply".. rep:toString())
    local reply = rep:get(0):asString()
    if reply ~= "ok" then  
        speak("I could not find the pose" )
        print("I could not find the pose" )
        return "invalid"   
    end 

    print("get orientation")
    cmd:clear()
    rep:clear()
    cmd:addString("getOri")
    print("--Sending".. cmd:toString())
    o3de_rpc:write(cmd, rep) 
    print("--Reply".. rep:toString())
   
    local deg
    if rep:get(1):asDouble() then 
        deg = rep:get(1):asDouble()
        print("Deg:".. deg)
    else
        print("Orientation couldnt be retreived" )
        return "invalid"   
    end 
 
    local pose = deg2ori(deg)
    if pose == "out" then
        print("Pose out of limits" )
        return "invalid"       
    end 

    speak("I have the tool in pose " .. pose)

    tool_pose = tool_loaded .. "_" .. pose

    print("Deg: ".. deg ..  " -> Pose", pose)
    print("Tool".. tool_loaded .. " oriented ".. pose.. " -> tool-pose " .. tool_pose)

    return tool_pose
end



--/---------------------------------------------------------------------/
function go_home()   
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:clear()
    cmd:addString("home")
    cmd:addString("head")
    cmd:addString("arms")  
    ar_rpc_io:write(cmd, rep)
end

function speak(msg)
    local cmd = yarp.Bottle()    
    cmd:addString(msg)
    ispeak_port:write(cmd)
    yarp.Time_delay(1.0)
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
    
    print("Sending: acteff_bot ".. bot:toString())
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
function set_tool_label(tool_pose)
    --print("Sending label to affCollector")
    local cmd = yarp.Bottle()    
    local rep = yarp.Bottle()
    cmd:addString("setlabel")
    cmd:addString(tool_pose)
    --print("Sending: ".. cmd:toString())
    affcollect_rpc:write(cmd, rep)
    --print("Received:".. rep:toString())
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

--/---------------------------------------------------------------------/
function perform_action(action, object)
    if action == "drag_down" then
        local cmd = yarp.Bottle()    
        local rep = yarp.Bottle()
        cmd:addString("drag3D")
        cmd:addDouble(object.x - 0.10)
        cmd:addDouble(object.y)
        cmd:addDouble(object.z + 0.00)
        cmd:addDouble(270)
        cmd:addDouble(math.abs(object.x -(CENTER_X + 0.07)))
        print(cmd:toString())
        tmanager_rpc:write(cmd, rep)
        return true
    end
    if action == "drag_left" then
        local cmd = yarp.Bottle()    
        local rep = yarp.Bottle()
        cmd:addString("drag3D")
        cmd:addDouble(object.x - 0.05)
        cmd:addDouble(object.y + 0.07)
        cmd:addDouble(object.z + 0.01)
        cmd:addDouble(180)
        cmd:addDouble(math.abs(object.y - (CENTER_Y - 0.10)))
        print(cmd:toString())
        tmanager_rpc:write(cmd, rep)
        return true
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
        cmd:addDouble(0.0)              -- remove tool tilt
        cmd:addInt(0)                   -- select no tool
        pm_print(cmd:toString())
        tmanager_rpc:write(cmd, rep)
        return
    end
    if action == "drag_right" then
        local cmd = yarp.Bottle()    
        local rep = yarp.Bottle()
        cmd:addString("drag3D")
        cmd:addDouble(object.x - 0.06)
        cmd:addDouble(object.y - 0.02)
        cmd:addDouble(object.z + 0.01)
        cmd:addDouble(0)
        cmd:addDouble(math.abs(object.y - (CENTER_Y + 0.10)))
        print(cmd:toString())
        tmanager_rpc:write(cmd, rep)
        return true
    end
    if action == "drag_down_right" then
        local cmd = yarp.Bottle()    
        local rep = yarp.Bottle()
        cmd:addString("drag3D")
        cmd:addDouble(object.x - 0.06)
        cmd:addDouble(object.y - 0.02)
        cmd:addDouble(object.z + 0.01)
        cmd:addDouble(315)
        local a = math.abs(object.y - (CENTER_Y + 0.10))
        local b = math.abs(object.x - (CENTER_X + 0.05))
        cmd:addDouble(math.sqrt(a*a + b*b))
        print(cmd:toString())
        tmanager_rpc:write(cmd, rep)
        return true
    end
    if action == "drag_up" then
        local cmd = yarp.Bottle()    
        local rep = yarp.Bottle()
        cmd:addString("drag3D")
        cmd:addDouble(object.x + 0.04)
        cmd:addDouble(object.y)
        cmd:addDouble(object.z)
        cmd:addDouble(90)
        cmd:addDouble(math.abs(object.x -(CENTER_X - 0.07)))        
        print(cmd:toString())
        tmanager_rpc:write(cmd, rep)
        return true
    end
    if action == "drag_up_left" then
        local cmd = yarp.Bottle()    
        local rep = yarp.Bottle()
        cmd:addString("drag3D")
        cmd:addDouble(object.x + 0.03)
        cmd:addDouble(object.y + 0.02)
        cmd:addDouble(object.z)
        cmd:addDouble(135)
        local a = math.abs(object.y - (CENTER_Y - 0.07))
        local b = math.abs(object.x - (CENTER_X - 0.07))
        cmd:addDouble(math.sqrt(a*a + b*b))
        print(cmd:toString())
        tmanager_rpc:write(cmd, rep)
        return true
    end
    -- no action was done
    return false
end

