var SerialPort = require("serialport");
var serialport = new SerialPort("COM3", {baudRate: 57600});
var fs = require("fs");

var counter = 0;
var imagedata = '';

serialport.on('open', function(){
  console.log('Serial Port Opend');
  serialport.on('data', function(chunk){
      imagedata += chunk;
    //   for(var i =0; i<chunk.length; i++){
    //     console.log(chunk[i]);
    //   }
      console.log(chunk);
  });
    send(); 
});

var test = '123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890';
var test2 = 0;
i=0;
 function send () {
    setTimeout(function() { 
        serialport.write(i.toString()+'                                                      \n', function(err) {
            if (err) {
                return console.log('Error on write: ', err.message);
                }
            console.log('message written');
            if(i<1000){
                i++;
                send();
            }
                
        });
    }, 300);
}

//saveImage("image.jpg", [0,43,0,43,0,43,0,43]);


// function saveImage(filename, data){
//   var myBuffer = new Buffer(data.length);
//   for (var i = 0; i < data.length; i++) {
//   var myBuffer = new Buffer(data.length);
//   for (var i = 0; i < data.length; i++) {
//       myBuffer[i] = data[i];
//   }
//   fs.writeFile(filename, myBuffer, function(err) {
//       if(err) {
//           console.log(err);
//       } else {
//           console.log("The file was saved!");
//       }
//   });
// }
