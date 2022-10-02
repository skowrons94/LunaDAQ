#ifndef CAENCONFIG_H_INCLUDED
#define CAENCONFIG_H_INCLUDED

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>

#include <QFileDialog>
#include <QString>

#include <nlohmann/json.hpp>

#include "CAENBoardInfo.h"
#include "CAENRegister.h"

using json = nlohmann::json;

class CAENConfig {

    public:
        CAENConfig( ){ };
        CAENConfig(CAENBoardInfo info, std::string confFile);
        CAENConfig(const CAENConfig& config);
        ~CAENConfig();

        void setFilename(std::string filename);
        std::string getFilename();

        void dumpConfig();
        json getConfig();

        bool create();

        bool load();
        bool save();

        bool commit();

        bool update();

        int checkSetting( std::string setting );

        int readSetting( std::string setting, int channel, uint32_t& value );
        int writeSetting( std::string setting, int channel, uint32_t value );

        bool write(uint32_t address, uint32_t value);
        bool read(uint32_t address, uint32_t& value);

        int  setRegisterSpecificBits( uint32_t address, uint8_t bit_lower, uint8_t bit_upper, uint32_t value );
        int  getRegisterSpecificBits( uint32_t reg_add, uint8_t bit_lower, uint8_t bit_upper, uint32_t& value );   

        uint32_t getMaxValue( std::string setting );   

        uint32_t nsPerSample;

        CAENRegister*  caenRegisters;

    private:

        std::string    name;
        std::ifstream  fileIn;
        std::ofstream  fileOut;
        json           parameters;

        int            BOARD_ID = 0;
        int            MAX_CHAN = 8;

        CAENBoardInfo  info;

};

#endif
