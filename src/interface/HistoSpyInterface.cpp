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

#include "interface/HistoSpyInterface.h"
#include "ui_HistoSpyInterface.h"

HistoSpyInterface::HistoSpyInterface(SpyServer* serv, CAENBoards* caenBoards, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HistoSpyInterface)
{
    ui->setupUi(this);
    server = serv;

    // Needed for QRootCanvas
    this->createWinId( );

    // Board information
    boards = caenBoards;

    // Populate boards
    populateBoards( );

    // Populate channels
    populateChannels( );

    // Create the HistoWorker
    histoWorker = new HistoWorker( histoCanvas, serv, this );

    // Connect Histo buttons
    connect(ui->startHistoButton, SIGNAL(clicked()), this,
	    SLOT(onStartHistoClicked()));
    
    connect(ui->stopHistoButton, SIGNAL(clicked()), this,
	    SLOT(onStopHistoClicked()));

    connect(ui->saveHistoButton, SIGNAL(clicked()), this,
	    SLOT(onSaveHistoClicked()));

    connect(ui->selectChannel,SIGNAL(currentIndexChanged(int)),this,
	    SLOT(populateHistoTab(int)));

    connect(ui->selectBoard,SIGNAL(currentIndexChanged(int)),this,
	    SLOT(populateChannels()));

    // ROOT timer to handle inputs on Canvas
    fRootTimer = new QTimer( this );
    QObject::connect( fRootTimer, SIGNAL(timeout()), this,
		  SLOT(handleRootEvents()) );

    fRootTimer->start( 10 );
    
}


HistoSpyInterface::~HistoSpyInterface()
{
    fRootTimer->stop( );
    histoWorker->stop( );
    delete fRootTimer;
    delete histoWorker;
    delete ui;
}

void HistoSpyInterface::populateBoards( ){

  std::map<int,CAENBoardInfo>::iterator it;
  for( it = boards->info.begin(); it != boards->info.end(); it++ ){

    QString board = "Board " + QString::number( it->first );
    ui->selectBoard->addItem( board );

    uint nchan = it->second.boardInfo.Channels;
    for( int i = 0; i < nchan; i++ ){
      histoCanvas[it->first].push_back( new QRootCanvas(this) );
      histoCanvas[it->first].back( )->getCanvas( )->SetBorderSize(5);
      histoCanvas[it->first].back( )->getCanvas( )->SetHighLightColor(1);
    }

  }

}

void HistoSpyInterface::populateChannels( ){

  ui->selectChannel->clear( );

  QString ch = "All";
  ui->selectChannel->addItem( ch );

  QString str = ui->selectBoard->currentText( );
  
  QRegExp tagExp(" ");
  QStringList firstList = str.split(tagExp);
  
  int bIdx = firstList.takeLast( ).toInt( );

  uint nchan = boards->info[bIdx].boardInfo.Channels;
  for( int i = 0; i < nchan; i++ ){

    ch = "Channel " + QString::number( i );
    ui->selectChannel->addItem( ch );

  }

  populateHistoTab( ui->selectChannel->currentIndex( ) );

}

void HistoSpyInterface::populateHistoTab( int idx ){

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

      ui->gridHisto->addWidget(histoCanvas[bIdx][i], row, col);
      histoCanvas[bIdx][i]->setVisible( true );

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
        ui->gridHisto->addWidget(histoCanvas[bIdx][i], row, col);
        histoCanvas[bIdx][i]->setVisible( true );
      }
      else{
        histoCanvas[bIdx][i]->setVisible( false );
      }

      col += 1;
    
    }
    
  }

  updateCanvas( );

}

void HistoSpyInterface::cleanLayout( ){

  int count  = ui->gridHisto->count();
  int colums = ui->gridHisto->columnCount();
  int rows   = ui->gridHisto->rowCount();

  int i=0;
  for(int j=0; j<rows; j++)
  {
    for(int k=0; k<colums && i<count; k++)
    {
      i++;
      QLayoutItem* item = ui->gridHisto->itemAtPosition(j, k);

      if (!item) continue;

      if (item->widget()) {
        ui->gridHisto->removeWidget(item->widget());
      } 
      else {
        ui->gridHisto->removeItem(item);
      }
    }
  }

  ui->gridHisto->activate( );

  std::map<int,std::vector<QRootCanvas*> >::iterator it;
  for( it = histoCanvas.begin(); it != histoCanvas.end(); it++ ){
    for( int i = 0; i < it->second.size(); i++ ){
      it->second[i]->setVisible( false );
    }
  }

}

void HistoSpyInterface::updateCanvas( ){

  std::map<int,std::vector<QRootCanvas*> >::iterator it;
  for( it = histoCanvas.begin(); it != histoCanvas.end(); it++ ){
    for( int i = 0; i < it->second.size(); i++ ){
      it->second[i]->getCanvas()->Resize();
    }
  }

}

void HistoSpyInterface::startServer( ){

  histoWorker->start( );

}

void HistoSpyInterface::stopServer( ){

  histoWorker->stop( );

}

void HistoSpyInterface::onStartHistoClicked( ){

  histoWorker->start( );

}

void HistoSpyInterface::onStopHistoClicked( ){

  histoWorker->stop( );

}

void HistoSpyInterface::onSaveHistoClicked( ){

  bool ok = false;
  
  QString name = QInputDialog::getText(0,
				       "Input dialog",
				       "File Name (without .root):",
				       QLineEdit::Normal,
				       "", &ok);

  if( !ok ){
    return;
  }
  
  QString dir = QFileDialog::getExistingDirectory(this,
						  tr("Select Directory"),
						  "./",
						  QFileDialog::ShowDirsOnly
						  | QFileDialog::DontResolveSymlinks
						  | QFileDialog::DontUseNativeDialog);

  if( dir.isEmpty( ) || dir.isNull( ) ){
    return;
  }

  std::string fileName = dir.toStdString( ) + "/" + name.toStdString( ) + ".root";
  server->Save( fileName );

}

void HistoSpyInterface::handleRootEvents()
{
  // Call the inner loop of ROOT
  gSystem->ProcessEvents();
}
