#include <QDebug>

#include <iostream>
#include <fstream>
#include <sstream>

#include "CAENBoards.h"

#include "InputFormDialog.h"

CAENBoards::CAENBoards( )
{

}

CAENBoards::CAENBoards( const CAENBoards& caenBoards )
{
  this->info = caenBoards.info;
  this->config = caenBoards.config;
}

CAENBoards::~CAENBoards( )
{
  int error;
  for( auto& it : info ){
    error = (int)CAEN_DGTZ_CloseDigitizer( it.second.handle );
    if( error != 0 ){
      emitMessage( "Connection error. Impossible to close the board " + std::to_string(it.first) );
    }
  }

}

void CAENBoards::emitMessage( std::string msg )
{
  fErrorDialog = new QDialog( );
  fErrorDialog->setAttribute( Qt::WA_DeleteOnClose );
  fErrorLabel = new QLabel( msg.c_str() );
  fErrorButton = new QPushButton( fErrorDialog );
  QObject::connect( fErrorButton, &QPushButton::clicked, fErrorDialog, &QDialog::accept );
  fErrorLayout = new QVBoxLayout( fErrorDialog );
  fErrorDialog->setWindowTitle( "Warning" );
  fErrorButton->setText( "OK" );
  fErrorDialog->setLayout( fErrorLayout );    
  fErrorLayout->addWidget( fErrorLabel, 4 );
  fErrorLayout->addWidget( fErrorButton, 10 );
  fErrorDialog->show();
}

bool CAENBoards::connect( int conType, int linkNb, int boardNb, int boardId, std::string boardAddress, int handle){

  int bAddress = std::stoul( boardAddress.c_str( ), nullptr, 16 );

  CAEN_DGTZ_ConnectionType connType;
  switch ( conType ){
  case CAEN_DGTZ_USB:
    connType = CAEN_DGTZ_USB;
    break;
  case CAEN_DGTZ_OpticalLink:
    connType = CAEN_DGTZ_OpticalLink;
    break;
  }

  CAENBoardInfo boardInfo( conType, linkNb, boardNb, boardId, boardAddress );
        
  int error = (int)CAEN_DGTZ_OpenDigitizer(connType, linkNb, boardNb, bAddress, &boardInfo.handle);

  if( error != 0 ){
    emitMessage( "Connection error. Impossible to connect to the board." );
    return false;
  }

  error = (int)CAEN_DGTZ_GetInfo( boardInfo.handle, &boardInfo.boardInfo );

  if( error != 0 ){
    emitMessage( "Connection error. Impossible to get board information." );
    return false;
  }

  error = (int)CAEN_DGTZ_GetDPPFirmwareType( boardInfo.handle, &boardInfo.firmType );

  if( error != 0 ){
    emitMessage( "Connection error. Impossible to get DPP firmware information." );
    return false;
  }

  error = (int)CAEN_DGTZ_WriteRegister( boardInfo.handle, 0xEF08, boardNb );

  if( error != 0 ){
    emitMessage( "Connection error. Impossible to set Board ID." );
    return false;
  }

  info[boardId] = boardInfo;

  return true;

}

bool CAENBoards::newProject( std::string dir )
{
  int nboards = QInputDialog::getInt(0, "Number of boards", "Number of boards", 1, 1, 64, 1);

  int i = 0;
  while( i < nboards ){
    if( addBoard( i ) ) i++;
    else return false;
  }

  if( createConfig( dir ) )
    return true;
  else return false;

}

bool CAENBoards::loadProject( std::string fileDir ){

  std::ifstream in( fileDir.c_str( ), std::ifstream::in );
  if( !in.is_open( ) ){
    emitMessage( "Error opening file " + fileDir + ". Can not load RUCaen.conf file." );
    return false;
  }

  info.clear( );
  config.clear( );

  int nBoards, boardId, conType, linkNb, boardNb, chan, handle;
  std::string address;
  std::string boardName, confFile;

  std::string line;
  while( std::getline( in, line ) ) {
    std::string word;
    std::stringstream ss(line);        
    ss >> word;

    if( word == "Board" ){
      ss >> boardName >> boardId >> address >> conType >> linkNb >> boardNb;
      if( !connect( conType, linkNb, boardNb, boardId, address, handle ) ){
        return false;
      }
    }
    
    else if( word == "BoardConf" ){
      ss >> boardId >> confFile;
      CAENConfig conf( info[boardId], confFile);
      config.insert( {boardId, conf} );
      config[boardId].load( );
    }

  }

  if( !info.size( ) || !config.size( ) ||
      ( info.size( ) != config.size( ) ) ){
    emitMessage( "Invalid RUCaen.conf file. Project not loaded." );
    return false;
  }

  return true;

}

void CAENBoards::saveProjectRU( std::string fileDir ){

  std::ofstream out( fileDir.c_str( ) );

  out << "NumberOfBoards\t" << info.size( ) << std::endl;
  out << std::endl;

  std::map<int,CAENBoardInfo>::iterator itB;
  for( itB = info.begin( ); itB != info.end( ); itB++ ){
    out << "Board\t" << itB->second.boardInfo.ModelName << "\t" 
                     << itB->second.boardId             << "\t" 
                     << itB->second.boardAddress        << "\t"
                     << itB->second.conType             << "\t" 
                     << itB->second.linkNb              << "\t" 
                     << itB->second.boardNb             << std::endl;
  }

  std::map<int,CAENConfig>::iterator itC;
  for( itC = config.begin( ); itC != config.end( ); itC++ ){
    out << "BoardConf\t" << itC->first                  << "\t" 
                         << itC->second.getFilename( ) << std::endl;

  }

  out.close( );

}

void CAENBoards::saveProjectLF( std::string fileDir, std::string graphiteHostname = "", int graphitePort = 0 ){

  std::ofstream out( fileDir.c_str( ) );

  out << "SaveDataDir\t." << std::endl;
  out << std::endl;

  std::map<int,CAENBoardInfo>::iterator itB;
  for( itB = info.begin( ); itB != info.end( ); itB++ ){
    out << "SpecPrefix\t" << itB->second.boardId             << "\t"
                          << itB->second.boardInfo.ModelName << std::endl;
  }

  out << std::endl;

  int nsPerTimeStamp = 1;  // TO DO  
  int nsPerSample    = 1;  // TO DO
  int chanOffset     = 0;
  std::string dppType = "";
  for( itB = info.begin( ); itB != info.end( ); itB++ ){
    
    switch( itB->second.firmType ){
      case( CAEN_DGTZ_DPPFirmware_PHA ):
        dppType = "DPP_PHA";
        break;
      case( CAEN_DGTZ_DPPFirmware_PSD ):
        dppType = "DPP_PSD";
        break;
      default:
        dppType = "STD";
        break;
    }

    out << "Board\t" << itB->second.boardId             << "\t" 
                     << itB->second.boardInfo.ModelName << "\t"
                     << dppType                         << "\t"
                     << itB->second.boardInfo.Channels  << "\t" 
                     << chanOffset                      << "\t" 
                     << nsPerTimeStamp                  << "\t"
                     << nsPerSample                     << std::endl;
    chanOffset += itB->second.boardInfo.Channels;
  }

  if( graphiteHostname != "" && graphitePort != 0 ){
    out << std::endl;
    out << "GraphiteServer\t" << graphiteHostname << "\t" << graphitePort << std::endl;    
  }

  out.close( );

}

bool CAENBoards::addBoard( int boardId ){

  InputFormDialog::FormData data;
  data["Connection Type"] = QStringList() << "USB" << "Optical Link";
  data["Link Number"] = 0;
  data["VME Address"] = "1";

  // Define form options
  InputFormDialog::FormOptions options;
  options.listDisplaysAsRadios = true;
  options.listReturnsIndex = true;

  int handle                =  0;
  int conType               = -1;
  int linkNb                = -1;
  std::string boardAddress  = "";
  int bAddress              = -1;

  CAEN_DGTZ_ConnectionType connType;

  if( InputFormDialog::getInput("Add Board", data, options) ){
    conType      = data.at<int>("Connection Type");
    linkNb       = data.at<int>("Link Number");
    boardAddress = data.at<QString>("VME Address").toStdString( );

    if( !connect( conType, linkNb, boardId, boardId, boardAddress, handle ) ){
      return false;
    }

  }

  if( conType == -1 || linkNb == -1 || boardAddress == "" ){
    return false;
  }

  return true;

}

bool CAENBoards::createConfig( std::string dir ){

  std::string fileName;
  std::map<int,CAENBoardInfo>::iterator it;
  for( it = info.begin(); it != info.end(); it++ ){
    fileName = dir + "/" + it->second.boardInfo.ModelName + "_" + std::to_string( it->first ) + ".json";
    CAENConfig conf( it->second, fileName );
    config.insert( {it->first,conf} );
    if( !config[it->first].create() ){
      emitMessage( "Cannot create JSON file for board " + std::to_string( it->first ) );
      return false;
    }

    config[it->first].load( );
  }

  return true;

}

bool CAENBoards::save( ){

  std::map<int,CAENConfig>::iterator it;
  for( it = config.begin(); it != config.end(); it++ )
    if( !it->second.save( ) )
      return false;
  
  return true;

}

void CAENBoards::load( ){

  std::map<int,CAENConfig>::iterator it;
  for( it = config.begin(); it != config.end(); it++ )
    it->second.load( );

}

void CAENBoards::loadConfFile( std::string confFile ){

  InputFormDialog::FormData data;
  QStringList boardList;

  for( auto it = info.begin(); it != info.end(); it++ )
    boardList << QString::fromStdString( std::to_string( it->first ) );

  data["Board ID"] = boardList;

  // Define form options
  InputFormDialog::FormOptions options;
  options.listDisplaysAsRadios = true;
  options.listReturnsIndex = false;

  int bIdx = 0;
  if (InputFormDialog::getInput("Add Board", data, options)){
    bIdx = data.at<QString>("Board ID").toInt( );

    if( config.find( bIdx ) == config.end() ){
      emitMessage("The board could not be found!");
    } else {
      config[bIdx].setFilename( confFile );
      config[bIdx].load( );
    }

  }
  
}

void CAENBoards::commit( ){

  std::map<int,CAENConfig>::iterator it;
  for( it = config.begin(); it != config.end(); it++ )
    it->second.commit( );

}

void CAENBoards::update( ){

  std::map<int,CAENConfig>::iterator it;
  for( it = config.begin(); it != config.end(); it++ )
    it->second.update( );

}
