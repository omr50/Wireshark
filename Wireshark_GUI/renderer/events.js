const { ipcRenderer } = require('electron');

let packet_num = 1;
let packets_info = [];

const start_time = Date.now();
let small_padding = "&emsp;&emsp;&emsp;";
let padding = "&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;";
console.log("CURRENT START TIME: ", start_time);

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
        console.log("True")
    } else {
        element.style.backgroundColor = "#FFAFAF";
        console.log("False")
        return;
    }

    console.log(searchQuery)
    // filter through every tr, if it contains search query get it
    const elements = document.getElementsByTagName('tr');
    for (element of elements) {
        console.log("CHecking element");
       if (!element.innerText.toLowerCase().includes(searchQuery)) {
            console.log("Diddn't work")
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


function renderDetailedInfo(packet_num, element) {
  console.log(packets_info) 
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
  </div>
  `
  detailedProtocolElement.innerHTML = "";
  detailedProtocolElement.innerHTML += list;
}



// Listen for the tcp-data event from the main process
ipcRenderer.on('tcp-data', (event, data) => {
  console.log("THE TCP DATA IS ", data);
  // test
  let packet_info = data['packet_info'];
  packets_info.push(data)
  let color_class = packet_info.protocol.toLowerCase(); 
  let exact_time = parseInt(packet_info.time) - start_time;
  let seconds = exact_time / 1000;
  let new_row = `<tr class="${color_class}" onclick="renderDetailedInfo(${packet_num-1}, this)">
                  <td>${packet_num}</td>
                  <td>${seconds}</td>
                  <td>${packet_info.source}</td>
                  <td>${packet_info.destination}</td>
                  <td>${packet_info.protocol}</td>
                  <td>${packet_info.length}</td>
                  <td>${packet_info.info}</td>
                </tr>`
  packet_num++;

  let packet_table = document.querySelector(".packet_table");
  packet_table.innerHTML += new_row;
  search(null, document.querySelector(".search-bar"));
  // const tcpDataDiv = document.getElementById('tcp-data');
  // tcpDataDiv.textContent = data; // Update the content
});