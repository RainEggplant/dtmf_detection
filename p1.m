filename = input('Input filename (empty line to exit): ');
while (~isempty(filename)) 
    [y, fs] = audioread(filename);
    key = get_key_fft(y, fs);
    disp(['Key: ', key]);
    filename = input('Input filename (empty line to exit): ');
end
