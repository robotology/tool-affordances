function mapped_i = map_aff2tool(tp_labels_aff, tp_labels_tool, can_repeat)
% can_repeat = 0 -> no label repetition allowed, in source or target
% can_repeat = 1 -> label repetition allowed in target
% can_repeat = 2 -> label repetition allowed in source


if (nargin <3) || isempty(can_repeat)
    can_repeat = 0;            % for TCDS experiment
end

verbose = 0;
% map feat with aff vectors
num_affLab = length(tp_labels_aff);
mapped_i = [];
%toolFeatsMapped = zeros(length(toolLabelsAff), size(toolFeats,2));
for a = 1:num_affLab
    if(verbose) fprintf('Looking for label %s \n', tp_labels_aff{a} ); end
     
    for f = 1:length(tp_labels_tool)
        if strcmp(tp_labels_aff{a}, tp_labels_tool{f})
            if(verbose)  fprintf('Found it at index %i \n',f ); end
            %toolFeatsMapped(a, :) = toolFeats(f,:);

            if can_repeat == 0     
                % delete index so same target label cant be used again                
                tp_labels_tool{f} = 'used';
                mapped_i(a) = f;            
                break;
            elseif can_repeat == 1   
                % same target label cant be used again 
                mapped_i(a) = f;
                break;
            elseif can_repeat == 2   
                % do not break search, so go on looking for the same source label
                mapped_i = [mapped_i f];
            end                    
        end       
    end

end