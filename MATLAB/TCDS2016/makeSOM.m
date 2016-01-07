function [sMap,sData] = makeSOM(data, plotF)
% Get data
D = data;

sData = som_data_struct(D,'name','tools');
sMap = som_make(sData, 'msize', [20 15], 'shape', 'sheet');

if any(plotF)
    figure(plotF);
    sMapClass = som_autolabel(sMap,sData,'add',1);
    som_show(sMapClass,'umat','all','bar','none', 'colormap', 1-gray); % visualize
end