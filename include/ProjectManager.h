#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <map>
#include <string>
#include <utility>
#include <stdio.h>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/map.hpp> 
#include <boost/serialization/string.hpp> 
#include <boost/serialization/list.hpp>

#include "CAENBoards.h"

// Class to manage the Projects files. It can create, open and save configurations.
// Need more testing, it seemed troublesome but in principle should work.
// It is based on the boost serialization procedure to save/load the class each time.

class ProjectManager
{

public:
  ProjectManager( );
  ~ProjectManager( );

  std::string         name;        // Name of the project
  std::string          dir;        // Directory of the project
  std::string   confFileRU;        // RU configuration file
  std::string   confFileLF;        // RU configuration file
  std::string      dataDir;        // Data directory of the project

  std::string   graphiteHostname;  // Hostname of the graphite server
  int           graphitePort;      // Port of the graphite server

  int nRun;                      // Number of the current run

  bool SetupProjectDirectory( std::string str );

  void AddRUData( int n, std::string str );
  void AddLFData( int n, std::string str );

  void LinkRUData( ); // These are called at the end of each acquisition run
  void LinkLFData( ); // to link the XDAQ data to the selected Data folder.

  void DumpRUData( ); // Functions to dump the XDAQ data directories for 
  void DumpLFData( ); // each run.

  std::string Exec( const char* cmd ); // Function to execute bash command.
                                       // Used when linking data files.

  void RegisterStartTime( );           // Start time and stop time of each
  void RegisterStopTime( );            // acquisition is registered in 
  void AddTimeData( );                 // UNIX time format.
  
private:

  // In the following maps of RunNumber and data directories are stored for each run.
  // In addition the pair of start and stop time for each run is stored.
  
  std::map<int,std::string>                                              RUDataMap;
  std::map<int,std::string>                                              LFDataMap;
  std::map<int,std::pair<unsigned long int, unsigned long int>>        TimeDataMap;

  unsigned long int start;             // Buffers for start and stop time 
  unsigned long int  stop;             // of current data acquistion

  // Class and template definition for the serialization of data.
  
  friend class boost::serialization::access;
  
  template<class Archive>
  void serialize(Archive &a, const unsigned version){
    a & name & dir & confFileRU & confFileLF & dataDir & graphiteHostname & graphitePort & nRun & RUDataMap & LFDataMap & TimeDataMap;
  }

};


#endif // PROJECTMANAGER_H
