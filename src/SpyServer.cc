#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <bitset>
#include <sstream>

#include "SpyServer.h"

SpyServer::SpyServer( CAENBoards* caenBoards ) : boards( caenBoards )
{

  // Creating the DataFrames
  std::map<int, CAENBoardInfo>::iterator it;
  for( it = boards->info.begin( ); it != boards->info.end( ); it++ ){
    fDataFrame[it->first] = DataFrame( it->second.firmType, it->second.boardInfo.ModelName );
    fDataFrame[it->first].Build( );
    fRO[it->first] = 0;
    for( int chan = 0; chan < it->second.boardInfo.Channels; chan++ ){
      fWaveUpdateMap[it->first][chan] = true;
    }
  }

  // Initializing the histograms  
  InitializeROOT( );

  // Initializing XDAQSpy
  xdaq = new XDAQSpy( );
  
}

SpyServer::~SpyServer(){

  // Clean up

  if( startCall ){ 
    startCall = 0;
    stopCall  = 0;
    spyThread->join();
  }

  for( auto it = fEnergy.begin(); it != fEnergy.end(); it++ ){
    for( auto jt = it->second.begin(); jt != it->second.end(); jt++ ){
      delete *jt;
    }
  }

  for( auto it = fAnalogProbe1.begin(); it != fAnalogProbe1.end(); it++ ){
    for( auto jt = it->second.begin(); jt != it->second.end(); jt++ ){
      delete *jt;
    }
  }

  for( auto it = fAnalogProbe2.begin(); it != fAnalogProbe2.end(); it++ ){
    for( auto jt = it->second.begin(); jt != it->second.end(); jt++ ){
      delete *jt;
    }
  }

  for( auto it = fQlong.begin(); it != fQlong.end(); it++ ){
    for( auto jt = it->second.begin(); jt != it->second.end(); jt++ ){
      delete *jt;
    }
  }

  for( auto it = fQshort.begin(); it != fQshort.end(); it++ ){
    for( auto jt = it->second.begin(); jt != it->second.end(); jt++ ){
      delete *jt;
    }
  }

  for( auto it = fDigitalProbe1.begin(); it != fDigitalProbe1.end(); it++ ){
    for( auto jt = it->second.begin(); jt != it->second.end(); jt++ ){
      delete *jt;
    }
  }

  for( auto it = fDigitalProbe2.begin(); it != fDigitalProbe2.end(); it++ ){
    for( auto jt = it->second.begin(); jt != it->second.end(); jt++ ){
      delete *jt;
    }
  }

  delete xdaq;
  
}

void SpyServer::InitializeROOT( ){
  
  int bIdx;
  std::map<int,CAENBoardInfo>::iterator it;
  for( it = boards->info.begin(); it != boards->info.end(); it++ ){

    bIdx = it->first;

    int chans = boards->info[bIdx].boardInfo.Channels;

    for(UInt_t j = 0 ; j < chans; j++){
      fEnergy[bIdx].push_back(new TH1F(Form("hEnergy_%i_%i",bIdx,j),
			                                 Form("Energy Board %i Channel %i ",bIdx,j),
			                                 32768,0,32768));
      fQshort[bIdx].push_back(new TH1F(Form("hQshort_%i_%i",j),
			                                 Form("Qshort Board %i Channel %i",bIdx,j),
			                                 32768,0,32768));
      fQlong[bIdx].push_back(new TH1F(Form("hQlong_%i_%i",j),
			                                Form("Qlong Board %i Channel %i",bIdx,j),
			                                32768,0,32768));
      fEnergy[bIdx].back()->SetXTitle("Energy [channels]");
      fEnergy[bIdx].back()->SetYTitle("Counts / channel");
      fQshort[bIdx].back()->SetXTitle("Qshort [channels]");
      fQshort[bIdx].back()->SetYTitle("Counts / channel");
      fQlong[bIdx].back()->SetXTitle("Qlong [channels]");
      fQlong[bIdx].back()->SetYTitle("Counts / channel");
    }

    // Initializing the graphs

    for(UInt_t j = 0 ; j < chans; j++){
      fAnalogProbe1[bIdx].push_back(new TGraph( ));
      fAnalogProbe2[bIdx].push_back(new TGraph( ));
      fDigitalProbe1[bIdx].push_back(new TGraph( ));
      fDigitalProbe2[bIdx].push_back(new TGraph( ));
      fAnalogProbe1[bIdx].back()->SetPoint( 0, 0, 0 );
      fAnalogProbe2[bIdx].back()->SetPoint( 0, 0, 0 );
      fDigitalProbe1[bIdx].back()->SetPoint( 0, 0, 0 );
      fDigitalProbe2[bIdx].back()->SetPoint( 0, 0, 0 );
    }

    // Initializing waves (might be useful?)

    for(UInt_t j = 0 ; j < chans; j++){
      fAnalogWaves[bIdx].push_back(new TMultiGraph( ));
      fDigitalWaves[bIdx].push_back(new TMultiGraph( ));
      fWaves[bIdx].push_back(new TMultiGraph( ));
      fAnalogWaves[bIdx][j]->Add( fAnalogProbe1[bIdx][j] );
      fAnalogWaves[bIdx][j]->Add( fAnalogProbe2[bIdx][j] );
      fDigitalWaves[bIdx][j]->Add( fDigitalProbe1[bIdx][j] );
      fDigitalWaves[bIdx][j]->Add( fDigitalProbe2[bIdx][j] );
      fWaves[bIdx][j]->Add( fAnalogProbe1[bIdx][j] );
      fWaves[bIdx][j]->Add( fAnalogProbe2[bIdx][j] );
      fWaves[bIdx][j]->Add( fDigitalProbe1[bIdx][j] );
      fWaves[bIdx][j]->Add( fDigitalProbe2[bIdx][j] );
      fWaves[bIdx].back()->GetXaxis( )->SetTitle("Sample Number");
      fAnalogWaves[bIdx].back()->GetXaxis( )->SetTitle("Sample Number");
    }

  }

}

void SpyServer::Reset( ){
  
  // Resetting the histograms
  
  for( auto it = fEnergy.begin(); it != fEnergy.end(); it++ ){
    for( auto jt = it->second.begin(); jt != it->second.end(); jt++ ){
      (*jt)->Reset( "ICESM" );
    }
  }
  
  for( auto it = fQlong.begin(); it != fQlong.end(); it++ ){
    for( auto jt = it->second.begin(); jt != it->second.end(); jt++ ){
      (*jt)->Reset( "ICESM" );
    }
  }
  
  for( auto it = fQshort.begin(); it != fQshort.end(); it++ ){
    for( auto jt = it->second.begin(); jt != it->second.end(); jt++ ){
      (*jt)->Reset( "ICESM" );
    }
  }

}

void SpyServer::Save( std::string fileName ){

  TFile* file = new TFile( fileName.c_str( ), "RECREATE" );

  std::string dir;
  for( auto it = fEnergy.begin(); it != fEnergy.end(); it++ ){
    for( auto jt = it->second.begin(); jt != it->second.end(); jt++ ){
      (*jt)->Write( );
    }
  }
  
  for( auto it = fQlong.begin(); it != fQlong.end(); it++ ){
    for( auto jt = it->second.begin(); jt != it->second.end(); jt++ ){
      (*jt)->Write( );
    }
  }
  
  for( auto it = fQshort.begin(); it != fQshort.end(); it++ ){
    for( auto jt = it->second.begin(); jt != it->second.end(); jt++ ){
      (*jt)->Write( );
    }
  }

  file->Close( );

}

bool SpyServer::Connect( ){

  // Connects to the XDAQ spy port.

  int bIdx;
  std::map<int,CAENBoardInfo>::iterator it;
  for( it = boards->info.begin(); it != boards->info.end(); it++ ){
    bIdx = it->first;

    for( auto h : fEnergy[bIdx] )
      h->Reset("ICESM");
  
    for( auto h : fQshort[bIdx] )
      h->Reset("ICESM");
  
    for( auto h : fQlong[bIdx]  )
      h->Reset("ICESM");

    for( auto g : fAnalogProbe1[bIdx] )
      g->Set( 0 );
    
    for( auto g : fAnalogProbe2[bIdx] )
      g->Set( 0 );

    for( auto g : fDigitalProbe1[bIdx] )
      g->Set( 0 );

    for( auto g : fDigitalProbe2[bIdx] ) 
      g->Set( 0 ); 

  }

  if( xdaq->Connect( "localhost", 10002 ) )
    return true;

  else 
    return false;

}

void SpyServer::Disconnect( ){

  xdaq->Disconnect( );

}

void SpyServer::Start( ){

  // The connection to the XDAQ spy starts only here.
  // The XDAQ spy socket is created only when the acquisition is active,
  // thus it must be called after the acquistion starts.

  if( xdaq->Connect( "localhost", 10002 ) ){ 

    startCall = 1;
    stopCall  = 0;
    
    spyThread = new boost::thread( &SpyServer::GetNextEvent, this );

  }

}

void SpyServer::Stop( ){

  // It is called just before the acquistion stops.

  if( startCall ){

    startCall = 0;
    stopCall  = 1;
    spyThread->join();
    xdaq->Disconnect( );

  }

}

void SpyServer::UpdateWave( ){

  for( auto& it : fWaveUpdateMap ){
    for( auto& jt : it.second ){
      jt.second = true;
    }
  }

}

TH1F* SpyServer::GetHistogram( int bIdx, int cIdx ){

  switch( boards->info[bIdx].firmType )
  {
    case CAEN_DGTZ_DPPFirmware_PHA:
      return fEnergy[bIdx][cIdx];
  
    case CAEN_DGTZ_DPPFirmware_PSD:
      return fQlong[bIdx][cIdx];
  
    default:
      return fEnergy[bIdx][cIdx];
  }

}

TGraph* SpyServer::GetAnalogProbe1( int bIdx, int cIdx ){

  return fAnalogProbe1[bIdx][cIdx];

}

TGraph* SpyServer::GetAnalogProbe2( int bIdx, int cIdx ){

  return fAnalogProbe2[bIdx][cIdx];

}

TGraph* SpyServer::GetDigitalProbe1( int bIdx, int cIdx ){

  return fDigitalProbe1[bIdx][cIdx];

}

TGraph* SpyServer::GetDigitalProbe2( int bIdx, int cIdx ){

  return fDigitalProbe2[bIdx][cIdx];

}

TMultiGraph* SpyServer::GetAnalogWaves( int bIdx, int cIdx ){

  return fAnalogWaves[bIdx][cIdx];

}

TMultiGraph* SpyServer::GetDigitalWaves( int bIdx, int cIdx ){

  return fDigitalWaves[bIdx][cIdx];

}

TMultiGraph* SpyServer::GetWaves( int bIdx, int cIdx ){
  
  return fWaves[bIdx][cIdx];

}

void SpyServer::UpdateGraphs( ){

  int bIdx;
  std::map<int,CAENBoardInfo>::iterator it;
  for( it = boards->info.begin(); it != boards->info.end(); it++ ){
    bIdx = it->first;

    for(UInt_t j = 0 ; j < 8; j++){
      fAnalogProbe1[bIdx][j]->SetLineColor( 1 );
      fAnalogProbe2[bIdx][j]->SetLineColor( 4 );
      fDigitalProbe1[bIdx][j]->SetLineColor( 3 );
      fDigitalProbe2[bIdx][j]->SetLineColor( 2 );

      fWaves[bIdx][j]->SetMinimum( fAnalogProbe1[bIdx][j]->GetHistogram( )->GetMinimum( )*0.95 );
      fWaves[bIdx][j]->SetMaximum( fAnalogProbe1[bIdx][j]->GetHistogram( )->GetMaximum( )*1.05 );   
    }

  }

}

void SpyServer::FillGraphs( uint32_t* inpBuffer, uint32_t board, uint32_t chan, DataFrame dataForm ){

  uint16_t numSamples = dataForm.GetConfig( "NumSamples" )/2 - 1;

  std::pair<int,int> formatSample = dataForm.GetFormat( "Sample" );
  std::pair<int,int> formatDP1    = dataForm.GetFormat( "DP1" );
  std::pair<int,int> formatDP2    = dataForm.GetFormat( "DP2" );

  short temp;
  for( int idx = 0; idx < numSamples; ++idx ){
    if( dataForm.CheckEnabled( "DT" ) ){
      temp = static_cast<short>(project_range( formatSample.first   , formatSample.second   , std::bitset<32>(inpBuffer[idx])).to_ulong());
      fAnalogProbe1[board][chan]->SetPoint( idx, idx, temp );
      temp = static_cast<short>(project_range( formatSample.first+16, formatSample.second+16, std::bitset<32>(inpBuffer[idx])).to_ulong());
      fAnalogProbe2[board][chan]->SetPoint( idx, idx, temp );
      temp = static_cast<bool>(project_range(  formatDP1.first      , formatDP1.second      , std::bitset<32>(inpBuffer[idx])).to_ulong());
      fDigitalProbe1[board][chan]->SetPoint( idx, idx/2, temp );
      temp = static_cast<bool>(project_range(  formatDP1.first+16   , formatDP1.second+16   , std::bitset<32>(inpBuffer[idx])).to_ulong());
      fDigitalProbe1[board][chan]->SetPoint( idx, idx, temp );
      temp = static_cast<bool>(project_range(  formatDP2.first      , formatDP2.second      , std::bitset<32>(inpBuffer[idx])).to_ulong());
      fDigitalProbe2[board][chan]->SetPoint( idx, idx/2, temp );
      temp = static_cast<bool>(project_range(  formatDP2.first+16   , formatDP2.second+16   , std::bitset<32>(inpBuffer[idx])).to_ulong());
      fDigitalProbe2[board][chan]->SetPoint( idx, idx, temp );
    }
    else{
      temp = static_cast<short>(project_range( formatSample.first   , formatSample.second   , std::bitset<32>(inpBuffer[idx])).to_ulong());
      fAnalogProbe1[board][chan]->SetPoint( 2*idx, 2*idx, temp );
      temp = static_cast<short>(project_range( formatSample.first+16, formatSample.second+16, std::bitset<32>(inpBuffer[idx])).to_ulong());
      fAnalogProbe1[board][chan]->SetPoint( 2*idx+1, 2*idx+1, temp );
      temp = static_cast<short>(project_range( formatDP1.first      , formatDP1.second      , std::bitset<32>(inpBuffer[idx])).to_ulong());
      fDigitalProbe1[board][chan]->SetPoint( 2*idx, 2*idx, temp );
      temp = static_cast<short>(project_range( formatDP1.first+16   , formatDP1.second+16   , std::bitset<32>(inpBuffer[idx])).to_ulong());
      fDigitalProbe1[board][chan]->SetPoint( 2*idx+1, 2*idx+1, temp );
      temp = static_cast<short>(project_range( formatDP2.first      , formatDP2.second      , std::bitset<32>(inpBuffer[idx])).to_ulong());
      fDigitalProbe2[board][chan]->SetPoint( 2*idx, 2*idx, temp );
      temp = static_cast<short>(project_range( formatDP2.first+16   , formatDP2.second+16   , std::bitset<32>(inpBuffer[idx])).to_ulong());
      fDigitalProbe2[board][chan]->SetPoint( 2*idx+1, 2*idx+1, temp );
    }

  }

  UpdateGraphs( );
  emit waveReady(board,chan);

}

void SpyServer::UnpackHeader( uint32_t* inpBuffer, uint32_t& aggLength, uint32_t& board, std::bitset<8>& channelMask, uint32_t& offset ){

  // Aggregate buffer has a header containing different information
  aggLength                  = inpBuffer[0+offset]&0x0FFFFFFF;
  board                      = (inpBuffer[1+offset]&0xF8000000)>>27;
  uint32_t boardFailFlag     = (inpBuffer[1+offset]&0x4000000)<<5 ;
  channelMask                = inpBuffer[1+offset]&0xFF;
  uint32_t aggregateCounter  = inpBuffer[2+offset]&0x7FFFFF;
  uint32_t aggregateTimeTag  = inpBuffer[3+offset];

}

void SpyServer::UnpackPHA( uint32_t* inpBuffer, uint32_t& board, std::bitset<8>& channelMask, uint32_t& offset ){

  // We need to reconstruct the channel number from the couple id for the 
  // 16 channels boards
  int index = 0;
  uint8_t couples[8]={0,0,0,0,0,0,0,0};
  for(uint16_t bit = 0 ; bit < 8 ; bit++){ 
    if(channelMask.test(bit)){
      couples[index]=bit;
      index++;
    }
  }

  // Getting the DataForm
  DataFrame dataForm = fDataFrame[board];
  std::pair<int,int> format;
  uint32_t pos, wavePos;

  // Variables to store the data
  uint32_t startingPos         = 4 + offset;
  uint32_t coupleAggregateSize = 0;
  uint32_t extras              = 0;
  uint32_t extras2             = 0;    
  uint16_t energy              = 0;
  uint64_t tstamp              = 0;
  uint16_t fineTS              = 0;
  uint32_t flags               = 0;
  uint16_t numSamples          = 0; 
  bool pur                     = false;
  bool satu                    = false;
  bool lost                    = false;

  // Here starts the real decoding of the data for the channels
  uint32_t chanNum = 0; 
  for(uint8_t cpl = 0 ; cpl < channelMask.count() ; cpl++){

    // Channel aggregate size is board dependant
    pos    = startingPos;
    format = dataForm.GetFormat( "Size" );
    coupleAggregateSize = project_range( format.first, format.second, std::bitset<32>( inpBuffer[pos] ) ).to_ulong( );

    dataForm.SetDataFormat(inpBuffer[startingPos+1]);
      
    for(uint16_t evt = 0 ; evt < (coupleAggregateSize-2)/dataForm.EvtSize();evt++){
	      
      // PU and Energy seems not to be dependant on the board
      pur      = static_cast<bool>(((inpBuffer[startingPos+dataForm.EvtSize()-1+dataForm.EvtSize()*evt+2])&0x8000)>>15);
      energy   = static_cast<uint16_t>((inpBuffer[startingPos+dataForm.EvtSize()-1+dataForm.EvtSize()*evt+2])&0x7FFF);

      // Time stamp is board dependant
      pos    = startingPos+2+dataForm.EvtSize()*evt;
      format = dataForm.GetFormat( "TS" );
      tstamp = static_cast<uint64_t>(project_range( format.first, format.second, std::bitset<32>(inpBuffer[pos])).to_ulong());

      // Channel number is board dependant
      if( dataForm.CheckFormat( "CH" ) ){
        chanNum  = static_cast<uint32_t>(((inpBuffer[startingPos+2+dataForm.EvtSize()*evt]&0x80000000)>>31)+couples[cpl]*2);
      }
      else{
        chanNum = couples[cpl];
      }

      // Some boards uses the Roll Over flag
      if( dataForm.CheckFormat( "RO" ) ){
        if( ((inpBuffer[startingPos+2+dataForm.EvtSize()*evt]&0x80000000)>>31) ){
          ++fRO[board];
        }
      }

      // Extras size is board dependant
      pos     = startingPos+dataForm.EvtSize()-1+dataForm.EvtSize()*evt+2;
      format  = dataForm.GetFormat( "Extras" );
      extras  = static_cast<uint16_t>(project_range( format.first, format.second, std::bitset<32>( inpBuffer[pos] ) ).to_ulong( ));

      // If Roll Over flag is set, we need to add it
      if( pur && ( energy == 0 ) && ( tstamp == 0 ) && (extras&0002) ){
        ++fRO[board];
      }

      // Adding RO to the Time Stamp
      format = dataForm.GetFormat( "TS" );
      tstamp = ( fRO[board]<<(format.second+1) | tstamp );

      // The Lost and Satu flags seems not to be dependant on the board
	    lost  = (extras&0x20)>>5;
	    satu  = (extras&0x10)>>4;

	    if(dataForm.CheckEnabled( "Extras" )){
	      extras2 = inpBuffer[startingPos+dataForm.EvtSize()-2+dataForm.EvtSize()*evt+2];
	      switch(dataForm.GetConfig( "Extras" )) {
	        case 0: // Extended Time Stamp [31:16] ; baseline*4 [15:0]
	          tstamp = ((uint64_t)(extras2&0xFFFF0000))<<15 | (uint64_t)tstamp;
	          break;
	        case 1: // Extended Time stamp [31:16] ; flags [15:0]
	          tstamp = ((uint64_t)(extras2&0xFFFF0000))<<15 | (uint64_t)tstamp;
	          break;
	        case 2: // Extended Time stamp [31:16] ; Flags [15:10] ; Fine Time Stamp [9:0]
	          fineTS = static_cast<uint16_t>(extras2&0x3FF);
	          tstamp = ((uint64_t)(extras2&0xFFFF0000))<<15 | (uint64_t)tstamp;
	          break;
	        case 4: // Lost Trigger Counter [31:16] ; Total Trigger [15:0]
	          break;
	        case 5: // Positive zero crossing [31:16] ; Negative zero crossing [15:0]
	          break;
	        case 7: // Debug fixed value;
	          break;
	        default:
	          break;
	      }
	  
      }
      else extras2 = 0;

      if( dataForm.CheckEnabled( "Trace" ) && fWaveUpdateMap[board][chanNum] ){
        if( waveThread != nullptr ){
          if( !waveThread->timed_join(boost::posix_time::seconds(0)) ){
            waveThread->join( );
            delete waveThread;
            waveThread = nullptr;
            numSamples = dataForm.GetConfig( "NumSamples" )/2;
            delete waveBuffer;
            waveBuffer = nullptr;
            waveBuffer = new uint32_t[numSamples];
            wavePos = (startingPos+3+dataForm.EvtSize()*evt)*sizeof(uint32_t);
            memcpy(waveBuffer,inpBuffer+wavePos,numSamples*sizeof(uint32_t));
            waveThread = new boost::thread( boost::bind( &SpyServer::FillGraphs, this, waveBuffer, board, chanNum, dataForm ) );
            fWaveUpdateMap[board][chanNum] = false;
          }
        }
        else{
          numSamples = dataForm.GetConfig( "NumSamples" )/2;
          waveBuffer = new uint32_t[numSamples];
          wavePos = (startingPos+3+dataForm.EvtSize()*evt)*sizeof(uint32_t);
          memcpy(waveBuffer,inpBuffer+wavePos,numSamples*sizeof(uint32_t));
          waveThread = new boost::thread( boost::bind( &SpyServer::FillGraphs, this, waveBuffer, board, chanNum, dataForm ) );
          fWaveUpdateMap[board][chanNum] = false;
        }
      }

      // Filling ROOT files
      fEnergy[board][chanNum]->Fill(energy);

    }

    startingPos+=coupleAggregateSize;

  }

}

void SpyServer::UnpackPSD( uint32_t* inpBuffer, uint32_t& board, std::bitset<8>& channelMask, uint32_t& offset ){
  
  // We need to reconstruct the channel number from the couple id for the 
  // 16 channels boards
  int index = 0 ;
  uint8_t couples[8]={0,0,0,0,0,0,0,0};
  for(uint16_t bit = 0 ; bit < 8 ; bit++){ 
    if(channelMask.test(bit)){
      couples[index]=bit;
      index++;
    }
  }

  // Getting the DataForm
  DataFrame dataForm = fDataFrame[board];
  std::pair<int,int> format;
  uint32_t pos, wavePos;

  // Variables to store the data
  uint32_t startingPos         = 4 + offset;
  uint32_t coupleAggregateSize = 0;
  uint64_t tstamp              = 0;
  uint32_t fineTS              = 0;
  uint32_t extras              = 0;
  uint32_t flags               = 0;
  uint16_t qshort              = 0;
  uint16_t qlong               = 0;
  uint16_t numSamples          = 0;
  bool pur                     = false;
  bool ovr                     = false;
  bool lost                    = false;
  
  // Here starts the real decoding of the data for the channels 
  uint32_t chanNum = 0;
  for(uint8_t cpl = 0 ; cpl < channelMask.count() ; cpl++){
    
    // Channel aggregate size is board dependant
    pos    = startingPos;
    format = dataForm.GetFormat( "Size" );
    coupleAggregateSize = project_range( format.first, format.second, std::bitset<32>( inpBuffer[pos] ) ).to_ulong( );

    dataForm.SetDataFormat(inpBuffer[startingPos+1]);

    for(uint16_t evt = 0 ; evt < (coupleAggregateSize-2)/dataForm.EvtSize();evt++){

      // PU and Charges seems not to be dependant on the board
      pur  = static_cast<bool>(((inpBuffer[startingPos+dataForm.EvtSize()-1+dataForm.EvtSize()*evt+2])&0x8000)>>15);
      qshort  = static_cast<uint16_t>((inpBuffer[startingPos+dataForm.EvtSize()-1+dataForm.EvtSize()*evt+2])&0x7FFF);
      qlong   = static_cast<uint16_t>((inpBuffer[startingPos+dataForm.EvtSize()-1+dataForm.EvtSize()*evt+2])>>16);

      // Time stamp is board dependant
      pos    = startingPos+2+dataForm.EvtSize()*evt;
      format = dataForm.GetFormat( "TS" );
      tstamp = static_cast<uint64_t>(project_range( format.first, format.second, std::bitset<32>(inpBuffer[pos])).to_ulong());

      // Channel number is board dependant
      if( dataForm.CheckFormat( "CH" ) ){
        chanNum  = static_cast<uint32_t>(((inpBuffer[startingPos+2+dataForm.EvtSize()*evt]&0x80000000)>>31)+couples[cpl]*2);
      }
      else{
        chanNum = couples[cpl];
      }
      
      if(dataForm.CheckEnabled( "Extras" )){
	      extras = inpBuffer[startingPos+dataForm.EvtSize()-2+dataForm.EvtSize()*evt+2];
        if(dataForm.CheckConfig( "Extras" )){
	        switch(dataForm.GetConfig( "Extras" )) {
	          case 0: // Extended Time Stamp [31:16] ; baseline*4 [15:0]
	            tstamp = (static_cast<uint64_t>((extras&0xFFFF0000))<<15) | tstamp;
	            break;
	          case 1: // Extended Time stamp [31:16] ; flags [15:0]
	            ovr    = static_cast<bool>((extras&0x00004000)>>14);
	            lost   = static_cast<bool>((extras&0x00001000)>>12);
	            flags  = static_cast<uint32_t>((extras&0x0000F000)<<15);
	            tstamp = (static_cast<uint64_t>((extras&0xFFFF0000))<<15) | tstamp;
	            break;
	          case 2: // Extended Time stamp [31:16] ; Flags [15:10] ; Fine Time Stamp [9:0]
	            flags  = static_cast<uint32_t>((extras&0x0000F000)<<15);
	            fineTS = static_cast<uint16_t>(extras&0x000003FF);
	            tstamp = ((uint64_t)(extras)&0xFFFF0000)<<15 | tstamp;
	            break;
	          case 4: // Lost Trigger Counter [31:16] ; Total Trigger [15:0]
	            break;
	          case 5: // Positive zero crossing [31:16] ; Negative zero crossing [15:0]
	            break;
	          case 7: // Debug fixed value;
	            break;
	          default:
	            break;
	        }

        }
        else tstamp = (static_cast<uint64_t>((extras&0x00007FFF))<<32) | tstamp;

      }

      if( dataForm.CheckEnabled( "Trace" ) && fWaveUpdateMap[board][chanNum] ){
        if( waveThread != nullptr ){
          if( !waveThread->timed_join(boost::posix_time::seconds(0)) ){
            waveThread->join( );
            delete waveThread;
            waveThread = nullptr;
            numSamples = dataForm.GetConfig( "NumSamples" )/2;
            delete waveBuffer;
            waveBuffer = nullptr;
            waveBuffer = new uint32_t[numSamples];
            wavePos = (startingPos+3+dataForm.EvtSize()*evt)*sizeof(uint32_t);
            memcpy(waveBuffer,inpBuffer+wavePos,numSamples*sizeof(uint32_t));
            waveThread = new boost::thread( boost::bind( &SpyServer::FillGraphs, this, waveBuffer, board, chanNum, dataForm ) );
            fWaveUpdateMap[board][chanNum] = false;
          }
        }
        else{
          numSamples = dataForm.GetConfig( "NumSamples" )/2;
          waveBuffer = new uint32_t[numSamples];
          wavePos = (startingPos+2+dataForm.EvtSize()*evt)*sizeof(uint32_t);
          memcpy(waveBuffer,inpBuffer+wavePos,numSamples*sizeof(uint32_t));
          waveThread = new boost::thread( boost::bind( &SpyServer::FillGraphs, this, waveBuffer, board, chanNum, dataForm ) );
          fWaveUpdateMap[board][chanNum] = false;
        }
      }

      // Filling ROOT files
      fQshort[board][chanNum]->Fill(qshort);
      fQlong[board][chanNum]->Fill(qlong);

    }
      
    startingPos+=coupleAggregateSize;

  }

}

uint32_t SpyServer::ReadHeader( char* buffer ){

  uint32_t hSize = 0;
  memcpy(&hSize,buffer,sizeof(uint32_t));

  // Here we can continue decrypt the header but it is not necessary
  // for the SpyServer

  return hSize;

}

void SpyServer::GetNextEvent( ){

  // Ask for the next buffer and unpacks it.
  // When thread is started, the startCall is up.
  // When stopped, it goes down and exits.
  
  int            length;
  uint32_t       aggLength, board;
  std::bitset<8> channelMask;

  char* buffer = new char [512*512];

  bool firstBuffer = true;
  while( startCall ){
    
    length = xdaq->GetNextBuffer( buffer );

    int      pos    = 0;
    uint32_t offset = 0;
    if( firstBuffer ){
      offset = ReadHeader( buffer );
      firstBuffer = false;
      continue; // FIXME: ReadHeader sometimes gives a wrong length and provokes a Segmentation Fault. 
                //        Thus we skip the first buffer.
    }

    int bIdx;
    while( pos < length && startCall ){

      UnpackHeader( (uint32_t*)buffer, aggLength, board, channelMask, offset );
      if( ( offset + aggLength )*sizeof(u_int32_t) > length )  // Just in case
        break;

      if( boards->info[board].firmType == CAEN_DGTZ_DPPFirmware_PHA )
        UnpackPHA( (uint32_t*)buffer, board, channelMask, offset );
      else if( boards->info[board].firmType == CAEN_DGTZ_DPPFirmware_PSD )
        UnpackPSD( (uint32_t*)buffer, board, channelMask, offset );
      else;

      offset += aggLength;
      pos    += offset*sizeof(uint32_t);
    
    }

    bzero(buffer,512*512);

  }

}
