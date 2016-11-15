#!/usr/bin/lua 

-- Copyright: (C) 2016 iCub Facility - Istituto Italiano di Tecnologia (IIT)
-- Authors: Tanis Mar <tanis.mar@iit.it>
-- Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


-- XXX All these consts could be set by ini file
-- -- -- DEclare consts.
OBJECT_MEMORY   = 0.5       -- seconds   
SENSITIVITY     = 0.8       -- 80 percent
XOFFSET         = 0.0       -- m
YOFFSET         = -0.03     -- m
ZOFFSET         = 0.0       -- m

--                  -0.3                   0.05                     0.4    
--                 _________________________|_________________________ -> Y
--            -0.6 |                      | |                        |
--                 |                        |                        |
--                 |         UPLEFT         |        UPRIGHT         |
--                 |                        |                        |
--           -0.41 |______________________|_|________________________|  (orig -0.38)
--                 |                      | |                        |
--                 |                        |                        |
--                 |       BOTTOMLEFT       |      BOTTOMRIGHT       |
--                 |                        |                        |
--            -0.1 |______________________|_|________________________|
--              X  V                      |    
--                                       0.0            -- area is dsipalced to right to help tool actions

-- LIMIT LINES
MIN_X = -0.6        -- meter
MAX_X = -0.1
MIN_Y = -0.4
MAX_Y = 0.4
CENTER_X = -0.41
CENTER_Y = 0.05

-- ZONES
-- UPRIGHT
UPRIGHT_ZONE_X = {min=MIN_X, max=CENTER_X}
UPRIGHT_ZONE_Y = {min=CENTER_Y, max=MAX_Y}

-- UPLEFT
UPLEFT_ZONE_X = {min=MIN_X, max=CENTER_X}
UPLEFT_ZONE_Y = {min=MIN_Y, max=CENTER_Y}

--BOTTOMRIGHT
BOTTOMRIGHT_ZONE_X = {min=CENTER_X, max=MAX_X}
BOTTOMRIGHT_ZONE_Y = {min=CENTER_Y, max=MAX_Y}

--BOTTOMLEFT
REACHABLE_ZONE_X  = {min=CENTER_X, max=MAX_X}    
REACHABLE_ZONE_Y  = {min=MIN_Y, max=CENTER_Y}      

-- ACTION LIST
TASK_LIST = {"no_act", "drag_down","drag_down_right", "drag_left", "drag_right", "take_hand", "drag_left_hand"}

TOOL_ACTIONS = {"drag_down","drag_down_right", "drag_left", "drag_right"}

--for key,value in pairs(ACTION_LIST) do print(key,value) end

-- TOOL LIST
TOOL_LIST = {"HOE1", "HOE2","HOE3","HOK1","HOK2", "HOK3", "RAK1", "RAK2", "RAK3",  "SHO1","SHO2","SHO3", "STI1","STI2","STI3"}
--rakeBlue = RAK2
--rakeGreen = RAK3 
--shovelGreen = SHO1
--shovelOrange = SHO2
--shovelYellow = SHO3

-- -- -- -- -- -- -- -- Begin module -- -- -- -- -- -- -- -- -- -- -- 
local signal = require("posix.signal")
require("yarp")

rf = yarp.ResourceFinder()
rf:setVerbose(false)
rf:configure(arg)
rf:setDefaultContext("AffordancesProject")

--robot = rf:check("robot",Value("icub")).asString()

explore_helper_file = rf:findFile("aff-xperience_helper.lua")
if explore_helper_file == "" then
    print("Cannot find aff-xperience_helper.lua")
    return false 
end
dofile(explore_helper_file)

yarp_helper_file = rf:findFile("yarp_helper.lua")
if yarp_helper_file == "" then
    print("Cannot find yarp_helper.lua")
    return false 
end
dofile(yarp_helper_file)

signal.signal(signal.SIGINT, function(signum)
  state = "exit";
end)

yarp.Network()

-- Declare ports
port_blobs = yarp.BufferedPortBottle()
port_acteff = yarp.Port()

--rpc
ar_rpc_io = yarp.RpcClient()
o3de_rpc = yarp.RpcClient()
tmanager_rpc = yarp.RpcClient()
ispeak_rpc = yarp.RpcClient()
affcollect_rpc = yarp.RpcClient()

-- Open ports
port_blobs:open("/affExplorer/blobs:i")
port_acteff:open("/affExplorer/act_eff:o")

-- rpc
ar_rpc_io:open("/affExplorer/are:rpc")
o3de_rpc:open("/affExplorer/o3de:rpc")
tmanager_rpc:open("/affExplorer/t3dm:rpc")
ispeak_rpc:open("/affExplorer/ispeak:rpc")
affcollect_rpc:open("/affExplorer/affcol:rpc")

-- Connect
if robot == "icub" then
    ret = yarp.NetworkBase_connect("/lbpExtract/blobs:o",port_blobs:getName())
    if ret == false then print("cannot connect  from /lbpExtract/blobs:o") end
end

ret = yarp.NetworkBase_connect(port_acteff:getName(), "/affCollector/aff:i")
if ret == false then print("cannot connect to /affCollector/aff:i") end

ret = yarp.NetworkBase_connect(o3de_rpc:getName(), "/objects3DExplorer/rpc:i")
if ret == false then print("cannot connect to /objects3DExplorer/rpc:i") end

ret = yarp.NetworkBase_connect(affcollect_rpc:getName(), "/affCollector/rpc:i")
if ret == false then print("cannot connect to /affCollector/rpc:i") end

ret = yarp.NetworkBase_connect(tmanager_rpc:getName(), "/tool3DManager/rpc:i")
if ret == false then print("cannot connect to /tool3DManager/rpc:i") end

ret = yarp.NetworkBase_connect(ar_rpc_io:getName(), "/actionsRenderingEngine/cmd:io")
if ret == false then print("cannot connect to /actionsRenderingEngine/cmd:io") end

-- XXX till here

-- Before exploring, check if it has a tool (received by rpc as label) 
-- If it doesnt, 
    --> ask for tool from list (so it can load the model)
    --> do alignment and get pose
    --> tell affCollector to setlabel  of tool-pose
-- When it has it:
    -- >xx find objects at blobs
    --> xx decide action (random based on zone)
    --> xx perform action
    --> observe effect (success/fail) -> update affCollector (action , effect)
    --> Do around 60 times, until each action is done about 10 times (more or less)
    --> Ask to change tool(-pose) and repeat

-- XXX have to change for:

-- Explore objects
    -- act on those possible without tool (bottom)
  -- Option user:
    -- select tool to act on further ones (probabilistaclly) --> from affCollector selectTool()
    -- ask for tool
        -- check if tool is the given one or not
        -- if yes: say 'thanks'
        -- if not: say 'this is not the tool I asked for'
        -- Check tool affordances (from affCollector)
            -- If required affordance available: do action (even if its another tool, but say 'i ll do it anyway') 
            -- If required affordance not available: ask for another tool (and say 'I cant do anyhitng with this tool')
    -- Do action
    -- Observe effect -> Update affCollector.
    -- Loop

  -- Option vision (FURTHER DEVELOPMENT):
    -- look to tool rack
    -- detect and recognize tools
    -- call affCollector to get affrodances of each tool in the possible poses
    -- Ask user to give best tool (or reach for it)


---------------- Modifiable variables for module flow:: 

bin_aff = false
SIM = false

 ------------------------------ Initialization -----------------------------
print("Module running ... ")
t0 = yarp.Time_now()
math.randomseed( os.time() )
state = "no_tool"
--state = "do_action"
object_list = {}                        -- for keeping the memory of objects
object = {}                             -- targeted object
tp_trial_count = 0
go_home()



if set_act_labels(TOOL_ACTIONS) then 
    print("Action labels set properly ")
else
    print("Prbolems setting action labels ")
end

-- -- -- -- -- -- -- -- -- -- -- updateModule -- -- -- -- -- -- -- -- -- -- -- 
while state ~= "exit" do

    print("")
    -- Observe the table to select object to act upon.
    if state == "observe" then
        print("State = ",state)
        local blobs = port_blobs:read(false)
        if blobs ~= nil and blobs:size() >= 0 then    
            if update_objects(object_list, blobs) == true then           -- updates objects in memory
                -- decide which object and corresponding action
                object =  target_object(object_list)
                if object ~= nil then
                    if object.task == "take_hand" or object.task == "drag_left_hand" then
                        state = "do_action"             -- these actiosn do not need tools
                    else        
                        state = "check_affordance"      -- check if tool affords, or which tool to ask for
                    end
                end
             end
         end
     end


    -- see if the desired task is doable
    if state == "check_affordance" then
        print("State = ",state)
        if check_affordance(object.task) then   -- task is doable with present tool
            state = "do_action"
        else
            state = "select_tool"               -- task is NOT doable with present tool
        end
    end
    

    -- Select a tool and get it
    if state == "select_tool" then
        print("State = ",state)
        tool_selected = select_tool(object.task)         
        print("Tool Selected:", tool_selected)

        if tool_selected  ~= "no_tool" then 
            tool_pose = ask_for_tool(tool_selected)   
            if tool_pose ~= "invalid" then
                -- XXX XXX Set tool received as active label on affCollector 
                -- XXX XXX Check if the tool given (tool_pose) is the same as asked for (tool_selected)
                -- If yes say thanks, and do action
                --  otherwise complain, and
                -- check affordance for given task.
                -- If action is doable, do it, otherwise ask for another tool 
                if 

                set_tool_label(tool_pose)         
                print("Tool-Pose:",tool_pose)
                state = "do_action"
                go_home()
            end
        else
            print("Tool Selection unsuccessful")
            state = "observe"
        end
    end


    -- Find object, perform action      
    if state ==  "do_action" then
        print("State = ", state)
        local blobs = port_blobs:read(false)
        if blobs ~= nil and blobs:size() >= 0 then    
            if update_objects(blobs) == true then           -- updates the global variable objects (objects in memory)
                object = object_list[1]               -- for exploring we assume there is only one object on the table
                act_i = explore(object)                     -- find the location of the object and select action
                action = find_key(ACTION_LIST, act_i)
                if act_i >= 0 then 
                    print("Performing action ", action)
                    local actOK = perform_action(action, object)
                    if actOK then
                        print("Action Performed: ", action)
                        state = "comp_effect"       
                    else
                        print("Action ", action, "could not be executed" )             
                    end           -- Perform selected action
                end
                --go_home()
            end
        end 
    end

    --  Observe and save effect  
    if state == "comp_effect" then
        print("")
        print("State = ",state)
        local blobs = port_blobs:read(false);
        if blobs ~= nil and blobs:size() >= 0 then    
            local object_prev = object                          -- saves previous object coordinates
            if update_objects(blobs) == true then           -- updates the global variable objects (objects in memory)
                object = object_list[1]
                local eff = comp_effect(object, object_prev, bin_aff)
                print("Effect Computed: ", eff)
                save_effect(act_i, eff)                    
                if tp_trial_count  >=  trials_x_tp then        -- after a number of trial with tool-pose, change tool
                    state = "no_tool"                
                    tp_trial_count = 0
                    save_affordances()
                else
                    state = "do_action"
                    tp_trial_count = tp_trial_count + 1                                   
                end
            end
        end
    end  

    yarp.Time_delay(0.2) -- acts as getPeriod()
end
   
-- close ports
port_blobs:close()
port_acteff:close()

-- close rpcs
ar_rpc_io:close()
o3de_rpc:close()
tmanager_rpc:close()
ispeak_rpc:close()
affcollect_rpc:close()

-- close YARP connection
yarp.Network_fini()
