#include <iostream>
#include <fstream>
#include <string>

int main()
{

    std::ifstream hex;
    std::ofstream coe;
    std::string buffer;
    
    hex.open("/home/xy/vscode/rv32i/verify/tinyrv32.hex4", std::ios::app);
    if (!hex)
        std::cout << "no hex file" << std::endl;

    coe.open("tinyrv32.coe");
    if (!coe)
        std::cout << "can not create coe file" << std::endl;

    coe << "MEMORY_INITIALIZATION_RADIX=16;" << std::endl;
    coe << "MEMORY_INITIALIZATION_VECTOR=" << std::endl;

    while (getline(hex, buffer))
    {
        if (!buffer.empty())
        {
            buffer.append(",");
            coe << buffer << std::endl;
        }
    }
    coe << "\b\b"
        << ";";
}
