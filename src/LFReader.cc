#include <iostream>

#include "LFReader.h"
#include "LFFrames.h"

LFReader::LFReader( std::map<int,int> boards, std::string inFile, std::string outFile )
{

  fBoards = boards;
  in      = inFile;
  out     = outFile;

  // To initialize the flags
  for( auto it : fBoards ){
    fROOTInitialized[it.first] = false;
  }
    
}

LFReader::~LFReader( )
{
  for( auto it : fEnergy ){
    for( auto jt : it.second ){
      delete jt;
    }
  }
  for( auto it : fQshort ){
    for( auto jt : it.second ){
      delete jt;
    }
  }
  for( auto it : fQlong ){
    for( auto jt : it.second ){
      delete jt;
    }
  }
  for( auto it : fTree ){
    delete it.second;
  }

  if( fFileOut != nullptr )
    delete fFileOut; 
    
}

bool LFReader::InitializeROOT( uint32_t& bIdx, uint32_t& aggSize ){

  if( fBoards.find(bIdx) == fBoards.end() ) {
    std::cout << "Board " << bIdx << " not found." << std::endl;
    return false;
  }

  for(UInt_t j = 0 ; j < fBoards[bIdx]; j++){
    if( aggSize == sizeof(caenEventPHA_t) ){
      fEnergy[bIdx].push_back(new TH1F(Form("lf_hEnergy_%i_%i",bIdx,j),
			        Form("Energy Board %i Channel %i",bIdx,j),
			        32768,0,32768));
      fEnergy[bIdx].back()->SetXTitle("Energy [channels]");
      fEnergy[bIdx].back()->SetYTitle("Counts / channel");
    }
    else if( aggSize == sizeof(caenEventPSD_t) ){
      fQshort[bIdx].push_back(new TH1F(Form("lf_hQshort_%i_%i",bIdx,j),
			        Form("Qshort Board %i Channel %i",bIdx,j),
			        32768,0,32768));
      fQlong[bIdx].push_back(new TH1F(Form("lf_hQlong_%i_%i",bIdx,j),
			        Form("Qlong Board %i Channel %i",bIdx,j),
			        65535,0,65535));
      fQshort[bIdx].back()->SetXTitle("Qshort [channels]");
      fQshort[bIdx].back()->SetYTitle("Counts / channel");
      fQlong[bIdx].back()->SetXTitle("Qlong [channels]");
      fQlong[bIdx].back()->SetYTitle("Counts / channel");
    }

  }

  if( aggSize == sizeof(caenEventPHA_t) ){
    fTree[bIdx] = new TTree(Form("Board %i",bIdx),Form("Board %i",bIdx));
    fTree[bIdx]->Branch( "pu"       ,        &fPu[bIdx],        "pu/O" );
    fTree[bIdx]->Branch( "satu"     ,      &fSatu[bIdx],      "satu/O" );
    fTree[bIdx]->Branch( "lost"     ,      &fLost[bIdx],      "lost/O" );
    fTree[bIdx]->Branch( "channel"  ,   &fChannel[bIdx],   "channel/s" );
    fTree[bIdx]->Branch( "cfd"      ,       &fCFD[bIdx],       "cfd/s" );
    fTree[bIdx]->Branch( "timeStamp", &fTimeStamp[bIdx], "timeStamp/l" );
    fTree[bIdx]->Branch( "energy"   ,  &fEnergies[bIdx],    "energy/s" );
  }
  else if( aggSize == sizeof(caenEventPSD_t) ){
    fTree[bIdx] = new TTree(Form("Board %i",bIdx),Form("Board %i",bIdx));
    fTree[bIdx]->Branch( "pu"       ,        &fPu[bIdx],        "pu/O" );
    fTree[bIdx]->Branch( "satu"     ,      &fSatu[bIdx],      "satu/O" );
    fTree[bIdx]->Branch( "lost"     ,      &fLost[bIdx],      "lost/O" );
    fTree[bIdx]->Branch( "channel"  ,   &fChannel[bIdx],   "channel/s" );
    fTree[bIdx]->Branch( "cfd"      ,       &fCFD[bIdx],       "cfd/s" );
    fTree[bIdx]->Branch( "timeStamp", &fTimeStamp[bIdx], "timeStamp/l" );
    fTree[bIdx]->Branch( "qShort"   ,    &fQShort[bIdx],    "qShort/s" );
    fTree[bIdx]->Branch( "qLong"    ,     &fQLong[bIdx],     "qLong/s" );
  }

  fROOTInitialized[bIdx] = true;

  return true;

}

void LFReader::UnpackPHA( char* buffer, uint32_t& offset, uint32_t& boardId, uint16_t& chan ){

  memcpy( &fDataPHA, buffer+offset+sizeof(dataKey), sizeof(fDataPHA) );

  fEnergies[boardId] = fDataPHA.energy;
  fEnergy[boardId][chan]->Fill( fDataPHA.energy );

  fCFD[boardId] = fDataPHA.cfd;

}

void LFReader::UnpackPSD( char* buffer, uint32_t& offset, uint32_t& boardId, uint16_t& chan ){

  memcpy( &fDataPSD, buffer+offset+sizeof(dataKey), sizeof(fDataPSD) );

  fQLong[boardId] = fDataPSD.qlong;
  fQlong[boardId][chan]->Fill( fDataPSD.qlong );

  fQShort[boardId] = fDataPSD.qshort;
  fQshort[boardId][chan]->Fill( fDataPSD.qshort );

  fCFD[boardId] = fDataPSD.cfd;

}

void LFReader::start( ){
  
  stopCall = 0;

  Read( );

  if( !stopCall )
    Write( );

}

void LFReader::stop( ){
  stopCall = 1;
}

void LFReader::Read(  ){

  std::ifstream input( in, std::ios::binary );
  if( !input.is_open( ) ){    
    return;
  }

  uint32_t perCent = 100;
  uint64_t length;
  uint32_t aggSize, boardId;

  input.seekg(0, std::ios::end);
  length = (uint64_t)input.tellg();

  uint32_t bufferSize = 1e8;
  char* buffer = new char[bufferSize];

  uint64_t pos = 0;
  while( pos < length && !stopCall ){

    input.seekg( pos, std::ios::beg);
    input.read( buffer, bufferSize );

    updateProgress( (pos*perCent)/length );

    uint32_t offset = 0;
    while( offset < bufferSize && !stopCall ){
      
      if( pos + (uint64_t)offset >= length ){
        pos += offset;
        break;
      }

      memcpy( &fKey, buffer+offset, sizeof( fKey ) );
      
      aggSize = fKey.GetBytes( );
      boardId = fKey.GetBoard( );

      if( aggSize + offset > bufferSize ){
        pos += offset;
        break;
      }

      fChannel[boardId]   = fKey.GetChannel( );
      fTimeStamp[boardId] = fKey.GetTstamp( );

      if( !fROOTInitialized[boardId] ){
        if( !InitializeROOT( boardId, aggSize ) ){
          return;
        }
      }

      if( aggSize == sizeof(caenEventPHA_t) )
        UnpackPHA( buffer, offset, boardId, fChannel[boardId] );
      else if( aggSize == sizeof(caenEventPSD_t) )
        UnpackPSD( buffer, offset, boardId, fChannel[boardId] );
      
      fTree[boardId]->Fill( );

      offset += aggSize;

      updateProgress( ((pos+offset)*perCent)/length );

    }

  }

}

void LFReader::Write( ){

  // Creating ROOT file
  fFileOut = new TFile( out.c_str( ), "RECREATE" ); 
  
  fFileOut->cd( );

  if( !fEnergy.empty( ) ){
    fFileOut->mkdir( "Energies" );
    for( auto it1 : fEnergy ){
      fFileOut->cd( "Energies" );
      for( auto it2 : it1.second ){
        it2->Write( );
      }
    }
  }

  if( !fQlong.empty( ) ){
    fFileOut->mkdir( "Qlong" );
    for( auto it1 : fQlong ){
      fFileOut->cd( "Qlong" );
      for( auto it2 : it1.second ){
        it2->Write( );
      }
    }
  }

  if( !fQshort.empty( ) ){
    fFileOut->mkdir( "Qshort" );
    for( auto it1 : fQshort ){
      fFileOut->cd( "Qshort" );
      for( auto it2 : it1.second ){
        it2->Write( );
      }
    }
  }

  fFileOut->cd( );
  for( auto it : fTree ){
    it.second->Write( 0, TObject::kOverwrite );
  }

  fFileOut->Close( );

}

