% XXX Load the workspace or file where the right models are saved
load modelsXXX
% We need here the K models model{} trained for the bestK, sMap, clusterIdx
% muNormIn{}, sigNormIn{},

initYarp

b=Bottle;
data = Bottle; 
out = Bottle; 
effect = Bottle; 


while(~done)%run until you get the quit signal
    
     b = portIn.read( false );%use false to have a non blocking port    
     if (sum(size(b)) ~= 0) %check size of bottle 
         %disp(b);
         out.clear();
         data.clear();
         %checking for quit signal
         if (strcmp(b.toString, 'quit'))
             done=1;
         else
             
            % Read Data 
            dataVec = zeros(1,75);
            disp('receiving bottle');
            disp(b);
            %data = b.get(1).asList();
            %Check the format of the data sent from YARP
            data = b.get(0).asList().get(0).asList().get(1).asList();
            disp(data);
            for i= 1:data.size()-2  % vectorize the data, ignoring the 2 space (32.0) caracters at the beginning and end of the bottle
                dataVec(i) = data.get(i).asDouble();
            end
            
            %% Find the model to apply for the received test sample (1xdimFeat)
            sDataTest = som_data_struct(dataVec,'name','testData');
            % Find the toolPose sample best matching unit (BMU, index of cell in SOM)
            testBMUs = som_bmus(sMap,sDataTest);
            % Find the cluster/model to which each test sample corresponds.
            testClusterIdx = somClusterIdx(testBMUs);
            
            %  model{testClusterIdx}
            % For that model, sample a range of possible actions            
            in = 0:20:360;
            lin = length(in);
            
            %Normalize the input with the normalization variables of the given model.
            inNorm = (in - muNormIn{testClusterIdx})./sigNormIn{testClusterIdx};    
            
            %Predict the output for all the imagined actions
            imaginedOutNorm = model{testClusterIdx}(inNorm);
            imaginedOut = outNorm.*sigNormOut{c} + muNormOut{testClusterIdx}; % denormalize to get real distance values
            
            % plot imagined effects
            figure(300)
            plot(in,out,'linewidth',2,'color',[1 0 0]);   
            axis([-10 360 0 0.25])
            xlabel('Action - Pull Angle');
            ylabel('Effect (cm)');            
            
            %Select best action
            bestAct = max(out);
            
            out.addInt(bestAct);
            fprintf('Replying to YARP: \n');
            disp(out);
            portOut.write(out);
            
         end
     
    end
    pause(0.01);
end
disp('Going to close the port');
portIn.close;
portOut.close;
