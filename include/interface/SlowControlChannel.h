#ifndef SLOWCONTROLCHANNEL_H
#define SLOWCONTROLCHANNEL_H

#include <CAENDigitizerType.h>

#include "interface/SlowControlInterface.h"

#include "CAENConfig.h"

// Class that handles the Channels Tab in the SlowControlInterface
// Should be cross-checked many times...

class SlowControlChannel : public QObject
{
  Q_OBJECT

public:
  SlowControlChannel(SlowControlInterface*);
  ~SlowControlChannel( );

  SlowControlInterface* interface;

  // Public since must be invoked in parents.

private:
  void updateSettingsGeneral();
  void updateSettingsChannel();
  void updateSettingsTrigger();
  void updateSettingsPHA();
  void updateSettingsPSD();

  void updatePSDAnalogProbes();

  void connectSlots();
  void disconnectSlots();

  void setMaximumValues();

  uint32_t MAX_DC = 65535;
  uint32_t temp_address = 0x10A8;

  CAENConfig* config;
  QTimer*     checkTempTimer;
		      
private slots:

  void update( );

  void checkTemp( );

  void onStartAcquisition();
  void onStopAcquisition();

  // General Settings

  void onAutoDataFlushChanged();
  void onTrgPropagationChanged();
  void onExtrasFlagChanged();
  void onDualTraceChanged();
  void onEnergyChanged();
  void onChargeChanged();
  void onTimeStampChanged();
  void onNumEvtPerAggChanged();
  void onNumAggPerBuffChanged();
  void onVetoWidthChanged();
  void onVetoStepsChanged();
  void onExtVetoDelayChanged();

  // Channel Settings
  
  void onInputRangeChanged();
  void onSignalPolarityChanged();
  void onSelfTriggerChanged();
  void onTraceLengthChanged();
  void onAdcOffsetChanged();
  void onTriggerThresholdChanged();
  void onTraceChanged();

  // DPP-PHA Settings
  
  void onFlatTopChanged();
  void onRiseTimeChanged();
  void onDecayTimeChanged();
  void onInputRiseTimeChanged();
  void onPeakingTimeChanged();
  void onPeakHoldOffChanged();
  void onRiseTimeValWindowChanged();
  
  void onTrpzRescalingChanged();
  void onFineGainChanged();
  void onDecimationChanged();
  void onDecimationGainChanged();

  void onPeakAverageChanged();
  void onBaselineAverageChanged(); 
  void onExtras2Changed();
  void onPUFlagChanged();
  void onROFlagChanged();
  void onRCCR2Changed();

  void onAnalogProbe1PHAChanged();
  void onAnalogProbe2PHAChanged();
  void onDigitalProbe1PHAChanged();
  
  // DPP-PSD Settings
  
  void onCFDDelayChanged();
  void onCFDFractionChanged();
  void onCFDPointsChanged();
  void onShortGateChanged();
  void onLongGateChanged();
  void onGateOffsetChanged();
  void onChargeSensitivityChanged();
  void onChargeThresholdChanged();
  void onChargeCutChanged();

  void onBaselineMeanChanged();
  void onDiscriminationModeChanged();
  void onPSDCutChanged();
  void onExtrasChanged();
  void onPURThresholdChanged();
  void onPSDCutThresholdChanged();
  
  void onChargePedestalChanged();
  void onPURChanged();
  void onOverRangeRejectionChanged();
  void onTriggerHysteresisChanged();

  void onAnalogProbePSDChanged();
  void onDigitalProbe1PSDChanged();
  void onDigitalProbe2PSDChanged();
  void onEnableDigitalProbePSDChanged();
  
  // Trigger Settings

  void onTrgModeChanged();
  void onLocalShapedTrgChanged();
  void onLocalShapedTrgModeChanged();
  void onShapedTrgWidthChanged();
  void onLocalTrgValChanged();
  void onLocalTrgValModeChanged();
  void onPreTriggerChanged();
  void onTrgHoldOffChanged();
  
};

#endif
