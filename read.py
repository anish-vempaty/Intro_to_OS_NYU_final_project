xor_value = 0
with open('ubuntu.iso', 'rb') as f:
    while True:
        byte = f.read(1)
        if not byte:
            break
        xor_value ^= ord(byte)

print('Final XOR value: {}'.format(xor_value))
