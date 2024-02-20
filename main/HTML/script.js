function actualizeData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            console.log(this.responseText);
            var obj = JSON.parse(this.responseText);
            document.getElementById("actual").innerText = parseFloat(obj.actualDB).toFixed(2);
        }
    };
    xhttp.open("GET", "getActualDB", true);
    xhttp.send();
}

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

function getLast() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            console.log(this.responseText);
            var obj = JSON.parse(this.responseText);
            document.getElementById("last").innerText = obj.last;
        }
    };
    xhttp.open("GET", "getLast", true);
    xhttp.send();
}

function getTotal() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            console.log(this.responseText);
            var obj = JSON.parse(this.responseText);
            document.getElementById("totaly").innerText = obj.total;
        }
    };
    xhttp.open("GET", "getTotal", true);
    xhttp.send();
}

getMAC()
getLast()
getTotal()
setInterval(actualizeData, 500);