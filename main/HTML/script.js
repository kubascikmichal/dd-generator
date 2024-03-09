function getMAC() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            console.log(this.responseText);
            var obj = JSON.parse(this.responseText);
            document.getElementById("MAC").innerText = obj.MAC;
        }
    };
    xhttp.open("GET", "getMAC", true);
    xhttp.send();
}

function setSettings(){
    var xhttp = new XMLHttpRequest();
        xhttp.open("POST", "setSettings", true);
        xhttp.setRequestHeader('Content-Type', 'application/json');
        xhttp.send(JSON.stringify({
            delay: document.getElementById("delay").value,
            minimum: document.getElementById("minimum").value,
            maximum: document.getElementById("maximum").value,
            step_size: document.getElementById("step").value,
            step_timeout: document.getElementById("step_timeout").value,
            interval: document.getElementById("interval").value,
            MAC: document.getElementById("detector").value,
            }));
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4) {
                var response = JSON.parse(this.responseText);
                console.log(response);
                if (response.status == 200) {
                    alert('Settings uploaded');
                } else {
                    alert('Settings are not uploaded');
                }
            }
        }
}

function getSettings() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "getSettings", true);
    xhttp.send();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4) {
            var settings = JSON.parse(this.responseText);
            console.log(settings);
            document.getElementById("delay").setAttribute('value', (settings.hasOwnProperty('delay') > 0 ? settings.delay : ""));
            document.getElementById("minimum").setAttribute('value', (settings.hasOwnProperty('minimum') > 0 ? settings.minimum : ""));
            document.getElementById("maximum").setAttribute('value', (settings.hasOwnProperty('maximum') > 0 ? settings.maximum : ""));
            document.getElementById("step").setAttribute('value', (settings.hasOwnProperty('step_size') > 0 ? settings.step_size : ""));
            document.getElementById("step_timeout").setAttribute('value', (settings.hasOwnProperty('step_timeout') > 0 ? settings.step_timeout : ""));
            document.getElementById("interval").setAttribute('value', (settings.hasOwnProperty('interval') > 0 ? settings.interval : ""));
            document.getElementById("detector").setAttribute('value', (settings.hasOwnProperty('MAC') > 0 ? settings.MAC : ""));
        }
    }
}

getMAC()
getSettings();