function [checksum] = gps_checksum_ubx(msg)
% [checksum] = gps_checksum_ubx(msg)
%
% Calculates the UBX checksum to proceed the msg in two bytes. Checksums
% are calculated by XORing the bytes from the msg. See Commands for more
% information.
%
% Arguments:
%   msg: an UBX msg packed in an array of unsigned bytes
%            ie. [181 98 1 12 04 00 01 01]
%
% Returns: the checksum for the UBX message packed in a cell array with
%   bytes CK_A and CK_B.
%
% Note: Length is packed in two bytes (positions 5 and 6).
%   Use uint8() on msg array to drop decimal and truncate.
%


if nargin < 1
    error('Missing argument ''msg''')
elseif ~iscell(msg)
    error('Expected ''msg'' as an integer array');
end

CHECK_RANGE_START = 3;
LENGTH_POS = 5;
PAYLOAD_POS = 7;
CHECKSUM_BYTE_NUMBER = 2;
checksum = {0, 0};

% Get packet length (little endian)
payloadlength = uint16(bitshift(msg{LENGTH_POS + 1}, 8) + msg{LENGTH_POS});
checkstop = PAYLOAD_POS + payloadlength;
msgsize = size(msg);
if (checkstop > msgsize(2))
    % Erroneous length field
    return;
end
ck_a = 0; ck_b = 0;

fprintf('Calculating chk over: %.0f to %.0f\n', CHECK_RANGE_START, checkstop);
for i=CHECK_RANGE_START:checkstop,
    ck_a = bitand(ck_a + msg{i}, 255, 'uint8');
    ck_b = bitand(ck_b + ck_a, 255, 'uint8');
    
    
end

checksum = {uint8(ck_a), uint8(ck_b)};
%msg(length + 1) = ck_a;
%msg(length + 2) = ck_b;

end % function


