function plotTPaffs(meanMat, stdMat, labsMat, fig_i, color)
if (nargin <4) || isempty(fig_i)
    fig_i = figure;
else
    figure(fig_i);
end

%tp = char(labsMat(1));          %read the first tp name        
%fig_i.Name = tp(1:3);           % keep only the tool type name

numTP = size(meanMat,1);
for i =1:numTP+3
    subplot((numTP+3)/3,3,i); hold on;
    if (i == numTP+1 )                % top graphs
        %title('right');
        ori_mat = meanMat(1:3:end,:); 
        plot(0:45:315,ori_mat)
    elseif (i == numTP+2 )
        %title('front');
        ori_mat = meanMat(2:3:end,:);   
        plot(0:45:315,ori_mat)
    elseif (i == numTP+3 )
        %title('left');               
        ori_mat = meanMat(3:3:end,:);   
        plot(0:45:315,ori_mat)
    else
        errorbar(0:45:315,meanMat(i,:) ,stdMat(i,:), stdMat(i,:))
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


