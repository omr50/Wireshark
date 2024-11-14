cd ./src
g++ -g -o main main.cpp Parser.cpp  TCP_Packet.cpp TCP_Server.cpp UDP_Packet.cpp  IP_Packet.cpp  Capture.cpp ARP_Packet.cpp Ether_Packet.cpp -lpcap