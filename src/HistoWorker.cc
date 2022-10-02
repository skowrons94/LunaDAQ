#include <iostream>

#include "HistoWorker.h"

HistoWorker::HistoWorker(std::map<int,std::vector<QRootCanvas*>> histoCanvas, SpyServer* spyServ, QObject* parent)
    : QObject(0)
{
    myHistoCanvas = histoCanvas;
    serv = spyServ;

    // Using a QTimer since the thread seems not working
    
    timer = new QTimer( this );
    QObject::connect( timer, SIGNAL(timeout()), this, SLOT(update()) );

    updateTimer = new QTimer( this );
    QObject::connect( updateTimer, SIGNAL(timeout()), this, SLOT(updateCanvas()) );
}

void HistoWorker::start()
{

  fRunning = true;
  timer->start( 1000 );
  updateTimer->start( 10 );

}

void HistoWorker::stop()
{

  timer->stop( );
  updateTimer->stop( );
  fRunning = false;
  
}

void HistoWorker::update()
{

  std::map<int,std::vector<QRootCanvas*>>::iterator it;
  for( it = myHistoCanvas.begin(); it != myHistoCanvas.end(); it++ ){

    int idx;    
    for( idx = 0; idx < it->second.size( ); ++idx ){

    // If the mouse is clicked fReady = false, when released fReady = True
    // Needed to update TCanvas while drawing histograms (is it working?).
    
      if( myHistoCanvas[it->first][idx]->fReady ){
        myHistoCanvas[it->first][idx]->getCanvas()->cd();
        serv->GetHistogram( it->first, idx )->Draw( );
        myHistoCanvas[it->first][idx]->getCanvas()->Update();
      }

    }

  }
  
}

void HistoWorker::updateCanvas( ){
  
  std::map<int,std::vector<QRootCanvas*>>::iterator it;
  for( it = myHistoCanvas.begin(); it != myHistoCanvas.end(); it++ ){
    int idx;
    for( idx = 0; idx < it->second.size( ); ++idx ){
      // If the mouse is clicked fReady = false, when released fReady = True
      // Needed to update TCanvas while drawing graphs (is it working?)
      if( myHistoCanvas[it->first][idx]->fReady ){
        myHistoCanvas[it->first][idx]->getCanvas()->Update();
      }
    }
  }

}