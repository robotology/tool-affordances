
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
               repeat   -- (until true) - workaround to break loops (as continue)

                   -- slow down the commands to the action rendering port
                   if (yarp.Time_now() - t0) < 1.0 then
                       break
                   else
                       t0 = yarp.Time_now()
                   end

                   -- Read blobs and update objects in memory and associate zones
                   if update_objects(object_list) == true then
                       -- receiving blobs, so reset clean table counter
                       pm_print("reseting clean table counter")
                       cleanTableSec = 0
                       tableClean = false

                       -- if arm is busy, ignore blobs.
                       if check_left_arm_busy() == true then break end

                       -- decide which object to target and the corresponding action
                       target_object = select_object(object_list)
                       if target_object ~= nil then
                            print("Targeting object at = ".. target_object.x .. target_object.y .. target_object.z)

                            state = "select_action"      -- select action given affordance
                            rfsm.send_events(fsm,'e_selectact')
                            --[[
                            if  target_object.zone == "BOTTOMRIGHT" or target_object.zone == "BOTTOMLEFT" then
                                rfsm.send_events(fsm,'e_action')
                                state = "do_action"             -- these actions do not need tools
                            else
                                state = "check_affordance"      -- check if tool affords required action
                                rfsm.send_events(fsm,'e_checkaff')
                            end
                            ]]--
                        end


                    else        -- No stable blobs being received
                        if check_left_arm_busy() == false then
                            if cleanTableSec > 4 then       -- it waits for 3 seconds before considering that the table is clean
                                if tableClean == false then
                                    say("The table is now clean, hurray!")
                                    tableClean = true
                                end
                                if holdingTool == true then
                                    -- ask ARE to drop tool
                                    drop_tool()
                                    holdingTool = false
                                end
                                cleanTableSec = 0;
                            else
                                print("counting up")
                                cleanTableSec = cleanTableSec + 1;
                            end
                        end
                    end
                until true
                rfsm.yield(true)
                end
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



   ----------------------------------
   -- state SUB_SELECTACT           --
   ----------------------------------
   SUB_SELECTACT = rfsm.state{
            entry=function()
                speak("I am checking what action to do")
                print("State = " .. state)
                if check_affordance(action) then        -- task is doable with tool present
                    state = "do_action"
                    speak("I can do action ", action)
                    rfsm.send_events(fsm,'e_action')
                else                    -- task is NOT doable with tool present (if at all)

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
   -- state SUB_EXIT               --
   ----------------------------------
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
   rfsm.transition { src='SUB_OBSERVE', tgt='SUB_SELECTACT', events={ 'e_selectact' } },
   rfsm.transition { src='SUB_OBSERVE', tgt='SUB_DOACTION', events={ 'e_doaction' } },
   rfsm.transition { src='SUB_OBSERVE', tgt='SUB_EXIT', events={ 'e_exit' } },

   -- From state SELECTACT
   rfsm.transition { src='SUB_SELECTACT', tgt='SUB_SELECTTOOL', events={ 'e_selecttool' } },
   rfsm.transition { src='SUB_SELECTACT', tgt='SUB_DOACTION', events={ 'e_doaction' } },

   -- From state SELECT
   rfsm.transition { src='SUB_SELECT', tgt='SUB_OBSERVE', events={ 'e_observe' } },
   --rfsm.transition { src='SUB_SELECT', tgt='SUB_SELECT', events={ 'e_done' } },

   -- From state ACTION
   rfsm.transition { src='SUB_DOACTION', tgt='SUB_OBSERVE', events={ 'e_done' } },
}
