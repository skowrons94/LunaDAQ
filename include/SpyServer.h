#ifndef SPYSERVER_H
#define SPYSERVER_H

#include <bitset>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <TFile.h>
#include <TH1.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TSocket.h>
#include <TServerSocket.h>
#include <TMonitor.h>
#include <TFrame.h>
#include <TMessage.h>

#include <QString>

#include <boost/thread.hpp>

#include "XDAQSpy.h"

#include "CAENFrames.h"
#include "CAENBoards.h"

#include "DataFrame.h"
#include "Utils.h"

// This class has double functionality:
// 1) It connect to XDAQSpy to get the data from current acquistion
//    and store them so WaveWorker and HistoWorker can retrieve it.
// 2) It creates an another socket so the data can be obtained from
//    other PCs (this part is still not fully implemented).
// The class creates a new thread in order to obtain and unpack the buffer.
// The connection with the XDAQ spy is handled by XDAQSpy class!

struct CAENBoardInfo;

class SpyServer : public QObject {
    
    Q_OBJECT

public:
  
  SpyServer( CAENBoards* caenBoards );
  ~SpyServer( );

  void InitializeROOT( );

  bool Connect( );                                         // Triggers the connection for XDAQSpy
  void Disconnect( );                                      // Triggers the disconnection for XDAQSpy

  void Reset( );                                           // Resets the histograms
  void Save( std::string fileName );                       // Saves the histograms

  uint32_t ReadHeader( char* buffer );                          // Reads the header in the first buffer

  void UnpackHeader( uint32_t* inpBuffer, uint32_t& aggLength, uint32_t& board, std::bitset<8>& channelMask, uint32_t& offset );
  void UnpackPHA( uint32_t* inpBuffer, uint32_t& board, std::bitset<8>& channelMask, uint32_t& offset );
  void UnpackPSD( uint32_t* inpBuffer, uint32_t& board, std::bitset<8>& channelMask, uint32_t& offset );

  // Functions used by the boost::thread
  
  void Start( );
  void Stop( );

  void ReadNextEvent( );
  void GetNextEvent( );

  void FillGraphs( uint32_t* inpBuffer, uint32_t board, uint32_t chan, DataFrame dataForm );  // Fills the TGraphs for the waves

  // Public functions for the access from HistoWorker and WaveWorker
  
  TH1F*         GetHistogram( int bIdx, int cIdx );

  void          UpdateWave( );                      // Changes the fWaveUpdate
  void          UpdateGraphs( );                    // Updates the style of the TGraphs

  TGraph*       GetAnalogProbe1( int bIdx, int cIdx );
  TGraph*       GetAnalogProbe2( int bIdx, int cIdx );

  TGraph*       GetDigitalProbe1( int bIdx, int cIdx );
  TGraph*       GetDigitalProbe2( int bIdx, int cIdx );

  TMultiGraph*  GetWaves( int bIdx, int cIdx );
  TMultiGraph*  GetAnalogWaves( int bIdx, int cIdx );
  TMultiGraph*  GetDigitalWaves( int bIdx, int cIdx );

signals:
  void waveReady(int,int);
  
private:

  // The TGraph is used not only for the AnalogProbes but also for the Digital Probes.
  // If TLine is used, it goes out of axes, thus looks kind of ugly.

  std::map<int,std::vector<TH1F*>>         fEnergy;         // Container for histograms
  std::map<int,std::vector<TH1F*>>         fQshort;         // Container for histograms
  std::map<int,std::vector<TH1F*>>         fQlong;          // Container for histograms
  std::map<int,std::vector<TGraph*>>       fAnalogProbe1;   // Container for Analog Probe 1
  std::map<int,std::vector<TGraph*>>       fAnalogProbe2;   // Container for Analog Probe 2
  std::map<int,std::vector<TMultiGraph*>>  fAnalogWaves;    // Container for all analog probes
  std::map<int,std::vector<TGraph*>>       fDigitalProbe1;  // Container for Digital Probe 1
  std::map<int,std::vector<TGraph*>>       fDigitalProbe2;  // Container for Digital Probe 2
  std::map<int,std::vector<TMultiGraph*>>  fDigitalWaves;   // Container for all digital probes
  std::map<int,std::vector<TMultiGraph*>>  fWaves;          // Container for all the probes

  // DataFrame to read the buffer depending on the board.

  std::map<int,DataFrame>  fDataFrame;      // Container for the data frames
  std::map<int,int>        fRO;             // Container for Roll Over flags

  // Connects to the XDAQ spy
  
  XDAQSpy* xdaq;

  // Board information

  CAENBoards* boards;

  // Thread variables
  
  int startCall         = 0;
  int stopCall          = 1;
  boost::thread*  spyThread;

  // Wave thread variables
  uint32_t* waveBuffer;
  boost::thread*  waveThread = nullptr;

  // It is used by the Workers to send a signal to update the Probes only once.
  
  std::map<int,std::map<int,bool>> fWaveUpdateMap;

  // From sampling speed to nanoseconds
  
  const int toNs = 2;
  int fVerbose   = 0;
  
};

#endif // SPYSERVER_H
