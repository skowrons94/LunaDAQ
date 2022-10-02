#ifndef CAENREGISTERBUILDERPSD_H_INCLUDED
#define CAENREGISTERBUILDERPSD_H_INCLUDED

#include <map>
#include <string>

#include "CAENRegisterBuilder.h"

class CAENRegisterBuilderPSD : public CAENRegisterBuilder {

    public:
        CAENRegisterBuilderPSD( std::string dgtzName );
        ~CAENRegisterBuilderPSD();

        void ProduceRegisters( ) override;

        std::map<std::string, uint16_t> GetRegisters( ) override;
        std::map<std::string, std::pair<std::string,std::pair<int,int>>> GetSettings( ) override;

    private:
        std::string                      fDgtzName;
        std::map<std::string, uint16_t>  fRegisters;
        std::map<std::string, std::pair<std::string,std::pair<int,int>>> fSettings;

};

#endif // CAENREGISTERBUILDERPSD_H_INCLUDED