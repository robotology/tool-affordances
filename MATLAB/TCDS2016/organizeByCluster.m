function [affInput, affOutput] = organizeByCluster(graspDataPose,actDataPose,effDataPose,toolNames,clusterIdx, canonF,singleModel, plotF)
numPoses = length(toolNames);

if ((nargin <5 )||isempty(clusterIdx))
    clusterIdx = ones(numPoses,1); % if no indices are given, consider all to belong to the same class (for testing)
end
if (nargin <6)
    canonF = 0;
end

if (nargin <7)
    plotF = 0;
end

numClusters = max(clusterIdx);
for m = unique(clusterIdx)'
    % Separe data by clusters kepping poses index
    posesClust = find(clusterIdx==m); 
    numPosesClust = length(posesClust); 
    modelDataGrasp= graspDataPose(:,:,clusterIdx==m);    
    modelDataAct= actDataPose(:,:,clusterIdx==m);
    modelDataEff= effDataPose(:,:,clusterIdx==m);   

    % concatenate data from all poses for each cluster
    dA = [];
    dG = [];
    dE = [];
    for t = 1: size(modelDataAct,3)
        dA = [dA;modelDataAct(:,:,t)];
        dG = [dG;modelDataGrasp(:,:,t)];
        dE = [dE;modelDataEff(:,:,t)];
    end
    
    if (singleModel==0)
        if (canonF ==0)     % for oriented features
            affInput{m} = dA(:,1);
        elseif (canonF==1)        
            affInput{m} = [dA(:,1) dG(:,2)];
        end
    elseif (singleModel ==1)
        if (canonF ==0)     % for oriented features
            affInput{m} = [dA(:,1) repmat(m,length(dA(:,1)),1)];
        elseif (canonF==1)        
            affInput{m} = [dA(:,1) dG(:,2) repmat(m,length(dA(:,1)),1)];            
        end
    end   
    affOutput{m} = dE(:,1);           % consider only displacement
    %affOutput{m} = [dE(:,1) dE(:,2)];  % Consider displacement and angle
    
    
    
    if any(plotF)     
        if (canonF == 1) 
%             affVec = [actDataPose(:,1,posesClust) graspDataPose(:,2,posesClust) effDataPose(:,1,posesClust)];
%             affMat = reshape(affVec,8,9,size(affVec,2),numPosesClust);
%             figure(plotF+1)
%             subplot(ceil(sqrt(numClusters)),ceil(sqrt(numClusters)),m);
%             for p = 1:numPosesClust
%                 plot3(squeeze(affMat(:,:,1,p)), squeeze(affMat(:,:,2,p)),affMat(:,:,3,p));hold on;
%             end
%             plot3(mean(affMat(:,:,1,:),4), mean(affMat(:,:,2,:),4),mean(affMat(:,:,3,:),4),'linewidth', 2);hold off;           
%             
            figure(plotF+1);
            posGrasp = [-90 0 90];
            posDisp = [-2 0 2];
            spI = 1;
            for g = posGrasp
                subplot(numClusters,length(posGrasp),(m-1)*length(posGrasp)+spI);
                affVecGr = [];
                for d = posDisp
                    gId = intersect(find(graspDataPose(:,2,1) == g), find(graspDataPose(:,3,1) == d));        
                    affVec = [actDataPose(gId,1,posesClust) effDataPose(gId,1,posesClust)];        
                    affVecGr = cat(3,affVecGr,affVec);
                end
                plot(squeeze(affVecGr(:,1,:)),squeeze(affVecGr(:,2,:)));hold on;
                plot(mean(affVecGr(:,1,:),3),mean(affVecGr(:,2,:),3),'linewidth', 2);hold off;
                axis([-10 360 0 0.25])
                xlabel(['Action - Pull Angle- Grasp ', num2str(g)], 'FontSize',18);
                ylabel('Effect (cm)', 'FontSize', 18); 
                spI = spI + 1;
            end
        else
            figure(plotF);
            affVec = [actDataPose(:,1,posesClust) effDataPose(:,1,posesClust) effDataPose(:,2,posesClust)];
            
            %plot displacement
            subplot(ceil(sqrt(numClusters)),ceil(sqrt(numClusters)),m);
            plot(squeeze(affVec(:,1,:)),squeeze(affVec(:,2,:)));hold on;
            plot(mean(affVec(:,1,:),3),mean(affVec(:,2,:),3),'linewidth', 2); hold off;
            axis([-10 360 0 0.25])
            xlabel('Action - Pull Angle', 'FontSize',18);
            ylabel('Effect (cm)', 'FontSize',18); 
            
            %plot angle           
            figure(plotF+1);
            subplot(ceil(sqrt(numClusters)),ceil(sqrt(numClusters)),m);
            plot(squeeze(affVec(:,1,:)),squeeze(affVec(:,3,:)));hold on;
            plot(mean(affVec(:,1,:),3),mean(affVec(:,3,:),3),'linewidth', 2); hold off;
            axis([-10 360 -180 180])
            xlabel('Action - Pull Angle', 'FontSize',18);
            ylabel('Effect (cm)', 'FontSize',18); 
                     
            figure(plotF+2)
            subplot(ceil(sqrt(numClusters)),ceil(sqrt(numClusters)),m);
            polar(squeeze(affVec(:,3,:)),squeeze(affVec(:,2,:)));            
            % affVecOr = [effDataPose(:,2,posesClust) effDataPose(:,1,posesClust)];
            % polar(squeeze(affVecOr(:,1,:)),squeeze(affVecOr(:,2,:)));
        end
    end
end

if numClusters == 1
    affInput = affInput{1};
    affOutput = affOutput{1};
end