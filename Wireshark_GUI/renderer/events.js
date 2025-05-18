const { ipcRenderer } = require('electron');

let packet_num = 1;
let packets_info = [];

const start_time = Date.now();
let small_padding = "&emsp;&emsp;&emsp;";
let padding = "&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;";
console.log("CURRENT START TIME: ", start_time);
const table = document.querySelector('.packet_table');


function set_highlight(column_num) {
    // loop through all rows for the specific column
    let rows = table.rows;    
    for (let i = 0; i < rows.length; i++) {
        rows[i].cells[column_num].classList.add('highlight');
    }
}

function remove_highlight(column_num) {
    // loop through all rows for the specific column
    let rows = table.rows;    
    for (let i = 0; i < rows.length; i++) {
        rows[i].cells[column_num].classList.remove('highlight');
    }
}

function toggleSubItems(event, element) {
  event.stopPropagation();
  const subItems = element.querySelector('.sub-items');
  const container = document.querySelector('.packet_details_container');
  const image = element.querySelector('.dropdown');
  if (subItems) {
    if (subItems.classList.contains('expanded')) {
      image.classList.remove('expanded');
      subItems.classList.remove('expanded');
    } else {
      // Expand the sub-items
      subItems.classList.add('expanded');
      image.classList.add('expanded');
    }
  }
}

function toggleFieldHighlight(field_name, event) {
    event.stopPropagation();
    if (field_name == 'stop_prop')
        return; 
    console.log("WORKING FIELD TOGGLE!");
    let allHighlightable = document.querySelectorAll(".highlightable");
    let allElements = document.querySelectorAll(`.${field_name}`);

    for (const elem of allHighlightable) {
        elem.style.backgroundColor = "";
        elem.style.color = "";
    }

    for (const elem of allElements) {
        elem.style.backgroundColor = "#0078D7";
        elem.style.color = "white";
    }

}

function search(event, element) {
    let searchTerms = ['http', 'tcp', 'udp', 'ip', 'eth', 'icmp', 'arp', '']
    const searchQuery = element.value.toLowerCase();
    if (searchTerms.includes(searchQuery)) {
        if (searchQuery === '')
            element.style.backgroundColor = "#FFFFFF";
        else
            element.style.backgroundColor = "#AFFFAF";
    } else {
        element.style.backgroundColor = "#FFAFAF";
        return;
    }

    // console.log(searchQuery)
    // filter through every tr, if it contains search query get it
    const elements = document.getElementsByTagName('tr');
    for (element of elements) {
       if (!element.innerText.toLowerCase().includes(searchQuery)) {
            element.style.display = 'none';
       } else {
        element.style.display = 'table-row';
       }
    };
}

function createARPDetailedInfo(detailedPacket) {
    let arp_detailed_packet_info = `
    <div class="item highlightable" onclick="toggleSubItems(event, this)">
        <div class="title"><img class="dropdown" src="./dropdown.png" width="14px" height="14px">
        ${detailedPacket["title"]} </div> 
        <div class="sub-items">
            <div class="item hardware_type highlightable" onclick="toggleFieldHighlight('hardware_type', event);">
                ${small_padding}${detailedPacket["hardware_type"]} 
            </div>
            <div class="item protocol_type highlightable" onclick="toggleFieldHighlight('protocol_type', event);">
                ${small_padding}${detailedPacket["protocol_type"]} 
            </div>

            <div class="hardware_len highlightable" onclick="toggleFieldHighlight('hardware_len', event);">
                ${small_padding}${detailedPacket["hardware_size"]} 
            </div>

            <div class="item protocol_len highlightable" onclick="toggleFieldHighlight('protocol_len', event);">
                ${small_padding}${detailedPacket["target_size"]} 
            </div>
            <div class="item operation highlightable" onclick="toggleFieldHighlight('operation', event);">
                ${small_padding}${detailedPacket["opcode"]} 
            </div>

            <div class="item sender_hardware_addr highlightable" onclick="toggleFieldHighlight('sender_hardware_addr', event);">
                ${small_padding}${detailedPacket["sender_mac"]} 
            </div>

            <div class="item sender_protocol_addr highlightable" onclick="toggleFieldHighlight('sender_protocol_addr', event);">
                ${small_padding}${detailedPacket["sender_ip"]} 
            </div>
            <div class="item target_hardware_addr highlightable" onclick="toggleFieldHighlight('target_hardware_addr', event);">
                ${small_padding}${detailedPacket["target_mac"]} 
            </div>

            <div class="item target_protocol_addr highlightable" onclick="toggleFieldHighlight('target_protocol_addr', event);">
                ${small_padding}${detailedPacket["target_ip"]} 
            </div>
        </div>
    </div> `
    return arp_detailed_packet_info;
}

function createEthIIDetailedInfo(detailedPacket) {
    let eth_detailed_packet_info = `
    <div class="item eth_packet highlightable" onclick="toggleSubItems(event, this); toggleFieldHighlight('eth_packet', event);">
      <div class="item" onclick="toggleSubItems(event, this)">
        <div class="title"><img class="dropdown" src="./dropdown.png" width="14px" height="14px">
        ${detailedPacket["title"]} </div> 
        <div class="sub-items">
        
            <div class="item destination_address highlightable" onclick="toggleSubItems(event, this); toggleFieldHighlight('destination_address', event);">
                ${small_padding}<img class="dropdown" src="./dropdown.png" width="14px" height="14px">
                ${detailedPacket["Destination"]["title"]} 
                <div class="sub-items">
                    <div class="lg_ig_bit_d highlightable" onclick="toggleFieldHighlight('lg_ig_bit_d', event);">
                        ${padding}${detailedPacket["Destination"]["LG"]} 
                    </div>
                    <div class="lg_ig_bit_d highlightable" onclick="toggleFieldHighlight('lg_ig_bit_d', event);">
                        ${padding}${detailedPacket["Destination"]["IG"]} 
                    </div>
                </div>
            </div>

            <div class="item source_address highlightable" onclick="toggleSubItems(event, this); toggleFieldHighlight('source_address', event);">
                ${small_padding}<img class="dropdown" src="./dropdown.png" width="14px" height="14px">
                ${detailedPacket["source"]["title"]} 
                <div class="sub-items">
                    <div class="item lg_ig_bit_s highlightable" onclick="toggleFieldHighlight('lg_ig_bit_s', event);">
                        ${padding}${detailedPacket["source"]["LG"]} 
                    </div>
                    <div class="item lg_ig_bit_s highlightable" onclick="toggleFieldHighlight('lg_ig_bit_s', event);">
                        ${padding}${detailedPacket["source"]["IG"]} 
                    </div>
                </div>
            </div>

            <div class="item type highlightable" onclick="toggleSubItems(event, this); toggleFieldHighlight('type', event);">
                ${small_padding}${detailedPacket["type"]} 
            </div>
        </div>
    </div>
    </div> `
    return eth_detailed_packet_info;
}

function createIPDetailedInfo(detailedPacket) {
      let ip_detailed_packet_info = `
  <div class="packet_details_container ip_packet highlightable" onclick="toggleFieldHighlight('ip_packet', event)">
      <div class="item" onclick="toggleSubItems(event, this)">
        <div class="title"><img class="dropdown" src="./dropdown.png" width="14px" height="14px">
        ${detailedPacket["title"]}</div> 
        <div class="sub-items">
            <div class="item version highlightable" onclick="toggleSubItems(event, this); toggleFieldHighlight('version', event);">
                ${small_padding}${detailedPacket["version"]} 
            </div>
            <div class="item header_length highlightable" onclick="toggleSubItems(event, this); toggleFieldHighlight('header_length', event)">
                ${small_padding}${detailedPacket["header_length"]} 
            </div>
            <div class="item diff_services highlightable" onclick="toggleSubItems(event, this); toggleFieldHighlight('diff_services', event)">
                ${small_padding}<img class="dropdown" src="./dropdown.png" width="14px" height="14px">
                ${detailedPacket["differentiated_services_field"]["title"]} 
                <div class="sub-items">
                    <div class="item codepoint highlightable" onclick="toggleFieldHighlight('codepoint', event);">
                        ${padding}${detailedPacket["differentiated_services_field"]["diff_services_codepoint"]} 
                    </div>
                    <div class="item congestion highlightable" onlick="toggleFieldHighlight('congestion', event);">
                        ${padding}${detailedPacket["differentiated_services_field"]["explicit_congestion_notification"]} 
                    </div>
                </div>
            </div>

            <div class="item total_length highlightable" onclick="toggleFieldHighlight('total_length', event);"> 
                ${small_padding}${detailedPacket["total_length"]} 
            </div>

            <div class="item identification highlightable" onclick="toggleFieldHighlight('identification', event);"> 
                ${small_padding}${detailedPacket["identification"]} 
            </div>

            <div class="item flags highlightable" onclick="toggleSubItems(event, this); toggleFieldHighlight('flags', event);"> 
                ${small_padding}<img class="dropdown" src="./dropdown.png" width="14px" height="14px">
                ${detailedPacket["flags"]["title"]} 
                <div class="sub-items">
                    <div class="item reserved highlightable" onclick="toggleFieldHighlight('reserved', event);">
                        ${padding}${detailedPacket["flags"]["reserved_bit"]} 
                    </div>

                    <div class="item dont_frag highlightable" onclick="toggleFieldHighlight('dont_frag', event);">
                        ${padding}${detailedPacket["flags"]["dont_fragment"]} 
                    </div>

                    <div class="item more_frag highlightable" onclick="toggleFieldHighlight('more_frag', event);">
                        ${padding}${detailedPacket["flags"]["more_fragments"]} 
                    </div>
                    
                </div>
            </div>
            
            <div class="item frag_offset highlightable" onclick="toggleFieldHighlight('frag_offset', event);">
                ${small_padding}${detailedPacket["fragment_offset"]} 
            </div>


            <div class="item ttl highlightable"  onclick="toggleFieldHighlight('ttl', event);">
                ${small_padding}${detailedPacket["time_to_live"]} 
            </div>

            <div class="item protocol highlightable" onclick="toggleFieldHighlight('protocol', event);">
                ${small_padding}${detailedPacket["protocol"]} 
            </div>

            <div class="item ip_checksum highlightable" onclick="toggleFieldHighlight('ip_checksum', event);">
                ${small_padding}${detailedPacket["header_checksum"]} 
            </div>

            <div class="item source_addr highlightable" onclick="toggleFieldHighlight('source_addr', event);">
                ${small_padding}${detailedPacket["source_ip"]} 
            </div>

            <div class="item dest_addr highlightable" onclick="toggleFieldHighlight('dest_addr', event);">
                \t${small_padding}${detailedPacket["dest_ip"]} 
            </div>

        </div>
    </div> 
  </div>
  </div>
  `
  return ip_detailed_packet_info;
}

function createUDPDetailedInfo(detailedPacket) {
    let udp_detailed_packet_info = `
    
  <div class="packet_details_container udp_packet highlightable" onclick="toggleFieldHighlight('udp_packet', event)">
    <div class="item highlightable" onclick="toggleSubItems(event, this);">
        <div class="title"><img class="dropdown" src="./dropdown.png" width="14px" height="14px">
        ${detailedPacket["title"]} </div> 
        <div class="sub-items">
            <div class="item src_port highlightable" onclick="toggleFieldHighlight('src_port', event);" >
                ${small_padding}${detailedPacket["Source_Port"]} 
            </div>

            <div class="item dest_port highlightable" onclick="toggleFieldHighlight('dest_port', event);">
                ${small_padding}${detailedPacket["Destination_Port"]} 
            </div>

            <div class="item length highlightable" onclick="toggleFieldHighlight('length', event);">
                ${small_padding}${detailedPacket["Length"]} 
            </div>

            <div class="item checksum highlightable" onclick="toggleFieldHighlight('checksum', event);">
                ${small_padding}${detailedPacket["Checksum"]} 
            </div>

            <div class="item" onclick="toggleFieldHighlight('stop_prop', event);">
                ${small_padding}${detailedPacket["Checksum_status"]} 
            </div>

            <div class="item" onclick="toggleFieldHighlight('stop_prop', event);">
                ${small_padding}${detailedPacket["Stream_Index"]} 
            </div>

            <div class="item" onclick="toggleSubItems(event, this); toggleFieldHighlight('stop_prop', event);">
                ${small_padding}<img class="dropdown" src="./dropdown.png" width="14px" height="14px">
                    ${detailedPacket["Timestamps"]["title"]} 
                <div class="sub-items">

                    <div class="item">
                        ${padding}${detailedPacket["Timestamps"]["first_frame"]} 
                    </div>

                    <div class="item">
                        ${padding}${detailedPacket["Timestamps"]["prev_frame"]} 
                    </div>
                </div>
            </div>

            <div class="item payload highlightable" onclick="toggleFieldHighlight('payload', event);">
                ${small_padding}${detailedPacket["UDP_Payload"]} 
            </div>
            
        </div>
    </div>
   </div>`
    return udp_detailed_packet_info;
}

function createTCPDetailedInfo(detailedPacket) {
    let tcp_detailed_packet_info = `
    
  <div class="packet_details_container tcp_packet highlightable" onclick="toggleFieldHighlight('tcp_packet', event)">
    <div class="item highlightable" onclick="toggleSubItems(event, this);">
        <div class="title"><img class="dropdown" src="./dropdown.png" width="14px" height="14px">
        ${detailedPacket["title"]} </div> 
        <div class="sub-items">
            <div class="item tcp_src_port highlightable" onclick="toggleFieldHighlight('tcp_src_port', event);" >
                ${small_padding}${detailedPacket["Source_Port"]} 
            </div>

            <div class="item tcp_dest_port highlightable" onclick="toggleFieldHighlight('tcp_dest_port', event);">
                ${small_padding}${detailedPacket["Destination_Port"]} 
            </div>

            <div class="item seq_num highlightable" onclick="toggleFieldHighlight('seq_num', event);">
                ${small_padding}${detailedPacket["Sequence_Number"]} 
            </div>

            <div class="item ack_num highlightable" onclick="toggleFieldHighlight('ack_num', event);">
                ${small_padding}${detailedPacket["Acknowledgement_Number"]} 
            </div>

            <div class="item tcp_header_len highlightable" onclick="toggleFieldHighlight('tcp_header_len', event);">
                ${small_padding}${detailedPacket["Header_Length"]} 
            </div>

            <div class="item tcp_flags highlightable" onclick="toggleSubItems(event, this); toggleFieldHighlight('tcp_flags', event);">
                ${small_padding}<img class="dropdown" src="./dropdown.png" width="14px" height="14px">
                    ${detailedPacket["Flags"]["Title"]} 
                <div class="sub-items">

                    <div class="item">
                        ${padding}${detailedPacket["Flags"]["Reserved"]} 
                    </div>

                    <div class="item">
                        ${padding}${detailedPacket["Flags"]["ECN"]} 
                    </div>
                    
                    <div class="item">
                        ${padding}${detailedPacket["Flags"]["Congestion_Window"]} 
                    </div>
                    
                    <div class="item">
                        ${padding}${detailedPacket["Flags"]["ECN-Echo"]} 
                    </div>

                    <div class="item">
                        ${padding}${detailedPacket["Flags"]["Urgent"]} 
                    </div>

                    <div class="item">
                        ${padding}${detailedPacket["Flags"]["Acknowledgement"]} 
                    </div>

                    <div class="item">
                        ${padding}${detailedPacket["Flags"]["Push"]} 
                    </div>
                    
                    <div class="item">
                        ${padding}${detailedPacket["Flags"]["Reset"]} 
                    </div>

                    <div class="item">
                        ${padding}${detailedPacket["Flags"]["Syn"]} 
                    </div>

                    <div class="item">
                        ${padding}${detailedPacket["Flags"]["Fin"]} 
                    </div>


                </div>
            </div>

            <div class="item window highlightable" onclick="toggleFieldHighlight('window', event);">
                ${small_padding}${detailedPacket["Window"]} 
            </div>
 
            <div class="item tcp_checksum highlightable" onclick="toggleFieldHighlight('tcp_checksum', event);">
                ${small_padding}${detailedPacket["Checksum"]} 
            </div>
 
            <div class="item urg_pointer highlightable" onclick="toggleFieldHighlight('urg_pointer', event);">
                ${small_padding}${detailedPacket["Urgent_Pointer"]} 
            </div>
            
        </div>
    </div>
   </div>`
    return tcp_detailed_packet_info;
}

function stringToHex(str) {
  let result = "";
  for (let i = 0; i < str.length; i++) {
    // Get char code, convert to hex, zero-pad to 2 chars
    const hexValue = str.charCodeAt(i).toString(16).padStart(2, '0');
    result += hexValue + " ";
  }
  return result.trim();
}

function createUDPPayload(detailedPacket) {
    let data = detailedPacket["data"];
    // clip data to certain size
    if (data.length >= 50) {
        data = data.slice(0, 50);
        data += "...";
    }
    let udp_payload_info = `
    <div class="item" onclick="toggleSubItems(event, this);">
        <div class="title"><img class="dropdown" src="./dropdown.png" width="14px" height="14px">
        Data (${detailedPacket["Length"]}) </div> 
        <div class="sub-items data highlightable" onclick="toggleFieldHighlight('data', event);">
            <div class="item">
                ${small_padding}Data: ${data} 
            </div>

            <div class="item">
                ${small_padding}[Length: ${detailedPacket["Length"]}] 
            </div>



            
        </div>
    </div> ` 
    return udp_payload_info;
}

function renderDetailedInfo(packet_num, element) {
//   console.log(packets_info) 
  let detailedPackets = packets_info[packet_num]["detailed_info"];
  // find the element where we can render the detailed packet info
  let detailedProtocolElement = document.querySelector(".left-protocol-info");
  // for every object set up a drop down.
  // for simple string just display it in p tag
  // later do a for each loop because the detailed packet will be an array of objects
  // each object containing the layer
  let list = `
  <div class="packet_details_container">
      ${createEthIIDetailedInfo(detailedPackets[0])}
      ${(detailedPackets.length > 1 && detailedPackets[1]["title"].includes("Internet Protocol Version 4")) ? createIPDetailedInfo(detailedPackets[1]) : ""}
      ${(detailedPackets.length > 1 && detailedPackets[1]["title"].includes("Address Resolution Protocol")) ? createARPDetailedInfo(detailedPackets[1]) : ""}
      ${(detailedPackets.length > 2 && detailedPackets[2]["title"].includes("User Datagram Protocol")) ? createUDPDetailedInfo(detailedPackets[2]) : ""}
      ${(detailedPackets.length > 2 && detailedPackets[2]["title"].includes("Transmission Control Protocol")) ? createTCPDetailedInfo(detailedPackets[2]) : ""}
      ${(detailedPackets.length === 3 && detailedPackets[2]["title"].includes("User Datagram Protocol")) ? createUDPPayload(detailedPackets[2]) : ""}
  </div>
  `
  detailedProtocolElement.innerHTML = "";
  detailedProtocolElement.innerHTML += list;
}

// renders all the packet info
function renderInfo(packet_num, element) {
    renderDetailedInfo(packet_num, element);
    renderHexInfo(packet_num, element);
}

function renderHexInfo(packet_num, element) {

  let detailedPackets = packets_info[packet_num]["detailed_info"];
  let hexElement = document.querySelector(".right-hex-info");

  let list = 
  `<div class="hex-info-container">
      ${(detailedPackets.length === 2 && detailedPackets[1]["title"].includes("Address Resolution Protocol")) ? createHexData(detailedPackets[1]["all_data"], "ARP") : ""}
      ${(detailedPackets.length === 3 && detailedPackets[2]["title"].includes("User Datagram Protocol")) ? createHexData(detailedPackets[2]["all_data"], "UDP") : ""}
      ${(detailedPackets.length === 3 && detailedPackets[2]["title"].includes("Transmission Control Protocol")) ? createHexData(detailedPackets[2]["all_data"], "TCP") : ""}
  </div>
  `
  hexElement.innerHTML = "";
  hexElement.innerHTML += list;
}

function createEtherHexData(hexData) {
    return `
        <span class="destination_address highlightable"><span class="lg_ig_bit highlightable">${get_bytes_spaced(hexData, 0, 2)}</span>${get_bytes_spaced(hexData, 3, 5)}</span>
        <span class="source_address highlightable"><span class="lg_ig_bit highlightable">${get_bytes_spaced(hexData, 6, 8)}</span>${get_bytes_spaced(hexData, 9, 11)}</span>
        <span class="type highlightable">${get_bytes_spaced(hexData, 12, 13)}</span>
        `
}

function createIPHexData(hexData) {
    return `

        <span class="version header_length highlightable">${get_bytes_spaced(hexData, 14, 14)}</span>
        <span class="diff_services codepoint congestion highlightable">${get_bytes_spaced(hexData, 15, 15)}</span>
        <div></div>
        <span class="total_length highlightable">${get_bytes_spaced(hexData, 16, 17)}</span>
        <span class="identification highlightable">${get_bytes_spaced(hexData, 18, 19)}</span>
        <span class="frag_offset highlightable"><span class="flags reserved dont_frag more_frag highlightable">${get_bytes_spaced(hexData, 20, 20)}</span>${get_bytes_spaced(hexData, 21, 21)}</span>
        <span class="ttl highlightable">${get_bytes_spaced(hexData, 22, 22)}</span>
        <span class="protocol highlightable">${get_bytes_spaced(hexData, 23, 23)}</span>
        <span class="ip_checksum highlightable">${get_bytes_spaced(hexData, 24, 25)}</span>
        <span class="source_addr highlightable">${get_bytes_spaced(hexData, 26, 29)}</span>
        <span class="dest_addr highlightable">${get_bytes_spaced(hexData, 30, 31)}</span>
        <div></div>
        <span class="dest_addr highlightable">${get_bytes_spaced(hexData, 32, 33)}</span>
        `
}

function createUDPHexData(hexData) {
    return `
        <span class="src_port highlightable">${get_bytes_spaced(hexData, 34, 35)}</span>
        <span class="dest_port highlightable">${get_bytes_spaced(hexData, 36, 37)}</span>
        <span class="length highlightable">${get_bytes_spaced(hexData, 38, 39)}</span>
        <span class="checksum highlightable">${get_bytes_spaced(hexData, 40, 41)}</span>
        <span class="payload data highlightable">${getPayload(hexData)}</span>
        `
        // <span class="payload">${get_bytes_spaced(hexData, 42, 47)}</span>
        // <div></div>
        // <span class="payload">${get_bytes_spaced(hexData, 48, "end")}</span>
}

function createTCPHexData(hexData) {
    return `
        <span class="tcp_src_port highlightable">${get_bytes_spaced(hexData, 34, 35)}</span>
        <span class="tcp_dest_port highlightable">${get_bytes_spaced(hexData, 36, 37)}</span>
        <span class="seq_num highlightable">${get_bytes_spaced(hexData, 38, 41)}</span>
        <span class="ack_num highlightable">${get_bytes_spaced(hexData, 42, 45)}</span>
        <span class="tcp_header_len highlightable">${get_bytes_spaced(hexData, 46, 46)}</span>
        <span class="tcp_flags highlightable">${get_bytes_spaced(hexData, 47, 47)}</span>
        <div></div>
        <span class="window highlightable">${get_bytes_spaced(hexData, 48, 49)}</span>
        <span class="tcp_checksum highlightable">${get_bytes_spaced(50, 51)}</span>
        <span class="urg_pointer highlightable">${get_bytes_spaced(52, 53)}</span>
        `
        // <span class="payload">${get_bytes_spaced(hexData, 42, 47)}</span>
        // <div></div>
        // <span class="payload">${get_bytes_spaced(hexData, 48, "end")}</span>
}

function createARPHexData(hexData) {
    return `
        <span class="hardware_type highlightable">${get_bytes_spaced(hexData, 14, 15)}</span>
        <div></div>
        <span class="protocol_type highlightable">${get_bytes_spaced(hexData, 16, 17)}</span>
        <span class="hardware_len highlightable">${get_bytes_spaced(hexData, 18, 18)}</span>
        <span class="protocol_len highlightable">${get_bytes_spaced(hexData, 19, 19)}</span>
        <span class="operation highlightable">${get_bytes_spaced(hexData, 20, 21)}</span>
        <span class="sender_hardware_addr highlightable">${get_bytes_spaced(hexData, 22, 27)}</span>
        <span class="sender_protocol_addr highlightable">${get_bytes_spaced(hexData, 28, 31)}</span>
        <div></div>
        <span class="target_hardware_addr highlightable">${get_bytes_spaced(hexData, 32, 37)}</span>
        <span class="target_protocol_addr highlightable">${get_bytes_spaced(hexData, 38, 41)}</span>
        `
        // <span class="payload">${get_bytes_spaced(hexData, 42, 47)}</span>
        // <div></div>
        // <span class="payload">${get_bytes_spaced(hexData, 48, "end")}</span>
}




function getPayload(hexData) {
    let total = "";
    for (let i = 42; i < hexData.length / 2; i++) {
        if (i % 16 == 0) {
            total += "<div></div>";
        }
        total += get_bytes_spaced(hexData, i, i);
    }
    return total;
}

function createHexData(hexData, packet_type) {
    if (packet_type == "UDP") {
        const hex_data = `
        <div class="hex-data-container">
            ${getByteCounter(hexData)}
            <div class="hex_bytes">
                ${createEtherHexData(hexData)}        
                ${createIPHexData(hexData)}        
                ${createUDPHexData(hexData)}        
            </div>
            ${getByteTranslation(hexData)}
        </div>
        `
        return hex_data;
    }
    if (packet_type == "TCP") {
        const hex_data = `
        <div class="hex-data-container">
            ${getByteCounter(hexData)}
            <div class="hex_bytes">
                ${createEtherHexData(hexData)}        
                ${createIPHexData(hexData)}        
                ${createTCPHexData(hexData)}        
            </div>
            ${getByteTranslation(hexData)}
        </div>
        `
        return hex_data;
    }
    if (packet_type == "ARP") {
        const hex_data = `
        <div class="hex-data-container">
            ${getByteCounter(hexData)}
            <div class="hex_bytes">
                ${createEtherHexData(hexData)}        
                ${createARPHexData(hexData)}        
            </div>
            ${getByteTranslation(hexData)}
        </div>
        `
        return hex_data;
    }
}

// Function displays the byte values for each row
// for each 16 bytes generate a row, use ceiling
function getByteCounter(data) {
    let numBytes = getBytes(data);
    let rows =  Math.ceil(numBytes / 16);

    let divs = `<div class="byte_nums">`;
    let val = 0;
    for (let i = 0; i < rows; i++) {
        divs += `<div>${val.toString(16).toUpperCase().padStart(4, '0')}</div>`;
        val += 16;
    }
    divs += "</div>";

    return divs;
}

function getBytes(data) {
    // each 2 chars is a byte
    return data.length / 2;
}

// returns bytes for the hex view
function get_bytes_spaced(data, start, end) {
    // end is inclusive
    console.log("length:", data.length);
    console.log(`(${start}, ${end})`);
    let out = "";
    start *= 2;
    if (end == "end") {
        end = data.length - 1;
    } else {
        end *= 2;
    }
    for (let i = start; i <= end; i+=2) {
        out += (data[i] + data[i+1]);
        out += " ";
    }
    return out;
}

// translates byte to ascii
function getByteTranslation(data) {
    let output = `<div class="byte_translation"><div>`;
    let counter = 0;
    let char = "";
    for (let i = 0; i < data.length; i+=2) {
        let hexStr = data[i] + data[i+1];

        let decimal = parseInt(hexStr, 16);
        if (decimal >= 32 && decimal <= 126) {
            char = String.fromCharCode(decimal);
        } else {
            char = '.'; // Non-printable
        }
        if (counter == 16) {
            counter = 0;
            output += "</div><div>";
        }
        output += char;
        counter++;
        console.log("The char!", char);
    }
    // if greater than one then we never closed the div.
    if (counter > 1) {
        output += "</div>"
    }
    output += "</div>";
    return output;
}



// Listen for the tcp-data event from the main process
ipcRenderer.on('tcp-data', (event, data) => {
//   console.log("THE TCP DATA IS ", data);
  // test
  let entire_packet = data;
  let packet_info = data['packet_info'];
  let detailed_info = data['detailed_info'];
  console.log("Entire Packet: ", entire_packet);
  packets_info.push(data)
  let color_class = packet_info.protocol.toLowerCase(); 
  let exact_time = parseInt(packet_info.time) - start_time;
  let seconds = exact_time / 1000;
  let new_row = `<tr class="${color_class}" 
                onclick="renderInfo(${packet_num-1}, this)">
                  <td onmouseenter="set_highlight(0)" onmouseleave="remove_highlight(0)">${packet_num}</td>
                  <td onmouseenter="set_highlight(1)" onmouseleave="remove_highlight(1)">${seconds}</td>
                  <td onmouseenter="set_highlight(2)" onmouseleave="remove_highlight(2)">${packet_info.source}</td>
                  <td onmouseenter="set_highlight(3)" onmouseleave="remove_highlight(3)">${packet_info.destination}</td>
                  <td onmouseenter="set_highlight(4)" onmouseleave="remove_highlight(4)">${packet_info.protocol}</td>
                  <td onmouseenter="set_highlight(5)" onmouseleave="remove_highlight(5)">${packet_info.length}</td>
                  <td onmouseenter="set_highlight(6)" onmouseleave="remove_highlight(6)">${packet_info.info}</td>
                </tr>`
  packet_num++;

  let packet_table = document.querySelector(".packet_table");
  packet_table.innerHTML += new_row;
  search(null, document.querySelector(".search-bar"));
  // const tcpDataDiv = document.getElementById('tcp-data');
  // tcpDataDiv.textContent = data; // Update the content
});