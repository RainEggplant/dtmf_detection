function key = my_goertzel(y, fs)
persistent freq_list;
persistent key_mapping;
freq_list = [697, 770, 852, 941, 1209, 1336, 1477, 1633];
key_mapping = [
    '1', '2', '3', 'A';
    '4', '5', '6', 'B';
    '7', '8', '9', 'C';
    '*', '0', '#', 'D'
];

N = length(y);
k_list = round(freq_list / fs * N);
omega_list = 2 * pi * k_list / N;

% iterate
v0 = zeros(1, 8);  % v[n-2]
v1 = zeros(1, 8);  % v[n-1]
v = zeros(1, 8);   % v[n]
for n = 1 : N
    v0 = v1;
    v1 = v;
    v = 2 * cos(omega_list) .* v1 - v0 + y(n);
end

% find largest two freqs
Y = v - v1 .* exp(-1j * omega_list);
[~, ind] = sort(abs(Y), 'descend');
if ind(1) > ind(2)
    tmp = ind(1);
    ind(1) = ind(2);
    ind(2) = tmp;
end

% convert to key
row = mod(ind(1) - 1, 4) + 1;
col = mod(ind(2) - 1, 4) + 1;
key = key_mapping(row, col);
end
