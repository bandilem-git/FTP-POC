import socket
import struct

s = socket.socket()
s.connect(('localhost', 8081))

# send filename
s.send(b'dh.mp3\n')

# read 8 byte file size header
size_data = s.recv(8)
fileSize = struct.unpack('q', size_data)[0]  # 'q' = int64
print(f"File size: {fileSize} bytes")

# recv loop
received = 0
with open('received_file', 'wb') as f:
    while received < fileSize:
        chunk = s.recv(4096)
        if not chunk:
            break
        f.write(chunk)
        received += len(chunk)

print(f"Done, received {received} bytes")
s.close()