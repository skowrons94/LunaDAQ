#include <json/value.h>
#include <json/writer.h>
#include <json/json.h>

#include <CAENDigitizer.h>

#include "CAENConfig.h"

CAENConfig::CAENConfig( CAENBoardInfo boardInfo, std::string confFile )
{
    this->name = confFile;
    this->info = boardInfo;
    this->BOARD_ID = info.boardId;
    this->MAX_CHAN = info.boardInfo.Channels; 
    caenRegisters = new CAENRegister( info.firmType, info.boardInfo.ModelName );
    caenRegisters->Build( );

    std::string boardName = info.boardInfo.ModelName;
    if( boardName.find( "730" ) != std::string::npos )
    {
        if( info.firmType == CAEN_DGTZ_DPPFirmware_PHA )
            nsPerSample = 8;
        else if( info.firmType == CAEN_DGTZ_DPPFirmware_PSD )
            nsPerSample = 2;
    }
    else if( boardName.find( "725" ) != std::string::npos ){
        if( info.firmType == CAEN_DGTZ_DPPFirmware_PHA )
            nsPerSample = 16;
        else if( info.firmType == CAEN_DGTZ_DPPFirmware_PSD )
            nsPerSample = 4;
    }
    else if( boardName.find( "724" ) != std::string::npos || 
             boardName.find( "781" ) != std::string::npos || 
             boardName.find( "782" ) != std::string::npos )
        nsPerSample = 10;
    else if( boardName.find( "720" ) != std::string::npos )
        nsPerSample = 4;
    else
        nsPerSample = 1;

}

CAENConfig::CAENConfig(const CAENConfig& config)
{
    this->name = config.name;
    this->info = config.info;
    this->BOARD_ID = config.BOARD_ID;
    this->MAX_CHAN = config.MAX_CHAN;
    this->nsPerSample = config.nsPerSample;
    this->parameters  = config.parameters;
    caenRegisters = new CAENRegister( info.firmType, info.boardInfo.ModelName );
    caenRegisters->Build( );
}

CAENConfig::~CAENConfig( )
{
    delete caenRegisters;
    caenRegisters = nullptr;
}

void CAENConfig::setFilename(std::string filename)
{
    name = filename;
}

std::string CAENConfig::getFilename()
{
    return name;
}

json CAENConfig::getConfig()
{
    return parameters;
}

void CAENConfig::dumpConfig()
{
    std::cout << parameters << std::endl;
}

bool CAENConfig::write( uint32_t address, uint32_t value )
{
    int error = (int)CAEN_DGTZ_WriteRegister(info.handle, address, value);
    if( error != 0 ) return false;
    else return true;
}

bool CAENConfig::read( uint32_t address, uint32_t& value )
{
    int error = (int)CAEN_DGTZ_ReadRegister(info.handle, address, &value);
    if( error != 0 ) return false;
    else return true;
}

int CAENConfig::setRegisterSpecificBits( uint32_t reg_add, uint8_t bit_lower, uint8_t bit_upper, uint32_t value )
{    
    int error;
    uint8_t bit_number = bit_upper - bit_lower + 1;
    if( (value < ((uint32_t)1<<bit_number)) && bit_upper<32 && bit_lower<=bit_upper ){
        uint32_t reg_data = 0;
        uint32_t reg_mask = 0;
    
        for( int bit = 0; bit < 32; bit++ ) { if( bit<bit_lower || bit>bit_upper ) { reg_mask+=(1<<bit); } }
    
        error = (int)CAEN_DGTZ_ReadRegister(info.handle, reg_add, &reg_data);
        if( error == 0 ) {
            reg_data = (reg_data & reg_mask) + (value << bit_lower);
            error = (int)CAEN_DGTZ_WriteRegister(info.handle, reg_add, reg_data);
            if( error != 0 ) return -1;
        }
        else return -1;

        return 0;
    }

    return -1;
}

int CAENConfig::getRegisterSpecificBits( uint32_t reg_add, uint8_t bit_lower, uint8_t bit_upper, uint32_t& value ) 
{
    int error;
    uint32_t reg_data = 0;
    uint32_t reg_mask = 0;
    if( bit_upper<32 && bit_lower<=bit_upper ){
        for( int bit = 0; bit < 32; bit++ ) { if (bit>=bit_lower && bit<=bit_upper) { reg_mask+=(1<<bit); } }
        error = (int)CAEN_DGTZ_ReadRegister(info.handle, reg_add, &reg_data);
        if( error != 0 ) return -1;
        else{
            value = ((reg_data & reg_mask) >> bit_lower);
            return 0;
        }
    }
    return -1;
}

int CAENConfig::checkSetting( std::string setting ){
    if( caenRegisters->settings.find( setting ) == caenRegisters->settings.end())
        return -1;
    std::string register_key = caenRegisters->settings[setting].first;
    if( caenRegisters->registers.find( register_key ) == caenRegisters->registers.end())
        return -1;    
    return 0;
}

int CAENConfig::readSetting( std::string setting, int channel, uint32_t& value )
{
    if( caenRegisters->settings.find( setting ) == caenRegisters->settings.end())
        return -1;

    std::string register_key = caenRegisters->settings[setting].first;
    if( caenRegisters->registers.find( register_key ) == caenRegisters->registers.end())
        return -1;

    uint32_t address = caenRegisters->registers[register_key];

    if(address == 0x8180)
        address = address + 0x4*channel/2;
    else if(address > 0x2000);
    else address = address + 0x100*channel;

    uint8_t  bit_lower = caenRegisters->settings[setting].second.first;
    uint8_t  bit_upper = caenRegisters->settings[setting].second.second;

    int error = getRegisterSpecificBits( address, bit_lower, bit_upper, value );

    return error;
}

int CAENConfig::writeSetting( std::string setting, int channel, uint32_t value )
{
    if( caenRegisters->settings.find( setting ) == caenRegisters->settings.end())
        return -1;

    std::string register_key = caenRegisters->settings[setting].first;
    if( caenRegisters->registers.find( register_key ) == caenRegisters->registers.end())
        return -1;

    uint32_t address = caenRegisters->registers[register_key];

    if(address == 0x8180)
        address = 0x4*channel/2;
    else if(address > 0x2000);
    else address = address + 0x100*channel;

    uint8_t  bit_lower = caenRegisters->settings[setting].second.first;
    uint8_t  bit_upper = caenRegisters->settings[setting].second.second;
    int error = setRegisterSpecificBits( address, bit_lower, bit_upper, value );
    return error;
}

uint32_t CAENConfig::getMaxValue( std::string setting )
{
    if( caenRegisters->settings.find( setting ) == caenRegisters->settings.end())
        return 0;

    std::string register_key = caenRegisters->settings[setting].first;
    if( caenRegisters->registers.find( register_key ) == caenRegisters->registers.end())
        return 0;

    uint8_t  bit_lower = caenRegisters->settings[setting].second.first;
    uint8_t  bit_upper = caenRegisters->settings[setting].second.second;

    uint32_t max_value = ((uint32_t)1<<(bit_upper-bit_lower+1)) - 1;
    return max_value;
}

bool CAENConfig::create( )
{
    Json::Value regList;
    Json::Value dgtList;
    Json::StreamWriterBuilder builder;
    builder["commentStyle"]         = "None";
    builder["indentation"]          = "  "  ;
    builder["dropNullPlaceholders"] = true  ;

    dgtList["BoardName"]      = info.boardInfo.ModelName;
    dgtList["Model"]          = info.boardInfo.Model;
    dgtList["NbChannels"]     = info.boardInfo.Channels;
    dgtList["SerialNumber"]   = info.boardInfo.SerialNumber;
    dgtList["LinkNb"]         = info.linkNb;
    dgtList["BoardNb"]        = info.boardId;
    if( info.conType == 0 )
        dgtList["ConnectionType"] = "USB Connection";
    else if( info.conType = 1 )
        dgtList["ConnectionType"] = "Optical Connection";
    if( info.firmType == CAEN_DGTZ_DPPFirmware_PHA )
        dgtList["Firmware"]       = "DPP-Pulse Height Analysis (PHA)";
    else if( info.firmType == CAEN_DGTZ_DPPFirmware_PSD )
        dgtList["Firmware"]       = "DPP-Pulse Shape Discrimination (PSD)";
  
    Json::Value dgtz;
    dgtz["dgtzs"]=dgtList;
    std::ostringstream regName;
    std::ostringstream regAddr;
    std::ostringstream regVals;
    uint32_t regValue;
    uint32_t registerAddr;
    uint32_t value;
    for (auto it=caenRegisters->registers.begin(); it!= caenRegisters->registers.end() ; ++it){
        regName.str("");regAddr.str("");regVals.str("");
        regName << "reg_" << std::hex << it->second;
        regAddr << "0x" << std::hex << it->second;
        registerAddr = std::stoul(regAddr.str(),nullptr,16);
        this->read(registerAddr,value);
        regVals << "0x" << std::hex << value;
        regList[regName.str()]["name"]   = it->first;
        regList[regName.str()]["channel"]= 0;
        regList[regName.str()]["address"]= regAddr.str();
        regList[regName.str()]["value"]  = regVals.str();

        if(it->second == 0x8180){
            uint16_t maxOffset = 0x4*info.boardInfo.Channels/2;
            for(uint16_t offset = 0x4 ; offset < maxOffset ; offset += 0x4){ // Channel depend register
	            regName.str("");regAddr.str("");regVals.str("");
	            regName << "reg_" << std::hex << it->second+offset;
	            regAddr << "0x" << std::hex << it->second+offset;
	            registerAddr = std::stoul(regAddr.str(),nullptr,16);
                this->read(registerAddr,value);
	            regVals << "0x" << std::hex << value;
	            regList[regName.str()]["name"]   =it->first;
	            regList[regName.str()]["channel"]=offset/0x4;
	            regList[regName.str()]["address"]=regAddr.str();
	            regList[regName.str()]["value"]  =regVals.str();
            }
        }
        else if(it->second > 0x2000) continue;
        else {
            uint16_t maxOffset = 0x100*info.boardInfo.Channels;
            for(uint16_t offset = 0x100 ; offset < maxOffset ; offset += 0x100){ // Channel depend register
	            regName.str("");regAddr.str("");regVals.str("");
	            regName << "reg_" << std::hex << it->second+offset;
	            regAddr << "0x" << std::hex << it->second+offset;
	            registerAddr = std::stoul(regAddr.str(),nullptr,16);
                this->read(registerAddr,value);
	            regVals << "0x" << std::hex << value;
	            regList[regName.str()]["name"]   =it->first;
	            regList[regName.str()]["channel"]=offset>>8;
	            regList[regName.str()]["address"]=regAddr.str();
	            regList[regName.str()]["value"]  =regVals.str();
            }
        }    
    }

    dgtz["registers"]=regList;
    std::ofstream file; file.open(name.c_str());
    file << dgtz ;
    file.close();

    return true;

}

bool CAENConfig::load( )
{
    if( fileIn.is_open( ) ){
        fileIn.close( );
    }

    if( fileOut.is_open( ) ){
        fileOut.close( );
    }

    fileIn.open( name, std::ifstream::binary );

    if( !fileIn.is_open( ) ){
        return false;
    }

    fileIn >> parameters;

    fileIn.close( );

    this->commit( );

    return true;

}

bool CAENConfig::save( )
{
    if( fileIn.is_open( ) ){
        fileIn.close( );
    }

    if( fileOut.is_open( ) ){
        fileOut.close( );
    }  

    fileOut.open( name );

    if( !fileOut.is_open( ) ){
        return false;
    }

    fileOut << std::setw(4) << parameters << std::endl;

    fileOut.close( );

    return true;

}

bool CAENConfig::commit()
{
  int ch;
  int error;
  uint32_t address;
  uint32_t value;
  std::string buff;  
  for (const auto& item : parameters["registers"].items())
    {        
      ch = parameters["registers"][item.key()].at("channel");
      if( ch > MAX_CHAN - 1 )
	    continue;
      buff    = parameters["registers"][item.key()].at("address");
      address = std::stoul( buff, NULL, 0 );
      buff  = parameters["registers"][item.key()].at("value");
      value = std::stoul( buff, NULL, 0 );
      this->write(address, value);
    }
  
  return true;

}

bool CAENConfig::update()
{  
    uint32_t value;
    for (const auto& item : parameters["registers"].items())
    {        
        int ch = parameters["registers"][item.key()].at("channel");
	    if( ch > MAX_CHAN - 1 ){
	    std::stringstream stream;
	    stream << "0x";
	    std::string result( stream.str() );
	    parameters["registers"][item.key()].at("value") = result;
	    }
	else{
	    std::string buff = parameters["registers"][item.key()].at("address");
	    uint32_t address = std::stoul( buff, NULL, 0 );
	    this->read(address,value);
	    std::stringstream stream;
	    stream << "0x" 
		    << std::setfill ('0') << std::setw(sizeof(uint32_t)*2) 
		    << std::hex << value;
	    std::string result( stream.str() );
	    parameters["registers"][item.key()].at("value") = result;
	    }
    }

  return true;

}
