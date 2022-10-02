#ifndef CAENREGISTER_H_INCLUDED
#define CAENREGISTER_H_INCLUDED

#include <map>
#include <string>

#include <CAENDigitizerType.h>

#include "CAENRegisterBuilderPHA.h"
#include "CAENRegisterBuilderPSD.h"

class CAENRegister {

    public:
        CAENRegister( CAEN_DGTZ_DPPFirmware_t dppType, std::string dgtzName );
        ~CAENRegister();

        void Build( );

        std::map<std::string, uint16_t> registers;
        std::map<std::string, std::pair<std::string,std::pair<int,int>>> settings;

    private:

        void Produce( );
        void SetBuilder( CAENRegisterBuilder* builder ) { fBuilder = builder; }

        CAENRegisterBuilder* fBuilder;

};

#endif // CAENREGISTER_H_INCLUDED