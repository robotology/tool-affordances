%dataDir= '..\data\dataICRA\dataSim';
%dataDir= '..\data\dataICRA\dataRobot';
dataDir = 'C:\iCub\Projects\toolModeler\affData\dataSIM' ;
%dataDir= 'C:\ProgramsTanis\MATLAB\data\dataOld\dataSIM';
dd=dir(dataDir);

normFlag = 1;
plotF = 1;
toolPoseI =1;
dataAffsAll = [];
dataFeatsAll = [];
stdTP=[];
meanTP =[];
        
Ntools = 7;
Npose= 3;
if plotF ==1
    figure;end;
for dirI =1:length(dd)
    if (length(dd(dirI).name)>3)
        tool = dd(dirI).name;
        [dataAff{toolPoseI}, toolLabels{toolPoseI}] = readActEff(dataDir,tool);        
        [dataFeatRaw{toolPoseI}, std_aux, mean_aux] = readFeatures(dataDir,tool,0);
        featsValid = dataFeatRaw{toolPoseI}(1:size(dataAff{toolPoseI},1),:);
        dataFeatRawExtra{toolPoseI} = dataFeatRaw{toolPoseI}(size(dataAff{toolPoseI})+1:end,:);
        stdTP = [stdTP;std_aux];
        meanTP = [meanTP;mean_aux];
        
        if plotF ==1
            subplot(Npose,Ntools,mod((toolPoseI-1),Npose)*Ntools+ceil(toolPoseI/Npose));
            app= -5:1:5;
            plot(app,dataAff{toolPoseI});
            ylim([0, 0.35]);
            title( toolLabels{toolPoseI}(1,:), 'interpreter','none');
            xlabel('approach [cm]');
            ylabel('effect [m]');
        end
        
        dataAffsAll = [dataAffsAll; dataAff{toolPoseI}];
        dataFeatsAll = [dataFeatsAll; featsValid];
        toolPoseI = toolPoseI + 1;
    end      
end

[normData,muNorm, sigNorm] = normalizeData(dataFeatsAll);

% normalize all feature data to the same mu and sig
dataFeatsAll = normalizeData(dataFeatsAll,muNorm, sigNorm);
for i=1:length(dataFeatRaw)
    dataFeat{i} = normalizeData(dataFeatRaw{i},muNorm, sigNorm); 
    dataFeatExtra{i} = normalizeData(dataFeatRawExtra{i},muNorm, sigNorm); 
end


%meanNorm = mean(meanTP);
%stdNorm = mean(stdTP);

clear tool dd toolPoseI dirI y meanTP mean_aux stdTP std_aux plotF