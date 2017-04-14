function [tool_labels,deg, eff] = readActSelectAffData(simF)

if simF == 1
    dirname = './data/selAct_data' 
else
    dirname = './data/predAct_Data/real' 
end


folders = dir(dirname);
tool_labels = [];
deg = [];
eff = [];

delimiter = {' ','(',')'};
formatSpec = '%*s%*s%s%f%f%*s%*s%f%*s%*s%[^\n\r]';
for foldI =1:length(folders)
    folder_name = folders(foldI).name;
    %if (length(folder_name>3)) && (strcmp(folder_name(1:7),'affData'))
    if (length(folders(foldI).name)>3) && (strcmp(folders(foldI).name(1:7),'affData'))
        
        fprintf('reading %s features data \n',folder_name);
        filename = [dirname,'/',folder_name, '/data.log'];
        %% Open the text file.
        fileID = fopen(filename,'r');

        dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'MultipleDelimsAsOne', true,  'ReturnOnError', false);
        
        tool_labels_aux = dataArray{:, 1};
        tool_id_aux = dataArray{:, 2};
        deg_aux = dataArray{:, 3};
        eff_aux = dataArray{:, 4};
        
        
        tool_labels = [tool_labels; tool_labels_aux];
        deg = [deg; deg_aux];
        eff = [eff; eff_aux];
        
        
        %% Close the text file.
        fclose(fileID);
        
    end
end

