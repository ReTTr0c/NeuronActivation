const build = false; //True if doing build, False if running via 'neu run'



await Neutralino.init();
let path;
let pathN0;
let pathN1;
let pathN2;
let pathN3;
let pathN4;
let simPath;
let jsonPath;

let nCounter = 0;
let nCounter_F = "0";
let placeSelect = "None"; //placeSelect is which button is selected
let synapseSelectType = "Mid";
let placeSelectStore = "";

let neurons = []; //to select neurons
let neuronsSynapses = new Array(); //multidimensional. [2, 5, 8, 3, 9] 2 is neuron, rest is to where its connected
let neuronCoordsX = []; 
let neuronCoordsY = [];
let neuronSelect = 0;
//I'm not sure if using '[]' or 'new Array()' makes a difference but it works so I'm not touching it.

let Nselect1 = 0;
let Nselect1prev = 0;
let Nselect1prevprev = 0;
let Nselect2 = 0;

let neuronStim = [];

//need these to stop 'synapse' from getting called twice for some reason
let antiRecurseCount = 0; 
let synapseDraw = false;
let simRun = true;


let time = 1000;
let time_F = "1000";
let refractoryDelay = 3;
let refractoryDelay_F = "3";

if(build == true){
path = await Neutralino.filesystem.getAbsolutePath('./'); //Get full windows path to folder
pathN0 = (`${path}/N0.png`); 
pathN1 = (`${path}/N1.png`); 
pathN2 = (`${path}/N2.png`); 
pathN3 = (`${path}/N3.png`); 
pathN4 = (`${path}/N4.png`); 
simPath = (`${path}/test.exe`); 
jsonPath = (`${path}/simData.json`); 
}
else{
path = await Neutralino.filesystem.getAbsolutePath('./resources'); //Get full windows path to folder
pathN0 = (`${path}/N0.png`); 
pathN1 = (`${path}/N1.png`); 
pathN2 = (`${path}/N2.png`); 
pathN3 = (`${path}/N3.png`); 
pathN4 = (`${path}/N4.png`); 
simPath = (`${path}/sim.exe`); 

jsonPath = (`${path}/simData.json`); 
}
console.log(`path: ${path}`);
console.log(`Full: ${pathN0}`);
console.log(`Jsonpath: ${jsonPath}`)


document.getElementById("debugButton").onclick = async function dbg(){
  for(let i = 0; i <= neurons.length-1; i++){
    console.log(`Neuron ${i} ID = ${neurons[i]}`)
  }
  console.log("nSynapse array len: ", neuronsSynapses.length);
  for(let i = 0; i <= neuronsSynapses.length-1; i++){
    console.log(`Neuron synapse ${i}:`, neuronsSynapses[i]);
  }
  console.table(neuronsSynapses);
  for(let i = 0; i <= neuronStim.length-1; i++){
    console.log(`Neuron stimulation ${i}:`, neuronStim[i]);
  }
}
document.getElementById("clearCanvas").onclick = async function(){
  console.log("Reload")
  await Neutralino.app.restartProcess();
}
document.getElementById("selectNeuron").onclick = async function(){
  placeSelect = "Neuron"; //placeSelect is which button is selected
  console.log(`placeSelect = ${placeSelect}`);
}

document.getElementById("selectSynapseLight").onclick = async function(){
  placeSelect = "Synapse";
  console.log(`placeSelect = ${placeSelect}`);
  let draw = true;
  synapseDraw = true;
  while(placeSelect = "Synapse"){

      let { x, y } = await new Promise(r =>
      maincanvas.addEventListener("mousedown", f => {
          const rect = maincanvas.getBoundingClientRect();
          r({ x: f.clientX - rect.left, y: f.clientY - rect.top });
      }, { once: true })
  );
      let { x2, y2 } = await new Promise(r =>
      maincanvas.addEventListener("mousedown", f => {
          const rect = maincanvas.getBoundingClientRect();
          r({ x2: f.clientX - rect.left, y2: f.clientY - rect.top });
      }, { once: true })
  );



    console.log(`Synapsedraw is ${synapseDraw} and placeselect is ${placeSelect}`);
  if(synapseDraw == true && placeSelect == "Synapse"){
      //Check whether user clicked on existing neuron
      console.log("Entered synapse func")
      for(let i = 0; i <= neurons.length-1; i++){
        if(x > neuronCoordsX[i]-30 && x < neuronCoordsX[i]+30 && y > neuronCoordsY[i]-30 && y < neuronCoordsY[i]+30){
          neuronSelect = i;
          break;
        }
      }

      for(let i = 0; i <= neurons.length-1; i++){
        if(x2 > neuronCoordsX[i]-30 && x2 < neuronCoordsX[i]+30 && y2 > neuronCoordsY[i]-30 && y2 < neuronCoordsY[i]+30){
          console.log(`Minor: neuron ${i} selected: ${x2}; ${y2}, N: ${neuronCoordsX[i]}, ${neuronCoordsY[i]}`)
          Nselect2 = i; 
          if(neuronsSynapses[neuronSelect].length < 5){
          //neuronsSynapses[neuronSelect] = [...new Set(neuronsSynapses)];
          neuronsSynapses[neuronSelect].push(Nselect2); //Add connection to array
          }
          else{
            alert("Max 4 connections per neuron");
            draw = false;
          }
          break;
        }
      }

    //draw line on maincanvas
    console.log("Draw is", draw);
    if(draw == true){
    var ctx = document.getElementById("maincanvas").getContext('2d');
    ctx.moveTo(neuronCoordsX[Nselect1prev], neuronCoordsY[Nselect1prev]) 
    ctx.lineTo(neuronCoordsX[Nselect2], neuronCoordsY[Nselect2]);
    console.log(`From ${neuronCoordsX[Nselect1prev]}, ${neuronCoordsY[Nselect1prev]} to ${neuronCoordsX[Nselect2]}, ${neuronCoordsY[Nselect2]}`)
    ctx.stroke();
    console.log("Remove");
    }

  }

  //Needed, otherwise it automatically changes placeSelect back to "Synapse" after placing 2 neurons
  //For some reason...
  //So this allows us to escape the loop:
  if(placeSelect != "Synapse"){ 
    break;
  }
  //The functionality of the whole synapse function depends on these 3 lines^
  //Even though it checks placeSelect in the while() loop
  //Idk anymore I'm just happy it works

  }

}
document.getElementById("selectSynapseMid").onclick = async function(){
  placeSelect = "Synapse";
  console.log(`placeSelect = ${placeSelect}`);
  let draw = true;
  synapseDraw = true;
  while(placeSelect = "Synapse"){

      let { x, y } = await new Promise(r =>
      maincanvas.addEventListener("mousedown", f => {
          const rect = maincanvas.getBoundingClientRect();
          r({ x: f.clientX - rect.left, y: f.clientY - rect.top });
      }, { once: true })
  );
      let { x2, y2 } = await new Promise(r =>
      maincanvas.addEventListener("mousedown", f => {
          const rect = maincanvas.getBoundingClientRect();
          r({ x2: f.clientX - rect.left, y2: f.clientY - rect.top });
      }, { once: true })
  );



    console.log(`Synapsedraw is ${synapseDraw} and placeselect is ${placeSelect}`);
  if(synapseDraw == true && placeSelect == "Synapse"){
      //Check whether user clicked on existing neuron
      console.log("Entered synapse func")
      for(let i = 0; i <= neurons.length-1; i++){
        if(x > neuronCoordsX[i]-30 && x < neuronCoordsX[i]+30 && y > neuronCoordsY[i]-30 && y < neuronCoordsY[i]+30){
          neuronSelect = i;
          break;
        }
      }

      for(let i = 0; i <= neurons.length-1; i++){
        if(x2 > neuronCoordsX[i]-30 && x2 < neuronCoordsX[i]+30 && y2 > neuronCoordsY[i]-30 && y2 < neuronCoordsY[i]+30){
          console.log(`Minor: neuron ${i} selected: ${x2}; ${y2}, N: ${neuronCoordsX[i]}, ${neuronCoordsY[i]}`)
          Nselect2 = i; 
          if(neuronsSynapses[neuronSelect].length < 5){
          //neuronsSynapses[neuronSelect] = [...new Set(neuronsSynapses)];
          neuronsSynapses[neuronSelect].push(Nselect2); //Add connection to array
          }
          else{
            alert("Max 4 connections per neuron");
            draw = false;
          }
          break;
        }
      }

    //draw line on maincanvas
    console.log("Draw is", draw);
    if(draw == true){
    var ctx = document.getElementById("maincanvas").getContext('2d');
    for(let i = 0; i <= 1; i++){ //Make it thicker by drawing multiple lines
      ctx.moveTo(neuronCoordsX[Nselect1prev]-i, neuronCoordsY[Nselect1prev]-i) 
      ctx.lineTo(neuronCoordsX[Nselect2]-i, neuronCoordsY[Nselect2]-i);
      ctx.stroke();
      ctx.moveTo(neuronCoordsX[Nselect1prev]+i, neuronCoordsY[Nselect1prev]+i) 
      ctx.lineTo(neuronCoordsX[Nselect2]+i, neuronCoordsY[Nselect2]+i);
      ctx.stroke();
    }
    console.log(`From ${neuronCoordsX[Nselect1prev]}, ${neuronCoordsY[Nselect1prev]} to ${neuronCoordsX[Nselect2]}, ${neuronCoordsY[Nselect2]}`)
    console.log("Remove");
    }

  }

  //Needed, otherwise it automatically changes placeSelect back to "Synapse" after placing 2 neurons
  //For some reason...
  //So this allows us to escape the loop:
  if(placeSelect != "Synapse"){ 
    break;
  }
  //The functionality of the whole synapse function depends on these 3 lines^
  //Even though it checks placeSelect in the while() loop
  //Idk anymore I'm just happy it works

  }

}
document.getElementById("selectSynapseHeavy").onclick = async function(){
  placeSelect = "Synapse";
  console.log(`placeSelect = ${placeSelect}`);
  let draw = true;
  synapseDraw = true;
  while(placeSelect = "Synapse"){

      let { x, y } = await new Promise(r =>
      maincanvas.addEventListener("mousedown", f => {
          const rect = maincanvas.getBoundingClientRect();
          r({ x: f.clientX - rect.left, y: f.clientY - rect.top });
      }, { once: true })
  );
      let { x2, y2 } = await new Promise(r =>
      maincanvas.addEventListener("mousedown", f => {
          const rect = maincanvas.getBoundingClientRect();
          r({ x2: f.clientX - rect.left, y2: f.clientY - rect.top });
      }, { once: true })
  );



    console.log(`Synapsedraw is ${synapseDraw} and placeselect is ${placeSelect}`);
  if(synapseDraw == true && placeSelect == "Synapse"){
      //Check whether user clicked on existing neuron
      console.log("Entered synapse func")
      for(let i = 0; i <= neurons.length-1; i++){
        if(x > neuronCoordsX[i]-30 && x < neuronCoordsX[i]+30 && y > neuronCoordsY[i]-30 && y < neuronCoordsY[i]+30){
          neuronSelect = i;
          break;
        }
      }

      for(let i = 0; i <= neurons.length-1; i++){
        if(x2 > neuronCoordsX[i]-30 && x2 < neuronCoordsX[i]+30 && y2 > neuronCoordsY[i]-30 && y2 < neuronCoordsY[i]+30){
          console.log(`Minor: neuron ${i} selected: ${x2}; ${y2}, N: ${neuronCoordsX[i]}, ${neuronCoordsY[i]}`)
          Nselect2 = i; 
          if(neuronsSynapses[neuronSelect].length < 5){
          //neuronsSynapses[neuronSelect] = [...new Set(neuronsSynapses)];
          neuronsSynapses[neuronSelect].push(Nselect2); //Add connection to array
          }
          else{
            alert("Max 4 connections per neuron");
            draw = false;
          }
          break;
        }
      }

    //draw line on maincanvas
    console.log("Draw is", draw);
    if(draw == true){
    var ctx = document.getElementById("maincanvas").getContext('2d');
    for(let i = 0; i <= 2; i++){ //Make it thicker by drawing multiple lines
      ctx.moveTo(neuronCoordsX[Nselect1prev]-i, neuronCoordsY[Nselect1prev]-i) 
      ctx.lineTo(neuronCoordsX[Nselect2]-i, neuronCoordsY[Nselect2]-i);
      ctx.stroke();
      ctx.moveTo(neuronCoordsX[Nselect1prev]+i, neuronCoordsY[Nselect1prev]+i) 
      ctx.lineTo(neuronCoordsX[Nselect2]+i, neuronCoordsY[Nselect2]+i);
      ctx.stroke();
    }

    console.log(`From ${neuronCoordsX[Nselect1prev]}, ${neuronCoordsY[Nselect1prev]} to ${neuronCoordsX[Nselect2]}, ${neuronCoordsY[Nselect2]}`)
    console.log("Remove");
    }

  }

  //Needed, otherwise it automatically changes placeSelect back to "Synapse" after placing 2 neurons
  //For some reason...
  //So this allows us to escape the loop:
  if(placeSelect != "Synapse"){ 
    break;
  }
  //The functionality of the whole synapse function depends on these 3 lines^
  //Even though it checks placeSelect in the while() loop
  //Idk anymore I'm just happy it works

  }

}
document.getElementById("selectStimulate").onclick = async function(){
  placeSelect = "Stimulate";
  console.log(`placeSelect = ${placeSelect}`);
  while(placeSelect == "Stimulate"){
      let { x, y } = await new Promise(r =>
        maincanvas.addEventListener("mousedown", f => {
            const rect = maincanvas.getBoundingClientRect();
            r({ x: f.clientX - rect.left, y: f.clientY - rect.top });
        }, { once: true })
    );

    let neuronSelectPrivate = -1;
    for(let i = 0; i <= neurons.length-1; i++){
        if(x > neuronCoordsX[i]-30 && x < neuronCoordsX[i]+30 && y > neuronCoordsY[i]-30 && y < neuronCoordsY[i]+30){
          neuronSelectPrivate = i;
          break;
      }
    }

    if(neuronSelectPrivate >= 0){
      neuronStim.push(neuronSelectPrivate);
      console.log(`Stimulation added at ID ${neuronStim.length-1} to Neuron ${neuronStim[neuronStim.length - 1]}`);
      neuronSelectPrivate = -1;
    }

    if(placeSelect != "Stimulate"){
      break;
    }
  }
  
}





maincanvas.addEventListener("mousedown", async e => {
  const rect = maincanvas.getBoundingClientRect();
  const x = e.clientX - rect.left;
  const y = e.clientY - rect.top;
  console.log("Main: Clicked at", x, y);


//Check whether user clicked on existing neuron
for(let i = 0; i <= neurons.length-1; i++){
  if(x > neuronCoordsX[i]-30 && x < neuronCoordsX[i]+30 && y > neuronCoordsY[i]-30 && y < neuronCoordsY[i]+30){
    Nselect1prevprev = Nselect1prev;
    Nselect1prev = Nselect1;
    Nselect1 = i;
    console.log(`Main: neuron ${i} selected: ${x}; ${y}, N: ${neuronCoordsX[i]}, ${neuronCoordsY[i]}. Prev: ${Nselect1prev}, Prevprev: ${Nselect1prevprev}`);
    break;
  }
}

if(placeSelect == "None"){ //placeSelect is which button is selected
alert("Please select option")
}
else if(placeSelect == "Neuron"){
  synapseDraw = false; //To prevent bugs with synapse connecting instead of neuron drawing
  if(nCounter < 10){ //check for maximum number of Neurons
    neurons[nCounter] = nCounter; //ads specific neuron number to available neurons array (lwky dont need this)
    neuronsSynapses.push([]); //add new row to synapses array.
    neuronsSynapses[nCounter][0]; //Make first element in that row the neuron ID (also dont need this tbh)
    neuronCoordsX[nCounter] = x; //add to array
    neuronCoordsY[nCounter] = y;
    nCounter++;
    console.log(`nCounter = ${nCounter}`);
    //display neuron img on canvas
    var ctx = document.getElementById("maincanvas").getContext('2d');
    var img = new Image();
    img.onload = function(){
      ctx.drawImage(img, x-35, y-35);
    };
    img.src = "/N0.png"
    placeSelect = "Neuron";
  }
  else{
    alert("Neuron limit reached (10)");
  }
}

});


//Timestep slider
const timeSlider = document.getElementById("setTime");
const timeDisplay = document.getElementById("timeDisplay");
timeSlider.value = time;
timeDisplay.textContent = time;
timeSlider.addEventListener("input", () => {
  time = Number(timeSlider.value);
  timeDisplay.textContent = `Simulation step timeout: ${time}ms`;
});

//Refractory period timer
const refSlider = document.getElementById("setRef");
const refDisplay = document.getElementById("refractoryDisplay");
refSlider.value = refractoryDelay;
refractoryDisplay.textContent = refractoryDelay;
refSlider.addEventListener("input", () => {
  refractoryDelay = Number(refSlider.value);
  refDisplay.textContent = `Refractory period: ${refractoryDelay} steps`;
});

function stringFormat(){
  //first convert numbers to strings
  nCounter_F = nCounter.toString();
  time_F = time.toString();
  refractoryDelay_F = refractoryDelay.toString();
  //Make sure nCounter always has two digits, and time always has 4 digits
  if(nCounter < 10){ nCounter_F = "0" + nCounter.toString();}
  if(time < 1000){time_F = "0" + time.toString()};
  console.log(`time: ${time}; time_F: ${time_F}, nC: ${nCounter}, nC_F: ${nCounter_F}`);

  //Add '/' symbol to end of every row in neuronsSynapses array
  try {
  for(let i = 0; i <= nCounter; i++){
    neuronsSynapses[i].push("/");
  }
  } catch (error) {
  
  }

}

document.getElementById("startSim").onclick = async function(){
console.log("simRun: ", simRun);
if(simRun == true){
simRun = false
//Update buttons
document.getElementById("selectNeuron").disabled = true;
document.getElementById("selectSynapseLight").disabled = true;
document.getElementById("selectSynapseMid").disabled = true;
document.getElementById("selectSynapseHeavy").disabled = true;
document.getElementById("selectStimulate").disabled = true;
document.getElementById("startSim").textContent = "Stop";
document.getElementById("startSim").style.backgroundColor = 'red';


//Spawn simulation .exe w/ std I/O streams
let streamData = "";
let streamVoltage = 1;
let imgSrcStr = "";
let proc = await Neutralino.os.spawnProcess(simPath);

//Check and print received stdout from sim.exe (continuous)
Neutralino.events.on('spawnedProcess', (evt) => {
    if(evt.detail.id === proc.id){
        streamData = evt.detail.data;
        console.log(streamData);
        if(streamData.at(0) == "n" && streamData.at(1) == "V"){
          streamVoltage = parseInt(streamData.substring(5, 8));
          var ctx = document.getElementById("maincanvas").getContext('2d');
          var img = new Image();

          if(streamVoltage <= 3){img.src = "/N0.png"; imgSrcStr = "N0";}
          else if(streamVoltage == 4 || streamVoltage == 5){img.src = "/N1.png"; imgSrcStr = "N1";}
          else if(streamVoltage == 6 || streamVoltage == 7){img.src = "/N2.png"; imgSrcStr = "N2";}
          else if(streamVoltage == 8 || streamVoltage == 9){img.src = "/N3.png"; imgSrcStr = "N3";}
          else if(streamVoltage >= 10){img.src = "/N4.png"; imgSrcStr = "N4";}
          let drawX = neuronCoordsX[streamData.at(3)];
          let drawY = neuronCoordsY[streamData.at(3)];
          //Center the newly drawn neurons (as well as possible)
          if(imgSrcStr == "N0"){img.onload = function(){
              ctx.drawImage(img, drawX-35, drawY-35);};}
          else if(imgSrcStr == "N1"){img.onload = function(){
              ctx.drawImage(img, drawX-31, drawY-35);};}
          else if(imgSrcStr == "N2"){img.onload = function(){
              ctx.drawImage(img, drawX-32, drawY-35);};}
          else if(imgSrcStr == "N3"){img.onload = function(){
              ctx.drawImage(img, drawX-32, drawY-35);};}
          else if(imgSrcStr == "N4"){img.onload = function(){
              ctx.drawImage(img, drawX-33, drawY-36);};}
          streamData = "";

        }
    }
});
console.log(`${time} ms`);
console.log(`${nCounter} neurons`);
stringFormat();

//ipcData: what will be transmitted.  
//To remove commas between rows of the array: 
const neuronsSynapsesStr = neuronsSynapses.map(row => row.join(",")).join(""); //This line was generated by AI

let ipcData = `clk${time_F}_nct${nCounter_F}_rft${refractoryDelay_F}/${neuronsSynapsesStr}?${neuronStim}!`;


//NeutralinoJS stdin stream to spawned process:
await Neutralino.os.updateSpawnedProcess(proc.id, 'stdIn', ipcData);
await Neutralino.os.updateSpawnedProcess(proc.id, 'stdInEnd');
}

else if(simRun == false){
  simRun = true;
  //update buttons
  document.getElementById("selectNeuron").disabled = false;
  document.getElementById("selectSynapseLight").disabled = false;
  document.getElementById("selectSynapseMid").disabled = false;
  document.getElementById("selectSynapseHeavy").disabled = false;
  document.getElementById("selectStimulate").disabled = false;
  document.getElementById("startSim").textContent = "Start";
  document.getElementById("startSim").style.backgroundColor = 'white';

  //kill all programs with the name of the sim.exe:
  await Neutralino.os.spawnProcess("taskkill /IM sim.exe /F"); //This line was written by AI
  }
}


/////////////////////////////////////////////////////////////////////////////////////

/*
Hello, sorry if this has already been answered (I just joined the server), but what do 
I need to modify to make a NeutralinoJS window not reload every time there is a change?
Since I have a JSON file that changes quite frequently so reloading is pre
*/