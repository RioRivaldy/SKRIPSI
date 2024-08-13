function getData(location) {
    fetch("https://data.bmkg.go.id/DataMKG/MEWS/DigitalForecast/DigitalForecast-JawaBarat.xml")
        .then(response => response.text())
        .then(str => (new window.DOMParser()).parseFromString(str, "text/xml"))
        .then(data => {
            var timeArray = [];
            var temArray = [];
            var humArray = [];
            var winddirArray = [];
            var windspeedArray = [];
            var weatherArray = [];

            var pathTem = `data/forecast/area[@description="${location}"]/parameter[@id="t"]/timerange/value[@unit="C"]`;
            var pathHum = `data/forecast/area[@description="${location}"]/parameter[@id="hu"]/timerange/value`;
            var pathWeather = `data/forecast/area[@description="${location}"]/parameter[@id="weather"]/timerange/value`;
            var pathTime = `data/forecast/area[@description="${location}"]/parameter[@id="hu"]/timerange`;
            var pathWD = `data/forecast/area[@description="${location}"]/parameter[@id="wd"]/timerange/value[@unit="deg"]`;
            var pathWS = `data/forecast/area[@description="${location}"]/parameter[@id="ws"]/timerange/value[@unit="MS"]`;

            if (data.evaluate) {
                var i = 0;
                var nodeT = data.evaluate(pathTem, data, null, XPathResult.ANY_TYPE, null);
                var nodeH = data.evaluate(pathHum, data, null, XPathResult.ANY_TYPE, null);
                var nodeWD = data.evaluate(pathWD, data, null, XPathResult.ANY_TYPE, null);
                var nodeWS = data.evaluate(pathWS, data, null, XPathResult.ANY_TYPE, null);
                var nodeW = data.evaluate(pathWeather, data, null, XPathResult.ANY_TYPE, null);
                var nodeTime = data.evaluate(pathTime, data, null, XPathResult.ANY_TYPE, null);
                var resultT = nodeT.iterateNext();
                var resultH = nodeH.iterateNext();
                var resultWD = nodeWD.iterateNext();
                var resultWS = nodeWS.iterateNext();
                var resultW = nodeW.iterateNext();
                var resultTime = nodeTime.iterateNext();

                while (resultT && resultH && resultWD && resultWS && resultW && resultTime) {
                    timeArray[i] = resultTime.getAttributeNode("datetime").nodeValue;
                    temArray[i] = resultT.childNodes[0].nodeValue;
                    humArray[i] = resultH.childNodes[0].nodeValue;
                    winddirArray[i] = resultWD.childNodes[0].nodeValue;
                    windspeedArray[i] = Math.floor(resultWS.childNodes[0].nodeValue);
                    weatherArray[i] = resultW.childNodes[0].nodeValue;

                    resultT = nodeT.iterateNext();
                    resultH = nodeH.iterateNext();
                    resultWD = nodeWD.iterateNext();
                    resultWS = nodeWS.iterateNext();
                    resultW = nodeW.iterateNext();
                    resultTime = nodeTime.iterateNext();
                    i++;
                }
            }

            var body = "";
            var datesDisplayed = {};

            var formatDate = datetime => {
                var year = datetime.substring(0, 4);
                var month = datetime.substring(4, 6);
                var day = datetime.substring(6, 8);
                return `${day}-${month}-${year}`;
            };

            for (var i = 0; i < timeArray.length; i++) {
                var date = timeArray[i].substring(0, 8);
                var hour = timeArray[i][8].toString() + timeArray[i][9].toString();

                if (!datesDisplayed[date]) {
                    if (i !== 0) body += `</div>`; // Close the previous date's card-grid if it exists
                    body += `<div class="date-header"><h2>${formatDate(date)}</h2></div><div class="card-grid">`;
                    datesDisplayed[date] = true;
                }

                body += `
                    <div class="card-1">
                        <div>
                            <h3>${hour}.00</h3>
                            <img src="icons/w_${parseInt(weatherArray[i])}.png" alt="Weather Icon">
                        </div>
                        <div>
                            <p><i class="fas fa-temperature-high"></i> ${temArray[i]}<sup>o</sup>C</p>
                            <p><i class="fas fa-tint"></i> ${humArray[i]}%</p>
                            <p><i class="fas fa-location-arrow"></i> ${winddirArray[i]}<sup>o</sup></p>
                            <p><i class="fas fa-wind"></i> ${windspeedArray[i]} m/s</p>
                        </div>
                    </div>
                `;
            }
            if (datesDisplayed) body += `</div>`; // Close the last date's card-grid

            document.getElementById(`data${location}`).innerHTML = body;
        });
}

function weatherPanel(locations) {
    var body = "";

    locations.forEach(location => {
        body += `
            <div class="location-container">
                <div class="header">${location}</div>
                <div id="data${location}"></div>
            </div>
        `;
    });

    document.getElementById("main").innerHTML = `<div class="main-container">${body}</div>`;

    locations.forEach(location => {
        getData(location);
    });
}

// Menampilkan panel untuk 3 wilayah default
weatherPanel(["Depok", "Bekasi", "Kota Bogor"]);

// Event listener untuk menu dropdown
document.getElementById("viewSelect").addEventListener("change", function () {
    var selectedView = this.value;
    if (selectedView === "default") {
        weatherPanel(["Depok", "Bekasi", "Kota Bogor"]);
    } else {
        weatherPanel([selectedView]);
    }
});
