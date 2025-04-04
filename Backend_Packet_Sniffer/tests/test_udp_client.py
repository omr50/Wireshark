import socket
import time

serverAddressPort   = ("127.0.0.1", 20020)
bufferSize          = 4096

 

# Create a UDP socket at client side
UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)


packetNum = 1 
while (True):
    # Send to server using created UDP socket
    msgFromClient = f'Hello: {packetNum}'
    packetNum += 1
    bytesToSend = str.encode(msgFromClient)
    UDPClientSocket.sendto(bytesToSend, serverAddressPort)
    msgFromServer = UDPClientSocket.recvfrom(bufferSize)
    msg = "Message from Server {}".format(msgFromServer[0])
    print(msg)
    time.sleep(3)