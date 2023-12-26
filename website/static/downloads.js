


const VIDEO_TABLE_CONTENTS = document.getElementById("video-table-contents");

const URL_ = "http://192.168.10.188:5000//"
const VIDEO_BUTTONS = "<td><button class='download-btn'>Download</button></td><td><button class='delete-btn'>Delete</button></td>"



async function getStatus(url) {
    try {
        const response = await fetch(url);
        if (!response.ok) {
          throw new Error(`HTTP error! Status: ${response.status}`);
        }
        const data = await response.json();
        return data

      } catch (error) {
        console.error('There was a problem fetching the data: ', error);
      }
    }



const videoAction = async (name, action) =>{
    if (action === 'download') {
        // Create a link and set the href to the download URL
        const link = document.createElement('a');
        link.href = URL_ + "editVideos/" + name + "/" + action;
        link.download = name;

        // Append the link to the body and click it
        document.body.appendChild(link);
        link.click();
        // Remove the link after clicking
        document.body.removeChild(link);
    
    } else {
        // For non-download actions, just call getStatus
        const _ = await getStatus(URL_ + "editVideos/" + name + "/" + action);
	updateTable();
    }
}

const updateTable = async () => {
    const videoStatus = await getStatus(URL_ + 'downloadPaths');
    let tableHtml = "";

    for (video of videoStatus){
        videoName = video[0];
        videoSize = video[1];
        tableHtml += "<tr><td>" + videoName + "</td><td>" + videoSize + " MB</td>"; // ADDS THE CONTENTS
        tableHtml += "<td><button class='download-btn' onclick='videoAction(\"" + videoName  + "\",\"" + "download" + "\")'>Download</button></td>";
        tableHtml += "<td><button class='delete-btn' onclick='videoAction(\"" + videoName  + "\",\"" + "delete" + "\")'>Delete</button></td></tr>";
    }
    VIDEO_TABLE_CONTENTS.innerHTML = tableHtml;

};

updateTable();
setInterval(updateTable, 5000);
