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

getMAC()