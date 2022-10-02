#include <QRootCanvas.h>
#include <TSystem.h>
#include <TFormula.h>
#include <TF1.h>
#include <TH1.h>
#include <TFrame.h>
#include <TTimer.h>
#include <TCanvas.h>
#include <QFileDialog>
#include <QProcess>
#include <stdlib.h>

#include "interface/WaveSpyInterface.h"
#include "ui_WaveSpyInterface.h"

WaveSpyInterface::WaveSpyInterface(SpyServer* serv, CAENBoards* caenBoards, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WaveSpyInterface)
{
    ui->setupUi(this);

    // Needed for QRootCanvas
    this->createWinId( );

    // Board information
    boards = caenBoards;
    
    // Populate channels
    populateBoards( );

    // Populate channels
    populateChannels( );

    // Create the WaveWorker
    waveWorker = new WaveWorker( waveCanvas, serv, this );

    // Connect Wave buttons
    connect(ui->startWaveButton, SIGNAL(clicked()), this,
	    SLOT(onStartWaveClicked()));
    
    connect(ui->stopWaveButton, SIGNAL(clicked()), this,
	    SLOT(onStopWaveClicked()));
    
    connect(ui->selectChannel,SIGNAL(currentIndexChanged(int)),this,
	    SLOT(populateWaveTab(int)));

    connect(ui->selectBoard,SIGNAL(currentIndexChanged(int)),this,
	    SLOT(populateChannels()));

    // ROOT timer to handle inputs on Canvas  
    fRootTimer = new QTimer( this );
    QObject::connect( fRootTimer, SIGNAL(timeout()), this,
		    SLOT(handleRootEvents()) );
  
    fRootTimer->start( 10 );

}


WaveSpyInterface::~WaveSpyInterface()
{
    fRootTimer->stop( );
    waveWorker->stop( );
    delete fRootTimer;
    delete waveWorker;
    delete ui;
}

void WaveSpyInterface::populateBoards( ){

  std::map<int,CAENBoardInfo>::iterator it;
  for( it = boards->info.begin(); it != boards->info.end(); it++ ){

    QString board = "Board " + QString::number( it->first );
    ui->selectBoard->addItem( board );

    uint nchan = it->second.boardInfo.Channels;
    for( int i = 0; i < nchan; i++ ){
      waveCanvas[it->first].push_back( new QRootCanvas(this) );
      waveCanvas[it->first].back( )->getCanvas( )->SetBorderSize(5);
      waveCanvas[it->first].back( )->getCanvas( )->SetHighLightColor(1);
    }

  }

}

void WaveSpyInterface::populateChannels( ){

  ui->selectChannel->clear( );

  QString ch = "All";
  ui->selectChannel->addItem( ch );

  QString str = ui->selectBoard->currentText( );
  
  QRegExp tagExp(" ");
  QStringList firstList = str.split(tagExp);
  QString last = firstList.takeLast( );

  uint nchan = boards->info[last.toInt( )].boardInfo.Channels;
  for( int i = 0; i < nchan; i++ ){

    ch = "Channel " + QString::number( i );
    ui->selectChannel->addItem( ch );

  }

  populateWaveTab( ui->selectChannel->currentIndex( ) );

}

void WaveSpyInterface::populateWaveTab( int idx ){

  // Automatically populates the interface tab.
  // Instead of 8 we should use the number of channels (to do).

  cleanLayout( );

  QString str = ui->selectBoard->currentText( );
  
  QRegExp tagExp(" ");
  QStringList firstList = str.split(tagExp);
  
  int bIdx = firstList.takeLast( ).toInt( );

  int row = 0;
  int col = 0;
  uint nchan = boards->info[bIdx].boardInfo.Channels;

  int ncol;
  if( nchan > 8 )
    ncol = 4;
  else
    ncol = 3;

  if( idx == 0 ){

    for( int i = 0; i < nchan; i++ ){

      if( !(i%ncol) )
        col = 0;
      row = (int)(i/ncol);

      ui->gridWave->addWidget(waveCanvas[bIdx][i], row, col);
      waveCanvas[bIdx][i]->setVisible( true );

      col += 1;
    
    }

  }

  else{
    idx -= 1;
    for( int i = 0; i < nchan; i++ ){

      if( !(i%ncol) )
        col = 0;
      row = (int)(i/ncol);

      if( i == idx ){
        ui->gridWave->addWidget(waveCanvas[bIdx][i], row, col);
        waveCanvas[bIdx][i]->setVisible( true );
      }
      else{
        waveCanvas[bIdx][i]->setVisible( false );
      }

      col += 1;
    
    }
    
  }

  updateCanvas( );

}

void WaveSpyInterface::cleanLayout( ){

  int count  = ui->gridWave->count();
  int colums = ui->gridWave->columnCount();
  int rows   = ui->gridWave->rowCount();

  int i=0;
  for(int j=0; j<rows; j++)
  {
    for(int k=0; k<colums && i<count; k++)
    {
      i++;
      QLayoutItem* item = ui->gridWave->itemAtPosition(j, k);

      if (!item) continue;

      if (item->widget()) {
        ui->gridWave->removeWidget(item->widget());
      } 
      else {
        ui->gridWave->removeItem(item);
      }
    }
  }

  ui->gridWave->activate( );

  std::map<int,std::vector<QRootCanvas*> >::iterator it;
  for( it = waveCanvas.begin(); it != waveCanvas.end(); it++ ){
    for( int i = 0; i < it->second.size(); i++ ){
      it->second[i]->setVisible( false );
    }
  }

}

void WaveSpyInterface::updateCanvas( ){

  std::map<int,std::vector<QRootCanvas*> >::iterator it;
  for( it = waveCanvas.begin(); it != waveCanvas.end(); it++ ){

    for( int i = 0; i < it->second.size(); i++ ){

      it->second[i]->getCanvas()->Resize();

    }

  }

}

void WaveSpyInterface::startServer( ){

  waveWorker->start( );

}

void WaveSpyInterface::stopServer( ){

  waveWorker->stop( );

}

void WaveSpyInterface::onStartWaveClicked( ){

  waveWorker->start( );

}

void WaveSpyInterface::onStopWaveClicked( ){

  waveWorker->stop( );

}

void WaveSpyInterface::handleRootEvents()
{
  // Call the inner loop of ROOT
  gSystem->ProcessEvents();
}
