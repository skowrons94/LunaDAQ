#include <CAENDigitizerType.h>

#include <fstream>
#include <stdlib.h>

#include <QFileDialog>

#include "interface/SlowControlInterface.h"
#include "ui_SlowControlInterface.h"

#include "interface/SlowControlChannel.h"
#include "interface/SlowControlBoard.h"

#include "RUReader.h"
#include "LFReader.h"

SlowControlInterface::SlowControlInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SlowControlInterface)
{
    ui->setupUi(this);

    boardTab   = new SlowControlBoard( this );
    channelTab = new SlowControlChannel( this );

    // Adding timer to check XDAQ
    checkXDAQTimer = new QTimer( this );
    QObject::connect( checkXDAQTimer, SIGNAL(timeout()), this,
		      SLOT(CheckXDAQ()) );

    checkXDAQTimer->start( 1000 );

    // Deactivating acquisiton controls: should be usable only if XDAQ is active
    ui->val_runNumber->setDisabled(true);
    ui->val_dataPath->setDisabled(true);
    ui->startAcqButton->setDisabled(true);
    ui->stopAcqButton->setDisabled(true);
    ui->configureXdaqButton->setDisabled(true);

    // Deactivating everything before loading or creating project
    ui->tabWidget->setDisabled( true );
    ui->actionSaveProject->setDisabled( true );
    ui->actionSave->setDisabled( true );
    ui->actionLoad->setDisabled( true );

    // Converting files
    connect(ui->actionConvertRU, SIGNAL(triggered()), this, SLOT(onActionConvertRU()));
    connect(ui->actionConvertLF, SIGNAL(triggered()), this, SLOT(onActionConvertLF()));

    // Opening Spy Windows
    connect(ui->actionOpenHistoSpy, SIGNAL(triggered()), this, SLOT(onActionOpenHistoSpy()));
    connect(ui->actionOpenWaveSpy, SIGNAL(triggered()), this, SLOT(onActionOpenWaveSpy()));

    // Saving and loading configuration
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(onActionSaveTriggered()));
    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(onActionLoadTriggered()));

    // Configuration Manager utilities
    connect(ui->actionNewProject, SIGNAL(triggered()), this, SLOT(onActionNewProject()));
    connect(ui->actionOpenProject, SIGNAL(triggered()), this, SLOT(onActionOpenProject()));
    connect(ui->actionSaveProject, SIGNAL(triggered()), this, SLOT(onActionSaveProject()));

    // Quit
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(onActionQuit()));
    
    // Connecting the XDAQ controls buttons
    connect(ui->startXdaqButton, SIGNAL(clicked()), this, SLOT(onStartXdaqClicked()));
    connect(ui->stopXdaqButton, SIGNAL(clicked()), this, SLOT(onStopXdaqClicked()));
    connect(ui->configureXdaqButton, SIGNAL(clicked()), this, SLOT(onConfigureXdaqClicked()));

    // Connecting acquisiton controls buttons
    connect(ui->startAcqButton, SIGNAL(clicked()), this, SLOT(onStartAcqClicked()));
    connect(ui->stopAcqButton, SIGNAL(clicked()), this, SLOT(onStopAcqClicked()));
    connect(ui->val_runNumber,SIGNAL(editingFinished()),this,SLOT(onRunNumberChanged()));
    connect(ui->val_dataPath,SIGNAL(editingFinished()),this,SLOT(onDataPathChanged()));

  // Stop acquistion
    connect(&xMan,SIGNAL(stop()),this,SLOT(onStopAcquisition( )) );
    connect(this,SIGNAL(stoppedSignal()),this->boardTab,SLOT(onStopAcquisition( )) );
    connect(this,SIGNAL(stoppedSignal()),this->channelTab,SLOT(onStopAcquisition( )) );

  // Start acquistion
    connect(this,SIGNAL(startedSignal()),this,SLOT(CheckXDAQ()) );
    connect(this,SIGNAL(startedSignal()),boardTab,SLOT(onStartAcquisition()) );
    connect(this,SIGNAL(startedSignal()),channelTab,SLOT(onStartAcquisition()) );

}

SlowControlInterface::~SlowControlInterface()
{

  xMan.killDaqActors( );
  CleanProject( );    
  delete ui;
  
}

void SlowControlInterface::emitMessage( std::string msg )
{
  fErrorDialog = new QDialog( );
  fErrorDialog->setAttribute( Qt::WA_DeleteOnClose );
  fErrorLabel = new QLabel( msg.c_str() );
  fErrorButton = new QPushButton( fErrorDialog );
  connect( fErrorButton, &QPushButton::clicked, fErrorDialog, &QDialog::accept );
  fErrorLayout = new QVBoxLayout( fErrorDialog );
  fErrorDialog->setWindowTitle( "Warning" );
  fErrorButton->setText( "OK" );
  fErrorDialog->setLayout( fErrorLayout );    
  fErrorLayout->addWidget( fErrorLabel, 4 );
  fErrorLayout->addWidget( fErrorButton, 10 );
  fErrorDialog->show();
}

void SlowControlInterface::SaveLastDirectory( std::string strOut )
{
  std::string homeDir( getenv("HOME") );
  std::string dir = homeDir + "/.lastDir.txt";
  std::ofstream outFile( dir.c_str( ) );
  if( !outFile.is_open( ) ){
    outFile.close( );
    return;
  }
  outFile << strOut;
  outFile.close();
}

void SlowControlInterface::LoadLastDirectory( std::string& strOut )
{
  std::string homeDir( getenv("HOME") );
  std::string dir = homeDir + "/.lastDir.txt";
  std::ifstream inFile( dir.c_str( ) );
  if( inFile.is_open() )
  {
    std::getline( inFile, strOut );
  }
  else strOut = "./";
  inFile.close();
}

void SlowControlInterface::CleanProject( )
{ 
  if( fProjectLoaded ){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save",
				                          "Do you want to save the project?",
				                          QMessageBox::Yes | QMessageBox::No);
    if( reply == QMessageBox::Yes )
	    SaveProject( );
  }

  if( pMan != nullptr ){
    delete pMan;
    pMan = nullptr;
  }

  if( caenBoards != nullptr ){
    delete caenBoards;
    caenBoards = nullptr;
  }

  if( spyServer != nullptr ){
    delete spyServer;
    spyServer = nullptr;
  }

  fProjectLoaded = false;

}

void SlowControlInterface::SaveProject( )
{

  std::string strOut = pMan->dir + "/" + pMan->name + ".conf";
  
  std::ofstream outfile( strOut.c_str( ) );
  boost::archive::text_oarchive archive( outfile );
  archive << *pMan;

  caenBoards->saveProjectRU( pMan->confFileRU );
  caenBoards->saveProjectLF( pMan->confFileLF, pMan->graphiteHostname, pMan->graphitePort );
  
}

bool SlowControlInterface::LoadProject( std::string strIn )
{

  std::ifstream infile(strIn);
  try{
    boost::archive::text_iarchive archive(infile);
    archive >> *pMan;
  }
  catch(...) {
    emitMessage( "Invalid project file.");
    return false;
  }
  SaveLastDirectory( pMan->dir );

  if( !caenBoards->loadProject( pMan->confFileRU ) ){
    return false;
  }

  ui->val_runNumber->setValue( pMan->nRun );
  ui->val_dataPath->setText( QString::fromStdString( pMan->dataDir ) );
  
  // Reading the topology
  if( !xMan.setTopologyName( pMan->dir + "/conf/file_smluna.xml" ) ){
    emitMessage( "Can not open XDAQ topology file." );
    return false;
  }

  xMan.readTopologyFile( );

  return true;
  
}

void SlowControlInterface::printErrorMessage(std::string Application, int error){

  if(error == CAEN_DGTZ_Success)
    return;

  std::ostringstream message;
  message << "Warning when trying to " << Application.c_str() << std::endl;
  message << "CAEN Digitizer error code recieved " << error << std::endl;

  switch (error){
  case CAEN_DGTZ_Success                 : message << "Operation completed successfully\n";
    break ;
  case CAEN_DGTZ_CommError               : message << "Communication error \n";
    break ;
  case CAEN_DGTZ_GenericError            : message << "Unspecified error\n";
    break ;
  case CAEN_DGTZ_InvalidParam            : message << "Invalid parameter\n";
    break ;
  case CAEN_DGTZ_InvalidLinkType         : message << "Invalid Link Type\n";
    break ;
  case CAEN_DGTZ_InvalidHandle           : message << "Invalid device handle\n";
    break ;
  case CAEN_DGTZ_MaxDevicesError         : message << "Maximum number of devices exceeded\n";
    break ;
  case CAEN_DGTZ_BadBoardType            : message << "The operation is not allowed on this type of board\n";
    break ;
  case CAEN_DGTZ_BadInterruptLev         : message << "The interrupt level is not allowed\n";
    break ;
  case CAEN_DGTZ_BadEventNumber          : message << "The event number is bad\n";
    break ;
  case CAEN_DGTZ_ReadDeviceRegisterFail  : message << "Unable to read the registry\n";
    break ;
  case CAEN_DGTZ_WriteDeviceRegisterFail : message << "Unable to write into the registry\n";
    break ;
  case CAEN_DGTZ_InvalidChannelNumber    : message << "The channel number is invalid\n";
    break ;
  case CAEN_DGTZ_ChannelBusy             : message << "The Channel is busy\n";
    break ;
  case CAEN_DGTZ_FPIOModeInvalid         : message << "Invalid FPIO Mode\n";
    break ;
  case CAEN_DGTZ_WrongAcqMode            : message << "Wrong acquisition mode\n";
    break ;
  case CAEN_DGTZ_FunctionNotAllowed      : message << "This function is not allowed for this module\n";
    break ;
  case CAEN_DGTZ_Timeout                 : message << "Communication Timeout\n";
    break ;
  case CAEN_DGTZ_InvalidBuffer           : message << "The buffer is invalid\n";
    break ;
  case CAEN_DGTZ_EventNotFound           : message << "The event is not found\n";
    break ;
  case CAEN_DGTZ_InvalidEvent            : message << "The event is invalid\n";
    break ;
  case CAEN_DGTZ_OutOfMemory             : message << "Out of memory\n";
    break ;
  case CAEN_DGTZ_CalibrationError        : message << "Unable to calibrate the board\n";
    break ;
  case CAEN_DGTZ_DigitizerNotFound       : message << "Unable to open the digitizer\n";
    break ;
  case CAEN_DGTZ_DigitizerAlreadyOpen    : message << "The Digitizer is already open\n";
    break ;
  case CAEN_DGTZ_DigitizerNotReady       : message << "The Digitizer is not ready to operate\n";
    break ;
  case CAEN_DGTZ_InterruptNotConfigured  : message << "The Digitizer has not the IRQ configured\n";
    break ;
  case CAEN_DGTZ_DigitizerMemoryCorrupted: message << "The digitizer flash memory is corrupted\n";
    break ;
  case CAEN_DGTZ_DPPFirmwareNotSupported : message << "DPP firmware not supported\n";
    break ;
  case CAEN_DGTZ_InvalidLicense          : message << "Invalid Firmware License\n";
    break ;
  case CAEN_DGTZ_InvalidDigitizerStatus  : message << "The digitizer is found in a corrupted status\n";
    break ;
  case CAEN_DGTZ_UnsupportedTrace        : message << "The given trace is not supported by the digitizer\n";
    break ;
  case CAEN_DGTZ_InvalidProbe            : message << "Invalid probe\n";
    break ;
  case CAEN_DGTZ_UnsupportedBaseAddress  : message << "The Base Address is not supported\n";
    break ;
  case CAEN_DGTZ_NotYetImplemented       : message << "The function is not yet implemented\n";
    break ;
  default: message << "Unknown error message !\n";
    }

  QMessageBox msgBox;
  msgBox.setText(message.str().c_str());
  msgBox.setIcon(QMessageBox::Warning);
  msgBox.exec();
  
}

bool SlowControlInterface::CheckTMuxStatus( std::vector<XdaqApplication>& theActors ){

  bool fTMuxOR   = false;
  bool fTMuxAND  =  true;

  bool temp;
  for( auto act : theActors ){
    temp = xMan.checkTmux( act );
    fTMuxOR  = fTMuxOR  || temp;
    fTMuxAND = fTMuxAND && temp;
    if( act.appClass.startsWith("ReadoutUnit") && !temp )
      ui->ruStatus->setText( "Inactive" );
    if( act.appClass.startsWith("LocalFilter") && !temp )
      ui->lfStatus->setText( "Inactive" );
  }

  if( fTMuxOR && fTMuxAND ){
    ui->startXdaqButton->setDisabled(true);
    ui->configureXdaqButton->setDisabled(false);
    ui->stopXdaqButton->setDisabled(false);
    ui->startAcqButton->setDisabled(true);
    ui->stopAcqButton->setDisabled(true);
    return true;
  }
  else if( fTMuxOR ){
    ui->startXdaqButton->setDisabled(true);
    ui->configureXdaqButton->setDisabled(true);
    ui->stopXdaqButton->setDisabled(false);
    ui->startAcqButton->setDisabled(true);
    ui->stopAcqButton->setDisabled(true);
    return false;
  }
  else{
    ui->startXdaqButton->setDisabled(false);
    ui->configureXdaqButton->setDisabled(true);
    ui->stopXdaqButton->setDisabled(true);
    ui->startAcqButton->setDisabled(true);
    ui->stopAcqButton->setDisabled(true);
    return false;
  }

}

bool SlowControlInterface::CheckLinksStatus( std::vector<XdaqLink>& theLinks ){

  bool fLinksOR   = false;
  bool fLinksAND  =  true;

  bool temp;
  for( auto lin : theLinks ){
    temp = (lin.status == "Enabled");
    fLinksOR  = fLinksOR  || temp;
    fLinksAND = fLinksAND && temp;
    if( lin.appClass.startsWith("ReadoutUnit") )
      ui->ruStatus->setText( QString::fromStdString( lin.status ) );
    if( lin.appClass.startsWith("LocalFilter") )
      ui->lfStatus->setText( QString::fromStdString( lin.status ) );
  }

  if( fLinksOR && fLinksAND ){
    ui->startXdaqButton->setDisabled(true);
    ui->configureXdaqButton->setDisabled(true);
    ui->stopXdaqButton->setDisabled(false);
    ui->startAcqButton->setDisabled(false);
    ui->stopAcqButton->setDisabled(true);
    ui->val_runNumber->setDisabled(false);
    ui->val_dataPath->setDisabled(false);
    return true;
  }
  else if( fLinksOR ){
    ui->startXdaqButton->setDisabled(true);
    ui->configureXdaqButton->setDisabled(false);
    ui->stopXdaqButton->setDisabled(false);
    ui->startAcqButton->setDisabled(true);
    ui->stopAcqButton->setDisabled(true);
    return false;
  }
  else{
    ui->startXdaqButton->setDisabled(true);
    ui->configureXdaqButton->setDisabled(false);
    ui->stopXdaqButton->setDisabled(false);
    ui->startAcqButton->setDisabled(true);
    ui->stopAcqButton->setDisabled(true);
    return false;
  }

}

bool SlowControlInterface::CheckActorsStatus( std::vector<XdaqApplication>& theActors ){

  bool fActorsOR   = false;
  bool fActorsAND  =  true;

  bool temp;
  for( auto act : theActors ){
    temp = (act.status == "Running");
    fActorsOR  = fActorsOR  || temp;
    fActorsAND = fActorsAND && temp;
    if( act.appClass.startsWith("ReadoutUnit") ){
      ui->ruStatus->setText( QString::fromStdString( act.status ) );
      if( act.status == "Running" ){
        int bandwidth = xMan.getOutputBW( &act );
        ui->ruDataRate->setText( QString::number( bandwidth ) + " Hz" );
      }
      else ui->ruDataRate->setText( "0 Hz" );
    }
    if( act.appClass.startsWith("LocalFilter") ){
      ui->lfStatus->setText( QString::fromStdString( act.status ) );
      if( act.status == "Running" ){
        int bandwidth = xMan.getInputBW( &act );
        ui->lfDataRate->setText( QString::number( bandwidth ) + " Hz" );
      }
      else ui->lfDataRate->setText( "0 Hz" );
    }
  }

  if( fActorsOR && fActorsAND ){
    ui->startXdaqButton->setDisabled(true);
    ui->configureXdaqButton->setDisabled(true);
    ui->stopXdaqButton->setDisabled(true);
    ui->startAcqButton->setDisabled(true);
    ui->stopAcqButton->setDisabled(false);
    ui->val_runNumber->setDisabled(true);
    ui->val_dataPath->setDisabled(true);
    return true;
  }
  else if( fActorsOR ){
    ui->startXdaqButton->setDisabled(true);
    ui->configureXdaqButton->setDisabled(true);
    ui->stopXdaqButton->setDisabled(false);
    ui->startAcqButton->setDisabled(true);
    ui->stopAcqButton->setDisabled(false);
    return false;
  }
  else{
    ui->startXdaqButton->setDisabled(true);
    ui->configureXdaqButton->setDisabled(true);
    ui->stopXdaqButton->setDisabled(false);
    ui->startAcqButton->setDisabled(false);
    ui->stopAcqButton->setDisabled(true);
    return false;
  }

}

void SlowControlInterface::CheckXDAQ( ){

  bool fTmux  = false;
  bool fLinks = false;

  xMan.checkActors( );
  xMan.checkLinks( );

  std::vector<XdaqApplication> theActors = xMan.getListActors( );
  std::vector<XdaqLink>        theLinks  = xMan.getPeerTransports();

  // First we check if tmux session is running.

  if( !CheckTMuxStatus( theActors ) ){
    ui->ruDataRate->setText( "0 Hz" );
    ui->lfDataRate->setText( "0 Hz" );
    return;
  }

  ui->ruStatus->setText( "Active" );
  ui->lfStatus->setText( "Active" );

  if( !CheckLinksStatus( theLinks ) ){
    ui->ruDataRate->setText( "0 Hz" );
    ui->lfDataRate->setText( "0 Hz" );
    return;
  }
    
  if( !CheckActorsStatus( theActors ) )
    return;

}

void SlowControlInterface::onRunNumberChanged()
{
  pMan->nRun = ui->val_runNumber->value( );
}

void SlowControlInterface::onDataPathChanged()
{
  pMan->dataDir = ui->val_dataPath->text( ).toStdString();
}

void SlowControlInterface::onConfigureXdaqClicked()
{
  xMan.configureDaqLinks( );
  xMan.enableDaqLinks( ); 
}

void SlowControlInterface::onStartXdaqClicked()
{
  // Initializing DAQ actors
  xMan.initializeDaqActors( );
}

void SlowControlInterface::onStopXdaqClicked()
{
  xMan.killDaqActors( );  
}

bool SlowControlInterface::CheckRunNumber( ){

  std::ifstream fileRU( pMan->dataDir + "/run" + std::to_string( pMan->nRun ) + ".ru" );
  std::ifstream fileLF( pMan->dataDir + "/run" + std::to_string( pMan->nRun ) + ".lf" );

  if( fileRU.good( ) || fileLF.good( ) ){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save",
				                          "Data with given run number already exists. Do you really want to overwrite it?",
				                          QMessageBox::Yes | QMessageBox::No);
    
    if( reply == QMessageBox::Yes )
      return true;
    else return false;
  }

  return true;

}

bool SlowControlInterface::SetupDAQ()
{
  xMan.getBuilderUnits();
  xMan.getLocalFilters();
  xMan.getReadoutUnits();

  xMan.setRUInputFilePath( pMan->dataDir );
  xMan.setLFInputFilePath( pMan->dataDir );

  xMan.setRUOutputFilePath( pMan->dataDir );
  xMan.setLFOutputFilePath( pMan->dataDir );

  std::string path = pMan->confFileRU.substr( 0, pMan->confFileRU.find_last_of( '/' ) + 1 );
  xMan.setRUConfigFilePath( path );
  xMan.setLFConfigFilePath( path );

  if( !CheckRunNumber( ) )
    return false;

  xMan.setRunNumber( std::to_string( pMan->nRun ) );

  if( ui->saveLF->isChecked( ) )
    xMan.setLFFileEnable( true );
  else
    xMan.setLFFileEnable( false );

  if( ui->saveRU->isChecked( ) )
    xMan.setRUFileEnable( true );
  else
    xMan.setRUFileEnable( false );

  xMan.configureDaqActors( );

  return true;

}

void SlowControlInterface::onStartAcqClicked()
{
  caenBoards->update( );
  if( !caenBoards->save( ) ){
    emitMessage( "Problem while saving the CAEN parameters. Can not start the acqquistion." );
    return;
  }

  if( !SetupDAQ( ) )
    return;

  spyServer->Reset( );

  xMan.startDaqActors( );
  pMan->RegisterStartTime( );
  
  xMan.checkActors( );
  std::vector<XdaqApplication> theActors = xMan.getListActors( );
  for( auto act : theActors ){
    if( act.status == "Running" && act.appClass.startsWith("ReadoutUnit") ){
      spyServer->Start( );
    }
  }

  ui->saveRU->setDisabled( true );
  ui->saveLF->setDisabled( true );

  emit startedSignal( );

  fRunning = true;
  
}

void SlowControlInterface::onStopAcquisition( )
{
  if( ui->saveRU->isChecked( ) )
    pMan->LinkRUData( );
  if( ui->saveLF->isChecked( ) )
    pMan->LinkLFData( );

  pMan->AddTimeData( );

  if( ui->saveRU->isChecked( ) || ui->saveLF->isChecked( ) ){  // We increase the run number if we save data.
    int runNumber = pMan->nRun + 1;
    pMan->nRun = runNumber;
    ui->val_runNumber->setValue( runNumber );
  }

  ui->saveRU->setDisabled( false );
  ui->saveLF->setDisabled( false );

  emit stoppedSignal( );

  fRunning = false;
}

void SlowControlInterface::onStopAcqClicked()
{
  spyServer->Stop( );
  pMan->RegisterStopTime( );

  fProgressDialog = new QProgressDialog( "Operation in progress.", "Close", 0, 100, this );
  fProgressDialog->setWindowModality( Qt::WindowModal );
  fProgressDialog->setAttribute( Qt::WA_DeleteOnClose );
  fProgressDialog->setValue( 0 );

  if( fStopThread != nullptr ){
    fStopThread->join( );
    delete fStopThread;
  }

  connect(&xMan, SIGNAL(updateStatus(QString)), fProgressDialog, SLOT(setLabelText(QString)) );
  connect(&xMan, SIGNAL(updateProgress(int)),   fProgressDialog, SLOT(setValue(int))         );
  
  fProgressDialog->show( );
  fStopThread = new boost::thread( &XDAQMessenger::stopDaqActors, &xMan );
}

void SlowControlInterface::onActionSaveTriggered()
{
  caenBoards->save( ); 
}

void SlowControlInterface::onActionLoadTriggered()
{
  std::string dirStr;
  LoadLastDirectory( dirStr );
  QString fileName = QFileDialog::getOpenFileName(this,
						  tr("Load JSON Configuration File"),
						  dirStr.c_str( ),
						  tr("All files (*.*);;Configuration Files (*.json)"),
						  Q_NULLPTR, QFileDialog::DontUseNativeDialog);

  caenBoards->loadConfFile( fileName.toStdString( ) );

  emit updateBoardSignal( );

}

void SlowControlInterface::onActionOpenHistoSpy(){

  // Initializing HistogramSpyInterface
  if( spyServer != nullptr ){
    histoSpy = new HistoSpyInterface( spyServer, caenBoards, this );
    histoSpy->setAttribute( Qt::WA_DeleteOnClose );
    histoSpy->setWindowModality( Qt::NonModal );
    histoSpy->show( );
  }

}

void SlowControlInterface::onActionOpenWaveSpy(){

  if( spyServer != nullptr ){
    waveSpy = new WaveSpyInterface( spyServer, caenBoards, this );
    waveSpy->setAttribute( Qt::WA_DeleteOnClose );
    waveSpy->setWindowModality( Qt::NonModal );
    waveSpy->show( );
  }

}

void SlowControlInterface::onActionNewProject()
{

  CleanProject( );

  caenBoards = new CAENBoards( );
  pMan       = new ProjectManager( );

  bool ok = false;
  
  QString name = QInputDialog::getText(this,
				                               "Input dialog",
				                               "Project Name:",
				                               QLineEdit::Normal,
				                               "", &ok);
  if( !ok ){
    CleanProject( );
    return;
  }
  
  std::string dirStr;
  LoadLastDirectory( dirStr );
  QString dir = QFileDialog::getExistingDirectory(this,
						                                      tr("Select Project Directory"),
						                                      dirStr.c_str( ),
						                                      QFileDialog::ShowDirsOnly
						                                      | QFileDialog::DontResolveSymlinks
						                                      | QFileDialog::DontUseNativeDialog);
  SaveLastDirectory( dir.toStdString( ) );
  if( dir.isEmpty( ) || dir.isNull( ) ){
    CleanProject( );
    return;
  }

  LoadLastDirectory( dirStr );
  QString dataDir = QFileDialog::getExistingDirectory(this,
						                                          tr("Select Data Directory"),
						                                          dirStr.c_str( ),
						                                          QFileDialog::ShowDirsOnly
						                                          | QFileDialog::DontResolveSymlinks
						                                          | QFileDialog::DontUseNativeDialog);
  if( dataDir.isEmpty( ) || dir.isNull( ) ){
    CleanProject( );
    return;
  }

  pMan->name    = name.toStdString( );
  pMan->dataDir = dataDir.toStdString( );
  pMan->nRun    = 0;

  InputFormDialog::FormData data;
  data["Hostname"] = "smluna";
  data["Port"] = 2003;

  InputFormDialog::FormOptions options;
  options.listDisplaysAsRadios = true;
  options.listReturnsIndex = true;
  
  std::string hostname = "";
  int port = 0;
  if( InputFormDialog::getInput("Add Graphite Server (optional)", data, options) ){
    hostname  = data.at<QString>("Hostname").toStdString( );
    port      = data.at<int>("Port");
  }

  pMan->graphiteHostname = hostname;
  pMan->graphitePort     = port;
  
  if( !pMan->SetupProjectDirectory( dir.toStdString( ) ) ){
    emitMessage( "Problem with creation of the project directory." );
    CleanProject( );
    return;
  };

  if( !caenBoards->newProject( pMan->dir + "/conf" ) ){
    emitMessage( "Problem with configuration of the CAEN boards." );
    CleanProject( );
    return;
  }

  caenBoards->saveProjectRU( pMan->confFileRU );
  caenBoards->saveProjectLF( pMan->confFileLF, pMan->graphiteHostname, pMan->graphitePort );

  ui->val_runNumber->setValue( pMan->nRun );
  ui->val_dataPath->setText( QString::fromStdString( pMan->dataDir ) );

  ui->tabWidget->setDisabled( false );
  ui->actionSaveProject->setDisabled( false );
  ui->actionSave->setDisabled( false );
  ui->actionLoad->setDisabled( false );
  
  // Reading the topology
  xMan.setTopologyName( dir.toStdString( ) + "/conf/file_smluna.xml" );
  xMan.readTopologyFile( );

  fProjectLoaded = true;

  emit updateBoardSignal( );

  // Initializing SpyServer
  spyServer = new SpyServer( caenBoards );
  
}

void SlowControlInterface::onActionOpenProject()
{

  CleanProject( );

  caenBoards = new CAENBoards( );
  pMan       = new ProjectManager( );

  std::string dirStr;
  LoadLastDirectory( dirStr );
  QString fileName = QFileDialog::getOpenFileName(this,
						                                      tr("Load Project"),
						                                      dirStr.c_str( ),
						                                      tr("All files (*.*);;Configuration Files (*.conf)"),
						                                      Q_NULLPTR, 
                                                  QFileDialog::DontUseNativeDialog);
  if( fileName.isEmpty( ) || fileName.isNull( ) ){
    CleanProject( );
    return;
  }

  if( !LoadProject( fileName.toStdString( ) ) ){
    CleanProject( );
    return;
  };
  ui->tabWidget->setDisabled( false );
  ui->actionSaveProject->setDisabled( false );
  ui->actionSave->setDisabled( false );
  ui->actionLoad->setDisabled( false );


  fProjectLoaded = true;
  
  emit updateBoardSignal();

  // Initializing SpyServer
  spyServer = new SpyServer( caenBoards );

}

void SlowControlInterface::onActionSaveProject()
{
  SaveProject( );
}

void SlowControlInterface::onActionQuit( ){
  
  QApplication::quit();
  
}

void SlowControlInterface::onActionConvertRU(){

  if( !fProjectLoaded ){
    emitMessage( "No project loaded." );
    return;
  }
  if( fReadThread != nullptr ){
    if( fReadThread->isRunning( ) ){
      emitMessage( "Conversion is already running." );
      return;
    }
  }
  fListDialog = new ListFileDialog( QString::fromStdString( pMan->dataDir ), "*.ru" );
  connect(fListDialog, SIGNAL(sendFiles(QStringList)), this, SLOT(convertRU(QStringList)));
  fListDialog->show();
}

void SlowControlInterface::convertRU( QStringList fileList ){

  if( fileList.empty( ) ){
    return;
  }

  fProgressDialog = new QProgressDialog( "Operation in progress.", "Cancel", 0, 100 );
  fProgressDialog->setAttribute( Qt::WA_DeleteOnClose );
  fProgressDialog->setValue( 0 );
  
  ruWorker = new RUReaderWorker( fileList, caenBoards, pMan->dataDir );
  if( fReadThread != nullptr ){
    fReadThread->wait( );
    fReadThread = nullptr;
  }

  fReadThread = new QThread( );
  ruWorker->moveToThread( fReadThread );
  
  connect( ruWorker, SIGNAL(updateLabel(QString)), fProgressDialog, SLOT(setLabelText(QString)) );
  connect( ruWorker, SIGNAL(updateProgress(int)),  fProgressDialog, SLOT(setValue(int))         );
  
  connect( fReadThread,     SIGNAL(started()),  ruWorker,        SLOT(start( )) );
  connect( ruWorker,        SIGNAL(quit()),     fReadThread,     SLOT(quit( ))  );
  connect( ruWorker,        SIGNAL(quit()),     fProgressDialog, SLOT(cancel( ))  );
  connect( fProgressDialog, SIGNAL(canceled()), ruWorker,        SLOT(stop( )), Qt::DirectConnection  );

  fReadThread->start( );
  fProgressDialog->show( );

}

void SlowControlInterface::onActionConvertLF(){

  if( !fProjectLoaded ){
    emitMessage( "No project loaded." );
    return;
  }
  if( fReadThread != nullptr ){
    if( fReadThread->isRunning( ) ){
      emitMessage( "Conversion is already running." );
      return;
    }
  }
  fListDialog = new ListFileDialog( QString::fromStdString( pMan->dataDir ), "*.lf" );
  connect(fListDialog, SIGNAL(sendFiles(QStringList)), this, SLOT(convertLF(QStringList)));
  fListDialog->show();
}

void SlowControlInterface::convertLF( QStringList fileList ){
  if( fileList.empty( ) ){
    return;
  }

  fProgressDialog = new QProgressDialog( "Operation in progress.", "Cancel", 0, 100 );
  fProgressDialog->setAttribute( Qt::WA_DeleteOnClose );
  fProgressDialog->setValue( 0 );
  
  lfWorker = new LFReaderWorker( fileList, caenBoards, pMan->dataDir );
  if( fReadThread != nullptr ){
    fReadThread->wait( );
    fReadThread = nullptr;
  }

  fReadThread = new QThread( );
  lfWorker->moveToThread( fReadThread );
  
  connect(lfWorker, SIGNAL(updateLabel(QString)), fProgressDialog, SLOT(setLabelText(QString)) );
  connect(lfWorker, SIGNAL(updateProgress(int)),  fProgressDialog, SLOT(setValue(int)) );
  
  connect(fReadThread,     SIGNAL(started()),  lfWorker,        SLOT(start( )) );
  connect(lfWorker,        SIGNAL(quit()),     fReadThread,     SLOT(quit( )) );
  connect(lfWorker,        SIGNAL(quit()),     fProgressDialog, SLOT(cancel( )) );
  connect(fProgressDialog, SIGNAL(canceled()), lfWorker,        SLOT(stop( )), Qt::DirectConnection );

  fReadThread->start( );
  fProgressDialog->show( );

}
