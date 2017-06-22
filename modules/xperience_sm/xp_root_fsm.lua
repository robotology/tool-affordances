-- initialize yarp
if yarp == nil then
    require("yarp")
    yarp.Network()
end

-- find all required files
if rf ~= nil then
    xp_interact_fsm = rf:findFile("xp_interact_fsm.lua")
    xp_funcs = rf:findFile("xp_funcs.lua")
else
    xp_interact_fsm = "xp_interact_fsm.lua"
    xp_funcs = "xp_funcs.lua"
end

--dofile(rf:findFile("xp_interact_fsm.lua"))
--dofile(rf:findFile("xp_funcs.lua"))
dofile("xp_funcs.lua")
dofile("xp_interact_fsm.lua")


return rfsm.state {
    ----------------------------------
    -- entry of root state          --
    ----------------------------------
   entry=function()
       dofile(xp_funcs)
   end,

   ----------------------------------
   -- state INIT_XP                --
   ----------------------------------
   ST_INITXP = rfsm.state{
           doo=function()
                   ret = XP_initialize()
                   if ret == false then
                           rfsm.send_events(fsm, 'e_error')
                   else
                           rfsm.send_events(fsm, 'e_xp_ok')
                   end
           end
   },

   ----------------------------------
   -- state INITPORTS
   ----------------------------------
   ST_INITPORTS = rfsm.state{
           doo=function()
                print("Opening ports ... ")
                -- Open ports
                ret = blobs_port:open("/xperience_sm/blobs:i")
                ret = ret and acteff_port:open("/xperience_sm/act_eff:o")
                ret = ret and ispeak_port:open("/xperience_sm/ispeak:o")
                ret = ret and speechRecog_port:open("/xperience_sm/speechRecog")
                if ret == false then
                    print("\nERROR OPENING PORTS\n")
                    rfsm.send_events(fsm, 'e_error')
                end

                -- rpc
                ret = are_get:open("/xperience_sm/are_get:rpc")
                ret = ret and are_cmd:open("/xperience_sm/are_cmd:rpc")
                ret = ret and are_rpc:open("/xperience_sm/are_rpc:rpc")
                ret = ret and wholebody_rpc:open("/xperience_sm/wb:rpc")

                ret = ret and toolinc_rpc:open("/xperience_sm/toolinc:rpc")
                ret = ret and tmanager_rpc:open("/xperience_sm/t3dm:rpc")
                ret = ret and affcollect_rpc:open("/xperience_sm/affcol:rpc")
                if ret == false then
                    print("\nERROR OPENING RPC PORTS\n")
                    rfsm.send_events(fsm, 'e_error')
                end
           end
   },

   ----------------------------------
   -- state CONNECTPORTS           --
   ----------------------------------
   ST_CONNECTPORTS = rfsm.state{
           doo=function()
                    print("Connecting ports ... ")
                    -- Connect
                    if not yarp.NetworkBase_connect("/lbpExtract/blobs:o", blobs_port:getName()) then print (blobs_port:getName() .. " NOT connected")  else print (blobs_port:getName() .. " connected")  end
                    if not yarp.NetworkBase_connect(acteff_port:getName(),"/affCollector/aff:i") then print (acteff_port:getName() .. " NOT connected")  else print (acteff_port:getName() .. " connected")  end


                    -- Connect to RPCs
                    -- To other APP modules
                    if not yarp.NetworkBase_connect(toolinc_rpc:getName(),"/toolIncorporator/rpc:i") then print (toolinc_rpc:getName() .. " NOT connected")  else print (toolinc_rpc:getName() .. " connected")  end
                    if not yarp.NetworkBase_connect(tmanager_rpc:getName(),"/tool3DManager/rpc:i") then print (tmanager_rpc:getName() .. " NOT connected")  else print (tmanager_rpc:getName() .. " connected")  end
                    if not yarp.NetworkBase_connect(affcollect_rpc:getName(),"/affCollector/rpc:i") then print (affcollect_rpc:getName() .. " NOT connected") else print (affcollect_rpc:getName() .. " connected") end

                    -- AREs
                    if not yarp.NetworkBase_connect(are_cmd:getName(),"/actionsRenderingEngine/cmd:io") then print (are_cmd:getName() .. " NOT connected")  else print (are_cmd:getName() .. " connected")  end
                    if not yarp.NetworkBase_connect(are_rpc:getName(),"/actionsRenderingEngine/rpc") then print (are_rpc:getName() .. " NOT connected")  else print (are_rpc:getName() .. " connected")  end
                    if not yarp.NetworkBase_connect(are_get:getName(),"/actionsRenderingEngine/get:io") then print (are_get:getName() .. " NOT connected")  else print (are_get:getName() .. " connected")  end

                    -- Wholebody for reset
                    if not yarp.NetworkBase_connect(wholebody_rpc:getName(),"/wholeBodyDynamics/rpc:i") then print (wholebody_rpc:getName() .. " NOT connected")  else print (wholebody_rpc:getName() .. " connected")  end

                    -- Speech
                    if not yarp.NetworkBase_connect(speechRecog_port:getName(),"/speechRecognizer/rpc") then print (speechRecog_port:getName() .. " NOT connected") else print (speechRecog_port:getName() .. " connected") end
                    if not yarp.NetworkBase_connect(ispeak_port:getName(),"/iSpeak") then print (ispeak_port:getName() .. " NOT connected")  else print (ispeak_port:getName() .. " connected")  end
                end

   },


   ----------------------------------
   -- state INITVOCABS             --
   ----------------------------------
   ST_INITVOCABS = rfsm.state{
           doo=function()
                   print("Initializing vocabs ... ")
                   SM_Expand_asyncrecog(speechRecog_port, "icub-stop-now")

                   --if ret == false then
                    --       rfsm.send_events(fsm, 'e_error')
                   --end
           end
   },


   ----------------------------------
   -- state HOME                   --
   ----------------------------------
   ST_HOME = rfsm.state{
           entry=function()
               print("Module running ... ")
               t0 = yarp.Time_now()
               math.randomseed( os.time() )

               print("everything is fine, going home!")
               go_home(1)

               speak("Ready")

           end
   },


   ----------------------------------
   -- state FATAL                  --
   ----------------------------------
   ST_FATAL = rfsm.state{
           entry=function()
               print("Fatal!")
               shouldExit = true;
           end
   },

   ----------------------------------
   -- state FINI                   --
   ----------------------------------
   ST_FINI = rfsm.state{
           entry=function()
                   print("Closing...")
                   -- close ports
                   blobs_port:close()
                   acteff_port:close()

                   -- close rpcs
                   toolinc_rpc:close()
                   tmanager_rpc:close()
                   affcollect_rpc:close()

                   are_rpc:close()
                   are_get:close()
                   are_cmd:close()
                   wholebody_rpc:close()

                   ispeak_port:close()
                   speechRecog_port:close()

                   shouldExit = true;
           end
   },


   --------------------------------------------
   -- state INTERACT  is defined in xp_interact_fsm.lua --
   --------------------------------------------
   ST_INTERACT = xp_interact_fsm,

   ----------------------------------
   -- setting the transitions      --
   ----------------------------------

   rfsm.transition { src='initial', tgt='ST_INITXP' },
   rfsm.transition { src='ST_INITXP', tgt='ST_INITPORTS', events={'e_xp_ok'} },
   rfsm.transition { src='ST_INITXP', tgt='ST_FATAL', events={ 'e_error' } },

   rfsm.transition { src='ST_INITPORTS', tgt='ST_CONNECTPORTS', events={ 'e_done' } },
   rfsm.transition { src='ST_INITPORTS', tgt='ST_FATAL', events={ 'e_error' } },

   rfsm.transition { src='ST_CONNECTPORTS', tgt='ST_FINI', events={ 'e_error' } },
   rfsm.transition { src='ST_CONNECTPORTS', tgt='ST_INITVOCABS', events={ 'e_done' } },

   rfsm.transition { src='ST_INITVOCABS', tgt='ST_FINI', events={ 'e_error' } },
   rfsm.transition { src='ST_INITVOCABS', tgt='ST_HOME', events={ 'e_done' } },

   rfsm.transition { src='ST_HOME', tgt='ST_INTERACT', events={ 'e_done' } },
   rfsm.transition { src='ST_INTERACT', tgt='ST_FINI', events={ 'e_done' } },

}
