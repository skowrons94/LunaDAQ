#include <iostream>
#include <fstream>
#include <memory>
#include <array>
#include <utility>
#include <sys/stat.h>

#include <sys/time.h>

#include <stdlib.h>

#include "ProjectManager.h"

ProjectManager::ProjectManager( )
{  
  this->name = "";
}

ProjectManager::~ProjectManager( )
{

}

bool ProjectManager::SetupProjectDirectory( std::string str )
{
  dir = str;
    
  std::string cmd;
  cmd  = "cp -r /opt/LunaDAQ/conf/ ";
  cmd += dir; 
  if( std::system( cmd.c_str( ) ) )
    return false;

  confFileRU = dir + "/conf/RUCaen.conf";
  confFileLF = dir + "/conf/LocalFilter.conf";

  return true;

}

void ProjectManager::AddRUData( int n, std::string str )
{
  RUDataMap[n] = str;
}

void ProjectManager::AddLFData( int n, std::string str )
{
  LFDataMap[n] = str;
}

void ProjectManager::DumpRUData( )
{

  for( auto const& iter : RUDataMap )
    std::cout << iter.first << "  " << iter.second << std::endl;
  
}

void ProjectManager::DumpLFData( )
{

  for( auto const& iter : LFDataMap )
    std::cout << iter.first << "  " << iter.second << std::endl;
  
}

std::string ProjectManager::Exec( const char* cmd )
{

  std::array<char, 128> buffer;
  std::string result;
  
  std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
  
  while (!feof(pipe.get())) {
    if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
      result += buffer.data();
  }

  return result;
  
}

void ProjectManager::LinkRUData( )
{

  std::string cmd;
  std::string data;
  std::string dataName = "run" + std::to_string( nRun ) + ".ru";

  // First we ls all files in XDAQ data directory and we take the most
  // recent one that contains "ru: extension. Then we link it to the data directory.

  cmd = "ls -Artd " + dataDir + "/ru* | tail -n 1";
  
  data = Exec( cmd.c_str( ) );
  data.erase(std::remove(data.begin(), data.end(), '\n'), data.end());

  cmd = "mv " + data + " " + dataDir + "/" + dataName;
  system( cmd.c_str( ) );

  AddRUData( nRun, dataName );

}

void ProjectManager::LinkLFData( )
{

  std::string cmd;
  std::string data;
  std::string dataName = "run" + std::to_string( nRun ) + ".lf";

  // First we ls all files in XDAQ data directory and we take the most
  // recent one that contains "lf" extension. Then we link it to the data directory.
  
  cmd = "ls -Artd " + dataDir + "/lf* | tail -n 1";
  
  data = Exec( cmd.c_str( ) );
  data.erase(std::remove(data.begin(), data.end(), '\n'), data.end());

  cmd = "mv " + data + " " + dataDir + "/" + dataName;
  system( cmd.c_str( ) );

  AddLFData( nRun, dataName );

}

void ProjectManager::RegisterStartTime( )
{
  start = time(NULL);  // UNIX time
}

void ProjectManager::RegisterStopTime( )
{
  stop = time(NULL);  // UNIX time
}

void ProjectManager::AddTimeData( )
{
  TimeDataMap[nRun] = std::pair<unsigned long int, unsigned long int>(start, stop);
}

