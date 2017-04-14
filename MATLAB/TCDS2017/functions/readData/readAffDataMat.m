function [tool_labels, deg,act, eff] = readAffDataMat()

filename = './data/affData/real/affData_dumpers_real.txt';


delimiter = {' ','(',')'};

%% Format string for each line of text:
%   column3: text (%s)
%	column4: double (%f)
%   column5: double (%f)
%   column6: double (%f)
%	column8: double (%f)
% For more information, see the TEXTSCAN documentation.
formatSpec = '%*s%*s%s%f%f%f%*s%f%*s%*s%[^\n\r]';

%% Open the text file.
fileID = fopen(filename,'r');

%% Read columns of data according to format string.
% This call is based on the structure of the file used to generate this
% code. If an error occurs for a different file, try regenerating the code
% from the Import Tool.
dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'MultipleDelimsAsOne', true,  'ReturnOnError', false);

%% Close the text file.
fclose(fileID);

%% Post processing for unimportable data.
% No unimportable data rules were applied during the import, so no post
% processing code is included. To generate code which works for
% unimportable data, select unimportable cells in a file and regenerate the
% script.

%% Allocate imported array to column variable names
tool_labels = dataArray{:, 1};
tool_id = dataArray{:, 2};
deg = dataArray{:, 3};
act = dataArray{:, 4};
eff = dataArray{:, 5};



%% Clear temporary variables
clearvars filename delimiter formatSpec fileID dataArray ans;