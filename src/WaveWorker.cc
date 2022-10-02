#include <iostream>

#include "WaveWorker.h"

WaveWorker::WaveWorker(std::map<int,std::vector<QRootCanvas*>> waveCanvas, SpyServer* spyServ, QObject* parent)
    : QObject(0)
{
    myWaveCanvas = waveCanvas;
    serv = spyServ;

    fHistos.reserve( myWaveCanvas.size( ) );

    // Creating blank histograms to have stable axes since
    // replotting the TGraphs with "A" option updates
    // continousely the axes not permitting to zoom on 
    // axes while updating the canvas.
    
    std::map<int,std::vector<QRootCanvas*>>::iterator it;
    for( it = myWaveCanvas.begin(); it != myWaveCanvas.end(); it++ ){

      for( int idx = 0; idx < it->second.size( ); ++idx ){
        myWaveCanvas[it->first][idx]->getCanvas()->cd();
        fHistos.push_back(new TH2F(Form("ax_%i",idx),
				                           Form("Wave channel %i",idx),
				                           200,0,20000,
				                           163,0,16383));
        fHistos.back()->Draw( );
        myWaveCanvas[it->first][idx]->getCanvas()->Update();

      }

    } 
    
    // Using a QTimer since the thread seems not working
    
    timer = new QTimer( this );
    QObject::connect( timer, SIGNAL(timeout()), this, SLOT(update()) );

    updateTimer = new QTimer( this );
    QObject::connect( updateTimer, SIGNAL(timeout()), this, SLOT(updateCanvas()) );

    QObject::connect( serv, SIGNAL(waveReady(int,int)), this, SLOT(updateWave(int,int)) );

}

void WaveWorker::start()
{

  fRunning = true;
  timer->start( 1000 );
  updateTimer->start( 10 );

}

void WaveWorker::stop()
{

  timer->stop( );
  updateTimer->stop( );
  fRunning = false;
  
}

void WaveWorker::update()
{
  // The TGraphs in SpyServer are updated only on request
  // since without it two different waveforms might overlap.

  serv->UpdateWave( );
}

void WaveWorker::updateWave( int board, int channel ){
  
  if( myWaveCanvas[board][channel]->fReady ){
    myWaveCanvas[board][channel]->getCanvas()->cd();
    serv->GetAnalogProbe1( board, channel )->Draw( "L" );
    serv->GetAnalogProbe2( board, channel )->Draw( "L" );
    serv->GetDigitalProbe1( board, channel )->Draw( "L" );
    serv->GetDigitalProbe2( board, channel )->Draw( "L" );
    myWaveCanvas[board][channel]->getCanvas()->Update();
  }

}

void WaveWorker::updateCanvas( ){  
  std::map<int,std::vector<QRootCanvas*>>::iterator it;
  for( it = myWaveCanvas.begin(); it != myWaveCanvas.end(); it++ ){
    for( int idx = 0; idx < it->second.size( ); ++idx ){
      if( myWaveCanvas[it->first][idx]->fReady ){
        myWaveCanvas[it->first][idx]->getCanvas()->Update();
      }
    }
  }
}
  

