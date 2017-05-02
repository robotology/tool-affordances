function plotTTPaffs(tpMat, label, fig_i, color, predF)
% if (nargin <3) || isempty(fig_i)
%     fig_i = figure;
% else
%     figure(fig_i);
% end

if (nargin <5) || isempty(predF)
    predF = -1;
end

%tp = char(labsMat(1));          %read the first tp name        
%fig_i.Name = tp(1:3);           % keep only the tool type name
A = size(tpMat,2);

for i =1:3  
    subplot(5,3,(fig_i)*3+i); hold on;
    if (mod(i,3) == 1 )         % first graph of each row
        yhandle = ylabel(label);        
        set(yhandle,'Fontname','Calibri', 'Fontsize',20);
    end
    
    if (fig_i == 0)
        if (i == 1 )                % top graphs            
            titlehandle = title('right');        
        elseif (i == 2 )
            titlehandle = title('front');        
        elseif (i == 3 )
            titlehandle = title('left');        
        end
        set(titlehandle,'Fontname','Calibri', 'Fontsize',22);
    end
    
    
    ttp_aux = tpMat(:,:,i:3:end);        
    ttp = reshape(permute(ttp_aux,[2,1,3]), size(ttp_aux,2),size(ttp_aux,1)*size(ttp_aux,3) )';
    
    meanMat = mean(ttp);
    stdMat = std(ttp);
    
    if predF == -1
        shadedErrorBar(0:360/A:315,meanMat ,stdMat,{'color' , color(i,:),'LineWidth', 2},1);
    elseif predF == 0 
        color = 'b';
        errorbar(0:360/A:315,meanMat ,stdMat,stdMat, color, 'LineWidth', 2)
    elseif predF == 1
        color = 'r';
        errorbar(0:360/A:315,meanMat ,stdMat,stdMat, color, 'LineWidth', 2)
    end
    
    ax = gca;
    set(ax,'XTick',0:90:360)
    set(ax,'XTickLabel',0:90:360)
    %ax.XTickLabelRotation=60;
    ylim([-0.03 0.3])
    xlim([-10, 325])    
end