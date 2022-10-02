#ifndef CAENBOARDINFO_H_INCLUDED
#define CAENBOARDINFO_H_INCLUDED

#include <string>
#include <iostream>

#include <CAENDigitizerType.h>

struct CAENBoardInfo{

  int                        handle;
  int                        conType;
  int                        linkNb;
  int                        boardNb;
  int                        boardId;
  std::string                boardAddress;

  CAEN_DGTZ_DPPFirmware_t    firmType;
  CAEN_DGTZ_ConnectionType   connType;
  CAEN_DGTZ_BoardInfo_t      boardInfo;  

  CAENBoardInfo( ){;};
  CAENBoardInfo( int cTyp, int lNb, int bNb, int bId, std::string bAdd ){

    switch ( cTyp ){
    case CAEN_DGTZ_USB:
      connType = CAEN_DGTZ_USB;
      break;
    case CAEN_DGTZ_OpticalLink:
      connType = CAEN_DGTZ_OpticalLink;
      break;
    default:
      std::cout << "Connection type not know." << std::endl;
    }
    
    conType      = cTyp;
    linkNb       = lNb;
    boardNb      = bNb;
    boardAddress = bAdd;
    boardId      = bId;
    handle       = 0;
    
  };
  
};

#endif // CAENBOARDINFO_H_INCLUDED