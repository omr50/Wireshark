const { ipcRenderer } = require('electron');

let packet_num = 1;
let packets_info = [];

const start_time = Date.now();
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


function renderDetailedInfo(packet_num, element) {
  console.log(packets_info) 
  let detailedPacket = packets_info[packet_num]["detailed_info"][0];
  // find the element where we can render the detailed packet info
  let detailedProtocolElement = document.querySelector(".left-protocol-info");
  // for every object set up a drop down.
  // for simple string just display it in p tag
  // later do a for each loop because the detailed packet will be an array of objects
  // each object containing the layer
  let list = `
  <div class="packet_details_container">
      <div class="item" onclick="toggleSubItems(event, this)">
        <img class="dropdown" src="./dropdown.png" width="14px" height="14px">
        ${detailedPacket["title"]} 
        <div class="sub-items">
            <div class="item" onclick="toggleSubItems(event, this)">
                &emsp;&emsp;&emsp;<img class="dropdown" src="./dropdown.png" width="14px" height="14px">
                ${detailedPacket["source"]["title"]} 
                <div class="sub-items">
                    <div>
                        &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;${detailedPacket["source"]["LG"]} 
                    </div>
                    <div>
                        &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;${detailedPacket["source"]["IG"]} 
                    </div>
                </div>
            </div>

            <div class="item" onclick="toggleSubItems(event, this)">
                &emsp;&emsp;&emsp;<img class="dropdown" src="./dropdown.png" width="14px" height="14px">
                ${detailedPacket["Destination"]["title"]} 
                <div class="sub-items">
                    <div>
                        &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;${detailedPacket["Destination"]["LG"]} 
                    </div>
                    <div>
                        &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;${detailedPacket["Destination"]["IG"]} 
                    </div>
                </div>
            </div>

            <div class="item" onclick="toggleSubItems(event, this)">
                &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;${detailedPacket["type"]} 
            </div>
        </div>
    </div> 
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