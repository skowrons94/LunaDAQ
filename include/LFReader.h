#ifndef LFREADER_H
#define LFREADER_H

#include <map>
#include <fstream>

#include <TH1.h>
#include <TTree.h>
#include <TFile.h>

#include <QObject>

#include <CAENDigitizerType.h>

#include "LFFrames.h"

class LFReader : public QObject {

    Q_OBJECT

    public:
        LFReader( std::map<int,int> boards, std::string inFile, std::string outFile );
       ~LFReader( );

        bool InitializeROOT( uint32_t& bIdx, uint32_t& aggSize );

        void UnpackPHA( char* buffer, uint32_t& offset, uint32_t& boardId, uint16_t& chan );
        void UnpackPSD( char* buffer, uint32_t& offset, uint32_t& boardId, uint16_t& chan );

        void Read( );
        void Write( );

        void start( );
        void stop( );
    
    private:

        // Thread variable
        int stopCall           = 1;

        std::string in, out;

        TFile* fFileOut = nullptr;

        // Container for TTrees
        std::map<int,TTree*>  fTree;                   

        // Containers for TTree variables
        std::map<int,bool>       fPu;
        std::map<int,bool>       fSatu;
        std::map<int,bool>       fLost;
        std::map<int,uint16_t>   fCFD;
        std::map<int,uint16_t>   fChannel;
        std::map<int,uint64_t>   fTimeStamp;
        std::map<int,uint16_t>   fEnergies;
        std::map<int,uint16_t>   fQLong;
        std::map<int,uint16_t>   fQShort;
  
        std::map<int,std::vector<TH1F*>>   fEnergy;    // Container for histograms
        std::map<int,std::vector<TH1F*>>   fQshort;    // Container for histograms
        std::map<int,std::vector<TH1F*>>   fQlong;     // Container for histograms

        std::map<int,int> fBoards;

        // To know if ROOT objects are initialized for each board
        std::map<int,bool> fROOTInitialized;

        // Structures to read the data
        dataKey  fKey;
        subDataPSD_t fDataPSD;
        subDataPHA_t fDataPHA;

    signals:
        void updateProgress( int progress );

};

#endif // LFREADER_H
