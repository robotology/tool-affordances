
--
-- YARP helper functions
--


function open_ports(name_list) 
    ports = {}
    for i=1,#name_list do 
        local port = yarp.Port()
        if not port:open(name_list[i]) then
            print("Cannot open port "..name_list[i])
            os.exit(1)
        end
        table.insert(ports, port)
    end
    return unpack(ports)
end


function open_bufports(name_list) 
    bufports = {}
    for i=1,#name_list do 
        local port = yarp.BufferedPortBottle()
        if not port:open(name_list[i]) then
            print("Cannot open bufport "..name_list[i])
            os.exit(1)
        end
        table.insert(bufports, port)
    end
    return unpack(bufports)
end

function close_ports(name_list) 
    for i=1,#name_list do 
        if not port:close(name_list[i]) then
            print("Cannot close port "..name_list[i])
            return false
        end
    end
    return true
end


--/---------------------------------------------------------------------/
function say(msg)
    local cmd = yarp.Bottle()
    local rep = yarp.Bottle()        
    cmd:addString(msg)
    ispeak_rpc:write(cmd,rep)
end



