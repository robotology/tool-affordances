#!/usr/bin/lua 

-- Copyright: (C) 2016 iCub Facility - Istituto Italiano di Tecnologia (IIT)
-- Authors: Tanis Mar <tanis.mar@iit.it>
-- Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


-- XXX All these consts could be set by ini file
-- -- -- Declare consts.
OBJECT_MEMORY   = 0.5       -- seconds   
SENSITIVITY     = 0.8       -- 80 percent
DISP_LENGTH     = 0.15      -- length of the drag action

--                  -0.3                   0.05                     0.4    
--                 _________________________________________________ -> Y
--            -0.6 |                              U                |                    drag.deg   ind
--                 |                       LU  ^  ^  ^ RU          | R  = right             0       0
--                 |                            \ | /              | RU = right_up         45       1
--                 |                             \|/               | U  = up               90       2
--                 |                        L <---O---> R          | LU = lef_up          135       3
--                 |                             /|\               | L  = left            180       4
--                 |                            / | \              | LD = left_down       225       5   
--                 |                       LD  v  v  v  RD         | D  = down            270       6 
--                 |                              D                | RD = right_down      315       7
--            -0.1 |_______________________________________________|
--              X  V                      |   
--                                       0.0 
--

-- ACTION LIST
ACTION_LIST = {["no_act"] = -1, ["drag_right"] = 0, ["drag_right_up"] = 1, ["drag_up"] = 2, ["drag_left_up"] = 3, ["drag_left"] = 4, ["drag_left_down"] = 5, ["drag_down"] = 6, ["drag_right_down"] = 7}

-- ORIENTATION LIST
ORI_LIST = {-89.0, 0.0, 89.0}


TOOL_LIST = {"HOE1", "HOE2","HOE3", "HOK1","HOK2","HOK3", "RAK1", "RAK2", "RAK3", "SHO1","SHO2","SHO3","STI1","STI2","STI3"}
-- rakeBigBlue = RAK1, rakeBlue = RAK2,  rakeGreen = RAK3, shovelGreen = SHO1, shovelOrange = SHO2, shovelYellow = SHO3
-- TOOL_LIST_SIM = {"hoe0","hoe1", "hoe2", "hoe3","hoe4", "hoe5", "hoe6","hoe7", "hoe8", "hoe9", 
--                 "hok0","hok1", "hok2", "hok3","hok4", "hok5", "hok6","hok7", "hok8", "hok9", 
--                 "rak0","rak1", "rak2", "rak3","rak4", "rak5", "rak6","rak7", "rak8", "rak9", 
--                 "sti0","sti1", "sti2", "sti3","sti4", "sti5", "sti6","sti7", "sti8", "sti9", 
--                 "sho0","sho1", "sho2", "sho3","sho4", "sho5", "sho6","sho7", "sho8", "sho9", 
--                 "flaT", "star"}


---------------- Modifiable variables for module flow:: 

bin_aff = false
trials_x_tp = 2
repeat_times = 1
SIM = true
activeExp = false

t_i = 1                                -- tool index (1 indexed, 0 random)
ori_i = 1                               -- orientation index (1-indexed)
act_i = 0                               -- action index (0-indexed)


t_i = t_i + 1                          -- so the name given is indexed as the tools :)
-- -- -- -- -- -- -- -- Begin module -- -- -- -- -- -- -- -- -- -- -- 
--local signal = require("posix.signal")
require("yarp")

rf = yarp.ResourceFinder()
rf:setVerbose(false)
rf:configure(arg)
rf:setDefaultContext("AffordancesProject")

--robot = rf:check("robot",Value("icub")).asString()

explore_helper_file = rf:findFile("aff-explorer-TCDS_helper.lua")
if explore_helper_file == "" then
    print("Cannot find aff-explorer-TCDS_helper.lua")
    return false 
end
dofile(explore_helper_file)

yarp_helper_file = rf:findFile("yarp_helper.lua")
if yarp_helper_file == "" then
    print("Cannot find yarp_helper.lua")
    return false 
end
dofile(yarp_helper_file)

--signal.signal(signal.SIGINT, function(signum)
--  state = "exit";
--end)

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

-- XXX this should be deleted when module works (connections should go through yarpmanager):
if robot == "icub" then
    ret = yarp.NetworkBase_connect("/lbpExtract/blobs:o",port_blobs:getName())
    if ret == false then print("cannot connect  from /lbpExtract/blobs:o") end
else
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
    --> xx ask for tool from list (so it can load the model)
    --> xx do alignment and get pose
    --> xx tell affCollector to setlabel  of tool-pose
-- When it has it:
    -- >xx find objects at blobs
    --> xx decide action (random based on zone)
    --> xx perform action
    --> xx observe effect (success/fail) -> update affCollector (action , effect)
    --> xx Do around 60 times, until each action is done about 10 times (more or less)
    --> xx Ask to change tool(-pose) and repeat



--for key,value in pairs(ACTION_LIST) do print(key,value) end

 ------------------------------ Initialization -----------------------------
print("Module running ... ")
t0 = yarp.Time_now()
math.randomseed( os.time() )
state = "no_tool"
object_list = {}                        -- for keeping the memory of objects
object = {}
tp_trial_count = 0
repeat_i = 0
go_home()

-- -- -- -- -- -- -- -- -- -- -- updateModule -- -- -- -- -- -- -- -- -- -- -- 
while state ~= "exit" do

    print("")
    -- Get a tool
    if state == "no_tool" then
        print("State = ",state)
        if SIM then
            if activeExp == true then
                tool_name, ori_i, act_i = active_exploration()  
            else         
                tool_name = select_tool(TOOL_LIST_SIM, t_i)                 -- SIM
            end
        else
            tool_name = select_tool(TOOL_LIST, 0)                           -- REAL
        end
        print("Tool Selected:", tool_name)

        if tool_name  ~= " " then 
            if SIM then
                tool_pose = load_tool(tool_name, ORI_LIST[ori_i])           -- SIM             
            else
                tool_pose = ask_for_tool(tool_name)                         -- REAL        
            end
            if tool_pose ~= "invalid" then
                set_tool_label(tool_pose)         
                print("Tool-Pose:",tool_pose)
                state = "do_action"
                go_home()
            end
       end
    end

    -- Find object, perform action      
    if state ==  "do_action" then
        print("State = ",state)

        if SIM then                             -- SIM
            --        act_i = aff_generator(0)  -- data generator for TEST
            object = localize_object()
            if object.x < -0.1 and object.y > 0.01 then       -- if SIM is OFF, object will be 0,0,0
                -- act_i = explore(object)          -- find the location of the object and select action
                
                action = find_key(ACTION_LIST, act_i )

                print("Performing action ", action, "(index: ", act_i, ")")
                actOK = perform_action(action, object)           -- Perform selected action
                if actOK then
                    state = "comp_effect"                        
                else
                    print("Action ", action, "could not be executed" )
                end
                go_home()
            else
                print("Weird object values, SIM pbbly not working") 
                state = "exit" 
            end
        else                                            -- REAL

            local blobs = port_blobs:read(false)
            if blobs ~= nil and blobs:size() >= 0 then    
                if update_objects(blobs) == true then           -- updates the global variable objects (objects in memory)
                    object = object_list[1]               -- for exploring we assume there is only one object on the table
                    act_i = explore(object)                     -- find the location of the object and select action
                    action = find_key(ACTION_LIST, act_i)
                    if act_i >= 0 then 
                        print("Performing action ", action)
                        perform_action(action, object)           -- Perform selected action
                        print("Action Performed: ", action)
                        state = "comp_effect"                    
                    end
                    go_home()
                end
            end 
        end       
    end

    --  Observe and save effect  
    if state == "comp_effect" then
        print("")
        print("State = ",state)
        
        if SIM then
        -- Compute effect, save and reset
            print("Computing effect for tool-pose: ", tool_pose, "action", act_i)
            --local eff = aff_generator(1) -- generate act_i
            local object_prev = object                          -- saves previous object coordinates
            object = localize_object()
            local eff = comp_effect(object, object_prev, false)
            print("Effect Computed: ", eff)
            save_effect(act_i, eff)
            save_to_file()                  -- save to file after every action (so we dont loose anything, and the label is always correct.
            reset_cube()

        -- Control experimental flow
            if activeExp == true then
		        if repeat_i == repeat_times then 
		            state = "no_tool"
                    repeat_i = 0 
                else           
                    repeat_i = repeat_i +1
                    state = "do_action"
                end
            else
                act_i  = act_i + 1                          --increase action counted
                if math.fmod(act_i,8) == 0 then             -- restart cycle after all actions             
                    
                    if repeat_i == repeat_times then 
                        ori_i = ori_i + 1                       -- try next orientation
                        repeat_i = 0                    
                        print("Cycle repeated ", repeat_times , " times")
                        state = "no_tool"                       -- change tool-pose (change tool!)
                    else
                        print("Performed cycle ", repeat_i, " times.")
                        repeat_i = repeat_i +1
                    end
                    act_i = 0                               -- reset action counter (0-indexed)
                    print("Performed all actions for tool-pose: ", tool_pose)


                    if ori_i > #ORI_LIST then     -- change tool after all orientations have been tried
                        t_i = t_i + 1                           -- increase tool counter
                        ori_i = 1;                              -- reset orientation counter
                        print("Performed all actions for TOOL: ", tool_name)
                        print("Increasing tool index and reseting orientation")      
                    end
                else
                    state = "do_action"     
                end
            end

        else                        -- REAL
            local blobs = port_blobs:read(false);
            if blobs ~= nil and blobs:size() >= 0 then    
                local object_prev = object                          -- saves previous object coordinates
                if update_objects(blobs) == true then           -- updates the global variable objects (objects in memory)
                    object = object_list[1]
                    local eff = comp_effect(object, object_prev, bin_aff)
                    print("Effect Computed: ", eff)
                    save_effect(act_i, eff)                    
                    act_i  = act_i + 1
                    if act_i == 8 * #ORI_LIST then               -- 8 actions x #ORI orientations
                        act_i = 0                               -- actions are 0-indexed
                        t_i = t_i + 1
                        state = "no_tool"                
                        print("Performed all actions for tool-pose: ", tool_pose)
                        save_to_file()
                    else
                        state = "do_action"           
                    end
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
