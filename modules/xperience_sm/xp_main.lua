#!/usr/bin/lua 

-- Copyright: (C) 2016 iCub Facility - Istituto Italiano di Tecnologia (IIT)
-- Authors: Tanis Mar <tanis.mar@iit.it>
-- Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


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

-- XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
-------------------------------   INTERACTION FLOW   ----------------------------------

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

------------------------------------------------------------------------------------------
-- DEFINES

-- Declare consts.
OBJECT_MEMORY   = 0.5       -- seconds   
SENSITIVITY     = 0.8       -- 80 percent
XOFFSET         = 0.0       -- m
YOFFSET         = -0.03     -- m
ZOFFSET         = 0.0       -- m

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

-------------------------------------------------------------------------------------------------
-------------------              GRAMMAR                ----------------

-- XXX XXX
-- defining objects and actions vocabularies
objects = {"Octopus", "Lego", "Toy", "Ladybug", "Turtle", "Car", "Bottle", "Box"}
actions = {"{point at}", "{what is this}"}

-- defining speech grammar for Menu
grammar = "Return to home position | Calibrate on table | Where is the #Object | Take the #Object | Grasp the #Object | See you soon  | I will teach you a new object | "
       .."Touch the #Object | Push the #Object | Let me show you how to reach the #Object with your right arm | Let me show you how to reach the #Object with your left arm | "
        .."Forget #Object | Forget all objects | Execute a plan | What is this | This is a #Object | Explore the #Object "

-- defining speech grammar for Reward
grammar_reward = "Yes you are | No here it is | Skip it"

-- defining speech grammar for teaching a new object
grammar_track = "There you go | Skip it"

-- defining speech grammar for what function (ack)
grammar_whatAck = "Yes you are | No you are not | Skip it | Wrong this is a #Object"

-- defining speech grammar for what function (nack)
grammar_whatNack = "This is a #Object | Skip it"

-- defining speech grammar teach reach
grammar_teach = "Yes I do | No I do not | Finished"


-- -- -- -- -- -- -- -- Begin module -- -- -- -- -- -- -- -- -- -- -- 
require("yarp")
require("rfsm")

rf = yarp.ResourceFinder()
rf:setVerbose(false)
rf:configure(arg)
rf:setDefaultContext("AffordancesProject")

-- load state machine model and initalize it
fsm_file = rf:findFile("xp_root_fsm.lua")
fsm_model = rfsm.load(fsm_file)
fsm = rfsm.init(fsm_model)
rfsm.run(fsm)

--explore_helper_file = rf:findFile("aff-xperience_helper.lua")
--if explore_helper_file == "" then
--    print("Cannot find aff-xperience_helper.lua")
--    return false 
--end
--dofile(explore_helper_file)

--yarp_helper_file = rf:findFile("yarp_helper.lua")
--if yarp_helper_file == "" then
--    print("Cannot find yarp_helper.lua")
--    return false 
--end
--dofile(yarp_helper_file)

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

-- Initialization
print("Module running ... ")
t0 = yarp.Time_now()
math.randomseed( os.time() )
object_list = {}                        -- for keeping the memory of objects
target_object = {}                      -- targeted object
go_home()

shouldExit = false

repeat
    rfsm.run(fsm)
    yarp.Time_delay(0.1)
until shouldExit ~= false

print("finishing")
-- Deinitialize yarp network
yarp.Network_fini()
