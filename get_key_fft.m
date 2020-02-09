function key = get_key_fft(y, fs)
persistent key_mapping;
key_mapping = [
    '1', '2', '3', 'A';
    '4', '5', '6', 'B';
    '7', '8', '9', 'C';
    '*', '0', '#', 'D'
];
len_y = length(y);    
N = 2 ^ nextpow2(len_y);
Y = fft(y, N);
Y_abs = abs(Y(1 : round(1700 / fs * N)));

[pks, locs] = findpeaks(Y_abs, 'Threshold', max(Y_abs) * 0.01, ...
    'MinPeakDistance', floor(200 / fs * N));
if length(locs) < 2
    key = 'X';
    return;
end

[~, ind] = sort(pks, 'descend');
freq1 = (min(locs(ind(1)), locs(ind(2))) - 1) / N * fs;
freq2 = (max(locs(ind(1)), locs(ind(2))) - 1) / N * fs;
if freq1 < 733.5
    row = 1;
elseif freq1 < 811
    row = 2;
elseif freq1 < 896.5
    row = 3;
else
    row = 4;
end

if freq2 < 1272.5
    col = 1;
elseif freq2 < 1406.5
    col = 2;
elseif freq2 < 1555
    col = 3;
else
    col = 4;
end

key = key_mapping(row, col);
end
