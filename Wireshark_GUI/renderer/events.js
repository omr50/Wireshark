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
    <div class="item" onclick="toggleSubItems(event, this)">
        <div class="title"><img class="dropdown" src="./dropdown.png" width="14px" height="14px">
        ${detailedPacket["title"]} </div> 
        <div class="sub-items">
            <div class="item">
                ${small_padding}${detailedPacket["hardware_type"]} 
            </div>
            <div class="item">
                ${small_padding}${detailedPacket["protocol_type"]} 
            </div>

            <div class="item">
                ${small_padding}${detailedPacket["hardware_size"]} 
            </div>

            <div class="item">
                ${small_padding}${detailedPacket["target_size"]} 
            </div>
            <div class="item">
                ${small_padding}${detailedPacket["opcode"]} 
            </div>

            <div class="item">
                ${small_padding}${detailedPacket["sender_mac"]} 
            </div>

            <div class="item">
                ${small_padding}${detailedPacket["sender_ip"]} 
            </div>
            <div class="item">
                ${small_padding}${detailedPacket["target_mac"]} 
            </div>

            <div class="item">
                ${small_padding}${detailedPacket["target_ip"]} 
            </div>
        </div>
    </div> `
    return arp_detailed_packet_info;
}

function createEthIIDetailedInfo(detailedPacket) {
    let eth_detailed_packet_info = `
    <div class="item" onclick="toggleSubItems(event, this)">
        <div class="title"><img class="dropdown" src="./dropdown.png" width="14px" height="14px">
        ${detailedPacket["title"]} </div> 
        <div class="sub-items">
            <div class="item" onclick="toggleSubItems(event, this)">
                ${small_padding}<img class="dropdown" src="./dropdown.png" width="14px" height="14px">
                ${detailedPacket["source"]["title"]} 
                <div class="sub-items">
                    <div class="item">
                        ${padding}${detailedPacket["source"]["LG"]} 
                    </div>
                    <div class="item">
                        ${padding}${detailedPacket["source"]["IG"]} 
                    </div>
                </div>
            </div>

            <div class="item" onclick="toggleSubItems(event, this)">
                ${small_padding}<img class="dropdown" src="./dropdown.png" width="14px" height="14px">
                ${detailedPacket["Destination"]["title"]} 
                <div class="sub-items">
                    <div>
                        ${padding}${detailedPacket["Destination"]["LG"]} 
                    </div>
                    <div>
                        ${padding}${detailedPacket["Destination"]["IG"]} 
                    </div>
                </div>
            </div>

            <div class="item" onclick="toggleSubItems(event, this)">
                ${small_padding}${detailedPacket["type"]} 
            </div>
        </div>
    </div> `
    return eth_detailed_packet_info;
}

function createIPDetailedInfo(detailedPacket) {
      let ip_detailed_packet_info = `
  <div class="packet_details_container">
      <div class="item" onclick="toggleSubItems(event, this)">
        <div class="title"><img class="dropdown" src="./dropdown.png" width="14px" height="14px">
        ${detailedPacket["title"]}</div> 
        <div class="sub-items">
            <div class="item" onclick="toggleSubItems(event, this)">
                ${small_padding}${detailedPacket["version"]} 
            </div>
            <div class="item" onclick="toggleSubItems(event, this)">
                ${small_padding}${detailedPacket["header_length"]} 
            </div>
            <div class="item" onclick="toggleSubItems(event, this)">
                ${small_padding}<img class="dropdown" src="./dropdown.png" width="14px" height="14px">
                ${detailedPacket["differentiated_services_field"]["title"]} 
                <div class="sub-items">
                    <div class="item">
                        ${padding}${detailedPacket["differentiated_services_field"]["diff_services_codepoint"]} 
                    </div>
                    <div class="item">
                        ${padding}${detailedPacket["differentiated_services_field"]["explicit_congestion_notification"]} 
                    </div>
                </div>
            </div>

            <div class="item"> 
                ${small_padding}${detailedPacket["total_length"]} 
            </div>

            <div class="item"> 
                ${small_padding}${detailedPacket["identification"]} 
            </div>

            <div class="item" onclick="toggleSubItems(event, this)"> 
                ${small_padding}<img class="dropdown" src="./dropdown.png" width="14px" height="14px">
                ${detailedPacket["flags"]["title"]} 
                <div class="sub-items">
                    <div class="item">
                        ${padding}${detailedPacket["flags"]["reserved_bit"]} 
                    </div>

                    <div class="item">
                        ${padding}${detailedPacket["flags"]["dont_fragment"]} 
                    </div>

                    <div class="item">
                        ${padding}${detailedPacket["flags"]["more_fragments"]} 
                    </div>
                    
                </div>
            </div>
            
            <div class="item">
                ${small_padding}${detailedPacket["fragment_offset"]} 
            </div>


            <div class="item">
                ${small_padding}${detailedPacket["time_to_live"]} 
            </div>

            <div class="item">
                ${small_padding}${detailedPacket["protocol"]} 
            </div>

            <div class="item">
                ${small_padding}${detailedPacket["header_checksum"]} 
            </div>

            <div class="item">
                ${small_padding}${detailedPacket["source_ip"]} 
            </div>

            <div class="item">
                \t${small_padding}${detailedPacket["dest_ip"]} 
            </div>

        </div>
    </div> 
  </div>
  `
  return ip_detailed_packet_info;
}

function createUDPDetailedInfo(detailedPacket) {
    let udp_detailed_packet_info = `
    <div class="item" onclick="toggleSubItems(event, this)">
        <div class="title"><img class="dropdown" src="./dropdown.png" width="14px" height="14px">
        ${detailedPacket["title"]} </div> 
        <div class="sub-items">
            <div class="item">
                ${small_padding}${detailedPacket["Source_Port"]} 
            </div>
            <div class="item">
                ${small_padding}${detailedPacket["Destination_Port"]} 
            </div>

            <div class="item">
                ${small_padding}${detailedPacket["Length"]} 
            </div>

            <div class="item">
                ${small_padding}${detailedPacket["Checksum"]} 
            </div>

            <div class="item">
                ${small_padding}${detailedPacket["Checksum_status"]} 
            </div>

            <div class="item">
                ${small_padding}${detailedPacket["Stream_Index"]} 
            </div>




            <div class="item" onclick="toggleSubItems(event, this)">
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

            <div class="item">
                ${small_padding}${detailedPacket["UDP_Payload"]} 
            </div>



            
        </div>
    </div> `
    return udp_detailed_packet_info;
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
    <div class="item" onclick="toggleSubItems(event, this)">
        <div class="title"><img class="dropdown" src="./dropdown.png" width="14px" height="14px">
        Data (${detailedPacket["Length"]}) </div> 
        <div class="sub-items">

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
      ${(detailedPackets.length === 3 && detailedPackets[2]["title"].includes("User Datagram Protocol")) ? createUDPPayload(detailedPackets[2]) : ""}
  </div>
  `
  detailedProtocolElement.innerHTML = "";
  detailedProtocolElement.innerHTML += list;
}

function renderInfo(packet_num, element) {
    renderDetailedInfo(packet_num, element);
    renderHexInfo(packet_num, element);
}

function renderHexInfo(packet_num, element) {

  let detailedPackets = packets_info[packet_num]["detailed_info"];
  let hexElement = document.querySelector(".right-hex-info");

  let list = 
  `<div class="hex-info-container">
      ${(detailedPackets.length === 3 && detailedPackets[2]["title"].includes("User Datagram Protocol")) ? createHexData(detailedPackets[2]["all_data"], "UDP") : ""}
  </div>
  `
  hexElement.innerHTML = "";
  hexElement.innerHTML += list;
}

function createEtherHexData(hexData) {
    return `
        <span class="destination_address"><span class="lg_ig_bit">${get_bytes_spaced(hexData, 0, 2)}</span>${get_bytes_spaced(hexData, 3, 5)}</span>
        <span class="source_address"><span class="lg_ig_bit">${get_bytes_spaced(hexData, 6, 8)}</span>${get_bytes_spaced(hexData, 9, 11)}</span>
        <span class="type">${get_bytes_spaced(hexData, 12, 13)}</span>
        `
}

function createIPHexData(hexData) {
    return `
        <span class="version header_length">${get_bytes_spaced(hexData, 14, 14)}</span>
        <span class="diff_services codepoint congestion">${get_bytes_spaced(hexData, 15, 15)}</span>
        <span class="total_length">${get_bytes_spaced(hexData, 16, 17)}</span>
        <span class="identification">${get_bytes_spaced(hexData, 18, 19)}</span>
        <span class="frag_offset"><span class="flags reserved dont_frag more_frag">${get_bytes_spaced(hexData, 20, 20)}</span>${get_bytes_spaced(hexData, 21, 21)}</span>
        <span class="ttl">${get_bytes_spaced(hexData, 22, 22)}</span>
        <span class="protocol">${get_bytes_spaced(hexData, 23, 23)}</span>
        <span class="ip_checksum">${get_bytes_spaced(hexData, 24, 25)}</span>
        <span class="source_addr">${get_bytes_spaced(hexData, 26, 29)}</span>
        <span class="dest_addr">${get_bytes_spaced(hexData, 30, 33)}</span>
        `
}

function createUDPHexData(hexData) {
    return `
        <span class="src_port">${get_bytes_spaced(hexData, 34, 35)}</span>
        <span class="src_port">${get_bytes_spaced(hexData, 36, 37)}</span>
        <span class="length">${get_bytes_spaced(hexData, 38, 39)}</span>
        <span class="checksum">${get_bytes_spaced(hexData, 40, 41)}</span>
        <span class="payload">${get_bytes_spaced(hexData, 42, "end")}</span>
        `
}

function createHexData(hexData, packet_type) {
    if (packet_type == "UDP") {
        const hex_data = `
        <div>
            ${createEtherHexData(hexData)}        
            ${createIPHexData(hexData)}        
            ${createUDPHexData(hexData)}        
        </div>
        `
        return hex_data;
    }
}

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
    if (start == 6) 
        console.log("WHYYYY", out);
    console.log("result", out)
    return out;
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