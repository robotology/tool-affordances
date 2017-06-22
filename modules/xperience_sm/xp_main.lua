#!/usr/bin/lua

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


-- -- -- -- -- -- -- -- Begin module -- -- -- -- -- -- -- -- -- -- --
require("yarp")
require("rfsm")

yarp.Network()

shouldExit = false
-- Set ResourceFinder
rf = yarp.ResourceFinder()
rf:setDefaultContext("AffordancesProject")
rf:configure(arg)
rf:setVerbose(false)

-- load state machine model and initalize it
fsm_file = rf:findFile("xp_root_fsm.lua")
fsm_model = rfsm.load(fsm_file)
fsm = rfsm.init(fsm_model)
rfsm.run(fsm)

repeat
    rfsm.run(fsm)
    yarp.Time_delay(0.1)
until shouldExit ~= false

print("finishing")
-- Deinitialize yarp network
yarp.Network_fini()
