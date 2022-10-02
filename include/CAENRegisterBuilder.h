#ifndef CAENREGISTERBUILDER_H_INCLUDED
#define CAENREGISTERBUILDER_H_INCLUDED

#include <map>
#include <string>

class CAENRegisterBuilder {

    public:
        virtual ~CAENRegisterBuilder() {};

        virtual void ProduceRegisters( ) = 0;

        virtual std::map<std::string, uint16_t> GetRegisters( ) = 0;
        virtual std::map<std::string, std::pair<std::string,std::pair<int,int>>> GetSettings( ) = 0;

};

#endif // CAENREGISTERBUILDER_H_INCLUDED