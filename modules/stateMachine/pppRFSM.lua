
require("yarp")
return rfsm.state {

    ----------------------------------
	-- state INIT   	            --
	----------------------------------
	ST_INIT = rfsm.state{
		entry=function()
	        print(">> in ST_INIT")
            ret = ppp_rpc:open("/fsm/ppp:o")
            ret = ret and are_rpc:open("/fsm/are:o")
            ret = ret and fe_rpc:open("/fsm/fe:o")
            
            if ret == false then
                print("cannot open some ports")
                rfsm.send_events(fsm, "e_open_failed")
            else    
                rfsm.send_events(fsm, "e_open_ok")
            end    
		end
	},


    ----------------------------------
	-- state CONNECT   	            --
	----------------------------------
	ST_CONNECT = rfsm.state{
		entry=function()
	        print(">> in ST_CONNECT")
            ret = yarp.NetworkBase_connect(are_rpc:getName(), "/actionsRendeingEngine/cmd:io")
            ret = yarp.NetworkBase_connect(ppp_rpc:getName(), "/poeticonManager/human:rpc")
            --ret = yarp.NetworkBase_connect(karma_rpc:getName(), "/karma/rpc")
            ret = yarp.NetworkBase_connect(fe_rpc:getName(), "/featExt/rpc:i")
            if ret == false then
                print("cannot connect to some ports")
                rfsm.send_events(fsm, "e_con_failed")
            else    
                rfsm.send_events(fsm, "e_con_ok")
            end    
		end
	},

    ----------------------------------
	-- state GETTOOL                 --
	----------------------------------
	ST_GETTOOL = rfsm.state{
		entry = function()
            print(">> in ST_GETTOOL ")
            request = yarp.Bottle()
            reply = yarp.Bottle()
            request:clear()
            request:addString('tool')
            ppp_rpc:write(req, reply)
            if reply:get(0):asString() == "ack" then
                 rfsm.send_events(fsm, "e_gettool_ok")
                 print(">> tool taken")
            else
                 rfsm.send_events(fsm, "e_gettool_failed")
                 print(">> taking tool failed")
            end
		end
	},

	----------------------------------
	-- state OBJECT    	            --
	----------------------------------
	ST_OBJECT = rfsm.state{
		entry = function()
            print(">> in ST_OBJECT")
            request = yarp.Bottle()
            reply = yarp.Bottle()
            request:clear()
            request:addString('o')
            ppp_rpc:write(req, reply)           
            if reply:get(0):asString() == "ack" then
                 rfsm.send_events(fsm, "e_obj_ok")
                 print("Looking at object successfully")
            else
                 rfsm.send_events(fsm, "e_obj_failed")
                  print("Looking at object failed")
            end
		end
	},
    
	----------------------------------
	-- state ACTION   	            --
	----------------------------------
	ST_ACTION = rfsm.state{
		entry = function()
            print(">> in ST_ACTION")
            request = yarp.Bottle()
            reply = yarp.Bottle()
            request:clear()
            request:addString('action')
            ppp_rpc:write(req, reply)           
            if reply:get(0):asString() == "ack" then
                 rfsm.send_events(fsm, "e_action_ok")
            else
                 rfsm.send_events(fsm, "e_action_failed")
                 print("Performing action failed")
            end
		end
	},
    
    
	----------------------------------
	-- state FINI                   --
	----------------------------------
	ST_FINI = rfsm.state{
		entry=function()
			print(">> in ST_FINI")
            shouldExit = true
		end
	},


	----------------------------------
	-- setting the transitions      --
	----------------------------------

	rfsm.transition { src='initial', tgt='ST_INIT'},
    
    rfsm.transition { src='ST_INIT', tgt='ST_FINI', events={ 'e_open_failed' } },
    rfsm.transition { src='ST_INIT', tgt='ST_CONNECT', events={ 'e_open_ok' } },
    
    rfsm.transition { src='ST_CONNECT', tgt='ST_FINI', events={ 'e_con_failed' } },
    rfsm.transition { src='ST_CONNECT', tgt='ST_GETTOOL', events={ 'e_con_ok' } },
    
    rfsm.transition { src='ST_GETTOOL', tgt='ST_FINI', events={ 'e_gettool_failed' } },
	rfsm.transition { src='ST_GETTOOL', tgt='ST_OBJECT', events={ 'e_gettool_ok' } },
    
    rfsm.transition { src='ST_OBJECT', tgt='ST_FINI', events={ 'e_obj_failed' } },
	rfsm.transition { src='ST_OBJECT', tgt='ST_ACTION', events={ 'e_obj_ok' } },

    rfsm.transition { src='ST_ACTION', tgt='ST_FINI', events={ 'e_action_failed' } },
	rfsm.transition { src='ST_ACTION', tgt='ST_FINI', events={ 'e_action_ok' } },
}




