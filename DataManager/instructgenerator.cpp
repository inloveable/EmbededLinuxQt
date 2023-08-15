
#include "instructgenerator.hpp"
#include <array>

InstructGenerator::InstructGenerator()
{

}


void InstructGenerator::parseData(std::array<unsigned char,8>& data){
    if(!(data[0]==0xAA&&data[1]==0x55)){
        return;
    }
    switch(data[2]){
    case 0x01:{this->read<0x01>(data);break;};
    case 0x02:{this->read<0x02>(data);break;};
    case 0x03:{this->read<0x03>(data);break;};
    case 0x04:{this->read<0x04>(data);break;};
    case 0x05:{this->read<0x05>(data);break;};
    case 0x06:{this->read<0x06>(data);break;};
    case 0x07:{this->read<0x07>(data);break;};
    }
}
