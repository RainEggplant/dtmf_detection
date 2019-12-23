filename = input('Input filename (empty line to exit): ');
while (~isempty(filename)) 
    [y, fs] = audioread(filename);
    key = my_goertzel(y, fs);
    disp(['Key: ', key]);
    filename = input('Input filename (empty line to exit): ');
end
