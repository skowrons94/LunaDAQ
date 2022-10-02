#ifndef RUREADER_H
#define RUREADER_H

#include <vector>
#include <string>
#include <fstream>

#include <TH1.h>
#include <TTree.h>
#include <TFile.h>
#include <TGraph.h>
#include <TObject.h>
#include <TMultiGraph.h>

#include <QObject>
#include <QString>

#include "DataFrame.h"

class RUReader : public QObject {
  
Q_OBJECT

public:
  
  RUReader( std::map<int,std::string> name, std::string inFile, std::string outFile );
  ~RUReader( );

  void start( );
  void stop( );

  void InitializeROOT( );
  void InitializeWave( int bIdx, uint32_t length, bool fDual );

  void UnpackHeader( uint32_t* inpBuffer, uint32_t& aggLength, uint32_t& board, std::bitset<8>& channelMask, uint64_t& offset);
  void UnpackPHA( uint32_t* inpBuffer, uint32_t& board, std::bitset<8>& channelMask, uint64_t& offset );
  void UnpackPSD( uint32_t* inpBuffer, uint32_t& board, std::bitset<8>& channelMask, uint64_t& offset );

  void UnpackWave( uint32_t* inpBuffer, uint32_t& board, DataFrame& dataForm, int pos );

  int ReadHeader( std::ifstream& input );
  void ReadData( std::ifstream& input, uint64_t pos );

  void Read( );
  void Write( );
  
private:
  
  // To stop the thread
  int stopCall           = 1;

  std::string in  = "";
  std::string out = "";

  std::map<int,CAEN_DGTZ_DPPFirmware_t>  dgtzDppType;
  std::map<int,std::string>              dgtzName; 
  std::map<int,int>                      dgtzChans;

  std::map<int,DataFrame> frames;
  std::map<int,uint32_t>  RO;
  
  std::map<int,TTree*>  fTree;                   

  std::map<int,bool>       fPu;
  std::map<int,uint16_t>   fChannel;
  std::map<int,uint64_t>   fTimeStamp;
  std::map<int,uint16_t>   fEnergies;
  std::map<int,uint16_t>   fQLong;
  std::map<int,uint16_t>   fQShort;
  std::map<int,uint32_t>   fExtras;
  std::map<int,uint32_t>   fExtras2;
  
  std::map<int,std::vector<TH1F*>>   fEnergy;    // Container for histograms
  std::map<int,std::vector<TH1F*>>   fQshort;    // Container for histograms
  std::map<int,std::vector<TH1F*>>   fQlong;     // Container for histograms

  std::map<int,std::vector<short>>   fWave1;     // Container for waveform 1
  std::map<int,std::vector<short>>   fWave2;     // Container for waveform 2
  std::map<int,std::vector<bool>>    fDigital11;  // Container for digital probe 1
  std::map<int,std::vector<bool>>    fDigital12;  // Container for digital probe 2
  std::map<int,std::vector<bool>>    fDigital21;  // Container for digital probe 1
  std::map<int,std::vector<bool>>    fDigital22;  // Container for digital probe 2

  TFile* fileOut = nullptr;

  // Miscellaneous
  std::map<int,bool> fWave;
  bool fUpdateWave = true;

signals:
  void updateProgress( int progress );

  
};

#endif // SPYSERVER_H
