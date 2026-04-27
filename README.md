A neural circuit simulator made with NeutralinoJS. Made for Hack Club Flavortown.

*Installation*:
--------------------------------------------------------------------------------------------------
- Unzip the .tar or .rar into a folder.
- Run NeuronActivation.exe
(Keep all components and binaries in the same directory)

*Guide*:
---------------------------------------------------------------------------------------------------
  - Open NeutronActivation.exe (close the terminal if needed) and select the Neuron button.
  - Place down as many Neurons as you want.
  - Connect them with synapses; click on one neuron, and then another to form a connection:
    - The synapses have 3 weights: Light, Medium, Heavy
    - The heavier a synapse, the stronger it stimulates the neuron connected to it.
    - The synapses store the property of where they are connected to as sideB. Therefore connections
    where a neuron has an activated neuron as its sideB will work, but may have a lower priority compared to 
    an activated neuron having its own synapse with a sideB.
    - The synapses are represented with lines, each setting thicker than the previous. (Though certain angles can slightly mess up ctx.draw)
  - Add stimulation to whichever neuron(s) you want. Each stimulation adds 10 to the voltage (The threshold for firing).
  Multiple can be applied to a single neuron. The max threshold is 999.
  - Use the top slider to set the timeout between simulation steps. 1 step means 1 round of firings.
  - Use the bottom slider to set refractory period of the neurons. This adjusts for how many simulation
  steps a neuron can't fire again after having fired once.
  - Click on Start to run the C++ simulation. As it runs, you will see the colors of the neurons changing.
  That shows the current voltage of the neuron:
    - Green: 0-3 Voltage units
    - Light blue: 4-5 Voltage units
    - Dark blue: 6-7 Voltage units
    - Orange: 8-9 Voltage units
    - Red: 10 or 10+ Voltage units *(10 is the voltage at which it fires)*
  - Clicking on Stop will terminate the simulation process.
  - There is a reset button, which closes and reopens the app, but simply reloading the page with
  Ctrl+r works just as well (since it's WebView)

*'Realism' features*
----------------------------------------------------------------------------------------------------
- Multiple synapse weights.
- Refractory periods.
- Leaky-integrate-and-fire equation to calculate current voltage level.


Technical details:
--------------------------------------------------------------------------------------------------------
- NeutralinoJS framework
- Web frontend
- C++ backend
- JS -> C++ IPC via JSON (The JS modifies the JSON each time the Start button is clicked, giving the C++
                          the base details)
- C++ -> JS IPC via NeutralinoJS API (stdout)

Only tested on 64-bit Windows; May or may not work on Linux.
**As of release v1.0.1, any system with a name containing non-English characters will break the simulation**
Under MIT license.
