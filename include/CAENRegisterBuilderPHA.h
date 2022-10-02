#ifndef CAENREGISTERBUILDERPHA_H_INCLUDED
#define CAENREGISTERBUILDERPHA_H_INCLUDED

#include <map>
#include <string>

#include "CAENRegisterBuilder.h"

class CAENRegisterBuilderPHA : public CAENRegisterBuilder {

    public:
        CAENRegisterBuilderPHA( std::string dgtzName );
        ~CAENRegisterBuilderPHA();

        void ProduceRegisters( ) override;

        std::map<std::string, uint16_t> GetRegisters( ) override;
        std::map<std::string, std::pair<std::string,std::pair<int,int>>> GetSettings( ) override;

    private:
        std::string                                                      fDgtzName;
        std::map<std::string, uint16_t>                                  fRegisters;
        std::map<std::string, std::pair<std::string,std::pair<int,int>>> fSettings;

};

#endif // CAENREGISTERBUILDERPHA_H_INCLUDED