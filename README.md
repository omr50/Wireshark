# Wireshark (Custom Packet Sniffer & Analyzer)

A custom-built packet capture, analysis, and visualization tool inspired by Wireshark.  
This project provides a low-level network sniffer backend written in C++ and a graphical interface for displaying captured packets in real time.

---

## Features

- **Packet Capture Engine** – sniffs live network traffic at a low level  
- **Protocol Parsing** – decodes common protocols from raw packets (Ethernet, IP, TCP, UDP, ARP, etc.)  
- **GUI Viewer** – displays packets in a structured interface with summary and detail views  
- **Filtering & Searching** – locate packets of interest quickly  
- **Detailed Inspection** – see packet fields, headers, and raw hex payload  
- **Modular Architecture** – backend and GUI are separate, making it easier to extend or replace parts
