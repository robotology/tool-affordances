function [data, tool_labels, tp_labels] = readDeepFeats()

% Initialize variables.
dirname = './data/deep_features/pose_database';
folders = dir(dirname);
delimiter = ' ';

data = [];
tp_labels = {};
tool_labels = {};

formatSpec = repmat('%f',1,4096);
for fold_i =1:length(folders)
    fold_name = folders(fold_i).name;
    if (length(fold_name)>=4)        
        fprintf('Reading deep features from tool %s \n', fold_name);
        folder = [dirname ,['/'] , fold_name];
        files =  dir(folder);
        for file_i =1:length(files)
            file_name = files(file_i).name;
            if ((strcmp(file_name,'.')==0 )  && (strcmp(file_name,'..')==0))
                filepath = [folder, '/', file_name ];
                fileID = fopen(filepath,'r');
                fprintf('Reading file %s \n', file_name);        
                
                %Get data
                dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'MultipleDelimsAsOne', true, 'EmptyValue' ,NaN, 'ReturnOnError', false);
                data_file = [dataArray{1:end}];         % convert from array to vector
                data =  [data; data_file];              % Accumulate
                
                % Get labels
                tool_name = fold_name(1:4);
                tp_name = fold_name;
                tool_file_names = repmat(tool_name, size(data_file,1), 1);
                tool_file_labels = mat2cell(tool_file_names, ones(size(tool_file_names,1),1));                
                tool_labels = [tool_labels; tool_file_labels];
                
                tp_file_names = repmat(tp_name, size(data_file,1), 1);
                tp_file_labels = mat2cell(tp_file_names, ones(size(tp_file_names,1),1));                
                tp_labels = [tp_labels; tp_file_labels];                
                fclose(fileID);
            end
        end  
    end
end
