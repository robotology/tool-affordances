function cell_out = cell_subindex(cell_in, subind)

cell_mat_in = cell2mat(cell_in);
cell_mat_out = cell_mat_in(:,subind);
cell_out = mat2cell(cell_mat_out, ones(length(cell_in),1));
