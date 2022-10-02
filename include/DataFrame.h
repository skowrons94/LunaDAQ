#ifndef DATAFRAME_H_INCLUDED
#define DATAFRAME_H_INCLUDED

#include <bitset>
#include <cstdint>

#include <CAENDigitizerType.h>

#include "DataFrameBuilderPHA.h"
#include "DataFrameBuilderPSD.h"

class DataFrame{

    public:
        DataFrame( ){};
        DataFrame( CAEN_DGTZ_DPPFirmware_t dppType, std::string dgtzName );
        ~DataFrame( );
        
        void Build( );

        uint16_t EvtSize( );
        
        bool CheckConfig( std::string key );
        bool CheckFormat( std::string key );
        bool CheckEnabled( std::string key );
        uint16_t GetConfig( std::string key );
        std::pair<int,int> GetFormat( std::string key );

        void SetDataFormat( uint32_t form ){ this->fDataFormat = form; };
        
    private:
        
        void Produce( );
        void SetBuilder( DataFrameBuilder* builder ) { fBuilder = builder; };

        DataFrameBuilder* fBuilder = nullptr;

        uint32_t fNumSamples;
        std::map<std::string, int>                 fFlags;
        std::map<std::string, std::pair<int,int> > fConfigs;
        std::map<std::string, std::pair<int,int> > fFormats;

        std::bitset<32> fDataFormat;
        
};

#endif // DATAFRAME_H_INCLUDED