# Wireshark (Custom Packet Sniffer & Analyzer)

A custom-built packet capture, analysis, and visualization tool inspired by Wireshark.  
This project provides a low-level network sniffer backend written in C++ and a graphical 
interface for displaying captured packets in real time.

## 🚀 Features

- **Packet Capture Engine** – sniffs live network traffic at a low level  
- **Protocol Parsing** – decodes common protocols from raw packets (Ethernet, IP, TCP, UDP, ARP, etc.)  
- **GUI Viewer** – displays packets in a structured interface with summary and detail views  
- **Filtering & Searching** – locate packets of interest quickly  
- **Detailed Inspection** – see packet fields, headers, and raw hex payload  
- **Modular Architecture** – backend and GUI are separate, making it easier to extend or replace parts

## 🛠 Technologies Used

- **C++** – main programming language for the backend packet sniffer
- **libpcap** – library for low-level packet capturing on different platforms
- **Sockets (BSD)** – used for handling raw packet data and communication between components
- **JavaScript / HTML / CSS** – used in the frontend GUI for interactive display of packet data
- **Electron** – wrapping the GUI in a desktop application environment
- **JSON** – serialization and communication format between backend and GUI
- **Protocol Headers (netinet / custom structs)** – parsing Ethernet, IP, TCP, UDP, and higher-level protocols
- **Make** – build systems for compiling the backend code
