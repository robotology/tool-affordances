function cleanLabels = clearSOMlabels(labels)
nL = size(labels,1);
cleanLabels = cell(nL,5);
for l = 1:nL
    unique_labs =  unique(labels(l,:));
    for uL = 1:length(unique_labs)
        if uL<5% limit in 5 to not clutter
            cleanLabels{l,uL} = unique_labs{uL};
        end
    end
end
