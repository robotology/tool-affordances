function [confMat, totalmse] = svmRegAffTest(testData, testOutput, model, plotF)

if (nargin < 4 )|| isempty(plotF)
    plotF = 0;
end

%%
% #######################
% Classify samples using OVR model
% #######################
NTest = length(testOutput);

% With specific kernel
[predict_label, accuracy, decis_values] = svmpredict(testOutput, testData, model);
[decis_value_winner, label_out] = max(decis_values,[],2);

%%
% #######################
% Make confusion matrix
% #######################
[confusionMatrix,order] = confusionmat(testLabels,label_out);
confMat= confusionMatrix'; % Taspose to generate the conventional confMat: column=actual, row = predicted.
totalmse = trace(confusionMatrix)/NTest;
disp(['Total accuracy from the SVM: ',num2str(totalmse*100),'%']);
    
%% Plot the results
if (plotF == 1)
    
    figure; 
    % plot predictions  
    subplot(1,6,1); imagesc(label_out); title('Output labels'); ylabel('observations'); 
    subplot(1,6,2); imagesc(testLabels); title('Original labels');ylabel('observations');
    
    % plot confusion matrix and class relevance
    subplot(1,3,2);
    imagesc(confusionMatrix');
    xlabel('actual class label');
    ylabel('predicted class label');
    colorbar;
    
    subplot(1,3,3);
    % plot sample labeling in 2D space projection 
    colorList = hsv(length(unique(testLabels)));
    distanceMatrix = pdist(testData,'euclidean'); % compute distance betweeen all points
    %newCoor = mdscale(distanceMatrix,3); % take longer time, but more beautiful
    newCoor = cmdscale(distanceMatrix); % 

    % Set sample colors depending on their label
    colorTrueClass = colorList(testLabels,:);% true (ground truth) class
    colorResultClass = colorList(label_out,:);% result Class

    % Plot the test data
    x = newCoor(:,1);
    y = newCoor(:,2);
    patchSize = 20; %80*max(decis_values,[],2);
    hold on;
    scatter(x,y,2*patchSize,colorTrueClass,'o','filled');
    scatter(x,y,patchSize,colorResultClass,'o','filled');
    legend;
    hold off;
    
end
if (plotF == 2)        
    figure; 
    % plot confusion matrix
    subplot(2,2,1);
    imagesc(confusionMatrix');
    xlabel('actual class label');
    ylabel('predicted class label');
    colorbar;
    % plot class relevance
    subplot(2,4,3);    
    pie(histc(testLabels, unique(testLabels)));
    ylabel('Class distribution - original');
    subplot(2,4,4);    
    pie(histc(label_out, unique(label_out)));  
    ylabel('Class distribution - predicted');
    
    % plot predictions    
    subplot(2,8,9); imagesc(predict_label); title('predicted labels'); xlabel('class k vs rest'); ylabel('observations'); 
    subplot(2,8,10); imagesc(decis_values); title('decision values'); xlabel('class k vs rest'); ylabel('observations'); 
    subplot(2,8,11); imagesc(label_out); title('Output labels'); ylabel('observations'); 
    subplot(2,8,12); imagesc(testLabels); title('Original labels');ylabel('observations');

    %plot sample labeling in 2D space projection 
    subplot(2,2,4);
    colorList = hsv(length(unique(testLabels)));
    distanceMatrix = pdist(testData,'euclidean'); % compute distance betweeen all points
    %newCoor = mdscale(distanceMatrix,3); % take longer time, but more beautiful
    newCoor = cmdscale(distanceMatrix); % 

    % Set sample colors depending on their label
    colorTrueClass = colorList(testLabels,:);% true (ground truth) class
    colorResultClass = colorList(label_out,:);% result Class

    % Plot the test data
    x = newCoor(:,1);
    y = newCoor(:,2);
    patchSize = 20; %80*max(decis_values,[],2);
    hold on;
    scatter(x,y,2*patchSize,colorTrueClass,'o','filled');
    scatter(x,y,patchSize,colorResultClass,'o','filled');
    legend;
    hold off;
end
    
end