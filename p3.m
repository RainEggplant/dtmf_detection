[y, fs] = audioread('data2/data.wav');

rms_threshold = 0.3;
win_len = 128;
frame = 1;

y_len = length(y);
start_frames = [];
end_frames = [];
silence = true;

% segmentation
while frame <= y_len
    rms_win = rms(y(frame : min(frame + win_len - 1, y_len)));
    if rms_win > rms_threshold
        if silence
            start_frames(end + 1) = frame;
            silence = false;
        end
    else
        if ~silence
            end_frames(end + 1) = frame;
            silence = true;
        end
    end
    frame = frame + win_len;
end

% get keys
n_key = length(start_frames);
keys = zeros(1, n_key);
for k = 1 : n_key
    keys(k) = my_goertzel(y(start_frames(k) : end_frames(k)), fs);
end

keys = char(keys);
disp(['Keys: ', keys]);
