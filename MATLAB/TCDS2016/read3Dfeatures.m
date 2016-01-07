function [tool3DFeatures,toolLabels] = read3Dfeatures(dataDir, tool)

%% Import data from text file.
% Script for importing data from the following text file:

%% Initialize variables.
tool3DFeatures = [];
%fprintf('reading %s features',tool);
dirname = [dataDir,'\',tool];
%dirname = dataDir;
folders = dir(dirname);

fprintf('reading %s features data \n',tool);

%% Format string for each line of text:
% For more information, see the TEXTSCAN documentation.
formatSpec = '%*s%*s%s%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%[^\n\r]';
delimiter = {' ','(',')'};

%% Loop throught the folders and files
for foldI =1:length(folders)
    if (length(folders(foldI).name)>3) && (strcmp(folders(foldI).name(1:7),'feats3D'))
        filename = [dirname,'\',folders(foldI).name, '\data.log'];

        % Open the text file.
        fileID = fopen(filename,'r');

        % Read columns of data according to format string.
        dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'MultipleDelimsAsOne', true, 'EmptyValue' ,NaN, 'ReturnOnError', false);

        % Close the text file.
        fclose(fileID);
        
        % Create output variable
        labelsRaw = dataArray{1}; 
        toolLabels = labelsRaw;
        data = [dataArray{2:end-1}];
        %data = rand(50,200);       
        
        orientData = data(:,end-17:end);
        graspOr = rad2deg(asin(orientData(:,5)));
        graspDisp = 100*orientData(:,10);
        
        for l = 1:length(labelsRaw)
            toolLabels{l}= [labelsRaw{l}(6:end-1) ':' num2str(graspOr(l)) ',' num2str(graspDisp(l))];
        end
        feats = data(:,1:end-18);      
        
        tool3DFeatures = [tool3DFeatures; feats];
        %tool3DFeatures = [tool3DFeatures; orient];        
        %tool3DFeatures = [tool3DFeatures; data];
        
        % Clear temporary variables
        clearvars filename delimiter formatSpec fileID dataArray ans;
    end
end
