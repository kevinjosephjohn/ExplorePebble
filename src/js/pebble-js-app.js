var places = {
  url : "https://api.foursquare.com/v2/venues/explore?",
  cid : "client_id=",
  secret : "client_secret=",
  section : "section=",
  ll : "ll="
  
};

// Foursquare API details go here
var keys = {
  client_id : "XXXX",
  client_secret : "XXXX"
  
};


var data={};

function success(e){
  console.log(
              "[JAVASCRIPT] Successfully delivered message with " + " transactionId=" + e.data.transactionId);
}

function fail(e) {
  console.log(
              "[JAVASCRIPT] Unable to deliver message with transactionId=" + e.data.transactionId + " Error is: " + e.error.message);
}



function search(code) {
  

  
  
  var lat, lng;
  navigator.geolocation.getCurrentPosition(function(position) {
    lat = position.coords.latitude;
    lng = position.coords.longitude;
    
    var url = places.url;
    
    url += places.section+code;
    url += "&" + places.ll + lat +"," + lng;  
    console.log("request:");
    //console.log(url + "&client_id=***&client_secret=***");
    url += "&" + places.cid + keys.client_id;
    url += "&" + places.secret + keys.client_secret;
    url += "&limit=10&v=20130815";
    console.log(url);
    
    var req = new XMLHttpRequest();
    req.open('GET', url, false);
    req.onload = function(e) {
      if (req.readyState == 4 && req.status == 200) {
        if(req.status == 200) {
          var x = JSON.parse(req.responseText);
          var results = x.response.groups[0].items;
          
          var len = Math.min(results.length, 10);
          for (var i = 0; i < len; i ++) {



            var name = results[i].venue.name;
            name=name.replace(/[^\x00-\x7F]/g, "");
            var rating = results[i].venue.rating;
            if(rating === undefined){
              rating="No rating available";
            }
            else{
              rating="Rating "+rating+"/10";
            }
            var distance=results[i].venue.location.distance/1000;
            var lat=results[i].venue.location.lat;
            var lng=results[i].venue.location.lng;
            
            distance=distance.toString()+" kms away";

            var body=name+"\n"+distance;
            var raw_address=results[i].venue.location.formattedAddress;
            
            for ( var j=0;j<raw_address.length;j++){
              body+="\n";
              body+=raw_address[j];
            }
            key=i.toString();
            data[key]=name+"|"+rating+"|"+body+"|"+lat+"|"+lng;
            console.log(data[key]);

            

          }
      //console.log(JSON.stringify(data));

      
      Pebble.sendAppMessage(data,success,fail);

    } 
    else {
     console.log("Error"); 
   }
 }
}
req.send(null);


});
}

//



Pebble.addEventListener("ready", function(e) {
  console.log(
              "[JAVASCRIPT] Ready event fired.");
} );



Pebble.addEventListener("appmessage", function(e) {
  if(e.payload.destination=="PebbleKitJS"){
    var code=e.payload.code;

    console.log("[JAVASCRIPT] ---code: " + e.payload.code); 
    search(code);
  }
  else{
    console.log("[JAVASCRIPT] for PebbleKitAndroid"); 
    console.log("[JAVASCRIPT] ---lat: " + e.payload.lat); 
    console.log("[JAVASCRIPT] ---lng: " + e.payload.lng); 
    Pebble.showSimpleNotificationOnPebble("Directions sent", "Check your phone for directions");




  }


  
});








