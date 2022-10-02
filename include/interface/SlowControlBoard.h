#ifndef SLOWCONTROLBOARD_H
#define SLOWCONTROLBOARD_H

#include <vector>
#include <QCheckBox>
#include <QButtonGroup>

#include <CAENDigitizerType.h>

#include "interface/SlowControlChannel.h"
#include "interface/SlowControlInterface.h"

// Class that handles the Board Information in the SlowControlInterface

class SlowControlBoard : public QObject
{
  Q_OBJECT

public:
  SlowControlBoard(SlowControlInterface*);
  ~SlowControlBoard( );

  SlowControlInterface*  interface;
  CAENBoards*            boards;
  CAENBoardInfo*         info;
  CAENConfig*            config;
  
private:

  void connectSlots();
  void disconnectSlots();

  void lockSignals();
  void unlockSignals();

  bool buttonChannelsCreated;

private slots:

  void  update( );

  void  populateListOfBoards();
  void  updateBoardInfoDisplay();
  void  updateListChannels();
  void  updateChannels();
  void  onGroupButtonClicked(int);
  void  onBoardSelectionClicked(int);

  void onStartAcquisition();
  void onStopAcquisition();
  
  
};

#endif
