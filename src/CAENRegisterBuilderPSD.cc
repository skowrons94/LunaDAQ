#include "CAENRegisterBuilderPSD.h"

#include "registers/Registers.h"
#include "registers/RegistersPSD.h"

CAENRegisterBuilderPSD::CAENRegisterBuilderPSD( std::string dgtzName )
{
    this->fDgtzName = dgtzName;
}

CAENRegisterBuilderPSD::~CAENRegisterBuilderPSD()
{

}

void CAENRegisterBuilderPSD::ProduceRegisters( )
{
    if( fDgtzName.find("730") != std::string::npos ||
        fDgtzName.find("725") != std::string::npos ){

        fRegisters["Record Length"]                         = REG_REC_LENG;
        fRegisters["Input Dynamic Range"]                   = REG_DYN_RANG;
        fRegisters["Number of Events per Aggregate"]        = REG_EVT_AGGR;
        fRegisters["Pre Trigger"]                           = REG_PRE_TRIG;
        fRegisters["CFD Settings"]                          = PSD_CFD_SETT;
        fRegisters["Charge Zero Suppression Threshold"]     = PSD_ZER_SUPP;
        fRegisters["Short Gate Width"]                      = PSD_SHO_GATE;
        fRegisters["Long Gate Width"]                       = PSD_LON_GATE;
        fRegisters["Gate Offset"]                           = PSD_GAT_OFFS;
        fRegisters["Trigger Threshold"]                     = PSD_TRG_TRSD;
        fRegisters["Fixed Baseline"]                        = PSD_FIX_BSLE;
        fRegisters["Trigger Latency"]                       = PSD_TRG_LATE;
        fRegisters["Shaped Trigger Width"]                  = PSD_TRG_WDTH;
        fRegisters["Trigger Hold-off Width"]                = REG_TRG_HDOF;
        fRegisters["Threshold for the PSD cut"]             = PSD_PSD_CUTT;
        fRegisters["PUR-GAP Threshold"]                     = PSD_PUR_THSD;
        fRegisters["DPP Algorithm Control"]                 = REG_DPP_CTR1;
        fRegisters["DPP Algorithm Control 2"]               = PSD_DPP_CTR2;
        fRegisters["DC Offset"]                             = REG_ADC_OFFS;
        fRegisters["Veto Width"]                            = REG_VET_WDTH;
        fRegisters["Early Baseline Freeze"]                 = PSD_BSL_FRTM;
        fRegisters["Board Configuration"]                   = REG_BRD_CONF;
        fRegisters["Aggregate Organization"]                = REG_AGG_ORGA;
        fRegisters["Acquisition Control"]                   = REG_ACQ_CTRL;
        fRegisters["Global Trigger Mask"]                   = REG_TRG_MASK;
        fRegisters["Front Panel TRG-OUT (GPO) Enable mask"] = REG_GPO_ENAM;
        fRegisters["Front Panel I/O Control"]               = REG_FPI_CTRL;
        fRegisters["Channel Enable Mask"]                   = REG_CHA_ENAM;
        fRegisters["Trigger Validation Mask"]               = REG_TRG_VLDM;
        fRegisters["Extended Veto Delay"]                   = REG_EXT_VETO;
        fRegisters["Readout Control"]                       = REG_RDO_CTRL;
        fRegisters["Board ID"]                              = REG_BRD_IDTT;
        fRegisters["Aggregate Number per BLT"]              = REG_AGR_NBLT;

	    fSettings["Record Length"]                      = std::make_pair("Record Length", std::make_pair(0,13));
	    fSettings["Input Dynamic Range"]                = std::make_pair("Input Dynamic Range", std::make_pair(0,0));
	    fSettings["Number of Events per Aggregate"]     = std::make_pair("Number of Events per Aggregate", std::make_pair(0,9));
	    fSettings["Pre Trigger"]                        = std::make_pair("Pre Trigger", std::make_pair(0,8));
	    fSettings["CFD Delay"]                          = std::make_pair("CFD Settings", std::make_pair(0,7));
	    fSettings["CFD Fraction"]                       = std::make_pair("CFD Settings", std::make_pair(8,9));
	    fSettings["CFD Interpolation"]                  = std::make_pair("CFD Settings", std::make_pair(10,11));
	    fSettings["Charge Zero Suppression Threshold"]  = std::make_pair("Charge Zero Suppression Threshold", std::make_pair(0,15));
	    fSettings["Short Gate Width"]                   = std::make_pair("Short Gate Width", std::make_pair(0,11));
	    fSettings["Long Gate Width"]                    = std::make_pair("Long Gate Width", std::make_pair(0,15));
	    fSettings["Gate Offset"]                        = std::make_pair("Gate Offset", std::make_pair(0,7));
	    fSettings["Fixed Baseline"]                     = std::make_pair("Fixed Baseline", std::make_pair(0,13));
	    fSettings["Trigger Threshold"]                  = std::make_pair("Trigger Threshold", std::make_pair(0,13));
	    fSettings["Shaped Trigger Width"]               = std::make_pair("Shaped Trigger Width", std::make_pair(0,9));
	    fSettings["Trigger Hold-off Width"]             = std::make_pair("Trigger Hold-off Width", std::make_pair(0,15));
	    fSettings["Threshold for the PSD cut"]          = std::make_pair("Threshold for the PSD cut", std::make_pair(0,9));
	    fSettings["PUR-GAP Threshold"]                  = std::make_pair("PUR-GAP Threshold", std::make_pair(0,11));
	    fSettings["Charge Sensitivity"]                 = std::make_pair("DPP Algorithm Control", std::make_pair(0,2));
	    fSettings["Charge Pedestal"]                    = std::make_pair("DPP Algorithm Control", std::make_pair(4,4));
	    fSettings["Trigger Counting"]                   = std::make_pair("DPP Algorithm Control", std::make_pair(5,5));
	    fSettings["Discrimination Mode"]                = std::make_pair("DPP Algorithm Control", std::make_pair(6,6));
	    fSettings["Pile-Up Counting"]                   = std::make_pair("DPP Algorithm Control", std::make_pair(7,7));
	    fSettings["Internal Pulse"]                     = std::make_pair("DPP Algorithm Control", std::make_pair(8,8));
	    fSettings["Internal Pulse Rate"]                = std::make_pair("DPP Algorithm Control", std::make_pair(9,10));
	    fSettings["Baseline Recalculation"]             = std::make_pair("DPP Algorithm Control", std::make_pair(15,15));
	    fSettings["Invert Input"]                       = std::make_pair("DPP Algorithm Control", std::make_pair(16,16));
	    fSettings["Trigger Mode"]                       = std::make_pair("DPP Algorithm Control", std::make_pair(18,19));
	    fSettings["Baseline Averaging Window"]          = std::make_pair("DPP Algorithm Control", std::make_pair(20,22));
	    fSettings["Disable Self Trigger"]               = std::make_pair("DPP Algorithm Control", std::make_pair(24,24));
	    fSettings["Long Threshold"]                     = std::make_pair("DPP Algorithm Control", std::make_pair(25,25));
	    fSettings["Pile-Up Rejection"]                  = std::make_pair("DPP Algorithm Control", std::make_pair(26,26));
	    fSettings["PSD Cut"]                            = std::make_pair("DPP Algorithm Control", std::make_pair(27,28));
	    fSettings["Trigger Hysteresis"]                 = std::make_pair("DPP Algorithm Control", std::make_pair(30,30));
	    fSettings["Inhibit Zero Crossing"]              = std::make_pair("DPP Algorithm Control", std::make_pair(31,31));
	    fSettings["Local Shaped Trigger"]               = std::make_pair("DPP Algorithm Control 2", std::make_pair(0,1));
	    fSettings["Enable Local Shaped Trigger"]        = std::make_pair("DPP Algorithm Control 2", std::make_pair(2,2));
	    fSettings["Local Trigger Validation"]           = std::make_pair("DPP Algorithm Control 2", std::make_pair(4,5));
	    fSettings["Enable Local Trigger Validation"]    = std::make_pair("DPP Algorithm Control 2", std::make_pair(6,6));
	    fSettings["Extras 2"]                           = std::make_pair("DPP Algorithm Control 2", std::make_pair(8,10));
	    fSettings["Smoothed Signal"]                    = std::make_pair("DPP Algorithm Control 2", std::make_pair(11,11));
	    fSettings["Smoothed Signal Samples"]            = std::make_pair("DPP Algorithm Control 2", std::make_pair(12,15));
	    fSettings["Count Trigger Step"]                 = std::make_pair("DPP Algorithm Control 2", std::make_pair(16,17));
	    fSettings["Veto Source"]                        = std::make_pair("DPP Algorithm Control 2", std::make_pair(18,19));
	    fSettings["Mark Saturated Pulses"]              = std::make_pair("DPP Algorithm Control 2", std::make_pair(24,24));
	    fSettings["Additional Local Trigger Val"]       = std::make_pair("DPP Algorithm Control 2", std::make_pair(25,26));
	    fSettings["Veto Signal Mode"]                   = std::make_pair("DPP Algorithm Control 2", std::make_pair(27,27));
	    fSettings["Reset Time Stamp"]                   = std::make_pair("DPP Algorithm Control 2", std::make_pair(28,28));
	    fSettings["Veto Width"]                         = std::make_pair("Veto Width", std::make_pair(0,15));
	    fSettings["Veto Width Steps"]                   = std::make_pair("Veto Width", std::make_pair(16,17));
	    fSettings["Automatic Data Flush"]               = std::make_pair("Board Configuration", std::make_pair(0,0));
	    fSettings["Trigger Propagation"]                = std::make_pair("Board Configuration", std::make_pair(2,2));
	    fSettings["Dual Trace"]                         = std::make_pair("Board Configuration", std::make_pair(11,11));
	    fSettings["Analog Probe"]                       = std::make_pair("Board Configuration", std::make_pair(12,13));
	    fSettings["Enable Waveform"]                    = std::make_pair("Board Configuration", std::make_pair(16,16));
	    fSettings["Enable Extras"]                      = std::make_pair("Board Configuration", std::make_pair(17,17));
	    fSettings["Digital Virtual Probe 1"]            = std::make_pair("Board Configuration", std::make_pair(23,25));
	    fSettings["Digital Virtual Probe 2"]            = std::make_pair("Board Configuration", std::make_pair(26,28));
	    fSettings["Enable Digital Probe"]               = std::make_pair("Board Configuration", std::make_pair(31,31));
	    fSettings["Aggregate Organization"]             = std::make_pair("Aggregate Organization", std::make_pair(0,3));
	    fSettings["Start/Stop Mode"]                    = std::make_pair("Acquisition Control", std::make_pair(0,1));
	    fSettings["Acquistion Start/Arm"]               = std::make_pair("Acquisition Control", std::make_pair(2,2));
	    fSettings["LVDS I/O Busy Enable"]               = std::make_pair("Acquisition Control", std::make_pair(8,8));
	    fSettings["LVDS I/O Veto Enable"]               = std::make_pair("Acquisition Control", std::make_pair(9,9));
	    fSettings["Veto In"]                            = std::make_pair("Acquisition Control", std::make_pair(12,12));
	    fSettings["Global Trigger Generation"]          = std::make_pair("Global Trigger Mask", std::make_pair(0,7));
	    fSettings["Majority Coincidence Window"]        = std::make_pair("Global Trigger Mask", std::make_pair(20,23));
	    fSettings["Majority Level"]                     = std::make_pair("Global Trigger Mask", std::make_pair(24,26));
	    fSettings["LVDS Trigger"]                       = std::make_pair("Global Trigger Mask", std::make_pair(29,29));
	    fSettings["External Trigger"]                   = std::make_pair("Global Trigger Mask", std::make_pair(30,30));
	    fSettings["Software Trigger"]                   = std::make_pair("Global Trigger Mask", std::make_pair(31,31));
	    fSettings["TRG-OUT Mask"]                       = std::make_pair("Front Panel TRG-OUT (GPO) Enable mask", std::make_pair(0,7));
	    fSettings["TRG-OUT Logic"]                      = std::make_pair("Front Panel TRG-OUT (GPO) Enable mask", std::make_pair(8,9));
	    fSettings["TRG-OUT Majority Level"]             = std::make_pair("Front Panel TRG-OUT (GPO) Enable mask", std::make_pair(10,12));
	    fSettings["TRG-OUT LVDS Trigger"]               = std::make_pair("Front Panel TRG-OUT (GPO) Enable mask", std::make_pair(29,29));
	    fSettings["TRG-OUT External Trigger"]           = std::make_pair("Front Panel TRG-OUT (GPO) Enable mask", std::make_pair(30,30));
	    fSettings["TRG-OUT Software Trigger"]           = std::make_pair("Front Panel TRG-OUT (GPO) Enable mask", std::make_pair(31,31));
	    fSettings["Channel Enable Mask"]                = std::make_pair("Channel Enable Mask", std::make_pair(0,15));
	    fSettings["Trigger Validation Generation"]      = std::make_pair("Trigger Validation Mask", std::make_pair(0,7));
	    fSettings["Trigger Validation Logic"]           = std::make_pair("Trigger Validation Mask", std::make_pair(8,9));
	    fSettings["Trigger Validation Majority"]        = std::make_pair("Trigger Validation Mask", std::make_pair(10,12));
	    fSettings["Trigger Validation LVDS Global"]     = std::make_pair("Trigger Validation Mask", std::make_pair(28,28));
	    fSettings["Trigger Validation LVDS Individual"] = std::make_pair("Trigger Validation Mask", std::make_pair(29,29));
	    fSettings["Trigger Validation External"]        = std::make_pair("Trigger Validation Mask", std::make_pair(30,30));
	    fSettings["Trigger Validation Software"]        = std::make_pair("Trigger Validation Mask", std::make_pair(31,31));
	    fSettings["Extended Veto Delay"]                = std::make_pair("Extended Veto Delay", std::make_pair(0,15));
	    fSettings["Aggregate Number per BLT"]           = std::make_pair("Aggregate Number per BLT", std::make_pair(0,9));

    }

    else if( fDgtzName.find("720") != std::string::npos ){

        fRegisters["Short Gate Width"]                      = PSD_SHO_GATE;
        fRegisters["Long Gate Width"]                       = PSD_LON_GATE;
        fRegisters["Gate Offset"]                           = PSD_GAT_OFFS;
        fRegisters["Trigger Threshold"]                     = PSD_TRG_TRSD;
        fRegisters["Fixed Baseline"]                        = PSD_FIX_BSLE;
        fRegisters["Trigger Latency"]                       = PSD_TRG_LATE;
        fRegisters["Shaped Trigger Width"]                  = PSD_TRG_WDTH;
        fRegisters["Threshold for the PSD cut"]             = PSD_PSD_CUTT;
        fRegisters["PUR-GAP Threshold"]                     = PSD_PUR_THSD;
        fRegisters["DPP Algorithm Control"]                 = REG_DPP_CTR1;
        fRegisters["DC Offset"]                             = REG_ADC_OFFS;
        fRegisters["Board Configuration"]                   = REG_BRD_CONF;
        fRegisters["Aggregate Organization"]                = REG_AGG_ORGA;
        fRegisters["Record Length"]                         = REG_REC_LEN2;
        fRegisters["Number of Events per Aggregate"]        = REG_EVT_AGG2;
        fRegisters["Pre Trigger"]                           = REG_PRE_TRI2;
        fRegisters["Trigger Hold-off Width"]                = REG_TRG_HDO2;
        fRegisters["Acquisition Control"]                   = REG_ACQ_CTRL;
        fRegisters["Global Trigger Mask"]                   = REG_TRG_MASK;
        fRegisters["Front Panel TRG-OUT (GPO) Enable mask"] = REG_GPO_ENAM;
        fRegisters["Front Panel I/O Control"]               = REG_FPI_CTRL;
        fRegisters["Channel Enable Mask"]                   = REG_CHA_ENAM;
        fRegisters["Trigger Validation Mask"]               = REG_TRG_VLDM;
        fRegisters["Extended Veto Delay"]                   = REG_EXT_VETO;
        fRegisters["Readout Control"]                       = REG_RDO_CTRL;
        fRegisters["Board ID"]                              = REG_BRD_IDTT;
        fRegisters["Aggregate Number per BLT"]              = REG_AGR_NBLT;

	    fSettings["Record Length"]                      = std::make_pair("Record Length", std::make_pair(0,11));
	    fSettings["Number of Events per Aggregate"]     = std::make_pair("Number of Events per Aggregate", std::make_pair(0,9));
	    fSettings["Pre Trigger"]                        = std::make_pair("Pre Trigger", std::make_pair(0,8));
	    fSettings["Short Gate Width"]                   = std::make_pair("Short Gate Width", std::make_pair(0,9));
	    fSettings["Long Gate Width"]                    = std::make_pair("Long Gate Width", std::make_pair(0,13));
	    fSettings["Gate Offset"]                        = std::make_pair("Gate Offset", std::make_pair(0,7));
	    fSettings["Fixed Baseline"]                     = std::make_pair("Fixed Baseline", std::make_pair(0,11));
	    fSettings["Trigger Threshold"]                  = std::make_pair("Trigger Threshold", std::make_pair(0,11));
	    fSettings["Shaped Trigger Width"]               = std::make_pair("Shaped Trigger Width", std::make_pair(0,9));
	    fSettings["Trigger Hold-off Width"]             = std::make_pair("Trigger Hold-off Width", std::make_pair(0,9));
	    fSettings["Threshold for the PSD cut"]          = std::make_pair("Threshold for the PSD cut", std::make_pair(0,9));
	    fSettings["PUR-GAP Threshold"]                  = std::make_pair("PUR-GAP Threshold", std::make_pair(0,11));
	    fSettings["Charge Sensitivity"]                 = std::make_pair("DPP Algorithm Control", std::make_pair(0,1));
	    fSettings["Charge Pedestal"]                    = std::make_pair("DPP Algorithm Control", std::make_pair(4,4));
	    fSettings["Trigger Counting"]                   = std::make_pair("DPP Algorithm Control", std::make_pair(5,5));
	    fSettings["Extended Time Stamp"]                = std::make_pair("DPP Algorithm Control", std::make_pair(7,7));
	    fSettings["Internal Pulse"]                     = std::make_pair("DPP Algorithm Control", std::make_pair(8,8));
	    fSettings["Internal Pulse Rate"]                = std::make_pair("DPP Algorithm Control", std::make_pair(9,10));
	    fSettings["Invert Input"]                       = std::make_pair("DPP Algorithm Control", std::make_pair(16,16));
	    fSettings["Trigger Mode"]                       = std::make_pair("DPP Algorithm Control", std::make_pair(18,19));
	    fSettings["Baseline Averaging Window"]          = std::make_pair("DPP Algorithm Control", std::make_pair(20,22));
	    fSettings["Disable Self Trigger"]               = std::make_pair("DPP Algorithm Control", std::make_pair(24,24));
	    fSettings["Pile-Up Rejection"]                  = std::make_pair("DPP Algorithm Control", std::make_pair(26,26));
	    fSettings["PSD Cut"]                            = std::make_pair("DPP Algorithm Control", std::make_pair(27,28));
        fSettings["Over Range Rejection"]               = std::make_pair("DPP Algorithm Control", std::make_pair(29,29));
        fSettings["Trigger Hysteresis"]                 = std::make_pair("DPP Algorithm Control", std::make_pair(30,30));
	    fSettings["Veto Width"]                         = std::make_pair("Veto Width", std::make_pair(0,15));
	    fSettings["Veto Width Steps"]                   = std::make_pair("Veto Width", std::make_pair(16,17));
	    fSettings["Trigger Propagation"]                = std::make_pair("Board Configuration", std::make_pair(2,2));
	    fSettings["Dual Trace"]                         = std::make_pair("Board Configuration", std::make_pair(11,11));
	    fSettings["Enable Waveform"]                    = std::make_pair("Board Configuration", std::make_pair(16,16));
	    fSettings["Enable Extras"]                      = std::make_pair("Board Configuration", std::make_pair(17,17));
	    fSettings["Enable Time Stamp"]                  = std::make_pair("Board Configuration", std::make_pair(18,18));
	    fSettings["Enable Charge"]                      = std::make_pair("Board Configuration", std::make_pair(19,19));
	    fSettings["Digital Virtual Probe 1"]            = std::make_pair("Board Configuration", std::make_pair(23,25));
	    fSettings["Digital Virtual Probe 2"]            = std::make_pair("Board Configuration", std::make_pair(26,28));
	    fSettings["Aggregate Organization"]             = std::make_pair("Aggregate Organization", std::make_pair(0,3));
	    fSettings["Start/Stop Mode"]                    = std::make_pair("Acquisition Control", std::make_pair(0,1));
	    fSettings["Acquistion Start/Arm"]               = std::make_pair("Acquisition Control", std::make_pair(2,2));
	    fSettings["LVDS I/O Busy Enable"]               = std::make_pair("Acquisition Control", std::make_pair(8,8));
	    fSettings["LVDS I/O Veto Enable"]               = std::make_pair("Acquisition Control", std::make_pair(9,9));
	    fSettings["Veto In"]                            = std::make_pair("Acquisition Control", std::make_pair(12,12));
	    fSettings["Global Trigger Generation"]          = std::make_pair("Global Trigger Mask", std::make_pair(0,7));
	    fSettings["Majority Coincidence Window"]        = std::make_pair("Global Trigger Mask", std::make_pair(20,23));
	    fSettings["Majority Level"]                     = std::make_pair("Global Trigger Mask", std::make_pair(24,26));
	    fSettings["LVDS Trigger"]                       = std::make_pair("Global Trigger Mask", std::make_pair(29,29));
	    fSettings["External Trigger"]                   = std::make_pair("Global Trigger Mask", std::make_pair(30,30));
	    fSettings["Software Trigger"]                   = std::make_pair("Global Trigger Mask", std::make_pair(31,31));
	    fSettings["TRG-OUT Mask"]                       = std::make_pair("Front Panel TRG-OUT (GPO) Enable mask", std::make_pair(0,7));
	    fSettings["TRG-OUT Logic"]                      = std::make_pair("Front Panel TRG-OUT (GPO) Enable mask", std::make_pair(8,9));
	    fSettings["TRG-OUT Majority Level"]             = std::make_pair("Front Panel TRG-OUT (GPO) Enable mask", std::make_pair(10,12));
	    fSettings["TRG-OUT LVDS Trigger"]               = std::make_pair("Front Panel TRG-OUT (GPO) Enable mask", std::make_pair(29,29));
	    fSettings["TRG-OUT External Trigger"]           = std::make_pair("Front Panel TRG-OUT (GPO) Enable mask", std::make_pair(30,30));
	    fSettings["TRG-OUT Software Trigger"]           = std::make_pair("Front Panel TRG-OUT (GPO) Enable mask", std::make_pair(31,31));
	    fSettings["Channel Enable Mask"]                = std::make_pair("Channel Enable Mask", std::make_pair(0,7));
	    fSettings["Trigger Validation Generation"]      = std::make_pair("Trigger Validation Mask", std::make_pair(0,7));
	    fSettings["Trigger Validation Logic"]           = std::make_pair("Trigger Validation Mask", std::make_pair(8,9));
	    fSettings["Trigger Validation Majority"]        = std::make_pair("Trigger Validation Mask", std::make_pair(10,12));
	    fSettings["Trigger Validation LVDS Global"]     = std::make_pair("Trigger Validation Mask", std::make_pair(28,28));
	    fSettings["Trigger Validation LVDS Individual"] = std::make_pair("Trigger Validation Mask", std::make_pair(29,29));
	    fSettings["Trigger Validation External"]        = std::make_pair("Trigger Validation Mask", std::make_pair(30,30));
	    fSettings["Trigger Validation Software"]        = std::make_pair("Trigger Validation Mask", std::make_pair(31,31));
	    fSettings["Extended Veto Delay"]                = std::make_pair("Extended Veto Delay", std::make_pair(0,15));
	    fSettings["Aggregate Number per BLT"]           = std::make_pair("Aggregate Number per BLT", std::make_pair(0,9));

    }

}


std::map<std::string, uint16_t> CAENRegisterBuilderPSD::GetRegisters( )
{
    return fRegisters;
}

std::map<std::string, std::pair<std::string,std::pair<int,int>>> CAENRegisterBuilderPSD::GetSettings( )
{
    return fSettings;
}
