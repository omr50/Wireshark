import socket

localIP = "127.0.0.1"
localPort = 20020
bufferSize = 4096
packetNum = 1

# create datagram sock
UDP_Server_Socket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

# bind to address + ip
UDP_Server_Socket.bind((localIP, localPort))

print("UDP server listening ")

while (True):
    bytesAddressPair = UDP_Server_Socket.recvfrom(bufferSize)
    message, address = bytesAddressPair[0], bytesAddressPair[1]

    clientMsg = f'Client Message: {message}'
    clientIP_Port = f'Client IP: {address}'

    print(clientMsg)
    print(clientIP_Port)

    serverMsg = f'Packet num: {packetNum}\n'
    packetNum += 1
    bytesToSend = str.encode(serverMsg)

    UDP_Server_Socket.sendto(bytesToSend, address)




