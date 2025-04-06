"""
Test UDP Server to control the
datagrams being sent and received
to test the UDP "stream" tracing
ability. So UDP is of course not
a stream and some datagrams can be
lost but the server and client allow
for a controlled environment for building
the tracing functionality to be able to 
follow any UDP conversation for a given
"4-tuple" which is the src ip, src port,
dest ip, dest port. 
"""

import socket
import time

serverAddressPort   = ("127.0.0.1", 20020)
bufferSize          = 4096

# Create a UDP socket at client side
UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

packetNum = 1 
while (True):
    # Send to server using created UDP socket
    msgFromClient = f'Fake Client msg #{packetNum}'
    packetNum += 1
    bytesToSend = str.encode(msgFromClient)
    UDPClientSocket.sendto(bytesToSend, serverAddressPort)
    msgFromServer = UDPClientSocket.recvfrom(bufferSize)
    msg = "Message from Server {}".format(msgFromServer[0])
    print(msg)
    time.sleep(3)