



const COUNTDOWN_CONTANIER = document.getElementById("countdown-timer-container");
const COUNTDOWN_TIMER = document.getElementById("countdown-timer");
const STATUS_CONTAINER = document.getElementById("status-container");
const LAUNCH_TIME_TAG = document.getElementById("launchedTime");

let hasStartedCountdown = false;
let firstRun = true
let startTime = 0; 

async function getStatus() {
    try {
        const response = await fetch('http://192.168.10.19:5000//status');
        if (!response.ok) {
          throw new Error(`HTTP error! Status: ${response.status}`);
        }
        const data = await response.json();
        return data


      } catch (error) {
        console.error('There was a problem fetching the data: ', error);
      }
    }

const updateStatus = async () => {
    const fullJsonStatus = await getStatus();
    const status = fullJsonStatus["status"];

    if (firstRun == true){
        startTime = fullJsonStatus["countDownTime"] // TAKES TWO SECOND TO START THE COUNTDOWN
        firstRun = false
    }

    // UPDATES THE TIMER
    hasStartedCountdown = fullJsonStatus["launch"];

    // UPDATES THE LAUNCH TIME
    if ("launchTime" in fullJsonStatus){
        LAUNCH_TIME_TAG.style.display="block"
        LAUNCH_TIME_TAG.innerHTML = "launchtime: " + fullJsonStatus["launchTime"] + " min";
    }else {
        LAUNCH_TIME_TAG.style.display = "none";
    };




    // UPDATES THE STATUS CODES
    let innerHtml_ = ""
    for (let key of Object.keys(status)){
        console.log(status[key]);
        if (status[key] === true){
            innerHtml_ += "<p class='success'>" + key + ": Ok" + "</p>";
        }else if (status[key] === false){
            innerHtml_ += "<p class='error'>" + key + ": Error" + "</p>";
        }

    }

    STATUS_CONTAINER.innerHTML = innerHtml_;


}



const updateTimer = () => {

    if (hasStartedCountdown == true){
        COUNTDOWN_CONTANIER.style.display = "block"


        let minutes = parseInt((startTime - (700%60))/60).toString();
        let seconds = parseInt((startTime%60)).toString();

        console.log(minutes, seconds)

        if (minutes.toString().length == 1){
            minutes = "0" + minutes.toString()
        }

        if (seconds.toString().length == 1){
            seconds = "0" + seconds.toString()
        }

        COUNTDOWN_TIMER.innerText = minutes + ":" + seconds;

        if (startTime <= 10 && startTime != 0){ // SO IT DOSENT PLAY SOUNDS ON REPEAT
            COUNTDOWN_TIMER.style.color = "red";
            var sound = document.getElementById('mySoundEffect');
            sound.play();
        }

        if (startTime > 0){
            startTime--;
        }
    }

}


updateStatus();
setInterval(updateTimer, 1000)
setInterval(updateStatus, 1000);
