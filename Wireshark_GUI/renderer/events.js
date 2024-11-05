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
  // const tcpDataDiv = document.getElementById('tcp-data');
  // tcpDataDiv.textContent = data; // Update the content
});