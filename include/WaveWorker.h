#ifndef WAVEWORKER_H
#define WAVEWORKER_H

#include <map>

#include <vector>
#include <QtWidgets>
#include <QRootCanvas.h>

#include <TCanvas.h>
#include <TH2.h>

#include "SpyServer.h"

// WaveWorker updates the Canvas of the WaveSpyInterface.
// It relies on the SpyServer which instance is passed int the constructor.

// QTimer is used to generate a signal each second to update the Canvas.
// Using a separate thread causes crashes (both with boost::thread and QThread)
// because of ROOT.

class WaveWorker : public QObject {
  
    Q_OBJECT

 public:
    WaveWorker(std::map<int,std::vector<QRootCanvas*>> waveCanvas, SpyServer* spyServ, QObject* parent = 0);

    void  start();  // Starts the QTimer
    void  stop();   // Stops the QTimer

    bool  fRunning = false;  // The status can be retrieved from SlowControlInterface
    
 private:
    std::map<int,std::vector<QRootCanvas*>>  myWaveCanvas;
	                      
    QTimer*                    timer;
    QTimer*                    updateTimer;
    SpyServer*                 serv;

    std::vector<TH2F*>        fHistos;   // Needed to create the axes (look in the src file)

    private slots:
      void   update();
      void   updateCanvas();
      void   updateWave(int board, int channel);
};

#endif // WAVEWORKER_H
