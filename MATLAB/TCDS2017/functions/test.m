function clust_ids = clustByLabel(labels)

tool_labs  = cell(1,length(labels));

for i = 1 :length(labels)
    tool_labs{i} = labels{i}(1:3);
end

tool_ids  = zeros(1,length(labels));
tool_types = unique(tool_labs);
for t =1: length(tool_types)
    tool_ids(~cellfun('isempty', strfind(tool_labs, tool_types(t)))) =t;
end
