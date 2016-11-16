
dofile(rf:findFile("xp_interact_fsm.lua"))
dofile(rf:findFile("xp_funcs.lua"))
return rfsm.state {

   ----------------------------------
   -- state INITPORTS                  --
   ----------------------------------
   ST_INITPORTS = rfsm.state{
           entry=function()
                -- Open ports
                ret = port_blobs:open("/affXperience/blobs:i")
                ret = ret and port_acteff:open("/affXperience/act_eff:o")
                ret = ret and speechRecog_port:open("/affXperience/speechRecog")
                if ret == false then
                    print("\nERROR OPENING PORTS\n")
                    rfsm.send_events(fsm, 'e_error')
                end

                -- rpc
                ret = ar_rpc_io:open("/affXperience/are:rpc")
                ret = ret and o3de_rpc:open("/affXperience/o3de:rpc")
                ret = ret and tmanager_rpc:open("/affXperience/t3dm:rpc")
                ret = ret and ispeak_rpc:open("/affXperience/ispeak:rpc")
                ret = ret and affcollect_rpc:open("/affXperience/affcol:rpc")
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
                    -- Connect
                    ret = yarp.NetworkBase_connect("/lbpExtract/blobs:o",port_blobs:getName())
                    ret = ret and yarp.NetworkBase_connect(port_acteff:getName(), "/affCollector/aff:i")
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
   --[[
   ----------------------------------
   -- state RETREIVEMEMORY         --
   ----------------------------------
   ST_RETREIVEMEMORY = rfsm.state{
           entry=function()
                   ret = true
                   ret = ret and (IH_Expand_vocab(object_port, objects) == "OK")
                   
                   if ret == false then
                           rfsm.send_events(fsm, 'e_error')
                   end
           end
           },

   ----------------------------------
   -- state INITVOCABS             --
   ----------------------------------
   ST_INITVOCABS = rfsm.state{
           entry=function()
                   ret = true
                   for key, word in pairs(objects) do
                           ret = ret and (SM_RGM_Expand(speechRecog_port, "#Object", word) == "OK")
                   end

                   SM_Expand_asyncrecog(speechRecog_port, "icub-stop-now")

                   if ret == false then
                           rfsm.send_events(fsm, 'e_error')
                   end
           end
   },
]]--
   ----------------------------------
   -- state HOME                   --
   ----------------------------------
   ST_HOME = rfsm.state{
           entry=function()
               print("everything is fine, going home!")
               speak("Ready")
               go_home()
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
                   port_blobs:close()
                   port_acteff:close()
                   speechRecog_port:close()

                   -- close rpcs
                   ar_rpc_io:close()
                   o3de_rpc:close()
                   tmanager_rpc:close()
                   ispeak_rpc:close()
                   affcollect_rpc:close()

                   shouldExit = true;
           end
   },


   --------------------------------------------
   -- state INTERACT  is defined in xp_interact_fsm.lua --
   --------------------------------------------
   ST_INTERACT = interact_fsm,


   ----------------------------------
   -- setting the transitions      --
   ----------------------------------

   rfsm.transition { src='initial', tgt='ST_INITPORTS' },
   rfsm.transition { src='ST_INITPORTS', tgt='ST_CONNECTPORTS', events={ 'e_connect' } },
   rfsm.transition { src='ST_INITPORTS', tgt='ST_FATAL', events={ 'e_error' } },


   rfsm.transition { src='ST_CONNECTPORTS', tgt='ST_FINI', events={ 'e_error' } },
   rfsm.transition { src='ST_CONNECTPORTS', tgt='ST_HOME', events={ 'e_done' } },

   rfsm.transition { src='ST_HOME', tgt='ST_INTERACT', events={ 'e_done' } },
   rfsm.transition { src='ST_INTERACT', tgt='ST_FINI', events={ 'e_observe_done' } },

   --rfsm.transition { src='ST_CONNECTPORTS', tgt='ST_FINI', events={ 'e_error' } },
   --rfsm.transition { src='ST_CONNECTPORTS', tgt='ST_RETREIVEMEMORY', events={ 'e_done' } },
   --rfsm.transition { src='ST_RETREIVEMEMORY', tgt='ST_INITVOCABS', events={ 'e_done' } },
   
   --rfsm.transition { src='ST_RETREIVEMEMORY', tgt='ST_FINI', events={ 'e_error' } },
   --rfsm.transition { src='ST_INITVOCABS', tgt='ST_FINI', events={ 'e_error' } },
   --rfsm.transition { src='ST_INITVOCABS', tgt='ST_HOME', events={ 'e_done' } },   
}
