
--dofile(rf:findFile("xp_interact_fsm.lua"))
dofile("xp_interact_fsm.lua")
--dofile(rf:findFile("xp_funcs.lua"))
return rfsm.state {

   ----------------------------------
   -- state INITPORTS                  --
   ----------------------------------
   ST_INITPORTS = rfsm.state{
           entry=function()
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
                ret = ar_rpc_io:open("/xperience_sm/are:rpc")
                ret = ret and o3de_rpc:open("/xperience_sm/toolinc:rpc")
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
           entry=function()
                    print("Connecting ports ... ")
                    -- Connect
                    ret = yarp.NetworkBase_connect("/lbpExtract/blobs:o", blobs_port:getName())
                    ret = ret and yarp.NetworkBase_connect(acteff_port:getName(), "/affCollector/aff:i")
                    ret = ret and yarp.NetworkBase_connect(o3de_rpc:getName(), "/objects3DExplorer/rpc:i")
                    ret = ret and yarp.NetworkBase_connect(affcollect_rpc:getName(), "/affCollector/rpc:i")
                    ret = ret and yarp.NetworkBase_connect(tmanager_rpc:getName(), "/tool3DManager/rpc:i")
                    ret = ret and yarp.NetworkBase_connect(ar_rpc_io:getName(), "/actionsRenderingEngine/cmd:io")
                    ret = ret and yarp.NetworkBase_connect(ispeak_port:getName(), "/iSpeak")
                    ret = ret and yarp.NetworkBase_connect(speechRecog_port:getName(), "/speechRecognizer/rpc")
                    if ret == false then
                        print("\n\nERROR WITH CONNECTIONS, PLEASE CHECK\n\n")
                        rfsm.send_events(fsm, 'e_error')
                    end
           end
   },


   ----------------------------------
   -- state INITVOCABS             --
   ----------------------------------
   ST_INITVOCABS = rfsm.state{
           entry=function()
                   print("Initializing vocabs ... ")
                   SM_Expand_asyncrecog(speechRecog_port, "icub-stop-now")

                   if ret == false then
                           rfsm.send_events(fsm, 'e_error')
                   end
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
               if set_act_labels(TOOL_ACTIONS) then 
                   print("Action labels set properly ")
               else
                   print("Prbolems setting action labels ")
               end


               object_list = {}                               -- for keeping the memory of objects
               target_object = {}                             -- targeted object
               state = "observe"
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
                   ispeak_port:close()
                   speechRecog_port:close()

                   -- close rpcs
                   ar_rpc_io:close()
                   o3de_rpc:close()
                   tmanager_rpc:close()
                   affcollect_rpc:close()

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

   rfsm.transition { src='initial', tgt='ST_INITPORTS' },
   rfsm.transition { src='ST_INITPORTS', tgt='ST_CONNECTPORTS', events={ 'e_done' } },
   rfsm.transition { src='ST_INITPORTS', tgt='ST_FATAL', events={ 'e_error' } },


   rfsm.transition { src='ST_CONNECTPORTS', tgt='ST_FINI', events={ 'e_error' } },
   rfsm.transition { src='ST_CONNECTPORTS', tgt='ST_INITVOCABS', events={ 'e_done' } },
   
   rfsm.transition { src='ST_INITVOCABS', tgt='ST_FINI', events={ 'e_error' } },
   rfsm.transition { src='ST_INITVOCABS', tgt='ST_HOME', events={ 'e_done' } },   

   rfsm.transition { src='ST_HOME', tgt='ST_INTERACT', events={ 'e_done' } },
   rfsm.transition { src='ST_INTERACT', tgt='ST_FINI', events={ 'e_done' } },

}
