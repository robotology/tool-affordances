#!/usr/bin/lua
print ("Including lua")

-- Copyright: (C) 2016 iCub Facility - Istituto Italiano di Tecnologia (IIT)
-- Authors: Tanis Mar <tanis.mar@iit.it>
-- Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

-- Description:
-- rFSM state machine controlling the flow of the tool use demo (aka Xperience demo).


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


-------------------------------   INTERACTION FLOW   ----------------------------------

-- Explore objects
    -- act on those possible without tool (bottom)
  -- Option user:
    -- If objects only tool-reachable -> ask for a tool
    -- Check tool affordances
        -- from affCollector
        -- from tool-affordances module
    -- If any useful action possible: execute best action
    -- If afforded actions not useful ask for another tool (and say 'I cant do anyhitng with this tool')
    -- Do action
    -- Observe effect -> Update affCollector.
    -- GoTo explore objects

  -- Option vision (FURTHER DEVELOPMENT):
    -- look to tool rack
    -- detect and recognize tools
    -- call affCollector to get affrodances of each tool in the possible poses
    -- Ask user to give best tool (or reach for it)

------------------------------------------------------------------------------------------
-- DEFINES

TOOL_SELECTION_FLAG = 0;

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

-- ZONES (in order of priority)
ZONES_LIST = {"BOTTOMLEFT", "BOTTOMRIGHT", "UPRIGHT", "UPLEFT", "OUT"}
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

--TOOL_ACTIONS = {"drag_left", "drag_down", "drag_down_right", "drag_right"}

--for key,value in pairs(ACTION_LIST) do print(key,value) end

-- TOOL LIST
--TOOL_LIST = {"HOE1", "HOE2","HOE3","HOK1","HOK2", "HOK3", "RAK1", "RAK2", "RAK3",  "SHO1","SHO2","SHO3", "STI1","STI2","STI3"}
TOOL_LIST = {"HOE1", "HOK1", "RAK2", "SHO3", "STI3"}

-------------------------------------------------------------------------------------------------
-------------------              GRAMMAR                ----------------

--tool_list_grammar = {HOE1 = "hoe square",
--                     HOE2 = "hoe diamond",
--                     HOE3 = "hoe metal",
--                     HOK1 = "one corner hook",
--                     HOK2 = "edgy hook",
--                     HOK3 = "all round hook",
--                     RAK1 = "big rake",
--                     RAK2 = "blue rake",
--                     RAK3 = "green rake",
--                     SHO1 = "green shovel",
--                     SHO2 = "orange shovel",
--                     SHO3 = "yellow shovel",
--                     STI1 = "tube stick",
--                     STI2 = "flat stick",
--                     STI3 = "markers stick"}

tool_list_grammar = {HOE1 = "hoe",
                     HOK3 = "hook",
                     RAK2 = "rake",
                     SHO3 = "shovel",
                     STI3 = "stick"}

-- defining speech grammar for Reward
-- grammar_reward = "Yes you are | No here it is | Skip it"

print("Constansts defined")

-- -- -- -- -- -- -- -- Begin module -- -- -- -- -- -- -- -- -- -- --
require("yarp")
print("yarp required")
require("rfsm")

yarp.Network()

print("YARP networked")

-- Declare ports
blobs_port = yarp.BufferedPortBottle()
ispeak_port = yarp.BufferedPortBottle()
acteff_port = yarp.Port()
speechRecog_port = yarp.Port()

--rpc
are_rpc_io = yarp.RpcClient()
toolinc_rpc = yarp.RpcClient()
tmanager_rpc = yarp.RpcClient()
affcollect_rpc = yarp.RpcClient()

rf = yarp.ResourceFinder()
rf:setVerbose(false)
rf:configure(arg)
rf:setDefaultContext("AffordancesProject")

-- load state machine model and initalize it
fsm_file = rf:findFile("xp_root_fsm.lua")
fsm_model = rfsm.load(fsm_file)
fsm = rfsm.init(fsm_model)
rfsm.run(fsm)

shouldExit = false
repeat
    rfsm.run(fsm)
    yarp.Time_delay(0.1)
until shouldExit ~= false

print("finishing")
-- Deinitialize yarp network
yarp.Network_fini()
