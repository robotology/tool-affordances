function [model, bestParam] = trainSVMaff(trainData, trainLabel, kernel, plotF)
if (nargin < 3 )|| isempty(kernel)
    kernel = 0;
end

if (nargin < 4 )|| isempty(plotF)
    plotF = 0;
end
NTrain = length(trainLabel);

%% 
% #######################
% Automatic Cross Validation 
% Parameter selection using n-fold cross validation
% #######################
stepSize = 20;
bestLog2c = 1;
bestLog2g = -1;
epsilon = 0.005;
bestcv = 0;
Ncv = 3; % Ncv-fold cross validation cross validation
deltacv = 10^6;

searchCnt = 1;
while abs(deltacv) > epsilon
    bestcv_prev = bestcv;
    prevStepSize = stepSize;
    stepSize = prevStepSize/2;
    log2c_list(searchCnt,:) = bestLog2c-prevStepSize:stepSize/2:bestLog2c+prevStepSize;
    log2g_list(searchCnt,:) = bestLog2g-prevStepSize:stepSize/2:bestLog2g+prevStepSize;
    
    numLog2c(searchCnt) = length(log2c_list(searchCnt,:));
    numLog2g(searchCnt) = length(log2g_list(searchCnt,:));
    
    
    cvMatrix{searchCnt} = zeros(numLog2c(searchCnt),numLog2g(searchCnt));    

    for i = 1:numLog2c(searchCnt)
        log2c = log2c_list(searchCnt,i);
        for j = 1:numLog2g(searchCnt)
            log2g = log2g_list(searchCnt,j);
            if kernel == 1          % With polynomial kernel   (gamma*u'*v + coef0)^degree
                cmd = ['-q -c ', num2str(2^log2c), ' -g ', num2str(2^log2g),' -t 1'];
            elseif kernel == 2      % With rbf kernel exp(-gamma*|u-v|^2)
                cmd = ['-q -c ', num2str(2^log2c), ' -g ', num2str(2^log2g),' -t 2'];
            elseif kernel == 3      % With sigmoid kernel tanh(gamma*u'*v + coef0)
                cmd = ['-q -c ', num2str(2^log2c), ' -g ', num2str(2^log2g),' -t 3'];
            else                    % linear kernel  u'*v
                cmd = ['-q -c ', num2str(2^log2c), ' -g ', num2str(2^log2g)];
            end      
            %cv = get_cv_ac(trainLabel, [(1:NTrain)' trainData*trainData'], cmd, Ncv);
            cv = get_cv_ac(trainLabel, trainData, cmd, Ncv);
            
            cvMatrix{searchCnt}(i,j) = cv;
            if (cv >= bestcv),
                bestcv = cv; bestc = 2^log2c; bestg = 2^log2g;
                bestLog2c = log2c; bestLog2g = log2g; 
            end
        end
    end
    deltacv = bestcv - bestcv_prev;  
    fprintf('Best Cross-validation Accuracy on grid search = %g%%\n', bestcv * 100);
    searchCnt = searchCnt + 1;
end
searchDepth = searchCnt - 1;

% Plot the results
if (plotF)
    for i = 1:searchDepth
        figure(51)
        subplot(ceil(sqrt(searchDepth)),ceil(sqrt(searchDepth)),i);
        imagesc(cvMatrix{i}); colormap('jet'); colorbar;
        set(gca,'XTick',1:numLog2g(i))
        set(gca,'XTickLabel',sprintf('%3.1f|',log2g_list(i,:)))
        xlabel('Log_2\gamma');
        set(gca,'YTick',1:numLog2c(i))
        set(gca,'YTickLabel',sprintf('%3.1f|',log2c_list(i,:)))
        ylabel('Log_2c');  
    end
end

fprintf (['Performed ', num2str(searchCnt) ,' gridsearches \n']);
disp(['The best parameters, yielding Accuracy=',num2str(bestcv*100),'%, are: C=',num2str(bestc),', gamma=',num2str(bestg)]);



%%
% #######################
% Train the SVM in one-vs-rest (OVR) mode
% #######################
if kernel == 1  % With polynomial kernel   (gamma*u'*v + coef0)^degree
    bestParam = ['-q -c ', num2str(bestc), ', -g ', num2str(bestg),' -t 1'];
    model = ovrtrainBot(trainLabel, [(1:NTrain)' trainData*trainData'], bestParam);
elseif kernel == 2  % With rbf kernel exp(-gamma*|u-v|^2)
    bestParam = ['-q -c ', num2str(bestc), ', -g ', num2str(bestg),' -t 2'];
    model = ovrtrainBot(trainLabel, [(1:NTrain)' trainData*trainData'], bestParam);
elseif kernel == 3  % With sigmoid kernel tanh(gamma*u'*v + coef0)
    bestParam = ['-q -c ', num2str(bestc), ', -g ', num2str(bestg),' -t 3'];
    model = ovrtrainBot(trainLabel, [(1:NTrain)' trainData*trainData'], bestParam);
else      
    bestParam = ['-q -c ', num2str(bestc), ', -g ', num2str(bestg)];
    % bestParam = ['-q -c 8 -g 0.0625'];
    model = ovrtrainBot(trainLabel, trainData, bestParam);
end

