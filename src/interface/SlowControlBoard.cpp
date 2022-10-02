#include "interface/SlowControlInterface.h"
#include "interface/SlowControlBoard.h"

#include "ui_SlowControlInterface.h"

SlowControlBoard::SlowControlBoard(SlowControlInterface* main)
{

  interface = main;

  // Starting the channel enabled buttons    
  this->buttonChannelsCreated = false;
  interface->chEnabled = new QButtonGroup();
  interface->chEnabled->setExclusive(false);

  connectSlots( );

}

SlowControlBoard::~SlowControlBoard( )
{

}

void SlowControlBoard::connectSlots(){
  // update Signal
  connect(interface, SIGNAL(updateBoardSignal()),this,
	  SLOT(update()));
  // Channel Enable
  connect(interface->chEnabled, SIGNAL(buttonClicked(int)),this,
	  SLOT(onGroupButtonClicked(int)));
  // Board Selection
  connect(interface->ui->boardSelection,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onBoardSelectionClicked(int)));
}

void SlowControlBoard::disconnectSlots(){
  // update Signal
  disconnect(interface, SIGNAL(updateBoardSignal()),this,
	  SLOT(update()));
  // Channel Enable
  disconnect(interface->chEnabled, SIGNAL(buttonClicked(int)),this,
	  SLOT(onGroupButtonClicked(int)));
  // Board Selection
  disconnect(interface->ui->boardSelection,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onBoardSelectionClicked(int)));
}

void SlowControlBoard::lockSignals( )
{
  interface->ui->channelSelection->blockSignals(true);
}

void SlowControlBoard::unlockSignals( )
{
  interface->ui->channelSelection->blockSignals(false);
}

void SlowControlBoard::update( ){
  
  // Populating all the variables
  disconnectSlots();
  if( interface->fProjectLoaded ){
    populateListOfBoards();
    updateBoardInfoDisplay();
    updateListChannels();
    updateChannels();
    emit interface->boardUpdatedSignal();
  }
  connectSlots();

}

void SlowControlBoard::populateListOfBoards()
{  
  interface->ui->boardSelection->clear();
  boards = interface->caenBoards;
  int nbBoards = boards->info.size();
  for(uint32_t j = 0 ; j < nbBoards ; j++){
	  interface->ui->boardSelection->addItem("(" + QString::number(boards->info[j].boardNb)+", " + QString::number(boards->info[j].linkNb) + "): " + std::to_string((int)boards->info[j].conType).c_str() + "; Digitizer " + boards->info[j].boardInfo.ModelName);
  }
}

void SlowControlBoard::updateBoardInfoDisplay()
{
  int board = interface->ui->boardSelection->currentIndex();
  info = &interface->caenBoards->info[board];

  interface->ui->lab_dgtzName->setText(QString(info->boardInfo.ModelName));
  interface->ui->lab_Model->setText(QString::number(info->boardInfo.Model));
  interface->ui->lab_nbChannels->setText(QString::number(info->boardInfo.Channels));
  interface->ui->lab_AMC->setText(QString(info->boardInfo.AMC_FirmwareRel));
  interface->ui->lab_ROC->setText(QString(info->boardInfo.ROC_FirmwareRel));
  interface->ui->lab_serialNb->setText(QString::number(info->boardInfo.SerialNumber));
  interface->ui->lab_adcBits->setText(QString::number(info->boardInfo.ADC_NBits));
  interface->ui->lab_Form->setText(QString::number(info->boardInfo.FormFactor));
  interface->ui->lab_famCode->setText(QString::number(info->boardInfo.FamilyCode));

  QString fwType;
  int16_t fwRelease = info->firmType; 
  switch( fwRelease ){
    case (int)CAEN_DGTZ_DPPFirmware_PHA:
      fwType = "DPP-Pulse Height Analysis (PHA)";
      interface->ui->tabFwPHA->setEnabled(true);
      interface->ui->tabFwPSD->setDisabled(true);
      interface->ui->tabFwWF->setEnabled(true);
      break;     
    case (int)CAEN_DGTZ_DPPFirmware_PSD:
      fwType = "DPP-Pulse Shape Discrimination (PSD)";
      interface->ui->tabFwPHA->setDisabled(true);
      interface->ui->tabFwPSD->setEnabled(true);
      interface->ui->tabFwWF->setEnabled(true);
      break;      
    case (int)CAEN_DGTZ_DPPFirmware_CI:
      fwType = "DPP-Charge Integration (CI)";
      break;      
    case (int)CAEN_DGTZ_DPPFirmware_ZLE:
      fwType = "DPP-Zero Length Encoding (ZLE)";
      break;      
    case (int)CAEN_DGTZ_DPPFirmware_QDC:
      fwType = "DPP-Charge Integration in High Density Systems (QDC)";
      break;      
    case (int)CAEN_DGTZ_DPPFirmware_DAW:
      fwType = "DPP-Dynamic Acquisition Window (DAW)";
      break;      
    case (int)CAEN_DGTZ_NotDPPFirmware:
      fwType = "WaveCatcher Firmware";
      interface->ui->tabFwPHA->setDisabled(true);
      interface->ui->tabFwPSD->setDisabled(true);
      interface->ui->tabFwWF->setEnabled(true);
      break;      
    default:
      fwType = "Firmware not recognized! Please check all the cases";
      break;
  }

  interface->ui->lab_fwName->setText(fwType);
}

void SlowControlBoard::updateListChannels()
{
  lockSignals();
  int board = interface->ui->boardSelection->currentIndex();
  info = &interface->caenBoards->info[board];

  // First we clear all the previous buttons
  QList<QAbstractButton*> allButtons = interface->chEnabled->buttons();
  for(int j = 0 ; j < allButtons.size() ; j++){
    interface->chEnabled->removeButton(allButtons[j]);
  }

  QLayoutItem* item;
  while( ( item = interface->ui->channelsLayout->layout()->takeAt( 0 ) ) != NULL )
  {
    delete item->widget();
    delete item;
  }

  interface->ui->channelSelection->clear();

  int ch;
  int line(0), column(0);
  QCheckBox* tmpChecked;
  for( ch = 0 ; ch < info->boardInfo.Channels ; ch++ ){

    tmpChecked = new QCheckBox("Channel " + QString::number(ch));
    interface->ui->channelsLayout->addWidget(tmpChecked,line,column);
    interface->ui->channelSelection->insertItem(ch,"Channel "+QString::number(ch));
    column++;

    if(column == 4 && info->boardInfo.Channels<20){
	    line++;
	    column=0;
    }
    else if(column == 8 && info->boardInfo.Channels<70){
	    line++;
	    column=0;
    }
    if( interface->fRunning )
      tmpChecked->setEnabled(false);
      
    interface->chEnabled->addButton(tmpChecked,ch);

  }
  unlockSignals();   
}

void SlowControlBoard::updateChannels( ){

  int board = interface->ui->boardSelection->currentIndex();
  info   = &interface->caenBoards->info[board];
  config = &interface->caenBoards->config[board];

  uint32_t channelMask;
  int error = config->readSetting( "Channel Enable Mask", 0, channelMask );
  if( error == -1 ){
    interface->emitMessage("Error reading Channel Enable Mask");
    return;
  }

  int ch;
  QList<QAbstractButton*> allButtons = interface->chEnabled->buttons();
  for( ch = 0; ch < info->boardInfo.Channels; ++ch ){
    if((channelMask>>ch)&0x0001)
	    (allButtons.at(ch))->setChecked(true);
    else
	    (allButtons.at(ch))->setChecked(false);
  }
  
}

void SlowControlBoard::onGroupButtonClicked(int ch)
{
  int board  = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  uint32_t address = config->caenRegisters->registers["Channel Enable Mask"];
  if(interface->chEnabled->button(ch)->isChecked())
    config->setRegisterSpecificBits( address, ch, ch, 1 );
  else
    config->setRegisterSpecificBits( address, ch, ch, 0 );
  
}

void SlowControlBoard::onBoardSelectionClicked(int index)
{
  if( interface->fProjectLoaded ){
    updateBoardInfoDisplay();
    updateListChannels();
    updateChannels();
    emit interface->boardUpdatedSignal();
  }
}

void SlowControlBoard::onStartAcquisition( ){
  QList<QAbstractButton*> allButtons = interface->chEnabled->buttons();
  for(int j = 0 ; j < allButtons.size() ; j++){
    allButtons[j]->setEnabled(false);
  }
}

void SlowControlBoard::onStopAcquisition( ){
  QList<QAbstractButton*> allButtons = interface->chEnabled->buttons();
  for(int j = 0 ; j < allButtons.size() ; j++){
    allButtons[j]->setEnabled(true);
  }
}