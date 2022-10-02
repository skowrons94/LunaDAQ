#ifndef HISTOWORKER_H
#define HISTOWORKER_H

#include <map>

#include <QtWidgets>
#include <vector>

#include <TCanvas.h>

#include "QRootCanvas.h"
#include "SpyServer.h"

// HistoWorker updates the Canvas of the HistoSpyInterface.
// It relies on the SpyServer which instance is passed int the constructor.

// QTimer is used to generate a signal each second to update the Canvas.
// Using a separate thread causes crashes (both with boost::thread and QThread)
// because of ROOT.

class HistoWorker : public QObject {
  
    Q_OBJECT

 public:
    HistoWorker(std::map<int,std::vector<QRootCanvas*>> histoCanvas, SpyServer* spyServ,
		QObject* parent = 0); 

    void start();  // Start the QTimer
    void stop();   // Stops the QTimer

    bool fRunning = false;  // So SlowControlInterface knows if the worker
                            // is running.
  
 private:
    std::map<int,std::vector<QRootCanvas*>>  myHistoCanvas;
	                       	                       
    QTimer*                    timer;
    QTimer*                    updateTimer;
    SpyServer*                 serv;

    private slots:
        void update(); // Used in the QTimer to update the Canvas
        void updateCanvas( );
  
};

#endif // HISTOWORKER_H
