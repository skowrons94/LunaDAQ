#ifndef HISTOSPYINTERFACE_H
#define HISTOSPYINTERFACE_H

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

#include "HistoWorker.h"
#include "SpyServer.h"
#include "CAENBoards.h"

// Simple interface to visualize the Histos.
// Handling of more boards should be implemented.
// Maybe add some analysis functions?

namespace Ui {
class HistoSpyInterface;
}

class HistoSpyInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit HistoSpyInterface(SpyServer* serv, CAENBoards* boards, QWidget *parent = 0);
    ~HistoSpyInterface();

    void startServer( );
    void stopServer( );

private:
    Ui::HistoSpyInterface         *ui;
					    
private slots:

    void populateBoards();
    void populateChannels();

    void populateHistoTab(int);

    void onStartHistoClicked();
    void onStopHistoClicked();
    void onSaveHistoClicked();

    void handleRootEvents();

    void cleanLayout( );

private:

    void updateCanvas( );

    QTimer*                                   fRootTimer;

    HistoWorker*                              histoWorker;
    std::map<int,std::vector<QRootCanvas*>>   histoCanvas;

    CAENBoards*                               boards;
    SpyServer*                                server;

};

#endif // HISTOSPYINTERFACE_H
