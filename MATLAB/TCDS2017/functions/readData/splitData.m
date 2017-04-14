function [train_data, test_data, train_labels, test_labels] = splitData(data, labels, test_ids, train_ids )

if (nargin < 4) || (isempty(train_ids))
    all_ids = 1:length(labels);
    train_ids = all_ids;
    train_ids(test_ids) = [];
end


% Split  data
train_data = data(train_ids,:);
test_data = data(test_ids,:);
train_labels = labels(train_ids);
test_labels = labels(test_ids);
