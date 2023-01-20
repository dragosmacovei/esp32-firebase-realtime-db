// Database Paths
var mode_state   = 'home/mode';

// Get a database reference 
const ref_mode_state    = database.ref(mode_state);

// Variables to save database current values
var mode_state = "Loading...";

// Attach an asynchronous callback to read the data
ref_mode_state.on('value', (snapshot) => {
    mode_state = snapshot.val();

    if(mode_state == "AUTO")
    {
        select_auto();
    }
    else if (mode_state == "MANUAL")
    {
        select_manual();
    }
    console.log('xxx' + mode_state);

}, (errorObject) => {
  console.log('Red: The read failed: ' + errorObject.name);
});












// ---------------------------------PRIZA------------------------------------------------

// Database Paths
var path                = 'home/plug';
const ref_mode_plug     = database.ref(path);
var tmp_state           = "Loading...";
var state;

document.getElementById("stare_priza_manual").innerHTML = tmp_state;

// Attach an asynchronous callback to read the data
ref_mode_plug.on('value', (snapshot) => {
    state = snapshot.val();
    document.getElementById("stare_priza_manual").innerHTML = state;
    console.log('PLUG state: ' + state);
}, (errorObject) => {
  console.log('Red: The read failed: ' + errorObject.name);
});
// ---------------------------------------------------------------------------------------------------



// --------------------------------LAMPA-----------------------------------------------------
// Database Paths
var path                = 'home/lamp';
const ref_mode_lamp     = database.ref(path);
var tmp_state           = "Loading...";
var state;

document.getElementById("stare_lampa_manual").innerHTML = tmp_state;

// Attach an asynchronous callback to read the data
ref_mode_lamp.on('value', (snapshot) => {
    state = snapshot.val();
    document.getElementById("stare_lampa_manual").innerHTML = state;
    console.log('LAMP state: ' + state);
}, (errorObject) => {
  console.log('Red: The read failed: ' + errorObject.name);
});

// ---------------------------------------------------------------------------------------------------


// --------------------------------LAST TIME VISIBLE-----------------------------------------------------
// Database Paths
var path                = 'home/pir';
const ref_mode_pir      = database.ref(path);
var tmp_state           = "Loading...";
var state;

document.getElementById("last_visible_move").innerHTML = tmp_state;

// Attach an asynchronous callback to read the data
ref_mode_pir.on('value', (snapshot) => {
    state = snapshot.val();
    document.getElementById("last_visible_move").innerHTML = Math.round(state/1000);
    console.log('last time visible state: ' + state);
}, (errorObject) => {
  console.log('Red: The read failed: ' + errorObject.name);
});

// ---------------------------------------------------------------------------------------------------


// --------------------------------STARE LAMPA PE AUTO-----------------------------------------------------
// Database Paths
var path                = 'home/auto/lamp';
const ref_mode_auto_lamp      = database.ref(path);
var tmp_state           = "Loading...";
var state;

document.getElementById("stare_lampa_auto").innerHTML = tmp_state;

// Attach an asynchronous callback to read the data
ref_mode_auto_lamp.on('value', (snapshot) => {
    state = snapshot.val();
    document.getElementById("stare_lampa_auto").innerHTML = state;
    console.log('auto lamp state: ' + state);
}, (errorObject) => {
  console.log('Red: The read failed: ' + errorObject.name);
});

// ---------------------------------------------------------------------------------------------------


// --------------------------------STARE PRIZA PE AUTO-----------------------------------------------------
// Database Paths
var path                = 'home/auto/plug';
const ref_mode_auto_plug      = database.ref(path);
var tmp_state           = "Loading...";
var state;

document.getElementById("stare_priza_auto").innerHTML = tmp_state;

// Attach an asynchronous callback to read the data
ref_mode_auto_plug.on('value', (snapshot) => {
    state = snapshot.val();
    document.getElementById("stare_priza_auto").innerHTML = state;
    console.log('auto plug state: ' + state);
}, (errorObject) => {
  console.log('Red: The read failed: ' + errorObject.name);
});

// ---------------------------------------------------------------------------------------------------





// Database Paths
var mode_state   = 'home/temp';

// Get a database reference 
const ref_mode_temperature    = database.ref(mode_state);

// Variables to save database current values
var mode_state = "Loading...";

// Attach an asynchronous callback to read the data
ref_mode_temperature.on('value', (snapshot) => {
    state = snapshot.val();

    document.getElementById("temperatura").innerHTML = state;

    console.log('temperatura value: ' + state);

}, (errorObject) => {
  console.log('Red: The read failed: ' + errorObject.name);
});


// ---------------------------------------------------------------------------------------------------

// Database Paths
var mode_state   = 'home/hum';

// Get a database reference 
const ref_mode_hum    = database.ref(mode_state);

// Variables to save database current values
var mode_state = "Loading...";

// Attach an asynchronous callback to read the data
ref_mode_hum.on('value', (snapshot) => {
    state = snapshot.val();

    document.getElementById("umiditatea").innerHTML = state;

    console.log('umiditatea value: ' + state);

}, (errorObject) => {
  console.log('Red: The read failed: ' + errorObject.name);
});

// ---------------------------------------------------------------------------------------------------
