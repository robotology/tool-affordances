function toolIms = readToolImages(imDir)


files = dir(imDir);

toolIms = cell(2,length(files)-2);
for fi =1:length(files)
    if (length(files(fi).name)>3)
        imfile = [imDir '/' files(fi).name];
        [im, map, alpha] = imread(imfile,'png'); 
        toolIms{1,fi-2} = im;
        toolIms{2,fi-2} = alpha;
        toolIms{3,fi-2} = files(fi).name(1:9);
    end
end