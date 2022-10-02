#ifndef WAVESPYINTERFACE_H
#define WAVESPYINTERFACE_H

#include <map>

#include <QMainWindow>
#include <QCheckBox>
#include <QButtonGroup>
#include <QPushButton>

#include <QTimer>
#include <QMessageBox>
#include <QRootCanvas.h>
#include <TCanvas.h>

#include <vector>
#include <sstream>

#include "WaveWorker.h"
#include "SpyServer.h"
#include "CAENBoards.h"

// Simple interface to visualize the Waves.
// Should be implemented to handle more boards.
// Maybe add some analysis functions?

namespace Ui {
class WaveSpyInterface;
}

class WaveSpyInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit WaveSpyInterface(SpyServer* serv, CAENBoards* caenBoards, QWidget *parent = 0);
    ~WaveSpyInterface();

    void startServer( );
    void stopServer( );

private:
    Ui::WaveSpyInterface* ui;
					    
private slots:

    void populateBoards();
    void populateChannels();

    void populateWaveTab(int);

    void onStartWaveClicked();
    void onStopWaveClicked();

    void handleRootEvents();

    void cleanLayout( );

private:

    void updateCanvas( );

    QTimer*                                    fRootTimer;

    WaveWorker*                                waveWorker;
    std::map<int,std::vector<QRootCanvas*>>    waveCanvas;

    CAENBoards*                                boards;

};

#endif // WAVESPYINTERFACE_H
