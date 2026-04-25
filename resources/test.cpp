// C++ Program to get the current working directory
#include <filesystem>
#include <iostream>
#include <typeinfo>
#include <cmath>

int main()
{
    // finding and printing the current working directory.
    
    std::filesystem::path filepath = std::filesystem::current_path() / "simdata.json";
    std::cout << "Current path is " << std::filesystem::path() << std::endl;
    std::cout << filepath <<'\n';
    float abc = 3.1415962;
    std::cout << abc << '\n';
    abc = std::round(abc * 100.f) / 100.f;
    return 0;
}