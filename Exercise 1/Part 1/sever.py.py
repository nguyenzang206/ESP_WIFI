import socket

HOST = "0.0.0.0"
PORT = 6000

sever = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sever.bind((HOST, PORT))
sever.listen(1)
print(f"Sever listening on {HOST}:{PORT}") 

conn, addr = sever.accept()
print("kết nối từ:", addr)

while True:
    data = conn.recv(1024)
    if not data:
        break
    message = data.decode().strip()
    if message: 
        print("Nhận:", message)
