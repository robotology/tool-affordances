% transform old data format into new:
function [affData, affsMeans, affsStd] = old2newEffData(dataPose)

 numAct = 4;
 effTP = squeeze(dataPose(:,1,:));
 effTPhist = reshape(effTP, numAct, 3,132);
 
 numTP = size(effTPhist,3);
 
 affsMeans = zeros(numTP,numAct);
 affsStds = zeros(numTP,numAct);
 
 for tp_i =1 : numTP
     
     for act_i =1:numAct
         
        aff_mean = mean(effTPhist(act_i, :, tp_i));
        aff_std = std(effTPhist(act_i, :, tp_i));

        affsMeans(tp_i, act_i) = aff_mean;
        affsStd(tp_i, act_i) = aff_std;     

    end
 end