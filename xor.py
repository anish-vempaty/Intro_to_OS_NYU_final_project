def xor_file_contents(file_path, chunk_size=4096):
    xor_value = 0
    with open(file_path, 'rb') as file:
        while True:
            chunk = file.read(chunk_size)
            if not chunk:
                break
            for byte in chunk:
                xor_value ^= ord(byte)
    return xor_value

# Replace 'path/to/your/file' with the actual file path
file_path = 'ubuntu.iso'
result = xor_file_contents(file_path)
print "The XOR value of the file is: %08x" % result
