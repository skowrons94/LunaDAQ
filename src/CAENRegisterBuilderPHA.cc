#include "CAENRegisterBuilderPHA.h"

#include "registers/Registers.h"
#include "registers/RegistersPHA.h"

CAENRegisterBuilderPHA::CAENRegisterBuilderPHA( std::string dgtzName )
{
    this->fDgtzName = dgtzName;
}

CAENRegisterBuilderPHA::~CAENRegisterBuilderPHA()
{

}

void CAENRegisterBuilderPHA::ProduceRegisters( )
{
    if( fDgtzName.find("730") != std::string::npos ||
        fDgtzName.find("725") != std::string::npos ){

        fRegisters["Record Length"]                          = REG_REC_LENG;
        fRegisters["Input Dynamic Range"]                    = REG_DYN_RANG;
        fRegisters["Number of Events per Aggregate"]         = REG_EVT_AGGR;
        fRegisters["Pre Trigger"]                            = REG_PRE_TRIG;
        fRegisters["RC-CR2 Smoothing Factor"]                = PHA_MOV_AVRG;
        fRegisters["Input Rise Time"]                        = PHA_INP_RTIM;
        fRegisters["Trapezoid Rise Time"]                    = PHA_TRZ_RTIM;
        fRegisters["Trapezoid Flat Top"]                     = PHA_TRZ_FTOP;
        fRegisters["Trapezoid Peaking Time"]                 = PHA_TRZ_PTIM;
        fRegisters["Trapezoid Decay Time"]                   = PHA_TRZ_DTIM;
        fRegisters["Trigger Threshold"]                      = PHA_TRG_TRSD;
        fRegisters["Rise Time Validation Window"]            = PHA_VAL_WIND;
        fRegisters["Trigger Hold-off Width"]                 = REG_TRG_HDOF;
        fRegisters["Peak Hold-off"]                          = PHA_PEK_HOFF;
        fRegisters["DPP Algorithm Control"]                  = REG_DPP_CTR1;
        fRegisters["Shaped Trigger Width"]                   = PHA_TRG_WDTH;
        fRegisters["DC Offset"]                              = REG_ADC_OFFS;
        fRegisters["DPP Algorithm Control 2"]                = PHA_DPP_CTR2;
        fRegisters["Fine Gain"]                              = PHA_FIN_GAIN;
        fRegisters["Veto Width"]                             = REG_VET_WDTH;
        fRegisters["Board Configuration"]                    = REG_BRD_CONF;
        fRegisters["Aggregate Organization"]                 = REG_AGG_ORGA;
        fRegisters["Acquisition Control"]                    = REG_ACQ_CTRL;
        fRegisters["Global Trigger Mask"]                    = REG_TRG_MASK;
        fRegisters["Front Panel TRG-OUT (GPO) Enable mask"]  = REG_GPO_ENAM;
        fRegisters["Front Panel I/O Control"]                = REG_FPI_CTRL;
        fRegisters["Channel Enable Mask"]                    = REG_CHA_ENAM;
        fRegisters["Trigger Validation Mask"]                = REG_TRG_VLDM;
        fRegisters["Extended Veto Delay"]                    = REG_EXT_VETO;
        fRegisters["Readout Control"]                        = REG_RDO_CTRL;
        fRegisters["Board ID"]                               = REG_BRD_IDTT;
        fRegisters["Aggregate Number per BLT"]               = REG_AGR_NBLT;

        fSettings["Record Length"]                      = std::make_pair("Record Length", std::make_pair(0,13));
	    fSettings["Input Dynamic Range"]                = std::make_pair("Input Dynamic Range", std::make_pair(0,0));
	    fSettings["Number of Events per Aggregate"]     = std::make_pair("Number of Events per Aggregate", std::make_pair(0,9));
	    fSettings["Pre Trigger"]                        = std::make_pair("Pre Trigger", std::make_pair(0,8));
	    fSettings["RC-CR2 Smoothing Factor"]            = std::make_pair("RC-CR2 Smoothing Factor", std::make_pair(0,5));
	    fSettings["Input Rise Time"]                    = std::make_pair("Input Rise Time", std::make_pair(0,7));
	    fSettings["Trapezoid Rise Time"]                = std::make_pair("Trapezoid Rise Time", std::make_pair(0,11));
	    fSettings["Trapezoid Flat Top"]                 = std::make_pair("Trapezoid Flat Top", std::make_pair(0,11));
	    fSettings["Trapezoid Peaking Time"]             = std::make_pair("Trapezoid Peaking Time", std::make_pair(0,11));
	    fSettings["Trapezoid Decay Time"]               = std::make_pair("Trapezoid Decay Time", std::make_pair(0,15));
	    fSettings["Trigger Threshold"]                  = std::make_pair("Trigger Threshold", std::make_pair(0,13));
	    fSettings["Rise Time Validation Window"]        = std::make_pair("Rise Time Validation Window", std::make_pair(0,9));
	    fSettings["Trigger Hold-off Width"]             = std::make_pair("Trigger Hold-off Width", std::make_pair(0,9));
	    fSettings["Peak Hold-off"]                      = std::make_pair("Peak Hold-off", std::make_pair(0,9));
	    fSettings["Trapezoid Rescaling"]                = std::make_pair("DPP Algorithm Control", std::make_pair(0,5));
	    fSettings["Decimation"]                         = std::make_pair("DPP Algorithm Control", std::make_pair(8,9));
	    fSettings["Decimation Gain"]                    = std::make_pair("DPP Algorithm Control", std::make_pair(10,11));
	    fSettings["Peak Mean"]                          = std::make_pair("DPP Algorithm Control", std::make_pair(12,13));
	    fSettings["Invert Input"]                       = std::make_pair("DPP Algorithm Control", std::make_pair(16,16));
	    fSettings["Trigger Mode"]                       = std::make_pair("DPP Algorithm Control", std::make_pair(18,19));
	    fSettings["Baseline Averaging Window"]          = std::make_pair("DPP Algorithm Control", std::make_pair(20,22));
	    fSettings["Disable Self Trigger"]               = std::make_pair("DPP Algorithm Control", std::make_pair(24,24));
	    fSettings["Enable Roll-Over"]                   = std::make_pair("DPP Algorithm Control", std::make_pair(26,26));
	    fSettings["Enable Pile-Up"]                     = std::make_pair("DPP Algorithm Control", std::make_pair(27,27));
	    fSettings["Shaped Trigger Width"]               = std::make_pair("Shaped Trigger Width", std::make_pair(0,9));
	    fSettings["DC Offset"]                          = std::make_pair("DC Offset", std::make_pair(0,15));
	    fSettings["Local Shaped Trigger"]               = std::make_pair("DPP Algorithm Control 2", std::make_pair(0,1));
	    fSettings["Enable Local Shaped Trigger"]        = std::make_pair("DPP Algorithm Control 2", std::make_pair(2,2));
	    fSettings["Local Trigger Validation"]           = std::make_pair("DPP Algorithm Control 2", std::make_pair(4,5));
	    fSettings["Enable Local Trigger Validation"]    = std::make_pair("DPP Algorithm Control 2", std::make_pair(6,6));
	    fSettings["Extras 2"]                           = std::make_pair("DPP Algorithm Control 2", std::make_pair(8,10));
	    fSettings["Veto Source"]                        = std::make_pair("DPP Algorithm Control 2", std::make_pair(14,15));
	    fSettings["Count Trigger Step"]                 = std::make_pair("DPP Algorithm Control 2", std::make_pair(16,17));
	    fSettings["Ready Baseline"]                     = std::make_pair("DPP Algorithm Control 2", std::make_pair(18,18));
	    fSettings["Baseline Restorer"]                  = std::make_pair("DPP Algorithm Control 2", std::make_pair(29,29));
	    fSettings["Fine Gain"]                          = std::make_pair("Fine Gain", std::make_pair(0,15));
	    fSettings["Veto Width"]                         = std::make_pair("Veto Width", std::make_pair(0,15));
	    fSettings["Veto Width Steps"]                   = std::make_pair("Veto Width", std::make_pair(16,17));
	    fSettings["Automatic Data Flush"]               = std::make_pair("Board Configuration", std::make_pair(0,0));
	    fSettings["Decimated Samples"]                  = std::make_pair("Board Configuration", std::make_pair(1,1));
	    fSettings["Trigger Propagation"]                = std::make_pair("Board Configuration", std::make_pair(2,2));
	    fSettings["Dual Trace"]                         = std::make_pair("Board Configuration", std::make_pair(11,11));
	    fSettings["Analog Probe 1"]                     = std::make_pair("Board Configuration", std::make_pair(12,13));
	    fSettings["Analog Probe 2"]                     = std::make_pair("Board Configuration", std::make_pair(14,15));
	    fSettings["Enable Waveform"]                    = std::make_pair("Board Configuration", std::make_pair(16,16));
	    fSettings["Enable Extras"]                      = std::make_pair("Board Configuration", std::make_pair(17,17));
	    fSettings["Digital Virtual Probe 1"]            = std::make_pair("Board Configuration", std::make_pair(20,23));
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

    else if( fDgtzName.find("724") != std::string::npos ||
             fDgtzName.find("781") != std::string::npos || 
             fDgtzName.find("782") != std::string::npos ){

        fRegisters["Pre Trigger"]                           = REG_PRE_TRIG;
        fRegisters["RC-CR2 Smoothing Factor"]               = PHA_MOV_AVRG;
        fRegisters["Input Rise Time"]                       = PHA_INP_RTIM;
        fRegisters["Trapezoid Rise Time"]                   = PHA_TRZ_RTIM;
        fRegisters["Trapezoid Flat Top"]                    = PHA_TRZ_FTOP;
        fRegisters["Trapezoid Peaking Time"]                = PHA_TRZ_PTIM;
        fRegisters["Trapezoid Decay Time"]                  = PHA_TRZ_DTIM;
        fRegisters["Trigger Threshold"]                     = PHA_TRG_TRSD;
        fRegisters["Trigger Hold-off Width"]                = REG_TRG_HDOF;
        fRegisters["Peak Hold-off"]                         = PHA_PEK_HOFF;
        fRegisters["DPP Algorithm Control"]                 = REG_DPP_CTR1;
        fRegisters["Shaped Trigger Width"]                  = PHA_TRG_WDTH;
        fRegisters["DC Offset"]                             = REG_ADC_OFFS;
        fRegisters["DPP Algorithm Control 2"]               = PHA_DPP_CTR2;
        fRegisters["Fine Gain"]                             = PHA_FIN_GAIN;
        fRegisters["Veto Width"]                            = REG_VET_WDTH;
        fRegisters["Board Configuration"]                   = REG_BRD_CONF;
        fRegisters["Aggregate Organization"]                = REG_AGG_ORGA;
        fRegisters["Record Length"]                         = REG_REC_LEN2;
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

        fSettings["Record Length"]                      = std::make_pair("Record Length", std::make_pair(0,15));
	    fSettings["Input Dynamic Range"]                = std::make_pair("Input Dynamic Range", std::make_pair(0,0));
	    fSettings["Number of Events per Aggregate"]     = std::make_pair("Number of Events per Aggregate", std::make_pair(0,9));
	    fSettings["Pre Trigger"]                        = std::make_pair("Pre Trigger", std::make_pair(0,10));
	    fSettings["RC-CR2 Smoothing Factor"]            = std::make_pair("RC-CR2 Smoothing Factor", std::make_pair(0,5));
	    fSettings["Input Rise Time"]                    = std::make_pair("Input Rise Time", std::make_pair(0,11));
	    fSettings["Trapezoid Rise Time"]                = std::make_pair("Trapezoid Rise Time", std::make_pair(0,11));
	    fSettings["Trapezoid Flat Top"]                 = std::make_pair("Trapezoid Flat Top", std::make_pair(0,11));
	    fSettings["Trapezoid Peaking Time"]             = std::make_pair("Trapezoid Peaking Time", std::make_pair(0,11));
	    fSettings["Trapezoid Decay Time"]               = std::make_pair("Trapezoid Decay Time", std::make_pair(0,15));
	    fSettings["Trigger Threshold"]                  = std::make_pair("Trigger Threshold", std::make_pair(0,13));
	    fSettings["Rise Time Validation Window"]        = std::make_pair("Rise Time Validation Window", std::make_pair(0,9));
	    fSettings["Trigger Hold-off Width"]             = std::make_pair("Trigger Hold-off Width", std::make_pair(0,15));
	    fSettings["Peak Hold-off"]                      = std::make_pair("Peak Hold-off", std::make_pair(0,9));
	    fSettings["Trapezoid Rescaling"]                = std::make_pair("DPP Algorithm Control", std::make_pair(0,5));
	    fSettings["Peak Mean"]                          = std::make_pair("DPP Algorithm Control", std::make_pair(12,13));
	    fSettings["Invert Input"]                       = std::make_pair("DPP Algorithm Control", std::make_pair(16,16));
	    fSettings["Trigger Mode"]                       = std::make_pair("DPP Algorithm Control", std::make_pair(18,19));
	    fSettings["Baseline Averaging Window"]          = std::make_pair("DPP Algorithm Control", std::make_pair(20,22));
	    fSettings["Disable Self Trigger"]               = std::make_pair("DPP Algorithm Control", std::make_pair(24,24));
	    fSettings["Enable Roll-Over"]                   = std::make_pair("DPP Algorithm Control", std::make_pair(26,26));
	    fSettings["Shaped Trigger Width"]               = std::make_pair("Shaped Trigger Width", std::make_pair(0,9));
	    fSettings["DC Offset"]                          = std::make_pair("DC Offset", std::make_pair(0,15));
	    fSettings["Veto Source"]                        = std::make_pair("DPP Algorithm Control 2", std::make_pair(0,5));
	    fSettings["Count Trigger Step"]                 = std::make_pair("DPP Algorithm Control 2", std::make_pair(16,17));
	    fSettings["Fine Gain"]                          = std::make_pair("Fine Gain", std::make_pair(0,15));
	    fSettings["Veto Width Steps"]                   = std::make_pair("Veto Width", std::make_pair(0,23));
	    fSettings["Automatic Data Flush"]               = std::make_pair("Board Configuration", std::make_pair(0,0));
	    fSettings["Trigger Propagation"]                = std::make_pair("Board Configuration", std::make_pair(2,2));
	    fSettings["Dual Trace"]                         = std::make_pair("Board Configuration", std::make_pair(11,11));
	    fSettings["Analog Probe 1"]                     = std::make_pair("Board Configuration", std::make_pair(12,13));
	    fSettings["Analog Probe 2"]                     = std::make_pair("Board Configuration", std::make_pair(14,15));
	    fSettings["Enable Waveform"]                    = std::make_pair("Board Configuration", std::make_pair(16,16));
	    fSettings["Enable Energy"]                      = std::make_pair("Board Configuration", std::make_pair(17,17));
	    fSettings["Enable Time Stamp"]                  = std::make_pair("Board Configuration", std::make_pair(18,18));
	    fSettings["Digital Virtual Probe 1"]            = std::make_pair("Board Configuration", std::make_pair(20,23));
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


std::map<std::string, uint16_t> CAENRegisterBuilderPHA::GetRegisters( )
{
    return fRegisters;
}

std::map<std::string, std::pair<std::string,std::pair<int,int>>> CAENRegisterBuilderPHA::GetSettings( )
{
    return fSettings;
}