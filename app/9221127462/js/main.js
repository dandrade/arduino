
   var config = {
    apiKey: "AIzaSyDUnCTzDnmGdjuI5BrC9ygQzZPYmzpbVqM",
    authDomain: "domotica-cc7a7.firebaseapp.com",
    databaseURL: "https://domotica-cc7a7-default-rtdb.firebaseio.com",
    projectId: "domotica-cc7a7",
    storageBucket: "domotica-cc7a7.appspot.com",
    messagingSenderId: "337549165249",
    appId: "1:337549165249:web:e0512b4a8e6e8111be0a40"
  };
  


  firebase.initializeApp(config);
  var db = firebase.database();

  var casa = 'senderos';

  var foco1 = firebase.database().ref(casa).child('foco1');
  var sala = firebase.database().ref(casa).child('sala');

 
$(document).ready(function(){

    foco1.on("value", function(snapshot){
          var valor = snapshot.val(); 
          console.log(valor); 
        
          if(valor != 'on'){
              $('#foco1').prop('checked', false);
          }else{
              $('#foco1').prop('checked', true);
          }
                
     });


    foco2.on("value", function(snapshot){
          var valor = snapshot.val(); 
          console.log(valor); 
        
          if(valor != 'on'){
              $('#foco2').prop('checked', false);
          }else{
              $('#foco2').prop('checked', true);
          }
                
     });


    sala.on("value", function(snapshot){
                            
          dim = snapshot.val(); 
          $('#dimvalue').val(dim); // acrualiza el slider 

     });



    $("#foco1 , #foco2 , #sensorMovimiento , #tv ").on( 'change', function() {

        var foco1 = $('#foco1').is(':checked');
        var foco2 = $('#foco2').is(':checked');
        var sensor = $('#sensorMovimiento').is(':checked');
        var tv = $('#tv').is(':checked');
          
         if(foco1 == true){
            db.ref(casa).update({ foco1: "on"});
         }else{
            db.ref(casa).update({ foco1: "off"});
         }

         if(foco2 == true){
            db.ref(casa).update({ foco2: "on"});
         }else{
            db.ref(casa).update({ foco2: "off"});
         }


    });


    }


// asistente    


  $('#dimvalue').on("input", function(e) {
          //console.log($("#dimvalue").val());
          valor = $("#dimvalue").val();
          db.ref(casa).update({ sala: parseInt(valor) });
          
          
  })

});
