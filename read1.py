def xor_file(file_path):
    xor_value = 0
    chunk_size = 4096  # Process 4096 bytes at a time

    with open(file_path, "rb") as file:
        while True:
            chunk = file.read(chunk_size)
            if not chunk:
                break
            for byte in chunk:
                xor_value ^= ord(byte)

    return xor_value

# Usage
file_path = "ubuntu.iso"  # Replace with your file path
result = xor_file(file_path)
print "The XOR value of the file is:", result
