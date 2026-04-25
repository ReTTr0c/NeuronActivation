#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <random>   
#include <future>   //i dont think I end up using this
#include <chrono>
#include <stdio.h>  //No clue why I would need this
#include <conio.h>  //I have so many headers here that I used for earlier versions
#include <tchar.h>  //But now idk what they're for and I'm too scared to delete them
#include <bits/stdc++.h> //Is this... trying to import the standard library? What guide needed this...?

//Github: ReTTr0c
//This is some dogwater code but I do electronics, not coding so forgive me
//Note: I don't like using 'using namespace std' so I'm not

//This code is mostly AI-free, only the substring commands are AI-made, but I
//didn't end up having a need for those in this version (I did still mark the exact lines tho)

const bool BUILD = false;

int nCount;
int threshold = 10;
int clockspeed;
int startcounter = 0;
int refTime;
float tau = 1.2; //Voltage leakage time variable

class synapses{
    public:
        int id;
        int uid;
        int sideA;
        int sideB = -1;
        bool excitatory = true;
};
class neurons{
    public:
        int id;
        float voltage = 0; 
        int refTimeStep = 0; //Counts ticks for refractory timeout
        float receiveStim;
        bool trigger;
        bool refractory = false;
        std::vector<synapses> synapse;
};
class stimuli{
    public:
        std::string type;
        int spotid;
        int voltage;
};

neurons neuron0, neuron1, neuron2, neuron3, neuron4, neuron5, neuron6, neuron7, neuron8, neuron9;

std::vector<neurons> neuron = {neuron0,neuron1,neuron2,neuron3,neuron4,neuron5,neuron6,neuron7,neuron8,neuron9};


void stimulate(std::vector<neurons>& neuron, std::string stimtype, int spotid, int stimvoltage){

    if(stimtype == "dc"){
        neuron.at(spotid).voltage += stimvoltage;
    }

}

void simulate(){
    for(int i = 0; i < nCount; i++){ //Output voltages of neurons
        if(neuron.at(i).voltage <= 9){
            std::cout << "nV-"<<i<<"="<<"00"<<abs(std::trunc(std::round(neuron.at(i).voltage)))<<'\n';
        }
        else if(neuron.at(i).voltage >= 10 && neuron.at(i).voltage <= 99){
            std::cout << "nV-"<<i<<"="<<"0"<<abs(std::trunc(std::round(neuron.at(i).voltage)))<<'\n';
        }
        else if(neuron.at(i).voltage >= 100 && neuron.at(i).voltage <= 999){
            std::cout << "nV-"<<i<<"="<<""<<abs(std::trunc(std::round(neuron.at(i).voltage)))<<'\n';
        }
        else{
            std::cout << "Voltage overflow\n";
            std::cout << "nV-"<<i<<"="<<abs(std::trunc(std::round(neuron.at(i).voltage)))<<'\n';
        }
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    for(int i = 0; i < nCount; i++){
        std::cout<<"Neuron at "<<i<<"receive = "<<neuron.at(i).receiveStim<<'\n';
        std::cout.flush();
    }
    for(int i = 0; i < nCount; i++){neuron.at(i).trigger = false;} //Precaution
    for(int i = 0; i < nCount; i++){
    bool stimDone = false;
    if(neuron.at(i).voltage >= 10){ 
       // int tempA = (neuron.at(i).voltage)-10;
       // neuron.at(i).voltage = tempA;
        neuron.at(i).trigger = true;
    }
    else{neuron.at(i).trigger = false;}
    if(neuron.at(i).trigger == true && neuron.at(i).refractory == false){
        for(int k = 0; k < neuron.at(i).synapse.size(); k++){
            if(neuron.at(i).synapse.at(k).sideB >= 0){
                if(neuron.at(i).synapse.at(k).excitatory == true){
                    neuron.at(neuron.at(i).synapse.at(k).sideB).receiveStim += 6;
                    neuron.at(i).refractory = true;
                    neuron.at(i).refTimeStep = 0;
                    stimDone = true;
                }
                else{
                    neuron.at(neuron.at(i).synapse.at(k).sideB).receiveStim -= 2;
                    neuron.at(i).refractory = true;
                    neuron.at(i).refTimeStep = 0;
                    stimDone = true;
                }
            }
        }
        /*
        Okay, so here comes the part where my past laziness makes my current-self suffer;
        Since synapses are only stored unidirectionally, we need to check whether ANY neurons
        Are connected to the neuron.at(i) we are looking at in the loop cycle, since the neurons
        are effectively blind to any ones that are incoming to them. But you still need to stimulate
        even those, so that's what happens here. It's too late on a school night for me to explain it
        here, but I don't think it's difficult to grasp lol.
        */
        for(int j = 0; j < nCount; j++){
            if(i == j){continue;}
                for(int l = 0; l < neuron.at(j).synapse.size(); l++){
                    if(neuron.at(j).synapse.at(l).sideB >= 0){
                        if(neuron.at(j).synapse.at(l).sideB == neuron.at(i).id && neuron.at(i).trigger == true && neuron.at(i).refractory == false){
                            if(neuron.at(j).synapse.at(l).excitatory == true){
                                neuron.at(j).receiveStim += 6;
                                neuron.at(i).refractory = true;
                                neuron.at(i).refTimeStep = 0;
                            }
                            else{
                                neuron.at(j).receiveStim -= 2;
                                neuron.at(i).refractory = true;
                                neuron.at(i).refTimeStep = 0;
                            }
                            stimDone = true;
                        }
                    }
                }
    }
        if(stimDone){neuron.at(i).voltage -= (int)round(neuron.at(i).voltage * 0.50);} //Decrease neuron voltage by 70%
     
            
    }
        std::cout << '\n';
        std::cout.flush();
        neuron.at(i).trigger = false;
    }

    //Update neuron voltages
    float vChange;
    for(int i = 0; i <nCount; i++){
        vChange = 0;
        vChange = (round(-neuron.at(i).voltage) + neuron.at(i).receiveStim) / tau;
        neuron.at(i).voltage += vChange;
        neuron.at(i).receiveStim *= 0.8; //Slowly decrease incoming stim level
    }

    //Refractory timer
    for(int i = 0; i < nCount; i++){
        if(neuron.at(i).refTimeStep >= refTime){
            neuron.at(i).refTimeStep = 0;
            neuron.at(i).refractory = false;
        }
        if(neuron.at(i).refractory == true){
            neuron.at(i).refTimeStep++;
        }
    }
}



int main(){
    const float DRATE = 0.5;
    std::vector<int> stimVector;

    std::cout << "unassigned nCounter: " << nCount << '\n';
if(startcounter == 0){
    //get starting info

    std::string Input, clkTH, clkH, clkT, clkO, neuO, neuT, ref;
    int eraseA, eraseB, markerA, markerB;
    std::getline(std::cin, Input);
    clkTH = Input.at(3); clkH = Input.at(4); clkT = Input.at(5); clkO = Input.at(6);
    std::cout << stoi(clkTH+clkH+clkT+clkO) << '\n';
    clockspeed = std::stoi(clkTH+clkH+clkT+clkO);
    neuT = Input.at(11); neuO = Input.at(12);
    nCount = std::stoi(neuT+neuO);
    ref = Input.at(17);
    refTime = std::stoi(ref);

    std::cout <<"tick speed received: " << clockspeed << '\n'; 
    std::cout <<"nCount received: " << nCount << '\n';
    std::cout <<"refractory timeout received: " << refTime<< '\n';
    std::cout.flush();
// --- /0,2,7,3,5/1,3,4,0,9/
// --- /0,2,7,3,5,/1,4,0,2,/2,3,0,/3,1,/4,/
Input.erase(0, 17);
std::string redo = "/";
for(int i = 0; i <= nCount-1; i++){
//TODO: Check if number is 2 digits long by searching for next comma
//TODO: Convert to integer, dumbass
    markerA = Input.find('/');
    Input.erase(markerA, 1);
    markerB = (Input.find('/')) + 1;
    Input.insert((markerA), "/");
    int diff = markerB-markerA;

    std::string s1; 
    std::string s2;
    std::string s3;
    std::string s4; 

    //FIXME: diff calc won't work if there is a double digit num.
    //String parsing is set up for it, but the diff calc isn't
    switch(diff){
        case 3:
        std::cout << "diff3\n";
        std::cout.flush();
        break;

        case 5:
        //If statement checks whether char after number is a comma (or a comma if it is the last one).
        //If yes; it is a one-digit number. If not; It is a double-digit number
        if((Input.at(markerA+4)) == ',' || Input.at(markerA+4) == '/'){s1 = Input.at((markerA + 3));} //Get single digit number
        else{s1 = Input.substr((markerA+3), 2);} //Get double digit number (This line was made by AI)
        neuron.at(i).synapse.push_back(synapses());
        neuron.at(i).synapse.at(0).sideB = std::stoi(s1);
        std::cout << "diff 5\n";
        std::cout.flush();
        break;

        case 7:
        if((Input.at(markerA+4)) == ','){s1 = Input.at((markerA + 3));} //Get single digit number
        else{s1 = Input.substr((markerA+3), 2);} //Get double digit number
        if(Input.at(markerA+6) == ',' || Input.at(markerA+6) == '/'){s2 = Input.at(markerA + 5);}
        else{s2 = Input.substr((markerA+5), 2);}
        neuron.at(i).synapse.push_back(synapses());
        neuron.at(i).synapse.push_back(synapses());
        neuron.at(i).synapse.at(0).sideB = std::stoi(s1);
        neuron.at(i).synapse.at(1).sideB = std::stoi(s2);
        std::cout << "diff 7\n";
        std::cout.flush();
        break;

        case 9:
        if(Input.at(markerA+4) == ','){s1 = Input.at((markerA + 3));} //Get single digit number
        else{s1 = Input.substr((markerA+3), 2);} //Get double digit number
        if(Input.at(markerA+6) == ','  ){s2 = Input.at(markerA + 5);}
        else{s2 = Input.substr((markerA+5), 2);}
        if(Input.at(markerA+8) == ',' || Input.at(markerA+8) == '/'){s3 = Input.at(markerA + 7);}
        else{s3 = Input.substr((markerA+7), 2);}
        neuron.at(i).synapse.push_back(synapses());
        neuron.at(i).synapse.push_back(synapses());
        neuron.at(i).synapse.push_back(synapses());
        neuron.at(i).synapse.at(0).sideB = std::stoi(s1);
        neuron.at(i).synapse.at(1).sideB = std::stoi(s2);
        neuron.at(i).synapse.at(2).sideB = std::stoi(s3);
        std::cout << "diff 9\n";
        std::cout.flush();
        break;

        case 11:
        if(Input.at(markerA+4) == ','){s1 = Input.at((markerA + 3));} //Get single digit number
        else{s1 = Input.substr((markerA+3), 2);} //Get double digit number
        if(Input.at(markerA+6) == ','){s2 = Input.at(markerA + 5);}
        else{s2 = Input.substr((markerA+5), 2);}
        if(Input.at(markerA+8) == ','){s3 = Input.at(markerA + 7);}
        else{s3 = Input.substr((markerA+7), 2);}
        if(Input.at(markerA+10) == ',' || Input.at(markerA+10) == '/'){s4 = Input.at(markerA+9);}
        else{s4 = Input.substr((markerA+9), 2);}
        neuron.at(i).synapse.push_back(synapses());
        neuron.at(i).synapse.push_back(synapses());
        neuron.at(i).synapse.push_back(synapses());
        neuron.at(i).synapse.push_back(synapses());
        neuron.at(i).synapse.at(0).sideB = std::stoi(s1);
        neuron.at(i).synapse.at(1).sideB = std::stoi(s2);
        neuron.at(i).synapse.at(2).sideB = std::stoi(s3);
        neuron.at(i).synapse.at(3).sideB = std::stoi(s4);
        std::cout << "diff 11\n";
        std::cout.flush();
        break;
        
        default:
        std::cout << "Error here with diff: "<< diff << '\n';
        std::cout.flush();
    }


    std::cout << "Before erase: " << Input << '\n';
    Input.erase(markerA, (markerB-1));  
    Input.erase(0,1); //Erase a comma it puts there for some reason
    std::cout << "After erase: " << Input << '\n';
    std::cout.flush();

}

markerA = Input.find("?");
int endMarker = Input.find("!");
int stimLen = endMarker-markerA;
int stimNum = stimLen/2;
std::string addStim;
int addStimInt;
std::cout<<"markerA: "<<markerA<<", endMarker: "<<endMarker<<", stimLen: "<<stimLen<<", stimNum: "<<stimNum<<'\n';
for(int i = 0; i <= stimNum-1; i++){
    addStim = Input.at((markerA+1)+(2*i));
    addStimInt = std::stoi(addStim);
    std::cout << "value of Input.at(): "<<((markerA+1)+(2*i))<<'\n';
    std::cout << "addStim on "<<i<<" is "<<addStim<<'\n';
    stimVector.push_back(addStimInt);
}
std::cout << "Stim vector is empty? "<<stimVector.empty()<<". And has a size of "<<stimVector.size()<<'\n';
if(!stimVector.empty()){
    for(int i = 0; i <= stimVector.size()-1; i++){
        std::cout << "Stim vector at "<<i<<" is: "<< stimVector.at(i) << '\n';
        neuron.at(stimVector.at(i)).voltage += 10;
    }
}
else{std::cout << "NO STIM\n";}
for(int i = 0; i < nCount; i++){
    std::cout << "Neuron "<<i<< " starting voltage: "<<neuron.at(i).voltage<<'\n';
}

}


    ///////////////////////////////////////////////////////////////////////

    for (int i = 0; i < nCount; i++) { 
        neuron.at(i).id = i; 
        for(int j = 0; j < neuron.at(i).synapse.size(); j++){
            neuron.at(i).synapse.at(j).sideA = i;
        }
        std::cout << "Neuron " << i << " id: " << neuron.at(i).id << '\n';
    }
    for(int i = 0; i < nCount; i++){
        std::cout<<"Neuron at "<<i<<" Synapse size: "<<neuron.at(i).synapse.size()<<'\n';
        for(int j = 0; j < neuron.at(i).synapse.size(); j++){
        std::cout<<"With i and j:"<<i<<" / "<<j<<'\n';
        std::cout<<"SideB: "<<neuron.at(i).synapse.at(j).sideB<<'\n';            
        }
    }

    int tickCnt = 0;
    while(1){
            std::this_thread::sleep_for(std::chrono::milliseconds(clockspeed));
            std::cout << "TICK " << tickCnt << '\n';
            std::cout.flush();
            simulate();
            std::cout << "TOCK " << tickCnt << '\n';
            tickCnt++;
            std::cout.flush();

    }

    ////////////////////////////////////////////////////////////////////////////////////
}
//Example input:
//clk2000_nct03_rft5/0,2,1,/1,2,/2,/_?1,1!

