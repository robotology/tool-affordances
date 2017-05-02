function [actData, graspData, effData] = readPredData3D(source)
%dataDir= 'C:\Users\Tanis Mar\Dropbox\experiments\data50tools\affData';
% dataDir= 'C:\Users\Tanis Mar\Dropbox\experiments\data50tools\featsNewBB';
% dataDir= 'C:\Users\Tanis\Dropbox\experiments\data50tools\featsNewBB' 

if source == 0              % REAL DATA
    dataDir= './data/predData/real';   
else
   dataDir= './data/predData/simOK'; 
end

dd=dir(dataDir);

actData = [];
effData = [];
graspData = [];
for dirI =1:length(dd)
    if (length(dd(dirI).name)>3) && (strcmp(dd(dirI).name(1:4),'tool'))
        filename = [dataDir,'\',dd(dirI).name];
        fprintf('\nReading data from folder %s \n', filename);
        tool = dd(dirI).name;
        ffDataF= readAffData(dataDir,tool);

        
    end      
end

% Clean data from outliers
actData = cleanOutliers(actData);
graspData = cleanOutliers(graspData);
effData = cleanOutliers(effData);