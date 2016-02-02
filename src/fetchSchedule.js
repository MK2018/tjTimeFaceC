var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

/*function locationSuccess(pos) {
  // Construct URL
  //console.log(pos.coords.latitude + ", " + pos.coords.longitude);
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude;

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      // Temperature in Kelvin requires adjustment
      //var temperature = Math.round(json.main.temp - 273.15);
      //console.log("Temperature is " + temperature);

      // Conditions
      var conditions = json.weather[0].main;      
      console.log("Conditions are " + conditions);
      
      // Assemble dictionary using our keys
      var dictionary = {
        "PD_DATA": conditions,
        //"KEY_CONDITIONS": conditions,
      };
      console.log(dictionary);

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Weather info sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending weather info to Pebble!");
        }
      );
    }      
  );
}

function locationError(err) {
  console.log("Error requesting location!");
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}*/

function getMinSinceMidnight(d) {
  var e = new Date(d);
  return ((d - e.setHours(0,0,0,0))/1000)/60;
}

function analyzeTime(jsonObj){
  var date = new Date();
  if(date.getDay()==6||date.getDay()===0)
    return "Happy weekend!";
  for(var i = 1; i <= parseInt(jsonObj[0]); i++){
    var currMin = parseInt(getMinSinceMidnight(new Date()));
    var startMin = parseInt(JSON.stringify(jsonObj[i].startInt));
    var endMin = parseInt(JSON.stringify(jsonObj[i].endInt));
    //console.log("Current time: " + parseInt(getMinSinceMidnight(new Date())));
    //console.log("start time: " + parseInt(JSON.stringify(jsonObj[i].startInt)));
    if(i==1&&currMin<startMin)
      return "School starts at " + JSON.stringify(jsonObj[i].startForm);
    else if(endMin>currMin&&startMin<=currMin)
        return JSON.stringify(jsonObj[i].name)+"\n"+JSON.stringify(jsonObj[i].startForm)+ " - " +JSON.stringify(jsonObj[i].endForm); 
    else if(parseInt(JSON.stringify(jsonObj[i-1].endInt))<=currMin&&startMin>currMin)
        return JSON.stringify(jsonObj[i].name)+" starts at "+ JSON.stringify(jsonObj[i].startForm);
  }
  return "No class right now. Enjoy!";
}

function fetchSchedule(){
  var url = "http://www.tjtimelib.appspot.com";
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var jsonObj = JSON.parse(responseText);
      //var jsonTxt = JSON.stringify(jsonObj);
      //var tmpSend = JSON.stringify(jsonObj[1].name)+"\n"+JSON.stringify(jsonObj[1].startForm)+ " - " +JSON.stringify(jsonObj[1].endForm);
      //tmpSend = tmpSend.replace(/\"/g, "");
      //console.log(jsonTxt);
      //console.log("array length" + jsonObj.length);
      //console.log(JSON.stringify(jsonObj[0].start));
      var pdData = analyzeTime(jsonObj);
      pdData = pdData.replace(/\"/g, "");
      console.log(pdData);
      var dictionary = {
        "PD_DATA": pdData,
      };
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Schedule info sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending schedule info to Pebble!");
        }
      );
      
    }      
  );
}
// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");
    fetchSchedule();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    fetchSchedule();
  }                     
);
