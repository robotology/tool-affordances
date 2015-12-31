function [model, bestParam] = svmRegAff(trainData, trainOutput, kernel, plotF)
if (nargin < 3 )|| isempty(kernel)
    kernel = 2;
end

if (nargin < 4 )|| isempty(plotF)
    plotF = 0;
end
NTrain = size(trainOutput,1);

%% 
% #######################
% Automatic Cross Validation 
% Parameter selection using n-fold cross validation
% #######################
stepSize = 10;
bestLog2c = 1;
bestLog2g = -1;
epsilon = 0.00001;
bestmse = 10^10;
%Ncv = 3; % Ncv-fold cross validation cross validation
deltamse = 10^6;

searchCnt = 1;
while ((abs(deltamse) > epsilon) && (searchCnt < 10))
    bestmse_prev = bestmse;
    prevStepSize = stepSize;
    stepSize = prevStepSize/2;
    log2c_list(searchCnt,:) = bestLog2c-prevStepSize:stepSize/2:bestLog2c+prevStepSize;
    log2g_list(searchCnt,:) = bestLog2g-prevStepSize:stepSize/2:bestLog2g+prevStepSize;
    
    numLog2c(searchCnt) = length(log2c_list(searchCnt,:));
    numLog2g(searchCnt) = length(log2g_list(searchCnt,:));
    
    
    mseMatrix{searchCnt} = zeros(numLog2c(searchCnt),numLog2g(searchCnt));    

    for i = 1:numLog2c(searchCnt)
        log2c = log2c_list(searchCnt,i);
        for j = 1:numLog2g(searchCnt)
            log2g = log2g_list(searchCnt,j);
            cmd = ['-q -s 4 -c ', num2str(2^log2c), ' -g ', num2str(2^log2g),' -t ', num2str(kernel), ' -v 5'];
            mse = svmtrain(trainOutput, trainData, cmd);
            
            mseMatrix{searchCnt}(i,j) = mse;
            if (mse <= bestmse),
                bestmse = mse; bestc = 2^log2c; bestg = 2^log2g;
                bestLog2c = log2c; bestLog2g = log2g; 
            end
        end
    end
    deltamse = bestmse - bestmse_prev;  
    fprintf('Best Cross-validation MSE on grid search = %g\n',mse);
    searchCnt = searchCnt + 1;
end
searchDepth = searchCnt - 1;

% Plot the results
if (plotF)
    for i = 1:searchDepth
        figure(51)
        subplot(ceil(sqrt(searchDepth)),ceil(sqrt(searchDepth)),i);
        imagesc(mseMatrix{i}); colormap('jet'); colorbar;
        set(gca,'XTick',1:numLog2g(i))
        set(gca,'XTickLabel',log2g_list(i,:))
        xlabel('Log_2\gamma');
        set(gca,'YTick',1:numLog2c(i))
        set(gca,'YTickLabel',log2c_list(i,:))
        ylabel('Log_2c');  
    end
end

fprintf (['Performed ', num2str(searchCnt) ,' gridsearches \n']);
disp(['The best parameters, yielding MSE of =',num2str(bestmse),', are: C=',num2str(bestc),', gamma=',num2str(bestg)]);



%%
% #######################
% Train the SVM in one-vs-rest (OVR) mode
% #######################
bestParam = ['-q -c ', num2str(bestc), ' -t ', num2str(kernel), ' -g ', num2str(bestg)];
model = svmtrain(trainOutput, trainData, bestParam);

