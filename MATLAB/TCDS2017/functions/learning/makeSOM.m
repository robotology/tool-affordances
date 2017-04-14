function [sMap,sData] = makeSOM(data,labels, name, plotF)

if (nargin < 4) || isempty(plotF)
    plotF = 0;
end

%% make SOM
sData = som_data_struct(data,'name',name, 'labels', labels);
if strcmp(name, 'tool')
    sMap = som_make(sData, 'msize', [15 25], 'shape', 'sheet', 'name',name, 'tracking', 0);
else
    sMap = som_make(sData, 'msize', [15 25], 'shape', 'sheet', 'name',name, 'tracking', 0);
end

%% plot U-Matrix
if any(plotF)
    figure(plotF);
    sMapClass = som_autolabel(sMap,sData,'add',1);
    som_show(sMapClass,'umat','all','bar','none', 'colormap', 1-gray); % visualize
end