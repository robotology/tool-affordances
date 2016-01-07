dataDir= '..\affData\dataSIM';
%dataDir= '..\affData\dataRobot';

dd=dir(dataDir);

plotF = 1;
toolPoseI =1;
dataAffsAll = [];
dataFeatsAll = [];
        
Ntools = 7;
Npose= 3;
if plotF ==1
    figure;
end;

for dirI =1:length(dd)
    if (length(dd(dirI).name)>3)
        tool = dd(dirI).name;
        [dataAff{toolPoseI}, toolLabels{toolPoseI}] = readActEff(dataDir,tool);        
        dataFeat{toolPoseI} = read3Dfeatures(dataDir,tool);
        featsValid = dataFeat{toolPoseI}(1:size(dataAff{toolPoseI},1),:);
        dataFeatExtra{toolPoseI} = dataFeat{toolPoseI}(size(dataAff{toolPoseI})+1:end,:);
        
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

clear tool dd toolPoseI dirI y  plotF