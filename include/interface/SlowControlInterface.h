#ifndef SLOWCONTROLINTERFACE_H
#define SLOWCONTROLINTERFACE_H

#include <map>

#include <QProgressDialog>
#include <QMainWindow>
#include <QProcess>
#include <QTimer>
#include <QThread>

#include "interface/HistoSpyInterface.h"
#include "interface/WaveSpyInterface.h"

#include "XDAQMessenger.h"
#include "ProjectManager.h"
#include "SpyServer.h"
#include "CAENConfig.h"

#include "RUReaderWorker.h"
#include "LFReaderWorker.h"

#include "ListFileDialog.h"
#include "InputFormDialog.h"

#include <boost/thread.hpp>

// Main interface that inglobates all the other ones

class SlowControlChannel;
class SlowControlBoard;

namespace Ui {
class SlowControlInterface;
}

class SlowControlInterface : public QMainWindow
{
    Q_OBJECT

public:
  explicit SlowControlInterface(QWidget *parent = 0);
  ~SlowControlInterface();

  // Defined as public since are accessed in child classes
  // created for the single tabs.
  
  Ui::SlowControlInterface*   ui;
  QButtonGroup*               chEnabled;

  CAENBoards*                 caenBoards = nullptr;

  SlowControlBoard*           boardTab;
  SlowControlChannel*         channelTab;

  bool  fProjectLoaded = false;
  bool  fRunning       = false;

  void  printErrorMessage(std::string, int);
  void  emitMessage( std::string msg );

private:
  
  void  CleanSpyServer( );

  void  InitializeServer( );

  void  SaveProject( );
  bool  LoadProject( std::string strIn );

  bool  SetupDAQ( ); 
  bool  CheckRunNumber( );

  bool  CheckTMuxStatus( std::vector<XdaqApplication>& theActors );
  bool  CheckLinksStatus( std::vector<XdaqLink>& theLinks );
  bool  CheckActorsStatus( std::vector<XdaqApplication>& theActors );

  void  SaveLastDirectory( std::string strOut );
  void  LoadLastDirectory( std::string& strOut );

  void  CleanProject( );

signals:
  void updateBoardSignal( );
  void boardUpdatedSignal( );
  
  void startedSignal( );
  void stoppedSignal( );
					    
private slots:

  // Load/Save
  void onActionSaveTriggered();
  void onActionLoadTriggered();

  // Convert
  void onActionConvertRU();
  void onActionConvertLF();

  void convertRU(QStringList);
  void convertLF(QStringList);

  // XDAQ Control
  void onStartXdaqClicked( );
  void onStopXdaqClicked( );
  void onConfigureXdaqClicked( );
  void CheckXDAQ( );

  // Acquistion Control
  void onRunNumberChanged( );
  void onDataPathChanged( );

  void onStartAcqClicked( );
  void onStopAcqClicked( );
  void onStopAcquisition( );

  // Configuration Manager
  void onActionNewProject( );
  void onActionOpenProject( );
  void onActionSaveProject( );
  
  // Histogram Spy
  void onActionOpenHistoSpy();
  
  // Wave Spy
  void onActionOpenWaveSpy();
  
  // Quit
  void onActionQuit( );

private:

  boost::thread*      fStopThread = nullptr;
  XDAQMessenger       xMan;

  SpyServer*          spyServer   = nullptr;
  ProjectManager*     pMan        = nullptr;

  HistoSpyInterface*  histoSpy    = nullptr;
  WaveSpyInterface*   waveSpy     = nullptr;

  QThread*           fReadThread  = nullptr;
  RUReaderWorker*    ruWorker     = nullptr;
  LFReaderWorker*    lfWorker     = nullptr;

  QThread*           fProjectThread  = nullptr;
  QInputDialog*      fInputDialog    = nullptr;
  QFileDialog*       fFileDialog     = nullptr;
  ListFileDialog*    fListDialog     = nullptr;
  QProgressDialog*   fProgressDialog = nullptr;

  QDialog*           fErrorDialog    = nullptr;
  QLabel*            fErrorLabel     = nullptr;
  QPushButton*       fErrorButton    = nullptr;
  QVBoxLayout*       fErrorLayout    = nullptr;

  QTimer*            checkXDAQTimer;

};

#endif // SLOWCONTROLINTERFACE_H
