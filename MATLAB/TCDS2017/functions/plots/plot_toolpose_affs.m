function plot_toolpose_affs(tpMat, labsMat, fig_i, simF)
if (nargin <3) || isempty(fig_i)
    fig_i = figure;
else
    figure(fig_i);
end
if (nargin <4) || isempty(simF)
    simF = 0;
end
if simF == 0
    x_ticks = 0:90:315;
else
    x_ticks = 0:45:315;
end


%tp = char(labsMat(1));          %read the first tp name        
%fig_i.Name = tp(1:3);           % keep only the tool type name

numRep = size(tpMat,1);
numAct = size(tpMat,2);
numTP  = size(tpMat,3);
mean_mat = zeros(numTP, numAct);
std_mat  = zeros(numTP, numAct);
ori_mat  = zeros(numRep * numTP/3, numAct, 3);
for i =1:numTP+3
    subplot((numTP+3)/3,3,i); hold on;
    if (i == numTP+1 )                % top graphs
        %title('right');
        %ori_mat = mean_mat(1:3:end,:); 
        plot(x_ticks,ori_mat(:,:,1))
        ylabel('All');
    elseif (i == numTP+2 )
        %title('front');
        %ori_mat = mean_mat(2:3:end,:);   
        plot(x_ticks,ori_mat(:,:,2))
    elseif (i == numTP+3 )
        %title('left');               
        %ori_mat = mean_mat(3:3:end,:);   
        plot(x_ticks,ori_mat(:,:,3))
    else
        mean_mat(i,:) = mean(tpMat(:,:,i));
        std_mat(i,:) = std(tpMat(:,:,i));
        
        ori = mod(i,3);
        if ori == 0
            ori = 3;
        end
        ori_mat((ceil(i/3)-1)*numRep+1:ceil(i/3)*numRep,:,ori) = tpMat(:,:,i);
        

        plot(x_ticks,tpMat(:,:,i)'); % dunno why, but although in both cases actions are columns and rows repetitions, plot() plots it wrong otherwise)
        
        
        ax = gca;
        set(ax,'XTick',0:90:360)
        set(ax,'XTickLabel',0:90:360)
        %ax.XTickLabelRotation=60;
        ylim([-0.03 0.3])
        xlim([-10, 325])
        
        
        %errorbar(0:45:315,mean_mat(i,:) ,std_mat(i,:), std_mat(i,:))
        if (mod(i,3) == 1 )         % first graph of each row
            tp = char(labsMat(i));   %read the actual tp name        
            ylabel(tp(1:4));        % remove the orientation to keep the tool name
        end
    end

    %shadedErrorBar(0:45:315,meanMat(i,:) ,stdMat(i,:),color)
    ylim([-0.03 0.3])
    xlim([-10, 325])


    if (i == 1 )                % top graphs
        title('right');
    elseif (i == 2 )
        title('front');
    elseif (i == 3 )
        title('left');               
    end    
end


