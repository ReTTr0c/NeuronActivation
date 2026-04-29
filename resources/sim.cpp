#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <random>   
#include <chrono>
#include <tchar.h>
#include <bits/stdc++.h>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <filesystem>
#include <iomanip>

//Note to anyone recompiling this Get rapidjson from github

//Github: ReTTr0c
//This is some dogwater code but I do electronics, not coding so forgive me
//Note: I don't like using 'using namespace std' so I'm not
//This code is mostly AI-free, only part of the JSON parser is AI-made


// 'D': Run Directly 
// 'N': Run via Neutralino
// 'B': Run via built Neutralino app
const char BUILD = 'D';
std::filesystem::path jsonPath;

int nCount;
int threshold = 10;
int clockspeed;
int startcounter = 0;
int refTime;
float tau = 1.2; //Voltage leakage time variable

class synapses{
    public:
        float weight;
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


std::vector<neurons> neuron;

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
        //Set precision so the number doesn't get too big and slow down the process
        neuron.at(i).receiveStim = std::round(neuron.at(i).receiveStim * 100.f) / 100.f;
    }

    for(int i = 0; i < nCount; i++){neuron.at(i).trigger = false;} //Precaution
    for(int i = 0; i < nCount; i++){
    bool stimDone = false;
    if(neuron.at(i).voltage >= 10){ 
        neuron.at(i).trigger = true;
    }
    else{neuron.at(i).trigger = false;}
    if(neuron.at(i).trigger == true && neuron.at(i).refractory == false){
        for(int k = 0; k < neuron.at(i).synapse.size(); k++){
            if(neuron.at(i).synapse.at(k).sideB >= 0){
                if(neuron.at(i).synapse.at(k).excitatory == true){
                    neuron.at(neuron.at(i).synapse.at(k).sideB).receiveStim += neuron.at(i).synapse.at(k).weight;
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
                                neuron.at(j).receiveStim += neuron.at(j).synapse.at(l).weight;
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
        if(stimDone){neuron.at(i).voltage -= (int)round(neuron.at(i).voltage * 0.50);} //Decrease neuron voltage by 50%
     
            
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

void readJson(){
    
    // Open the file for reading
    std::cout << "Jsonpath is "<<jsonPath<<'\n';
    FILE* fp = fopen(jsonPath.string().c_str(), "r");

    // Use a FileReadStream to
    // read the data from the file
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer,
                                 sizeof(readBuffer));

    // Parse the JSON data 
      // using a Document object
    rapidjson::Document data;
    data.ParseStream(is);

    // Close the file
    fclose(fp);

    // Access the data in the JSON document
    nCount = std::stoi(data["nct"].GetString());
    for(int i = 0; i < nCount; i++){
        neuron.push_back(neurons());
    }
    clockspeed = std::stoi(data["clk"].GetString());
    refTime = std::stoi(data["rft"].GetString());

    // Not fully written by AI, but I needed heavy help for this parser:
    const rapidjson::Value& neuronsSynapses = data["neuronsSynapses"];
    // Iterate over outer array (each neuron's synapse list)
    for (rapidjson::SizeType i = 0; i < neuronsSynapses.Size(); i++) {
        const rapidjson::Value& synapseJ = neuronsSynapses[i];
         if (!synapseJ.IsArray()){continue;}

        std::cout << "Neuron " << i << ": ";

        if (synapseJ.Empty()) {
            std::cout << "(no synapses)";
        }
        // Iterate over inner array (alternating int - string pairs)

        for(int i = 0; i < nCount; i++){
            std::cout << "Number of synapses of Neuron "<<i<<": "<<neuron.at(i).synapse.size()<<'\n';
        }


        for (rapidjson::SizeType j = 0; j < synapseJ.Size(); j++) {
            const rapidjson::Value& element = synapseJ[j];
            std::cout<<"j is "<<j<<", j/2 is"<<j/2<<'\n';
            if (element.IsInt()) {
                std::cout << "Element at "<<j<<" is an Int";
                std::cout << element.GetInt();
                neuron.at(i).synapse.push_back(synapses());
                neuron.at(i).synapse.at(j/2).sideB = element.GetInt();
                
            }   
                else if (element.IsString()) {
                    std::string weightChar = element.GetString(); // convert to std::string first
                    
                    if (weightChar == "l") {
                        neuron.at(i).synapse.at(j/2).weight = 3;
                    }
                    else if (weightChar == "m") {
                        neuron.at(i).synapse.at(j/2).weight = 5;
                    }
                    else if (weightChar == "h") {
                        neuron.at(i).synapse.at(j/2).weight = 8;
                    }
                
                std::cout << "\nNeuron "<<i<<", Synapse "<<j<<"weight = "<<neuron.at(i).synapse.at(j/2).weight<<'\n';
            }

            if (j < synapseJ.Size() - 1) std::cout << ", ";
        }

        std::cout << "\n";
    }
    //Parser end^
std::vector<int> stimVector;
const rapidjson::Value& neuronStim = data["stim"];
for (rapidjson::SizeType i = 0; i < neuronStim.Size(); i++) {
    const rapidjson::Value& element = neuronStim[i];
    stimVector.push_back(element.GetInt());
}
if(!stimVector.empty()){
    for(int i = 0; i <= stimVector.size()-1; i++){
        std::cout << "Stim vector at "<<i<<" is: "<< stimVector.at(i) << '\n';
        neuron.at(stimVector.at(i)).voltage += 10;
    }
}


}


int main(){
    if(BUILD == 'D'){
    jsonPath = std::filesystem::current_path() / "simdata.json";
    }
    else if(BUILD == 'N'){
    jsonPath = std::filesystem::current_path() / "resources" / "simdata.json";
    }
    else if(BUILD == 'B'){
    jsonPath = std::filesystem::current_path() / "simdata.json";
    }

readJson();
std::cout <<"tick speed received: " << clockspeed << '\n'; 
std::cout <<"nCount received: " << nCount << '\n';
std::cout <<"refractory timeout received: " << refTime<< '\n';
std::cout.flush();

for(int i = 0; i < nCount; i++){
    std::cout << "Neuron "<<i<< " starting voltage: "<<neuron.at(i).voltage<<'\n';
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

//"C:\\Users\\totht\\Desktop\\C++_projects\\NeuronActivation\\resources\\simdata.json"

