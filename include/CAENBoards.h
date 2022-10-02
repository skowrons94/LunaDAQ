#ifndef CAENBOARDS_H_INCLUDED
#define CAENBOARDS_H_INCLUDED

#include <iostream>
#include <string>
#include <map>

#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>

#include <CAENDigitizer.h>
#include <CAENDigitizerType.h>

#include "CAENBoardInfo.h"
#include "CAENConfig.h"

class CAENServer;

class CAENBoards {

  public:

    CAENBoards( );
    CAENBoards( const CAENBoards& caenBoards );
    ~CAENBoards( );

    bool save( );
    void load( );
    void update( );
    void commit( );

    void loadConfFile( std::string confFile );

    bool newProject( std::string dir );
    bool loadProject( std::string fileDir );
    
    void saveProjectRU( std::string fileDir );
    void saveProjectLF( std::string fileDir, std::string graphiteHostname, int graphitePort );

    bool connect( int conType, int linkNb, int boardNb, int boardId, std::string boardAddress, int handle);

    void emitMessage( std::string msg );

    std::map<int,CAENBoardInfo>  info;
    std::map<int,CAENConfig>     config;
   
  private:

    bool createConfig( std::string dir );
    bool addBoard( int boardId );

    QDialog*           fErrorDialog    = nullptr;
    QLabel*            fErrorLabel     = nullptr;
    QPushButton*       fErrorButton    = nullptr;
    QVBoxLayout*       fErrorLayout    = nullptr;

};

#endif
