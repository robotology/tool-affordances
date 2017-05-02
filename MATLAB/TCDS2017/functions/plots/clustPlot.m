
function [clustIds, colorIds] = clustPlot(mappedX, labels, dim, nC, textF,figId)
    if (nargin<=5) || isempty(figId)
        figId = 125;
    end
    if (nargin<=4) || isempty(textF)
        textF = 0;
    end
    if textF == 2
        for i = 1:length(labels)
            labs(i,:) = labels{i};
        end
    end

    figure(figId)

    distX = pdist(mappedX);
    tree = linkage(distX,'ward');
    %figure; dendrogram(tree);
    ff = figId;
    for c= nC
        
        clustIds = cluster(tree,'maxclust', c);
        figure(ff);
        colorIds = clusterColor(clustIds);
        if dim ==3
            scatter3(mappedX(:,1),mappedX(:,2), mappedX(:,3),[],colorIds );
        else 
            scatter(mappedX(:,1),mappedX(:,2),[],colorIds );
        end

        if textF == 1
            fprintf('Plotting labels\n');
            if dim ==3
                text(mappedX(:,1), mappedX(:,2), mappedX(:,3), labs(:,1:end-3))
            else
                text(mappedX(:,1), mappedX(:,2), labs(:,1:end-3))
            end
        end
        ff = ff+1;
    end
end
    
