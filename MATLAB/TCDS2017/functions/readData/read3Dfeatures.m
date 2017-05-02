function [tool3DFeatures, toolLabels, tpLabels, toolIms] = read3Dfeatures(folder, simF)

if ((nargin<2) || (isempty(simF)))
    simF =0 ;       % By default simulation
end

% Set paths
if (simF==0)
    dataDir = './data/featData/real';
    imsDir = './figs/toolIms_trans/real';
    
elseif (simF ==1)
    dataDir= './data/featData/sim' ;
    imsDir = './figs/toolIms_trans/sim';
end


%% Initialize variables.
tool3DFeatures = [];
toolLabels = [];
tpLabels = [];

maxFeats = 1000;
dirname = [dataDir,'/',folder];

folders = dir(dirname);

fprintf('reading %s features data \n',folder);

%% Format string for each line of text:
% For more information, see the TEXTSCAN documentation.
formatSpec = ['%*s%*s%s',repmat('%f',1,maxFeats), '%[^\n\r]'];
%formatSpec = '%*s%*s%s%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%[^\n\r]';

delimiter = {' ','(',')'};

%% Loop throught the folders and files
for foldI =1:length(folders)
    if (length(folders(foldI).name)>3) && (strcmp(folders(foldI).name(1:7),'feats3D'))
        filename = [dirname,'/',folders(foldI).name, '/data.log'];

        % Open the text file.
        fileID = fopen(filename,'r');

        % Read columns of data according to format string.
        dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'MultipleDelimsAsOne', true, 'EmptyValue' ,NaN, 'ReturnOnError', false);
        % Create output variable
        
        % Get data
        dataRaw = [dataArray{2:end-1}];    % remove bottle parenthesis
        nanInds = find(isnan(dataRaw(1,:)));
        numCols = nanInds(1)-1;
        data = dataRaw(:,1:numCols);
        %data = rand(50,200);       
        
        orientData = data(:,end-17:end);
        graspDeg = -rad2deg(asin(orientData(:,5))); %(the - is because the computed rotation matrix is along positive axes (hence +Y, but the given rotation is around -Y).
        %graspDisp = 100*orientData(:,10);
        
        graspOri = deg2ori(graspDeg);
        
        feats = data(:,1:end-18);     
        

        
        % Get labels (with same size)
        labelsRaw = dataArray{1}; 
        tpLabelsRaw = cell(1,length(labelsRaw));
        toolLabelsRaw = cell(1,length(labelsRaw));
        % toolLabelsMat = zeros(length(labelsRaw),9);
        bad_i = [];
        bad_cnt = 1;
        for l = 1:length(labelsRaw)

            if (simF ==1)
                toolLabelsRaw{l}= [labelsRaw{l}(6:9)];
                tpLabelsRaw{l}= [labelsRaw{l}(6:9) '_' graspOri{l}];
                % toolLabelsMat(l,:)= [labelsRaw{l}(6:9) '_' graspOri{l}];
                tool = tpLabelsRaw{l}(1:4);
                if (strcmp(tool,'Lstk') || strcmp(tool,'star') || strcmp(tool,'flaT'))
                    bad_i(bad_cnt) = l;
                    bad_cnt = bad_cnt +1;
                end
            else
                toolLabelsRaw{l}= [labelsRaw{l}(7:10)];
                tpLabelsRaw{l}= [labelsRaw{l}(7:10) '_' graspOri{l}];
                % toolLabelsMat(l,:)= [labelsRaw{l}(7:10) '_' graspOri{l}];
            end            
        end
        
        tpLabelsRaw(bad_i) = [];
        toolLabelsRaw(bad_i) = [];
        feats(bad_i,:) = [];
        
        tool3DFeatures = [tool3DFeatures; feats];
        tpLabels = [tpLabels tpLabelsRaw];        
        toolLabels = [toolLabels toolLabelsRaw];
        
         
        
        % Close the text file.
        fclose(fileID);
        
    end    
end

% Clear temporary variables
clearvars filename delimiter formatSpec fileID dataArray labelsRaw;

%% read the images
toolIms = readToolImages(imsDir);

end

