function clust_ids = clustByLabel(labels, name_ids)

if nargin<2 || isempty(name_ids) % name ids represent the first and last idx of the name
    name_ids = [1 lenght(labels{1})];
end

name_labs  = cell(1,length(labels));

for i = 1 :length(labels)
    %name_labs{i} = labels{i}(name_ids(1):name_ids(2)); % Generic cluster
    %name_labs{i} = [labels{i}(1:3) labels{i}(5:9)];    % cluster by tool-pose
    %name_labs{i} = [labels{i}(5:9)];                   % cluster only by orientation
    name_labs{i} = [labels{i}(1:3)];                    % cluster only by tool
end

clust_ids  = zeros(1,length(labels));
name_types = unique(name_labs);
for t =1: length(name_types)
    clust_ids(~cellfun('isempty', strfind(name_labs, name_types{t}))) =t;
end
