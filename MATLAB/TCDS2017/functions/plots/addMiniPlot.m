
function addMiniPlot(X, mappedX,labels, typeF, plotVar, figId,freq)

    if (typeF == 2) 
        images = plotVar;
    elseif (typeF == 1)
        colorInd = plotVar;
    end
    
    h = figure(figId);
    ax = h.Children(end);
    
    
    
    % Plot small axis instead of text in 1 out of each 10 points
    [Xf, Yf] = ds2nfu(ax,mappedX(:,1), mappedX(:,2));
    Xf = Xf - 0.025;
    Yf = Yf - 0.015;
    for i =1:size(X,1)
        % plot the X individual graphs on top of the plot (for affVectors)
        if (typeF == 1)
            if (mod(i,freq)==0)                
                axes('Position',[Xf(i), Yf(i) .05 .03]);
                plot(X(i,:), 'LineWidth',2, 'Color', colorInd(i,:));
                axis off;
            end   

        % plot the tool images on top of their corresponding point in the cluster plot
        elseif (typeF ==2)
            if (mod(i,freq)==1) 
                % Find indices of the tools being considered
                foundCells = strfind(images(3,:), labels{i});
                im_ids= find(~cellfun('isempty',foundCells));
                
                % axes(ff);
                axes('Position',[Xf(i), Yf(i) .05 .09]);
                axis off;
                f = imshow(images{1,im_ids(1)});
                set(f, 'AlphaData', images{2,im_ids(1)});
            end                
        end
    end    
end