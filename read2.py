xor_value = 0
chunk_size = 4096  # Read in chunks of 4KB

with open('ubuntu.iso', 'rb') as f:
    while True:
        chunk = f.read(chunk_size)
        if not chunk:
            break
        for byte in chunk:
            xor_value ^= ord(byte)

# If file size is not a multiple of 4, pad with 0x00 bytes
file_size = f.tell()
if file_size % 4 != 0:
    padding = 4 - (file_size % 4)
    xor_value ^= (0x00 * padding)

print('Final XOR value: {}'.format(xor_value))
