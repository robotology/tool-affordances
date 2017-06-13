
event_table = {
   stop      = "e_exit",
   clean     = "e_clean",
   }

xp_interact_fsm = rfsm.state{

   ----------------------------------
   -- state SUB_OBSERVE               --
   ----------------------------------
   SUB_OBSERVE = rfsm.state{
           entry=function()
                 print("State = "..state)
                 print("in substate OBSERVE, checking objects on the table!")
           end,

           doo = function()
               while true do   
                    if update_objects(object_list) == true then           -- updates objects in memory and associates task
                        -- decide which object and corresponding action
                        target_object = select_object(object_list)
                        if target_object ~= nil then
                            print("Targeting object at = ".. target_object.x .. target_object.y .. target_object.z)
                            if  target_object.zone == "BOTTOMRIGHT" or target_object.zone == "BOTTOMLEFT" then
                                rfsm.send_events(fsm,'e_action')
                                state = "do_action"             -- these actions do not need tools
                            else        
                                state = "check_affordance"      -- check if tool affords
                                rfsm.send_events(fsm,'e_checkaff')
                            end
                        end
                     end



--[[
                    XXX eventually make a previous state "WAIT" where it waits for 'clean' or 'stop'
                    result = SM_Reco_Grammar(speechRecog_port, grammar)
                    print("received REPLY: ".. result:toString() )
                    local cmd =  result:get(1):asString()
                    rfsm.send_events(fsm, event_table[cmd])
]]--
                rfsm.yield(true)
               end
           end
   },


   ----------------------------------
   -- state SUB_CHECKAFF           --
   ----------------------------------
   SUB_CHECKAFF = rfsm.state{
            entry=function()
                speak("I am checking if I can do the action")
                print("State = " .. state)
                if check_affordance(action) then        -- task is doable with tool present
                    state = "do_action"
                    speak("I can do action ", action)
                    rfsm.send_events(fsm,'e_action')
                else                                    -- task is NOT doable with tool present (if at all)

                    if tool_selection_flag then 
                        state = "select_tool"           
                        rfsm.send_events(fsm,'e_select')
                    else    
                        state = "ask_tool"
                    end
                    speak("I need a tool ")

                end 
            end
   },



   ----------------------------------
   -- state SUB_SELECT             --
   ----------------------------------
   SUB_SELECT = rfsm.state{
           entry=function()
                print("State = "..state)
                tool_selected = select_tool(action)         
                print("Tool Selected:".. tool_selected)

                if tool_selected  ~= "no_tool" then 
                    tool_given = ask_for_tool(tool_selected)   -- Grasps and recognizes tool
                    print("Tool given: " .. tool_given)
                    if tool_given ~= "invalid" then
                        set_tool_label(tool_given)    --  Set tool received as active label on affCollector 
                        -- Check if the tool given (tool_pose) is the same as asked for (tool_selected)
                        if (tool_given == tool_selected) then 
                            speak("Thanks!")      
                            print("Thanks!")
                            go_home(0)
                            state = "observe"
                            rfsm.send_events(fsm,'e_observe')
                        else
                            speak("Not the tool I asked for")      
                            print("Not the tool I asked for...")
                            if check_affordance(action) then
                                print("...but I can do the action")
                                speak(" but I will do the action", action, " anyway")
                                go_home(0)
                                state = "observe"
                                rfsm.send_events(fsm,'e_observe')
                            else
                                print("... and its not useful")
                                speak("and I can not do the action", action) 
                                state = "select_tool"            
                                --rfsm.send_events(fsm,'e_done')            
                            end
                        end                
                    else
                        print("Could not get the tool, lets try again")
                        state = "select_tool"
                        rfsm.send_events(fsm,'e_observe')
                    end
                else
                    print("Could not select a proper tool")
                    state = "observe"
                end
                go_home(0)                
           end
   },


   ----------------------------------
   -- state SUB_ACTION             --
   ----------------------------------
   SUB_ACTION = rfsm.state{
          entry=function()
                print("State = "..state)
                print("Performing action ", action)
                local actOK = perform_action(action, target_object)
                if actOK then
                    print("Action Performed: ", action)
                    -- state = "comp_effect"
                    state = "observe"  
                else
                    print("Action ", action, "could not be executed")                                
                end 
          end
   },


   SUB_EXIT = rfsm.state{
           entry=function()
                   speak("Ok, bye bye")
                   rfsm.send_events(fsm, 'e_observe_done')
           end
   },

   ----------------------------------
   -- state transitions            --
   ----------------------------------

   rfsm.trans{ src='initial', tgt='SUB_OBSERVE'},

   -- From base state OBSERVE
   rfsm.transition { src='SUB_OBSERVE', tgt='SUB_CHECKAFF', events={ 'e_checkaff' } },
   rfsm.transition { src='SUB_OBSERVE', tgt='SUB_ACTION', events={ 'e_action' } },
   rfsm.transition { src='SUB_OBSERVE', tgt='SUB_EXIT', events={ 'e_exit' } },

   -- From state CHECKAFF
   rfsm.transition { src='SUB_CHECKAFF', tgt='SUB_SELECT', events={ 'e_select' } },
   rfsm.transition { src='SUB_CHECKAFF', tgt='SUB_ACTION', events={ 'e_action' } },

   -- From state SELECT
   rfsm.transition { src='SUB_SELECT', tgt='SUB_OBSERVE', events={ 'e_observe' } },
   --rfsm.transition { src='SUB_SELECT', tgt='SUB_SELECT', events={ 'e_done' } },

   -- From state ACTION
   rfsm.transition { src='SUB_ACTION', tgt='SUB_OBSERVE', events={ 'e_done' } },
}
