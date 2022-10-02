#include "CAENRegister.h"

CAENRegister::CAENRegister( CAEN_DGTZ_DPPFirmware_t dppType, std::string dgtzName )
{
    switch( dppType ){
        case( CAEN_DGTZ_DPPFirmware_PHA ):
            this->SetBuilder( new CAENRegisterBuilderPHA( dgtzName ) );
            break;
        case( CAEN_DGTZ_DPPFirmware_PSD ):
            this->SetBuilder( new CAENRegisterBuilderPSD( dgtzName ) );
            break;
    }

}

CAENRegister::~CAENRegister()
{
    delete this->fBuilder;
}

void CAENRegister::Produce( )
{
    this->fBuilder->ProduceRegisters( );
}

void CAENRegister::Build( )
{
    this->Produce( );
    registers = this->fBuilder->GetRegisters( );
    settings  = this->fBuilder->GetSettings( );
}