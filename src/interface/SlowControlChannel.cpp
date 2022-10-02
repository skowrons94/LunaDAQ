#include "interface/SlowControlInterface.h"
#include "interface/SlowControlChannel.h"

#include "ui_SlowControlInterface.h"

SlowControlChannel::SlowControlChannel(SlowControlInterface* main)
{

  interface = main;

  // Start Timer for Temperature Check
  checkTempTimer = new QTimer( this );
  checkTempTimer->start( 10000 );

  connectSlots( );
    
}

SlowControlChannel::~SlowControlChannel( )
{

}

void SlowControlChannel::onStartAcquisition()
{
  interface->ui->val_inputRange->setEnabled(false);
  interface->ui->val_trace->setEnabled(false);
  interface->ui->val_stcLen->setEnabled(false);
  interface->ui->val_adcOffset->setEnabled(false);
  interface->ui->val_dualTrace->setEnabled(false);
  interface->ui->autoDataFlush->setEnabled(false);
  interface->ui->val_aggPerBuff->setEnabled(false);
  interface->ui->val_evtsPerAgg->setEnabled(false);
}

void SlowControlChannel::onStopAcquisition( )
{
  int board = interface->ui->boardSelection->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error;
  uint32_t value;

  error = config->checkSetting( "Input Dynamic Range" );
  if( error != -1 )
    interface->ui->val_inputRange->setEnabled(true);
  error = config->checkSetting( "Enable Waveform" );
  if( error != -1 )
    interface->ui->val_trace->setEnabled(true);
  error = config->checkSetting( "Record Length" );
  if( error != -1 )
    interface->ui->val_stcLen->setEnabled(true);
  error = config->checkSetting( "DC Offset" );
  if( error != -1 )
    interface->ui->val_adcOffset->setEnabled(true);
  error = config->checkSetting( "Dual Trace" );
  if( error != -1 )
    interface->ui->val_dualTrace->setEnabled(true);
  error = config->checkSetting( "Automatic Data Flush" );
  if( error != -1 )
    interface->ui->autoDataFlush->setEnabled(true);
  error = config->checkSetting( "Aggregate Number per BLT" );
  if( error != -1 )
    interface->ui->val_aggPerBuff->setEnabled(true);
  error = config->checkSetting( "Number of Events per Aggregate" );
  if( error != -1 )
    interface->ui->val_evtsPerAgg->setEnabled(true);
}

void SlowControlChannel::connectSlots()
{
  // Check Temperature Timer
  connect( checkTempTimer, SIGNAL(timeout()), this,
		      SLOT(checkTemp()) );
  // Channel selection
  connect(interface->ui->channelSelection,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(update()));
  // Updated Board Signal
  connect(interface,SIGNAL(boardUpdatedSignal()),this,
	  SLOT(update()));

  ///////// GENERAL SETTINGS ///////////

  // Automatic Data Flush
  connect(interface->ui->val_autoDataFlush,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onAutoDataFlushChanged()));
  // Trigger Propagation
  connect(interface->ui->val_trgPropagation,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onTrgPropagationChanged()));
  // Extras Flag
  connect(interface->ui->val_extrasFlag,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onExtrasFlagChanged()));
  // Dual Trace
  connect(interface->ui->val_dualTrace,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onDualTraceChanged()));
  // Energy
  connect(interface->ui->val_energy,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onEnergyChanged()));
  // Charge
  connect(interface->ui->val_charge,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onChargeChanged()));
  // Time Stamp
  connect(interface->ui->val_timeStamp,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onTimeStampChanged()));
  // Number of Events per Aggregate
  connect(interface->ui->val_evtsPerAgg,SIGNAL(editingFinished()),this,
	  SLOT(onNumEvtPerAggChanged()));
  //  Number of Aggregates per Buffer
  connect(interface->ui->val_aggPerBuff,SIGNAL(editingFinished()),this,
	  SLOT(onNumAggPerBuffChanged()));
  // Veto Width
  connect(interface->ui->val_vetoWidth,SIGNAL(editingFinished()),this,
	  SLOT(onVetoWidthChanged()));
  //  Veto Width Steps
  connect(interface->ui->val_vetoSteps,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onVetoStepsChanged()));
  //  Number of Aggregates per Buffer
  connect(interface->ui->val_extVetoDelay,SIGNAL(editingFinished()),this,
	  SLOT(onExtVetoDelayChanged()));
  
  ///////// CHANNEL SETTINGS ///////////

  // Signal Polarity
  connect(interface->ui->val_sigPol,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onSignalPolarityChanged()));
  // Self Trigger
  connect(interface->ui->val_selfTrg,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onSelfTriggerChanged()));
  // Trace Length
  connect(interface->ui->val_stcLen,SIGNAL(editingFinished()),this,
	  SLOT(onTraceLengthChanged()));
  // ADC Offset
  connect(interface->ui->val_adcOffset,SIGNAL(editingFinished()),this,
	  SLOT(onAdcOffsetChanged()));
  // Trigger Threshold
  connect(interface->ui->val_trgThr,SIGNAL(editingFinished()),this,
	  SLOT(onTriggerThresholdChanged()));
  // Input Range
  connect(interface->ui->val_inputRange,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onInputRangeChanged()));
  // Enable Trace
  connect(interface->ui->val_trace,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onTraceChanged()));

  ///////// PHA SETTINGS ///////////

  // TRPZ Flat Top
  connect(interface->ui->val_flatTop,SIGNAL(editingFinished()),this,
	  SLOT(onFlatTopChanged()));
  // TRPZ Rise Time
  connect(interface->ui->val_riseTime,SIGNAL(editingFinished()),this,
	  SLOT(onRiseTimeChanged()));
  // TRPZ Decay Time
  connect(interface->ui->val_decayTime,SIGNAL(editingFinished()),this,
	  SLOT(onDecayTimeChanged()));
  // Input Rise Time
  connect(interface->ui->val_inputRise,SIGNAL(editingFinished()),this,
	  SLOT(onInputRiseTimeChanged()));
  // TRPZ Peaking Time
  connect(interface->ui->val_peakTime,SIGNAL(editingFinished()),this,
	  SLOT(onPeakingTimeChanged()));
  // Peak Hold-Off
  connect(interface->ui->val_peakHoldOff,SIGNAL(editingFinished()),this,
	  SLOT(onPeakHoldOffChanged()));
  // Rise Time Validation Window
  connect(interface->ui->val_riseTimeValWindow,SIGNAL(editingFinished()),this,
	  SLOT(onRiseTimeValWindowChanged()));
  // Trapezoid Rescaling
  connect(interface->ui->val_trpzRescaling,SIGNAL(editingFinished()),this,
	  SLOT(onTrpzRescalingChanged()));
  // Fine Gain
  connect(interface->ui->val_fineGain,SIGNAL(editingFinished()),this,
	  SLOT(onFineGainChanged()));
  // Decimation
  connect(interface->ui->val_decimation,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onDecimationChanged()));
  // Decimation Gain
  connect(interface->ui->val_decimationGain,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onDecimationGainChanged()));
  // Peak Average
  connect(interface->ui->val_peakAverage,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onPeakAverageChanged()));
  // Baseline Average
  connect(interface->ui->val_baselineAverage,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onBaselineAverageChanged()));
  // Extras2
  connect(interface->ui->val_extras2,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onExtras2Changed()));
  // Pile Up Flag
  connect(interface->ui->val_puFlag,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onPUFlagChanged()));
  // Roll-Over Flag
  connect(interface->ui->val_roFlag,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onROFlagChanged()));
  // RC-CR2
  connect(interface->ui->val_rccr2,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onRCCR2Changed()));
  // Analog Probe 1
  connect(interface->ui->val_analogProbe1PHA,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onAnalogProbe1PHAChanged()));
  // Analog Probe 2
  connect(interface->ui->val_analogProbe2PHA,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onAnalogProbe2PHAChanged()));
  // Digital Probe 1
  connect(interface->ui->val_digitalProbe1PHA,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onDigitalProbe1PHAChanged()));
  
  ///////// PSD SETTINGS ///////////
  
  // CFD Delay
  connect(interface->ui->val_cfdDelay,SIGNAL(editingFinished()),this,
	  SLOT(onCFDDelayChanged()));
  // CFD Fraction
  connect(interface->ui->val_cfdFraction,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onCFDFractionChanged()));
  // CFD Points
  connect(interface->ui->val_cfdPoints,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onCFDPointsChanged()));
  // Short Gate
  connect(interface->ui->val_shortGate,SIGNAL(editingFinished()),this,
	  SLOT(onShortGateChanged()));
  // Long Gate
  connect(interface->ui->val_longGate,SIGNAL(editingFinished()),this,
	  SLOT(onLongGateChanged()));
  // Gate Offset
  connect(interface->ui->val_gateOffset,SIGNAL(editingFinished()),this,
	  SLOT(onGateOffsetChanged()));
  // Charge Sensitivity
  connect(interface->ui->val_chargeSensitivity,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onChargeSensitivityChanged()));
  // Charge Threshold
  connect(interface->ui->val_chargeThreshold,SIGNAL(editingFinished()),this,
	  SLOT(onChargeThresholdChanged()));
  // Charge Cut
  connect(interface->ui->val_chargeCut,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onChargeCutChanged()));
  // Baseline Mean
  connect(interface->ui->val_baselineMean,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onBaselineMeanChanged()));
  // Discrimination Mode
  connect(interface->ui->val_discrMode,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onDiscriminationModeChanged()));
  // PSD Cut
  connect(interface->ui->val_psdCut,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onPSDCutChanged()));
  // Extras
  connect(interface->ui->val_extras,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onExtrasChanged()));
  // PUR Threshold
  connect(interface->ui->val_purThreshold,SIGNAL(editingFinished()),this,
	  SLOT(onPURThresholdChanged()));
  // PSD Cut Threshold
  connect(interface->ui->val_psdCutThreshold,SIGNAL(editingFinished()),this,
	  SLOT(onPSDCutThresholdChanged()));
  // Charge Pedestal
  connect(interface->ui->val_chargePedestal,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onChargePedestalChanged()));  
  // PUR
  connect(interface->ui->val_pur,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onPURChanged()));
  // Over Range Rejection
  connect(interface->ui->val_overRangeReject,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onOverRangeRejectionChanged()));
  // Trigger Hysteresis
  connect(interface->ui->val_trgHysteresis,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onTriggerHysteresisChanged()));
   // Analog Probe
  connect(interface->ui->val_analogProbePSD,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onAnalogProbePSDChanged()));
  // Digital Probe 1
  connect(interface->ui->val_digitalProbe1PSD,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onDigitalProbe1PSDChanged()));
  // Digital Probe 2
  connect(interface->ui->val_digitalProbe2PSD,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onDigitalProbe2PSDChanged()));
  
  ///////// TRIGGER SETTINGS ///////////
  
  // Trigger Mode
  connect(interface->ui->val_trgMode,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onTrgModeChanged()));
  // Local Shaped Trigger
  connect(interface->ui->val_localShapedTrg,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onLocalShapedTrgChanged()));
  // Local Shaped Trigger Mode
  connect(interface->ui->val_localShapedTrgMode,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onLocalShapedTrgModeChanged()));
  // Shaped Trigger Width
  connect(interface->ui->val_shapedTrgWidth,SIGNAL(editingFinished()),this,
	  SLOT(onShapedTrgWidthChanged()));
  // Local Trigger Validation
  connect(interface->ui->val_localTrgVal,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onLocalTrgValChanged()));
  // Local Trigger Validation Mode
  connect(interface->ui->val_localTrgValMode,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onLocalTrgValModeChanged()));
  // Pre Trigger
  connect(interface->ui->val_preTrigger,SIGNAL(editingFinished()),this,
	  SLOT(onPreTriggerChanged()));
  // Trigger Hold-Off
  connect(interface->ui->val_trgHoldOff,SIGNAL(editingFinished()),this,
	  SLOT(onTrgHoldOffChanged()));
}

void SlowControlChannel::disconnectSlots()
{
  // Check Temperature Timer
  disconnect( checkTempTimer, SIGNAL(timeout()), this,
		      SLOT(checkTemp()) );
  // Channel selection
  disconnect(interface->ui->channelSelection,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(update()));
  // Updated Board Signal
  disconnect(interface,SIGNAL(boardUpdatedSignal()),this,
	  SLOT(update()));

  ///////// GENERAL SETTINGS ///////////

  // Automatic Data Flush
  disconnect(interface->ui->val_autoDataFlush,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onAutoDataFlushChanged()));
  // Trigger Propagation
  disconnect(interface->ui->val_trgPropagation,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onTrgPropagationChanged()));
  // Extras Flag
  disconnect(interface->ui->val_extrasFlag,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onExtrasFlagChanged()));
  // Dual Trace
  disconnect(interface->ui->val_dualTrace,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onDualTraceChanged()));
  // Energy
  disconnect(interface->ui->val_energy,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onEnergyChanged()));
  // Charge
  disconnect(interface->ui->val_charge,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onChargeChanged()));
  // Time Stamp
  disconnect(interface->ui->val_timeStamp,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onTimeStampChanged()));
  // Number of Events per Aggregate
  disconnect(interface->ui->val_evtsPerAgg,SIGNAL(editingFinished()),this,
	  SLOT(onNumEvtPerAggChanged()));
  //  Number of Aggregates per Buffer
  disconnect(interface->ui->val_aggPerBuff,SIGNAL(editingFinished()),this,
	  SLOT(onNumAggPerBuffChanged()));
  // Veto Width
  disconnect(interface->ui->val_vetoWidth,SIGNAL(editingFinished()),this,
	  SLOT(onVetoWidthChanged()));
  //  Veto Width Steps
  disconnect(interface->ui->val_vetoSteps,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onVetoStepsChanged()));
  //  Number of Aggregates per Buffer
  disconnect(interface->ui->val_extVetoDelay,SIGNAL(editingFinished()),this,
	  SLOT(onExtVetoDelayChanged()));
  
  ///////// CHANNEL SETTINGS ///////////

  // Signal Polarity
  disconnect(interface->ui->val_sigPol,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onSignalPolarityChanged()));
  // Self Trigger
  disconnect(interface->ui->val_selfTrg,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onSelfTriggerChanged()));
  // Trace Length
  disconnect(interface->ui->val_stcLen,SIGNAL(editingFinished()),this,
	  SLOT(onTraceLengthChanged()));
  // ADC Offset
  disconnect(interface->ui->val_adcOffset,SIGNAL(editingFinished()),this,
	  SLOT(onAdcOffsetChanged()));
  // Trigger Threshold
  disconnect(interface->ui->val_trgThr,SIGNAL(editingFinished()),this,
	  SLOT(onTriggerThresholdChanged()));
  // Input Range
  disconnect(interface->ui->val_inputRange,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onInputRangeChanged()));
  // Enable Trace
  disconnect(interface->ui->val_trace,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onTraceChanged()));

  ///////// PHA SETTINGS ///////////

  // TRPZ Flat Top
  disconnect(interface->ui->val_flatTop,SIGNAL(editingFinished()),this,
	  SLOT(onFlatTopChanged()));
  // TRPZ Rise Time
  disconnect(interface->ui->val_riseTime,SIGNAL(editingFinished()),this,
	  SLOT(onRiseTimeChanged()));
  // TRPZ Decay Time
  disconnect(interface->ui->val_decayTime,SIGNAL(editingFinished()),this,
	  SLOT(onDecayTimeChanged()));
  // Input Rise Time
  disconnect(interface->ui->val_inputRise,SIGNAL(editingFinished()),this,
	  SLOT(onInputRiseTimeChanged()));
  // TRPZ Peaking Time
  disconnect(interface->ui->val_peakTime,SIGNAL(editingFinished()),this,
	  SLOT(onPeakingTimeChanged()));
  // Peak Hold-Off
  disconnect(interface->ui->val_peakHoldOff,SIGNAL(editingFinished()),this,
	  SLOT(onPeakHoldOffChanged()));
  // Rise Time Validation Window
  disconnect(interface->ui->val_riseTimeValWindow,SIGNAL(editingFinished()),this,
	  SLOT(onRiseTimeValWindowChanged()));
  // Trapezoid Rescaling
  disconnect(interface->ui->val_trpzRescaling,SIGNAL(editingFinished()),this,
	  SLOT(onTrpzRescalingChanged()));
  // Fine Gain
  disconnect(interface->ui->val_fineGain,SIGNAL(editingFinished()),this,
	  SLOT(onFineGainChanged()));
  // Decimation
  disconnect(interface->ui->val_decimation,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onDecimationChanged()));
  // Decimation Gain
  disconnect(interface->ui->val_decimationGain,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onDecimationGainChanged()));
  // Peak Average
  disconnect(interface->ui->val_peakAverage,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onPeakAverageChanged()));
  // Baseline Average
  disconnect(interface->ui->val_baselineAverage,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onBaselineAverageChanged()));
  // Extras2
  disconnect(interface->ui->val_extras2,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onExtras2Changed()));
  // Pile Up Flag
  disconnect(interface->ui->val_puFlag,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onPUFlagChanged()));
  // Roll-Over Flag
  disconnect(interface->ui->val_roFlag,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onROFlagChanged()));
  // RC-CR2
  disconnect(interface->ui->val_rccr2,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onRCCR2Changed()));
  // Analog Probe 1
  disconnect(interface->ui->val_analogProbe1PHA,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onAnalogProbe1PHAChanged()));
  // Analog Probe 2
  disconnect(interface->ui->val_analogProbe2PHA,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onAnalogProbe2PHAChanged()));
  // Digital Probe 1
  disconnect(interface->ui->val_digitalProbe1PHA,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onDigitalProbe1PHAChanged()));
  
  ///////// PSD SETTINGS ///////////
  
  // CFD Delay
  disconnect(interface->ui->val_cfdDelay,SIGNAL(editingFinished()),this,
	  SLOT(onCFDDelayChanged()));
  // CFD Fraction
  disconnect(interface->ui->val_cfdFraction,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onCFDFractionChanged()));
  // CFD Points
  disconnect(interface->ui->val_cfdPoints,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onCFDPointsChanged()));
  // Short Gate
  disconnect(interface->ui->val_shortGate,SIGNAL(editingFinished()),this,
	  SLOT(onShortGateChanged()));
  // Long Gate
  disconnect(interface->ui->val_longGate,SIGNAL(editingFinished()),this,
	  SLOT(onLongGateChanged()));
  // Gate Offset
  disconnect(interface->ui->val_gateOffset,SIGNAL(editingFinished()),this,
	  SLOT(onGateOffsetChanged()));
  // Charge Sensitivity
  disconnect(interface->ui->val_chargeSensitivity,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onChargeSensitivityChanged()));
  // Charge Threshold
  disconnect(interface->ui->val_chargeThreshold,SIGNAL(editingFinished()),this,
	  SLOT(onChargeThresholdChanged()));
  // Charge Cut
  disconnect(interface->ui->val_chargeCut,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onChargeCutChanged()));
  // Baseline Mean
  disconnect(interface->ui->val_baselineMean,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onBaselineMeanChanged()));
  // Discrimination Mode
  disconnect(interface->ui->val_discrMode,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onDiscriminationModeChanged()));
  // PSD Cut
  disconnect(interface->ui->val_psdCut,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onPSDCutChanged()));
  // Extras
  disconnect(interface->ui->val_extras,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onExtrasChanged()));
  // PUR Threshold
  disconnect(interface->ui->val_purThreshold,SIGNAL(editingFinished()),this,
	  SLOT(onPURThresholdChanged()));
  // PSD Cut Threshold
  disconnect(interface->ui->val_psdCutThreshold,SIGNAL(editingFinished()),this,
	  SLOT(onPSDCutThresholdChanged()));
  // Charge Pedestal
  disconnect(interface->ui->val_chargePedestal,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onChargePedestalChanged()));  
  // PUR
  disconnect(interface->ui->val_pur,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onPURChanged()));
  // Over Range Rejection
  disconnect(interface->ui->val_overRangeReject,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onOverRangeRejectionChanged()));
  // Trigger Hysteresis
  disconnect(interface->ui->val_trgHysteresis,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onTriggerHysteresisChanged()));
   // Analog Probe
  disconnect(interface->ui->val_analogProbePSD,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onAnalogProbePSDChanged()));
  // Digital Probe 1
  disconnect(interface->ui->val_digitalProbe1PSD,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onDigitalProbe1PSDChanged()));
  // Digital Probe 2
  disconnect(interface->ui->val_digitalProbe2PSD,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onDigitalProbe2PSDChanged()));
  
  ///////// TRIGGER SETTINGS ///////////
  
  // Trigger Mode
  disconnect(interface->ui->val_trgMode,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onTrgModeChanged()));
  // Local Shaped Trigger
  disconnect(interface->ui->val_localShapedTrg,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onLocalShapedTrgChanged()));
  // Local Shaped Trigger Mode
  disconnect(interface->ui->val_localShapedTrgMode,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onLocalShapedTrgModeChanged()));
  // Shaped Trigger Width
  disconnect(interface->ui->val_shapedTrgWidth,SIGNAL(editingFinished()),this,
	  SLOT(onShapedTrgWidthChanged()));
  // Local Trigger Validation
  disconnect(interface->ui->val_localTrgVal,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onLocalTrgValChanged()));
  // Local Trigger Validation Mode
  disconnect(interface->ui->val_localTrgValMode,SIGNAL(currentIndexChanged(int)),this,
	  SLOT(onLocalTrgValModeChanged()));
  // Pre Trigger
  disconnect(interface->ui->val_preTrigger,SIGNAL(editingFinished()),this,
	  SLOT(onPreTriggerChanged()));
  // Trigger Hold-Off
  disconnect(interface->ui->val_trgHoldOff,SIGNAL(editingFinished()),this,
	  SLOT(onTrgHoldOffChanged()));
}

void SlowControlChannel::checkTemp( )
{
  if( !interface->fProjectLoaded )
    return;

  int board   = interface->ui->boardSelection->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  uint32_t address = temp_address + 0x100*channel;

  uint32_t value;
  int error = config->getRegisterSpecificBits(address,0,7,value);  

  interface->ui->val_adcTemp->display(QString::number(((int8_t)value)));

}

void SlowControlChannel::updatePSDAnalogProbes(){
  
  int board   = interface->ui->boardSelection->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  config      = &interface->caenBoards->config[board];

  interface->ui->val_analogProbePSD->clear();

  uint32_t value;
  int error = config->readSetting( "Dual Trace", channel, value );

  if( error == -1 ){
    interface->emitMessage( "Error reading setting Dual Trace" );
    return;
  }

  if( value ){
    interface->ui->val_analogProbePSD->addItem("Input / Baseline");
    interface->ui->val_analogProbePSD->addItem("CFD / Baseline");
    interface->ui->val_analogProbePSD->addItem("Input / CFD");
  }
  else{
    interface->ui->val_analogProbePSD->addItem("Input");
    interface->ui->val_analogProbePSD->addItem("CFD");
  }

}

void SlowControlChannel::setMaximumValues( )
{
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  interface->ui->val_stcLen->setMaximum( config->getMaxValue( "Record Length" ) );
  interface->ui->val_trgThr->setMaximum( config->getMaxValue( "Trigger Threshold" ) );
  interface->ui->val_evtsPerAgg->setMaximum( config->getMaxValue( "Number of Events per Aggregate" ) );
  interface->ui->val_aggPerBuff->setMaximum( config->getMaxValue( "Aggregate Number per BLT" ) );
  interface->ui->val_vetoWidth->setMaximum( config->getMaxValue( "Veto Width" ) );
  interface->ui->val_extVetoDelay->setMaximum( config->getMaxValue( "Extended Veto Delay" )*config->nsPerSample );

  interface->ui->val_flatTop->setMaximum( config->getMaxValue( "Trapezoid Flat Top" )*config->nsPerSample );
  interface->ui->val_riseTime->setMaximum( config->getMaxValue( "Trapezoid Rise Time" )*config->nsPerSample );
  interface->ui->val_decayTime->setMaximum( config->getMaxValue( "Trapezoid Decay Time" )*config->nsPerSample );
  interface->ui->val_peakTime->setMaximum( config->getMaxValue( "Trapezoid Peaking Time" )*config->nsPerSample );
  interface->ui->val_inputRise->setMaximum( config->getMaxValue( "Input Rise Time" )*config->nsPerSample );
  interface->ui->val_peakHoldOff->setMaximum( config->getMaxValue( "Peak Hold-off" )*config->nsPerSample );
  interface->ui->val_riseTimeValWindow->setMaximum( config->getMaxValue( "Rise Time Validation Window" )*(config->nsPerSample/4) );
  interface->ui->val_trpzRescaling->setMaximum( config->getMaxValue( "Trapezoid Rescaling" ) );
  interface->ui->val_fineGain->setMaximum( config->getMaxValue( "Fine Gain" ) );

  interface->ui->val_cfdDelay->setMaximum( config->getMaxValue( "CFD Delay" )*config->nsPerSample );
  interface->ui->val_shortGate->setMaximum( config->getMaxValue( "Short Gate Width" )*config->nsPerSample );
  interface->ui->val_longGate->setMaximum( config->getMaxValue( "Long Gate Width" )*config->nsPerSample );
  interface->ui->val_gateOffset->setMaximum( config->getMaxValue( "Gate Offset" )*config->nsPerSample );
  interface->ui->val_chargeThreshold->setMaximum( config->getMaxValue( "Charge Zero Suppression Threshold" ) );
  interface->ui->val_purThreshold->setMaximum( config->getMaxValue( "PUR-GAP Threshold" ) );
  interface->ui->val_psdCutThreshold->setMaximum( config->getMaxValue( "Threshold for the PSD cut" ) );

  interface->ui->val_shapedTrgWidth->setMaximum( config->getMaxValue( "Shaped Trigger Width" ) );
  interface->ui->val_preTrigger->setMaximum( config->getMaxValue( "Pre Trigger" ) );
  interface->ui->val_trgHoldOff->setMaximum( config->getMaxValue( "Trigger Hold-off Width" ) );

}

void SlowControlChannel::update( )
{  
  disconnectSlots( );
  if( interface->fProjectLoaded ){
    
    int board = interface->ui->boardSelection->currentIndex();
    interface->ui->tabWidget->setTabEnabled(1,true);
    
    setMaximumValues();

    updateSettingsGeneral();
    updateSettingsChannel();
    updateSettingsTrigger();

    if( interface->caenBoards->info[board].firmType == CAEN_DGTZ_DPPFirmware_PHA ){
      updateSettingsPHA();
      interface->ui->fwSpecific->setTabEnabled(1,false);
      interface->ui->fwSpecific->setTabEnabled(2,true);
    }
    else if( interface->caenBoards->info[board].firmType == CAEN_DGTZ_DPPFirmware_PSD ){
      updatePSDAnalogProbes();
      updateSettingsPSD();
      interface->ui->fwSpecific->setTabEnabled(1,true);
      interface->ui->fwSpecific->setTabEnabled(2,false);
    }

    checkTemp( );

  }
  else interface->ui->tabWidget->setTabEnabled(1,false);
  connectSlots( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void SlowControlChannel::updateSettingsGeneral( ){

  int board = interface->ui->boardSelection->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error;
  uint32_t value;

  // Automatic Data Flush
  error = config->readSetting( "Automatic Data Flush", channel, value );
  if( error == -1 ) interface->ui->val_autoDataFlush->setDisabled( true );
  else{
    interface->ui->val_autoDataFlush->setDisabled( false );
    interface->ui->val_autoDataFlush->setCurrentIndex( value );
  }
  
  // Trigger Propagation
  error = config->readSetting( "Trigger Propagation", channel, value );
  if( error == -1 ) interface->ui->val_trgPropagation->setDisabled( true );
  else{
    interface->ui->val_trgPropagation->setDisabled( false );
    interface->ui->val_trgPropagation->setCurrentIndex( value );
  }
  
  // Extras Word
  error = config->readSetting( "Enable Extras", channel, value );
  if( error == -1 ) interface->ui->val_extrasFlag->setDisabled( true );
  else{
    interface->ui->val_extrasFlag->setDisabled( false );
    interface->ui->val_extrasFlag->setCurrentIndex( value );
  }
  
  // Dual Trace
  error = config->readSetting( "Dual Trace", channel, value );
  if( error == -1 ) interface->ui->val_dualTrace->setDisabled( true );
  else{
    interface->ui->val_dualTrace->setDisabled( false );
    interface->ui->val_dualTrace->setCurrentIndex(value );
  }
  
  // Energy
  error = config->readSetting( "Enable Energy", channel, value );
  if( error == -1 ) interface->ui->val_energy->setDisabled( true );
  else{
    interface->ui->val_energy->setDisabled( false );
    interface->ui->val_energy->setCurrentIndex( value );
  }

  // Charge
  error = config->readSetting( "Enable Charge", channel, value );
  if( error == -1 ) interface->ui->val_charge->setDisabled( true );
  else{
    interface->ui->val_charge->setDisabled( false );
    interface->ui->val_charge->setCurrentIndex( value );
  }

  // Time Stamp
  error = config->readSetting( "Enable Time Stamp", channel, value );
  if( error == -1 ) interface->ui->val_timeStamp->setDisabled( true );
  else{
    interface->ui->val_timeStamp->setDisabled( false );
    interface->ui->val_timeStamp->setCurrentIndex( value );
  }

  // Events per Aggregate
  error = config->readSetting( "Number of Events per Aggregate", channel, value );
  if( error == -1 ) interface->ui->val_evtsPerAgg->setDisabled( true );
  else{
    interface->ui->val_evtsPerAgg->setDisabled( false );
    interface->ui->val_evtsPerAgg->setValue( value );
  }

  // Aggregates per Buffer
  error = config->readSetting( "Aggregate Number per BLT", channel, value );
  if( error == -1 ) interface->ui->val_aggPerBuff->setDisabled( true );
  else{
    interface->ui->val_aggPerBuff->setDisabled( false );
    interface->ui->val_aggPerBuff->setValue( value );
  }

  // Veto Width
  error = config->readSetting( "Veto Width", channel, value );
  if( error == -1 ) interface->ui->val_vetoWidth->setDisabled( true );
  else{
    interface->ui->val_vetoWidth->setDisabled( false );
    interface->ui->val_vetoWidth->setValue( value );
  }

  // Veto Width Steps
  error = config->readSetting( "Veto Width Steps", channel, value );
  if( error == -1 ) interface->ui->val_vetoSteps->setDisabled( true );
  else{
    interface->ui->val_vetoSteps->setDisabled( false );
    interface->ui->val_vetoSteps->setCurrentIndex( value );
  }

  // Extended Veto Delay
  error = config->readSetting( "Extended Veto Delay", channel, value );
  error = value*config->nsPerSample;
  if( error == -1 ) interface->ui->val_extVetoDelay->setDisabled( true );
  else{
    interface->ui->val_extVetoDelay->setDisabled( false );
    interface->ui->val_extVetoDelay->setValue( value );
  }

}

void SlowControlChannel::onAutoDataFlushChanged()
{
  int value = interface->ui->val_autoDataFlush->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Automatic Data Flush", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Automatic Data Flush" );
  }
}

void SlowControlChannel::onDualTraceChanged()
{
  int value = interface->ui->val_dualTrace->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Dual Trace", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Dual Trace" );
  }

  if( interface->caenBoards->info[board].firmType == CAEN_DGTZ_DPPFirmware_PSD )
    updatePSDAnalogProbes();
}

void SlowControlChannel::onTrgPropagationChanged()
{
  int value = interface->ui->val_trgPropagation->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Trigger Propagation", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Trigger Propagation" );
  }
}

void SlowControlChannel::onExtrasFlagChanged()
{
  int value = interface->ui->val_extrasFlag->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Enable Extras", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Enable Extras" );
  }
}

void SlowControlChannel::onEnergyChanged()
{
  int value = interface->ui->val_energy->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Enable Energy", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Enable Energy" );
  }
}

void SlowControlChannel::onChargeChanged()
{
  int value = interface->ui->val_charge->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Enable Charge", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Enable Charge" );
  }
}

void SlowControlChannel::onTimeStampChanged()
{
  int value = interface->ui->val_timeStamp->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Enable Time Stamp", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Enable Time Stamp" );
  }
}

void SlowControlChannel::onNumEvtPerAggChanged()
{
  int value = interface->ui->val_evtsPerAgg->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Number of Events per Aggregate", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Number of Events per Aggregate" );
  }
}

void SlowControlChannel::onNumAggPerBuffChanged()
{
  int value = interface->ui->val_aggPerBuff->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Aggregate Number per BLT", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Aggregate Number Per BLT" );
  }
}

void SlowControlChannel::onVetoWidthChanged()
{
  int value = interface->ui->val_vetoWidth->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Veto Width", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Veto Width" );
  }
}

void SlowControlChannel::onVetoStepsChanged()
{
  int value = interface->ui->val_vetoSteps->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Veto Width Steps", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Veto Width Steps" );
  }
}

void SlowControlChannel::onExtVetoDelayChanged()
{
  int value = interface->ui->val_extVetoDelay->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  value /= config->nsPerSample;
  interface->ui->val_extVetoDelay->setValue( value*config->nsPerSample );
  int error = config->writeSetting( "Extended Veto Delay", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Extended Veto Delay" );
  }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void SlowControlChannel::updateSettingsChannel( ){

  int board = interface->ui->boardSelection->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error;
  uint32_t value;

  // Signal Polarity
  error = config->readSetting( "Invert Input", channel, value );
  if( error == -1 ) interface->ui->val_sigPol->setDisabled( true );
  else{
    interface->ui->val_sigPol->setDisabled( false );
    interface->ui->val_sigPol->setCurrentIndex( value );
  }

  // DC Offset
  error = config->readSetting( "DC Offset", channel, value );
  value = (100*value)/MAX_DC;
  if( error == -1 ) interface->ui->val_adcOffset->setDisabled( true );
  else{
    interface->ui->val_adcOffset->setDisabled( false );
    interface->ui->val_adcOffset->setValue( value );
  }

  // Self Trigger
  error = config->readSetting( "Self Trigger", channel, value );
  if( error == -1 ) interface->ui->val_selfTrg->setDisabled( true );
  else{
    interface->ui->val_selfTrg->setDisabled( false );
    interface->ui->val_selfTrg->setCurrentIndex( value );
  }

  // Dynamic Input Range
  error = config->readSetting( "Input Dynamic Range", channel, value );
  if( error == -1 ) interface->ui->val_inputRange->setDisabled( true );
  else{
    interface->ui->val_inputRange->setDisabled( false );
    interface->ui->val_inputRange->setCurrentIndex( value );
  }

  // Trace Length
  error = config->readSetting( "Record Length", channel, value );
  if( error == -1 ) interface->ui->val_stcLen->setDisabled( true );
  else{
    interface->ui->val_stcLen->setDisabled( false );
    interface->ui->val_stcLen->setValue( value );
  }

  // Trigger Threshold
  error = config->readSetting( "Trigger Threshold", channel, value );
  if( error == -1 ) interface->ui->val_trgThr->setDisabled( true );
  else{
    interface->ui->val_trgThr->setDisabled( false );
    interface->ui->val_trgThr->setValue( value );
  }

  // Enable Trace
  error = config->readSetting( "Enable Waveform", channel, value );
  if( error == -1 ) interface->ui->val_trace->setDisabled( true );
  else{
    interface->ui->val_trace->setDisabled( false );
    interface->ui->val_trace->setCurrentIndex( value );
  }

}

void SlowControlChannel::onSignalPolarityChanged()
{
  int value = interface->ui->val_sigPol->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Invert Input", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Invert Input" );
  }
}

void SlowControlChannel::onTraceLengthChanged()
{
  int value = interface->ui->val_stcLen->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Record Length", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Record Length" );
  }
}

void SlowControlChannel::onSelfTriggerChanged()
{
  int value = interface->ui->val_selfTrg->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Self Trigger", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Self Trigger" );
  } 
}

void SlowControlChannel::onAdcOffsetChanged()
{
  int value = interface->ui->val_adcOffset->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  if( value > 100 ){ 
    value = 100;
    interface->ui->val_adcOffset->setValue(value);
  }

  value = (value*MAX_DC)/100;
  interface->ui->val_adcOffset->setValue((value*100)/MAX_DC);
  int error = config->writeSetting( "DC Offset", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at DC Offset" );
  }
}

void SlowControlChannel::onTriggerThresholdChanged()
{
  int value = interface->ui->val_trgThr->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Trigger Threshold", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Trigger Threshold" );
  }
}

void SlowControlChannel::onInputRangeChanged()
{
  int value = interface->ui->val_inputRange->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Input Dynamic Range", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Input Dynamic Range" );
  }
}

void SlowControlChannel::onTraceChanged()
{
  int value = interface->ui->val_trace->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Enable Waveform", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Enable Waveform" );
  }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void SlowControlChannel::updateSettingsPHA( ){
  
  int board = interface->ui->boardSelection->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error;
  uint32_t value;

  // Trapezoid Flat Top
  error = config->readSetting( "Trapezoid Flat Top", channel, value );
  value = value*config->nsPerSample;
  if( error == -1 ) interface->ui->val_flatTop->setDisabled( true );
  else{
    interface->ui->val_flatTop->setDisabled( false );
    interface->ui->val_flatTop->setValue( value );
  }

  // Trapezoid Rise Time
  error = config->readSetting( "Trapezoid Rise Time", channel, value );
  value = value*config->nsPerSample;
  if( error == -1 ) interface->ui->val_riseTime->setDisabled( true );
  else{
    interface->ui->val_riseTime->setDisabled( false );
    interface->ui->val_riseTime->setValue( value );
  }

  // Trapezoid Decay Time
  error = config->readSetting( "Trapezoid Decay Time", channel, value );
  value = value*config->nsPerSample;
  if( error == -1 ) interface->ui->val_decayTime->setDisabled( true );
  else{
    interface->ui->val_decayTime->setDisabled( false );
    interface->ui->val_decayTime->setValue( value );
  }

  // Input Rise Time
  error = config->readSetting( "Input Rise Time", channel, value );
  value = value*config->nsPerSample;
  if( error == -1 ) interface->ui->val_inputRise->setDisabled( true );
  else{
    interface->ui->val_inputRise->setDisabled( false );
    interface->ui->val_inputRise->setValue( value );
  }

  // Peaking Time
  error = config->readSetting( "Trapezoid Peaking Time", channel, value );
  value = value*config->nsPerSample;
  if( error == -1 ) interface->ui->val_peakTime->setDisabled( true );
  else{
    interface->ui->val_peakTime->setDisabled( false );
    interface->ui->val_peakTime->setValue( value );
  }

  // Peak Hold Off
  error = config->readSetting( "Peak Hold-off", channel, value );
  value = value*config->nsPerSample;
  if( error == -1 ) interface->ui->val_peakHoldOff->setDisabled( true );
  else{
    interface->ui->val_peakHoldOff->setDisabled( false );
    interface->ui->val_peakHoldOff->setValue( value );
  }

  // Peak Hold Off
  error = config->readSetting( "Rise Time Validation Window", channel, value );
  value = value*config->nsPerSample/4;
  if( error == -1 ) interface->ui->val_riseTimeValWindow->setDisabled( true );
  else{
    interface->ui->val_riseTimeValWindow->setDisabled( false );
    interface->ui->val_riseTimeValWindow->setValue( value );
  }

  // Peak Average
  error = config->readSetting( "Peak Mean", channel, value );
  if( error == -1 ) interface->ui->val_peakAverage->setDisabled( true );
  else{
    interface->ui->val_peakAverage->setDisabled( false );
    interface->ui->val_peakAverage->setCurrentIndex( value );
  }

  // Baseline Average
  error = config->readSetting( "Baseline Averaging Window", channel, value );
  if( error == -1 ) interface->ui->val_baselineAverage->setDisabled( true );
  else{
    interface->ui->val_baselineAverage->setDisabled( false );
    interface->ui->val_baselineAverage->setCurrentIndex( value );
  }

  // RC-CR2 Average
  error = config->readSetting( "RC-CR2 Smoothing Factor", channel, value );
  if( error == -1 ) interface->ui->val_rccr2->setDisabled( true );
  else{
    interface->ui->val_rccr2->setDisabled( false );
    interface->ui->val_rccr2->setCurrentIndex( value );
  }

  // Extras2
  error = config->readSetting( "Extras 2", channel, value );
  if( error == -1 ) interface->ui->val_extras2->setDisabled( true );
  else{
    interface->ui->val_extras2->setDisabled( false );
    interface->ui->val_extras2->setCurrentIndex( value );
  }

  // Roll-Over Flag
  error = config->readSetting( "Enable Roll-Over", channel, value );
  if( error == -1 ) interface->ui->val_roFlag->setDisabled( true );
  else{
    interface->ui->val_roFlag->setDisabled( false );
    interface->ui->val_roFlag->setCurrentIndex( value );
  }

  // Pile-Up Flag
  error = config->readSetting( "Enable Pile-Up", channel, value );
  if( error == -1 ) interface->ui->val_puFlag->setDisabled( true );
  else{
    interface->ui->val_puFlag->setDisabled( false );
    interface->ui->val_puFlag->setCurrentIndex( value );
  }

  // Trapezoid Rescaling
  error = config->readSetting( "Trapezoid Rescaling", channel, value );
  if( error == -1 ) interface->ui->val_trpzRescaling->setDisabled( true );
  else{
    interface->ui->val_trpzRescaling->setDisabled( false );
    interface->ui->val_trpzRescaling->setValue( value );
  }

  // Fine Gain
  error = config->readSetting( "Fine Gain", channel, value );
  if( error == -1 ) interface->ui->val_fineGain->setDisabled( true );
  else{
    interface->ui->val_fineGain->setDisabled( false );
    interface->ui->val_fineGain->setValue( value );
  }

  // Decimation
  error = config->readSetting( "Decimation", channel, value );
  if( error == -1 ) interface->ui->val_decimation->setDisabled( true );
  else{
    interface->ui->val_decimation->setDisabled( false );
    interface->ui->val_decimation->setCurrentIndex( value );
  }

  // Decimation Gain
  error = config->readSetting( "Decimation Gain", channel, value );
  if( error == -1 ) interface->ui->val_decimationGain->setDisabled( true );
  else{
    interface->ui->val_decimationGain->setDisabled( false );
    interface->ui->val_decimationGain->setCurrentIndex( value );
  }

  // Analog Probe 1
  error = config->readSetting( "Analog Probe 1", channel, value );
  if( error == -1 ) interface->ui->val_analogProbe1PHA->setDisabled( true );
  else{
    interface->ui->val_analogProbe1PHA->setDisabled( false );
    interface->ui->val_analogProbe1PHA->setCurrentIndex( value );
  }

  // Analog Probe 2
  error = config->readSetting( "Analog Probe 2", channel, value );
  if( error == -1 ) interface->ui->val_analogProbe2PHA->setDisabled( true );
  else{
    interface->ui->val_analogProbe2PHA->setDisabled( false );
    interface->ui->val_analogProbe2PHA->setCurrentIndex( value );
  }

  // Digital Virtual Probe 1
  error = config->readSetting( "Digital Virtual Probe 1", channel, value );
  if( error == -1 ) interface->ui->val_digitalProbe1PHA->setDisabled( true );
  else{
    interface->ui->val_digitalProbe1PHA->setDisabled( false );
    interface->ui->val_digitalProbe1PHA->setCurrentIndex( value );
  }
  
}

void SlowControlChannel::onFlatTopChanged()
{
  int value = interface->ui->val_flatTop->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  value = value/config->nsPerSample;
  interface->ui->val_flatTop->setValue( value*config->nsPerSample );
  int error = config->writeSetting( "Trapezoid Flat Top", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Trapezoid Flat Top" );
  }  
}

void SlowControlChannel::onRiseTimeChanged()
{
  int value = interface->ui->val_riseTime->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  value = value/config->nsPerSample;
  interface->ui->val_riseTime->setValue( value*config->nsPerSample );
  int error = config->writeSetting( "Trapezoid Rise Time", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Trapezoid Rise Time" );
  } 
}

void SlowControlChannel::onDecayTimeChanged()
{
  int value = interface->ui->val_decayTime->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  value = value/config->nsPerSample;
  interface->ui->val_decayTime->setValue( value*config->nsPerSample );
  int error = config->writeSetting( "Trapezoid Decay Time", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Trapezoid Decay Time" );
  }
}

void SlowControlChannel::onInputRiseTimeChanged()
{
  int value = interface->ui->val_inputRise->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  value = value/config->nsPerSample;
  interface->ui->val_inputRise->setValue( value*config->nsPerSample );
  int error = config->writeSetting( "Input Rise Time", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Input Rise Time" );
  }
}

void SlowControlChannel::onPeakingTimeChanged()
{
  int value = interface->ui->val_peakTime->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  value = value/config->nsPerSample;
  interface->ui->val_peakTime->setValue( value*config->nsPerSample );
  int error = config->writeSetting( "Trapezoid Peaking Time", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Trapezoid Peaking Time" );
  }
}

void SlowControlChannel::onPeakHoldOffChanged()
{
  int value = interface->ui->val_peakHoldOff->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  value = value/config->nsPerSample;
  interface->ui->val_peakHoldOff->setValue( value*config->nsPerSample );
  int error = config->writeSetting( "Peak Hold-off", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Peak Hold-off" );
  }
}

void SlowControlChannel::onRiseTimeValWindowChanged()
{
  int value = interface->ui->val_riseTimeValWindow->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  value = value/(config->nsPerSample/4);
  interface->ui->val_riseTimeValWindow->setValue( value*(config->nsPerSample/4) );
  int error = config->writeSetting( "Rise Time Validation Window", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Rise Time Validation Window" );
  }
}

void SlowControlChannel::onTrpzRescalingChanged()
{
  int value = interface->ui->val_trpzRescaling->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Trapezoid Rescaling", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Trapezoid Rescaling" );
  }
}

void SlowControlChannel::onFineGainChanged()
{
  int value = interface->ui->val_fineGain->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Fine Gain", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Fine Gain" );
  }
}

void SlowControlChannel::onDecimationChanged()
{
  int value = interface->ui->val_decimation->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Decimation", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Decimation" );
  }
}

void SlowControlChannel::onDecimationGainChanged()
{
  int value = interface->ui->val_decimationGain->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Decimation Gain", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Decimation Gain" );
  }
}

void SlowControlChannel::onPeakAverageChanged()
{
  int value = interface->ui->val_peakAverage->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Peak Mean", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Peak Mean" );
  }
}

void SlowControlChannel::onBaselineAverageChanged()
{
  int value = interface->ui->val_baselineAverage->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Baseline Averaging Window", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Baseline Averaging Window" );
  }  
}

void SlowControlChannel::onExtras2Changed()
{
  QString text = interface->ui->val_extras2->currentText();
  if( text == "Reserved" ){
    interface->emitMessage( "Set at reserved value. Setting not changed." );
    return;
  }
  
  int value = interface->ui->val_extras2->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Extras 2", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Extras 2" );
  } 
}

void SlowControlChannel::onPUFlagChanged()
{
  int value = interface->ui->val_puFlag->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Enable Pile-Up", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Enable Pile-Up" );
  }
}

void SlowControlChannel::onROFlagChanged()
{
  int value = interface->ui->val_roFlag->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Enable Roll-Over", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Enable Roll-Over" );
  }
}

void SlowControlChannel::onRCCR2Changed()
{
  int value = interface->ui->val_rccr2->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "RC-CR2 Smoothing Factor", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at RC-CR2 Smoothing Factor" );
  }
}

void SlowControlChannel::onAnalogProbe1PHAChanged()
{
  int value = interface->ui->val_analogProbe1PHA->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Analog Probe 1", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Analog Probe 1" );
  }
}

void SlowControlChannel::onAnalogProbe2PHAChanged()
{
  int value = interface->ui->val_analogProbe2PHA->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Analog Probe 2", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Analog Probe 2" );
  }
}

void SlowControlChannel::onDigitalProbe1PHAChanged()
{
  int value = interface->ui->val_digitalProbe1PHA->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Digital Virtual Probe 1", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Digital Virtual Probe 1" );
  }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void SlowControlChannel::updateSettingsPSD( ){

  int board = interface->ui->boardSelection->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error;
  uint32_t value;

  // CFD Delay
  error = config->readSetting( "CFD Delay", channel, value );
  value = value*config->nsPerSample;
  if( error == -1 ) interface->ui->val_cfdDelay->setDisabled( true );
  else{
    interface->ui->val_cfdDelay->setDisabled( false );
    interface->ui->val_cfdDelay->setValue( value );
  }

  // CFD Fraction
  error = config->readSetting( "CFD Fraction", channel, value );
  if( error == -1 ) interface->ui->val_cfdFraction->setDisabled( true );
  else{
    interface->ui->val_cfdFraction->setDisabled( false );
    interface->ui->val_cfdFraction->setCurrentIndex( value );
  }

  // CFD Interpolation
  error = config->readSetting( "CFD Interpolation", channel, value );
  if( error == -1 ) interface->ui->val_cfdPoints->setDisabled( true );
  else{
    interface->ui->val_cfdPoints->setDisabled( false );
    interface->ui->val_cfdPoints->setCurrentIndex( value );
  }

  // Short Gate
  error = config->readSetting( "Short Gate Width", channel, value );
  value = value*config->nsPerSample;
  if( error == -1 ) interface->ui->val_shortGate->setDisabled( true );
  else{
    interface->ui->val_shortGate->setDisabled( false );
    interface->ui->val_shortGate->setValue( value );
  }

  // Long Gate
  error = config->readSetting( "Long Gate Width", channel, value );
  value = value*config->nsPerSample;
  if( error == -1 ) interface->ui->val_longGate->setDisabled( true );
  else{
    interface->ui->val_longGate->setDisabled( false );
    interface->ui->val_longGate->setValue( value );
  }

  // Gate Offset
  error = config->readSetting( "Gate Offset", channel, value );
  value = value*config->nsPerSample;
  if( error == -1 ) interface->ui->val_gateOffset->setDisabled( true );
  else{
    interface->ui->val_gateOffset->setDisabled( false );
    interface->ui->val_gateOffset->setValue( value );
  }

  // Charge Senisitivity
  error = config->readSetting( "Charge Sensitivity", channel, value );
  if( error == -1 ) interface->ui->val_chargeSensitivity->setDisabled( true );
  else{
    interface->ui->val_chargeSensitivity->setDisabled( false );
    interface->ui->val_chargeSensitivity->setCurrentIndex( value );
  }
  
  // Charge Threshold
  error = config->readSetting( "Charge Zero Suppression Threshold", channel, value );
  if( error == -1 ) interface->ui->val_chargeThreshold->setDisabled( true );
  else{
    interface->ui->val_chargeThreshold->setDisabled( false );
    interface->ui->val_chargeThreshold->setValue( value );
  }

  // Charge Threshold Flag
  error = config->readSetting( "Long Threshold", channel, value );
  if( error == -1 ) interface->ui->val_chargeCut->setDisabled( true );
  else{
    interface->ui->val_chargeCut->setDisabled( false );
    interface->ui->val_chargeCut->setCurrentIndex( value );
  }

  // Baseline Mean
  error = config->readSetting( "Baseline Averaging Window", channel, value );
  if( error == -1 ) interface->ui->val_baselineMean->setDisabled( true );
  else{
    interface->ui->val_baselineMean->setDisabled( false );
    interface->ui->val_baselineMean->setCurrentIndex( value );
  }

  // Discrimination Mode
  error = config->readSetting( "Discrimination Mode", channel, value );
  if( error == -1 ) interface->ui->val_discrMode->setDisabled( true );
  else{
    interface->ui->val_discrMode->setDisabled( false );
    interface->ui->val_discrMode->setCurrentIndex( value );
  }

  // PSD Cut Type
  error = config->readSetting( "PSD Cut", channel, value );
  if( error == -1 ) interface->ui->val_psdCut->setDisabled( true );
  else{
    if( value > 2 ){ 
      value = 0;
      config->writeSetting( "PSD Cut", channel, value );
    }
    interface->ui->val_psdCut->setDisabled( false );
    interface->ui->val_psdCut->setCurrentIndex( value );
  }

  // Extras
  error = config->readSetting( "Extras 2", channel, value );
  if( error == -1 ) interface->ui->val_discrMode->setDisabled( true );
  else{
    interface->ui->val_discrMode->setDisabled( false );
    interface->ui->val_discrMode->setCurrentIndex( value );
  }

  // PUR-GAP Threshold
  error = config->readSetting( "PUR-GAP Threshold", channel, value );
  if( error == -1 ) interface->ui->val_purThreshold->setDisabled( true );
  else{
    interface->ui->val_purThreshold->setDisabled( false );
    interface->ui->val_purThreshold->setValue( value );
  }

  // PSD Cut Threshold
  error = config->readSetting( "Threshold for the PSD cut", channel, value );
  if( error == -1 ) interface->ui->val_psdCutThreshold->setDisabled( true );
  else{
    interface->ui->val_psdCutThreshold->setDisabled( false );
    interface->ui->val_psdCutThreshold->setValue( value );
  }

  // Charge Pedestal
  error = config->readSetting( "Charge Pedestal", channel, value );
  if( error == -1 ) interface->ui->val_chargePedestal->setDisabled( true );
  else{
    interface->ui->val_chargePedestal->setDisabled( false );
    interface->ui->val_chargePedestal->setCurrentIndex( value );
  }

  // Pile-Up Rejection
  error = config->readSetting( "Pile-Up Rejection", channel, value );
  if( error == -1 ) interface->ui->val_pur->setDisabled( true );
  else{
    interface->ui->val_pur->setDisabled( false );
    interface->ui->val_pur->setCurrentIndex( value );
  }

  // Over Range Rejection
  error = config->readSetting( "Over Range Rejection", channel, value );
  if( error == -1 ) interface->ui->val_overRangeReject->setDisabled( true );
  else{
    interface->ui->val_overRangeReject->setDisabled( false );
    interface->ui->val_overRangeReject->setCurrentIndex( value );
  }

  // Trigger Hysteresis
  error = config->readSetting( "Trigger Hysteresis", channel, value );
  if( error == -1 ) interface->ui->val_trgHysteresis->setDisabled( true );
  else{
    interface->ui->val_trgHysteresis->setDisabled( false );
    interface->ui->val_trgHysteresis->setCurrentIndex( value );
  }

  // Analog Probe
  error = config->readSetting( "Analog Probe", channel, value );
  if( error == -1 ) interface->ui->val_analogProbePSD->setDisabled( true );
  else{
    interface->ui->val_analogProbePSD->setDisabled( false );
    interface->ui->val_analogProbePSD->setCurrentIndex( value );
  }

  // Digital Probe 1
  error = config->readSetting( "Digital Virtual Probe 1", channel, value );
  if( error == -1 ) interface->ui->val_digitalProbe1PSD->setDisabled( true );
  else{
    interface->ui->val_digitalProbe1PSD->setDisabled( false );
    interface->ui->val_digitalProbe1PSD->setCurrentIndex( value );
  }

  // Digital Probe 2
  error = config->readSetting( "Digital Virtual Probe 2", channel, value );
  if( error == -1 ) interface->ui->val_digitalProbe2PSD->setDisabled( true );
  else{
    interface->ui->val_digitalProbe2PSD->setDisabled( false );
    interface->ui->val_digitalProbe2PSD->setCurrentIndex( value );
  }

  // Enable Digital Probe
  error = config->readSetting( "Enable Digital Probe", channel, value );
  if( error == -1 ) interface->ui->val_enableDigitalProbePSD->setDisabled( true );
  else{
    interface->ui->val_enableDigitalProbePSD->setDisabled( false );
    interface->ui->val_enableDigitalProbePSD->setCurrentIndex( value );
  }

}

void SlowControlChannel::onCFDDelayChanged()
{
  int value = interface->ui->val_cfdDelay->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  value = value/config->nsPerSample;
  interface->ui->val_flatTop->setValue( value*config->nsPerSample );
  int error = config->writeSetting( "CFD Delay", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at CFD Delay" );
  }  
}

void SlowControlChannel::onCFDFractionChanged()
{
  int value = interface->ui->val_cfdFraction->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "CFD Fraction", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at CFD Fraction" );
  }  
}

void SlowControlChannel::onCFDPointsChanged()
{
  int value = interface->ui->val_cfdPoints->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "CFD Interpolation", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at CFD Interpolation" );
  }  
}

void SlowControlChannel::onShortGateChanged()
{
  int value = interface->ui->val_shortGate->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  value = value/config->nsPerSample;
  interface->ui->val_flatTop->setValue( value*config->nsPerSample );
  int error = config->writeSetting( "Short Gate Width", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Short Gate Width" );
  }  
}

void SlowControlChannel::onLongGateChanged()
{
  int value = interface->ui->val_longGate->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  value = value/config->nsPerSample;
  interface->ui->val_flatTop->setValue( value*config->nsPerSample );
  int error = config->writeSetting( "Long Gate Width", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Long Gate Width" );
  }  
}

void SlowControlChannel::onGateOffsetChanged()
{
  int value = interface->ui->val_gateOffset->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  value = value/config->nsPerSample;
  interface->ui->val_flatTop->setValue( value*config->nsPerSample );
  int error = config->writeSetting( "Gate Offset", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Gate Offset" );
  }  
}

void SlowControlChannel::onChargeSensitivityChanged()
{
  int value = interface->ui->val_chargeSensitivity->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Charge Sensitivity", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Charge Sensitivity" );
  }  
}

void SlowControlChannel::onChargeThresholdChanged()
{
  int value = interface->ui->val_chargeThreshold->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Charge Zero Suppression Threshold", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Charge Zero Suppression Threshold" );
  }  
}

void SlowControlChannel::onChargeCutChanged()
{
  int value = interface->ui->val_chargeCut->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Long Threshold", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Long Threshold" );
  }  
}

void SlowControlChannel::onBaselineMeanChanged()
{
  int value = interface->ui->val_baselineMean->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Baseline Averaging Window", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Baseline Averaging Window" );
  }  
}

void SlowControlChannel::onDiscriminationModeChanged()
{
  int value = interface->ui->val_discrMode->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Discrimination Mode", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Discrimination Mode" );
  }  
}

void SlowControlChannel::onPSDCutChanged()
{
  int value = interface->ui->val_psdCut->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "PSD Cut", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at PSD Cut" );
  }  
}

void SlowControlChannel::onExtrasChanged()
{
  QString text = interface->ui->val_extras2->currentText();
  if( text == "Reserved" ){
    interface->emitMessage( "Set at reserved value. Setting not changed." );
    return;
  }

  int value = interface->ui->val_extras->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Extras 2", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Extras 2" );
  }  
}

void SlowControlChannel::onPURThresholdChanged()
{
  int value = interface->ui->val_purThreshold->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "PUR-GAP Threshold", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at PUR-GAP Threshold" );
  }  
}

void SlowControlChannel::onPSDCutThresholdChanged()
{
  int value = interface->ui->val_psdCutThreshold->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Threshold for the PSD cut", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Threshold for the PSD cut" );
  }  
}

void SlowControlChannel::onChargePedestalChanged()
{
  int value = interface->ui->val_chargePedestal->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Charge Pedestal", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Charge Pedestal" );
  }  
}

void SlowControlChannel::onPURChanged()
{
  int value = interface->ui->val_pur->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Pile-Up Rejection", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Pile-Up Rejection" );
  }  
}

void SlowControlChannel::onOverRangeRejectionChanged()
{
  int value = interface->ui->val_overRangeReject->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Over Range Rejection", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Over Range Rejection" );
  }  
}

void SlowControlChannel::onTriggerHysteresisChanged()
{
  int value = interface->ui->val_trgHysteresis->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Trigger Hysteresis", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Trigger Hysteresis" );
  }  
}

void SlowControlChannel::onAnalogProbePSDChanged()
{
  int value = interface->ui->val_analogProbePSD->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Analog Probe", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Analog Probe" );
  }  
}

void SlowControlChannel::onDigitalProbe1PSDChanged()
{
  int value = interface->ui->val_digitalProbe1PSD->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Digital Virtual Probe 1", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Digital Virtual Probe 1" );
  }  
}

void SlowControlChannel::onDigitalProbe2PSDChanged()
{
  int value = interface->ui->val_digitalProbe2PSD->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Digital Virtual Probe 2", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Digital Virtual Probe 2" );
  }  
}

void SlowControlChannel::onEnableDigitalProbePSDChanged()
{
  int value = interface->ui->val_enableDigitalProbePSD->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Enable Digital Probe", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Enable Digital Probe" );
  }  
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void SlowControlChannel::updateSettingsTrigger()
{
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error;
  uint32_t value;

  // Trigger Mode
  error = config->readSetting( "Trigger Mode", channel, value );
  if( error == -1 ) interface->ui->val_trgMode->setDisabled( true );
  else{
    interface->ui->val_trgMode->setDisabled( false );
    interface->ui->val_trgMode->setCurrentIndex( value );
  }

  // Local Shaped Trigger
  error = config->readSetting( "Enable Local Shaped Trigger", channel, value );
  if( error == -1 ) interface->ui->val_localShapedTrg->setDisabled( true );
  else{
    interface->ui->val_localShapedTrg->setDisabled( false );
    interface->ui->val_localShapedTrg->setCurrentIndex( value );
  }

  // Local Shaped Trigger Mode
  error = config->readSetting( "Local Shaped Trigger", channel, value );
  if( error == -1 ) interface->ui->val_localShapedTrgMode->setDisabled( true );
  else{
    interface->ui->val_localShapedTrgMode->setDisabled( false );
    interface->ui->val_localShapedTrgMode->setCurrentIndex( value );
  }

  // Shaped Trigger Width
  error = config->readSetting( "Shaped Trigger Width", channel, value );
  if( error == -1 ) interface->ui->val_shapedTrgWidth->setDisabled( true );
  else{
    interface->ui->val_shapedTrgWidth->setDisabled( false );
    interface->ui->val_shapedTrgWidth->setValue( value );
  }

  // Local Trigger Validation
  error = config->readSetting( "Enable Local Trigger Validation", channel, value );
  if( error == -1 ) interface->ui->val_localTrgVal->setDisabled( true );
  else{
    interface->ui->val_localTrgVal->setDisabled( false );
    interface->ui->val_localTrgVal->setCurrentIndex( value );
  }

  // Local Trigger Validation Mode
  error = config->readSetting( "Local Trigger Validation", channel, value );
  if( error == -1 ) interface->ui->val_localTrgValMode->setDisabled( true );
  else{
    interface->ui->val_localTrgValMode->setDisabled( false );
    interface->ui->val_localTrgValMode->setCurrentIndex( value );
  }

  // Pre Trigger
  error = config->readSetting( "Pre Trigger", channel, value );
  if( error == -1 ) interface->ui->val_preTrigger->setDisabled( true );
  else{
    interface->ui->val_preTrigger->setDisabled( false );
    interface->ui->val_preTrigger->setValue( value );
  }

  // Trigger Hold-Off
  error = config->readSetting( "Trigger Hold-off Width", channel, value );
  if( error == -1 ) interface->ui->val_trgHoldOff->setDisabled( true );
  else{
    interface->ui->val_trgHoldOff->setDisabled( false );
    interface->ui->val_trgHoldOff->setValue( value );
  }

}

void SlowControlChannel::onTrgModeChanged()
{
  QString text = interface->ui->val_extras2->currentText();
  if( text == "Reserved" ){
    interface->emitMessage( "Set at reserved value. Setting not changed." );
    return;
  }

  int value = interface->ui->val_trgMode->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Trigger Mode", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Trigger Mode" );
  }  
}

void SlowControlChannel::onLocalShapedTrgChanged()
{
  int value = interface->ui->val_localShapedTrg->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Enable Local Shaped Trigger", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Enable Local Shaped Trigger" );
  }  
}

void SlowControlChannel::onLocalShapedTrgModeChanged()
{
  int value = interface->ui->val_localShapedTrgMode->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Local Shaped Trigger", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Local Shaped Trigger" );
  }  
}

void SlowControlChannel::onShapedTrgWidthChanged()
{
  int value = interface->ui->val_shapedTrgWidth->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Shaped Trigger Width", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Shaped Trigger Width" );
  }  
}

void SlowControlChannel::onLocalTrgValChanged()
{
  int value = interface->ui->val_localTrgVal->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Enable Local Trigger Validation", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Enable Local Trigger Validation" );
  }  
}

void SlowControlChannel::onLocalTrgValModeChanged()
{
  int value = interface->ui->val_localTrgValMode->currentIndex();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Local Trigger Validation", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Local Trigger Validation" );
  }  
}

void SlowControlChannel::onPreTriggerChanged()
{
  int value = interface->ui->val_preTrigger->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Pre Trigger", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Pre Trigger" );
  }  
}

void SlowControlChannel::onTrgHoldOffChanged()
{
  int value = interface->ui->val_trgHoldOff->value();
  int channel = interface->ui->channelSelection->currentIndex();
  int board = interface->ui->boardSelection->currentIndex();
  config = &interface->caenBoards->config[board];

  int error = config->writeSetting( "Trigger Hold-off Width", channel, value );
  if( error == -1 ){
    interface->emitMessage( "Error writing to board at Trigger Hold-off Width" );
  }  
}