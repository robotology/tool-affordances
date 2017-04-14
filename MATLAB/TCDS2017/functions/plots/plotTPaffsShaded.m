function plotTPaffsShaded(affVecMat, labsMat, fig_i)
if (nargin <4) || isempty(fig_i)
    fig_i = figure;
else
    figure(fig_i);
end

%tp = char(labsMat(1));          %read the first tp name        
%fig_i.Name = tp(1:3);           % keep only the tool type name

numTP = size(meanMat,1);
for i =1:numTP
    subplot(numTP/3,3,i); hold on;
    shaded(1:8,affVecMat(i,:))
    ylim([-0.03 0.3])
    
    
    if (mod(i,3) == 1 )         % first graph of each row
        tp = char(labsMat(i));   %read the actual tp name        
        ylabel(tp(1:4));        % remove the orientation to keep the tool name
    end
    if (i == 1 )                % top graphs
        title('right');
    elseif (i == 2 )
        title('front');
    elseif (i == 3 )
        title('left');               
    end
end
