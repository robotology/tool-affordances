% XXX Load the workspace or file where the right models are saved
load modelsXXX


% initialize YARP
LoadYarp;
import yarp.BufferedPortImageRgb
import yarp.BufferedPortBottle
import yarp.Port
import yarp.Bottle
import yarp.Time
import yarp.ImageRgb
import yarp.Image
import yarp.PixelRgb
done=0;
b=Bottle;

portIn=BufferedPortBottle;        %port for reading "quit" signal
portOut=Port;               %port for sending image

%first close the port just in case
%(this is to try to prevent matlab from being unresponsive)
portIn.close;
portOut.close;

disp('opening ports...');
portIn.open('/matlab/read');
disp('opened port /matlab/read');
pause(0.5);
portOut.open('/matlab/data:o');
disp('opened port /matlab/data:o');
pause(0.5);
disp('done.');
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
            data = b.get(0).asList().get(0).asList().get(1).asList();
            disp(data);
            for i= 1:data.size()-2  % vectorize the data, ignoring the 2 space (32.0) caracters at the beginning and end of the bottle
                dataVec(i) = data.get(i).asDouble();
            end
            %normalize data
            testData = (dataVec-muNorm)./sigNorm;
            testData(isnan(testData)) = 0;
            
            % predict the tool affordance from its feature vectors
            %testLabel=0;
            %[predict_label, accuracy, decis_values] = ovrpredictBot(testLabel, testData, model);
            %[decis_value_winner, affClass] = max(decis_values,[],2);            
            predClass = testSVMaff(model,testData);
            fprintf('Affordance cluster: %i \n\n', predClass);
            
            %Given Choose the best action based on the tool pose
            predAff = bestCentroid(predClass,:);
            plot(predAff)
            
            out.addInt(predClass);
            effect = out.addList();
                    
            %Put the predicted affordance vector into a bottle
            for i = 1:length(predAff)
                effect.addDouble(predAff(i));
            end     
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
