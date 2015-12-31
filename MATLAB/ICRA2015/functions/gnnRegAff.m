function [net, bestSpread] = gnnRegAff(trainData, trainOutput, plotVar)
if (nargin < 3 )|| isempty(plotVar)
    plotVar = [0 0]; % Plot var contains the number of plots, and the index    
end

%%
% #######################
% Automatic Cross Validation 
% Parameter selection using n-fold cross validation
% #######################
stepSize = 1;
bestSpread = 1.0001;
epsilon = 0.000001;
bestmse = 10^10;
deltamse = 10^6;
Ncv = 5; % Ncv-fold cross validation cross validation


searchCnt = 1;
while abs(deltamse) > epsilon
    bestmse_prev = bestmse;
    prevStepSize = stepSize;
    stepSize = prevStepSize/2;
    spread_list(searchCnt,:) = bestSpread-prevStepSize:stepSize/2:bestSpread+prevStepSize;
    if any(spread_list(searchCnt,:)<= 0)
        spread_list(searchCnt,:) = spread_list(searchCnt,:) - min(spread_list(searchCnt,:))+ 0.00001;
    end
    numSpread = length(spread_list(searchCnt,:));
    
    mseVec{searchCnt} = zeros(numSpread,1);    

    for j = 1:numSpread
        spread = spread_list(searchCnt,j);
        mse = cv_newgrnn(trainData',trainOutput',spread, Ncv);
        mseVec{searchCnt}(j) = mse;
        if (mse <= bestmse),
            bestmse = mse; 
            bestSpread = spread; 
        end
    end
    deltamse = bestmse - bestmse_prev;  
    fprintf('Best Cross-validation MSE = %g, with spread= %g\n',bestmse, bestSpread);
    searchCnt = searchCnt + 1;
end
searchDepth = searchCnt - 1;

% Plot the results
if any(plotVar)
    for i = 1:searchDepth
        figure(51)
        subplot(ceil(sqrt(plotVar(1))),ceil(sqrt(plotVar(1))),plotVar(2));
        %subplot(ceil(sqrt(searchDepth)),ceil(sqrt(searchDepth)),i);
        plot(spread_list(i,:), mseVec{i} ); hold on;
        %set(gca,'XTick',1:numSpread)
        %set(gca,'XTickLabel',spread_list(i,:))
        xlabel('RBF spread');
        ylabel('MSE');  
    end
    hold off;
end

fprintf (['Performed ', num2str(searchDepth) ,' line searches \n']);
disp(['The best parameter, yielding MSE of =',num2str(bestmse),', is: Spread = ',num2str(bestSpread)]);



%%
% #######################
% Train the GRNN using the best spread
% #######################
net = newgrnn(trainData',trainOutput',bestSpread);

