// function toggleSubItems(event, element) {
//     event.stopPropagation();
//     const subItems = element.querySelector('.sub-items');
//     console.log(subItems.innerText)
//     if (subItems.classList.contains('expanded')) {
//         subItems.classList.remove('expanded');
//         subItems.offsetHeight;
//         // 
//     } else {
//         subItems.classList.add('expanded');
//     }
// }
const { ipcRenderer } = require('electron');

let packet_num = 1;
const start_time = Date.now();
function toggleSubItems(event, element) {
  event.stopPropagation();

  const subItems = element.querySelector('.sub-items');
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



// Listen for the tcp-data event from the main process
ipcRenderer.on('tcp-data', (event, data) => {
  console.log("THE TCP DATA IS ", data);

  let packet_info = data['packet_info'];
  let new_row = `<tr>
                  <td>${packet_num}</td>
                  <td>${start_time - parseInt(packet_info.time)}</td>
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