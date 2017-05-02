% Load the workspace or file where the right models are saved
% In case they were left open
if exist('portOut', 'var')
    portOut.close;
    portIn.close;
end
clear

simF = 1;
%regr_mode = 'svm';    
regr_mode = 'gnn';

if (simF==1) 
    if strcmp(regr_mode,'gnn')
        load ws_pred_sim_1out50_OMSEGI_gnn
    else
        load ws_pred_sim_1out50_OMSEGI_svm
    end
else
    if strcmp(regr_mode,'gnn')
        load ws_pred_real_1out50_OMSEGI_gnn
    else
        load ws_pred_real_1out50_OMSEGI_svm
    end
end

for t=1:length(net_feat2aff) 
    if strcmp(regr_mode,'gnn')
        tools{t} = net_feat2aff{t}.userdata{1};
    elseif strcmp(regr_mode,'svm')        
        tools{t} = net_feat2aff{t}{3}{1};
    else
        printf('Please select a proper regression mode');
    end
end


initYarp

b=Bottle;
data = Bottle; 
feat_bottle= Bottle; 
out = Bottle; 
predEff = Bottle; 


while(~done)%run until you get the quit signal
     b = portIn.read( false );%use false to have a non blocking port    
     if (sum(size(b)) ~= 0) %check size of bottle 
         disp('Bottle received');
         tool_feats = [];
         %disp(b);         
         %checking for quit signal
         disp('Checking for quit signal')
         if (strcmp(b.toString, 'quit'))
             disp('Received quit signal');
             done=1;
             disp('Going to close the port');
             portIn.close;
             portOut.close;
         else
             
            % Read Data 
            disp('Processing data bottle');
            % Check the received bottle
            disp(b);
            fprintf('Read bottle form port, size %d \n', b.size() );
            
            % Read out tool name to select the model corresponding to the tested tool
            tool_name_java = b.get(0).asString();            
            tool_name = char(tool_name_java);
            fprintf('Read toolpose name \n %s \n',tool_name);
            foundCells = strfind(tools, tool_name);
            tool_id= find(~cellfun('isempty',foundCells));
            
            fprintf('Loading model trained without tool %s   \n', tool_name);
            
            
            %Check the format of the data sent from YARP 
            feat_bottle = b.get(1).asList(); %.get(0).asList();
            %disp('toolpose features:');
            %disp(voxHist);
            
            % Read out tool features
            bL = feat_bottle.size();
            for v = 0:bL-1;
                data = feat_bottle.get(v).asList();
                for i = 0:data.size()-1
                    tool_feats = [tool_feats data.get(i).asDouble()];
                end
            end
            subplot(2,1,1);
            bar(tool_feats);
            
            
            %% Predict   the model trained with all but 'tool'
            pred_affvector = som2som_predict(net_feat2aff{tool_id}, sM_tool{tool_id}, sM_aff{tool_id}, tool_feats, regr_mode);
            
            subplot(2,1,2);
            plot(pred_affvector);
            
            for i =1:length(pred_affvector)
                out.addDouble(pred_affvector(i));
            end
            fprintf('Replying to YARP: \n');
            disp(out);
            portOut.write(out);           
            fprintf('Data sent \n');             
            out.clear();
         end 
    end
    pause(0.1);
end
disp('Going to close the port');
portIn.close;
portOut.close;
