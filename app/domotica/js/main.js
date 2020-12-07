
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

  var exterior2 = firebase.database().ref(casa).child('exterior2');
  var exterior3 = firebase.database().ref(casa).child('exterior3');
  
  var alarma = firebase.database().ref(casa).child('alarma');
  var movimiento = firebase.database().ref(casa).child('movimiento');
  var temperatura = firebase.database().ref(casa).child('temperatura');
  var humedad = firebase.database().ref(casa).child('humedad');
  var rgbLed = firebase.database().ref(casa).child('rgb');
  var gasSensor = firebase.database().ref(casa).child('gasEstado');
  var sala = firebase.database().ref(casa).child('sala');

  var tv = firebase.database().ref(casa).child('tv');
  var netflix = firebase.database().ref(casa).child('netflix');
  var youtube = firebase.database().ref(casa).child('youtube');

  var temFin;
  var humFin;
  var gasStatus;
  
  var colorRGB; // color leds
  
  var parpadeo;
  


  function detectaMov(){

       $('.mov').text(' Moviemiento!!');
       $(".mov").fadeTo(500, .1)
                .fadeTo(500, 1);
       $(".movImg").fadeTo(500, .1)
                .fadeTo(500, 1);

  }

$(document).ready(function(){

  // setTimeout(cargaColor, 1000); // carga la paleta de colores depues de 1 segundo

    exterior2.on("value", function(snapshot){
          var valor = snapshot.val(); 
          console.log(valor); 
        
          if(valor != 'true'){
              $('#exterior2').prop('checked', false);
          }else{
              $('#exterior2').prop('checked', true);
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



    alarma.on("value", function(snapshot){
          var valor = snapshot.val(); 
          console.log(valor); 
        
          if(valor != 'on'){
              console.log('sensor activado');
              $('#sensorMovimiento').prop('checked', false);
          }else{
              db.ref(casa).update({ movimiento: "off"});
              console.log('sensor desactivado');
              $('#sensorMovimiento').prop('checked', true);
          }
                
     });

    movimiento.on("value", function(snapshot){
          var valor = snapshot.val(); 
          console.log(valor); 
        
          if(valor == 'on'){
            console.log('se detecta Moviemiento');
            parpadeo =  setTimeout(detectaMov, 1000); // le damos un delay para que alcance a cargar el ultimo valor en fire
          }else{
            clearInterval(parpadeo); // detenido
            $('.mov').text(' Movimiento');
              
          }                
     });


    

    temperatura.on("value", function(snapshot){
          var tem = snapshot.val(); 
          temFin = snapshot.val(); 

          $("#grados").gaugeMeter({percent:tem});
     });


    humedad.on("value", function(snapshot){
          var hum = snapshot.val(); 
          humFin = snapshot.val(); 

          $("#humedadMarca").gaugeMeter({percent:hum});
                  
     });


    rgbLed.on("value", function(snapshot){
                            
          colorRGB = snapshot.val(); 
          console.log(colorRGB)

     });


    sala.on("value", function(snapshot){
                            
          dim = snapshot.val(); 
          $('#dimvalue').val(dim); // acrualiza el slider 

     });



    gasSensor.on("value", function(snapshot){
          var valor = snapshot.val(); 
          console.log(valor)
          gasStatus = valor;
          $(".gasStatus").text("Gas detectado");

          if(valor == 'G' ){
             $(".star").hide();
             $(".gasStatus").text("Gas");
             }
          
          if(valor == 'P'){
            $(".star").show();
            responsiveVoice.speak('Peligro! altos grados de gas detectados', "Spanish Latin American Female");
          }

          if(valor == 'D'){
            $(".star").show();
            responsiveVoice.speak('Cuidado, Gas detectado', "Spanish Latin American Female");
          }
                
     });

    tv.on("value", function(snapshot){
          var valor = snapshot.val(); 
          console.log(valor); 
        
          if(valor != 'on'){
              $('#tv').prop('checked', false);
          }else{
              $('#tv').prop('checked', true);
          }
                
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

         if(sensor == true){
            db.ref(casa).update({ alarma: "on"});
         }else{
            db.ref(casa).update({ alarma: "off"});
         }

         if(tv == true){
            db.ref(casa).update({ tv: "on"});
         }else{
            db.ref(casa).update({ tv: "off"});
         }

    });


    $('#aromatizante').on('click',function(){

        db.ref(casa).update({ aromatizante: "on"});

      });


    $('#netflix').on('click',function(){

        db.ref(casa).update({ netflix: "on"});

      });

    $('#youtube').on('click',function(){

        db.ref(casa).update({ youtube: "on"});

      });






$(".GaugeMeter").gaugeMeter();

// selector de colores
    

    // function cargaColor(){
    //       var example = new iro.ColorPicker(".wrapper", {
                   
    //                 width: 320,
    //                 height: 320,
    //                 color: colorRGB,
    //                 anticlockwise: true,
    //                 borderWidth: 1,
    //                 borderColor: "#fff",
    //                  });


    //       example.on("color:change", function (color) {
                      
    //                 //console.log(color.rgbString)
    //                 rgbFire = color.rgb;
    //                 db.ref(casa).update({ rgb: rgbFire });

    //        });




 
// end selector colores


   /* preloader
    * -------------------------------------------------- */
    var clPreloader = function() {
        
        $("html").addClass('cl-preload');
        $('#preloader').hide();
        
    };



    clPreloader();


    }


// asistente    


  $('#dimvalue').on("input", function(e) {
          //console.log($("#dimvalue").val());
          valor = $("#dimvalue").val();
          db.ref(casa).update({ sala: parseInt(valor) });
          
          
  })
//valores
//$('#focoEntrada').prop('checked', true);
//$('#focoEntrada').prop('checked', false);

});
