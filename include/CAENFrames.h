#ifndef CAENFRAMES_H_INCLUDED
#define CAENFRAMES_H_INCLUDED

// CAEN structures in order to retrieve the data in the SpyServer
// For more details about the data frames, consult CAEN manuals

struct CAENTimeTag
{
  uint32_t  timetag = 0; // [30:0] TimeTag, [31] CH

  uint32_t  GetTimeTag  () const { return (0x7FFFFFFF & timetag); }
  uint32_t  GetChannel  () const { return (0x80000000 & timetag)>>31; }
};

struct CAENWave
{
  
  uint32_t sample = 0;  // [13:0] S0, [14] DP0, [15] T0
                        // [29:16] S1, [30] DP1, [31] T1
                          
  int16_t   GetSample0  () const { return ((int16_t)((0x00003FFF & sample)<<2))/4; }
  uint32_t  GetDP0      () const { return (0x00004000 & sample)>>14; }
  uint32_t  GetT0       () const { return (0x00008000 & sample)>>15; }
  int16_t   GetSample1  () const { return ((int16_t)((0x3FFF0000 & sample)>>14))/4; }
  uint32_t  GetDP1      () const { return (0x40000000 & sample)>>30; }
  uint32_t  GetT1       () const { return (0x80000000 & sample)>>31; }
};

struct CAENEnergyPSD
{
  uint32_t  extras  = 0;  // [31:0] Extras
  uint32_t  energy  = 0;  // [14:0] Qshort, [15] PU, [31:16] Qlong

  uint32_t  GetExtras   () const { return (extras); }
  uint32_t  GetQshort   () const { return (0x00007FFF & energy); }
  uint32_t  GetPU       () const { return (0x00008000 & energy); }
  uint32_t  GetQlong    () const { return (0xFFFF0000 & energy); }
};

struct CAENEnergyPHA
{
  uint32_t  extras2 = 0;  // [31:0] Extras2
  uint32_t  energy  = 0;  // [14:0] Energy, [15] PU, [20] Extras

  uint32_t  GetExtras2  () const { return (extras2); }
  uint32_t  GetEnergy   () const { return (0x00007FFF & energy); }
  uint32_t  GetPU       () const { return (0x00008000 & energy); }
  uint32_t  GetExtras   () const { return (0x001F0000 & energy); }
};

struct CAENChannelHeaderPSD
{ 
  uint32_t size     = 0; // [30:0] Channel Aggregate Size, [31] FI
  uint32_t format   = 0; // [15:0] Num Samples/8, [19:16] DP, [21:20] AP2,
                         // [23:22] AP1, [26:24] EX, [27] ES, [28] E2, [29] ET,
                         // [30] EE, [31] DT

  uint32_t GetSize               () const { return (0x7FFFFFFF & size); }
  uint32_t GetFI                 () const { return (0x80000000 & size)>>31; }
  uint32_t GetNumSamples         () const { return (0x0000FFFF & format)<<3; }
  uint32_t GetDP1                () const { return (0x000F0000 & format)>>16; }
  uint32_t GetDP2                () const { return (0x00300000 & format)>>20; }
  uint32_t GetAP                 () const { return (0x00C00000 & format)>>22; }
  uint32_t GetEX                 () const { return (0x07000000 & format)>>24; }
  uint32_t GetES                 () const { return (0x08000000 & format)>>27; }
  uint32_t GetEE                 () const { return (0x10000000 & format)>>28; }
  uint32_t GetET                 () const { return (0x20000000 & format)>>29; }
  uint32_t GetEQ                 () const { return (0x40000000 & format)>>30; }
  uint32_t GetDT                 () const { return (0x80000000 & format)>>31; }
};

struct CAENChannelHeaderPHA
{ 
  uint32_t size     = 0; // [30:0] Channel Aggregate Size, [31] FI
  uint32_t format   = 0; // [15:0] Num Samples/8, [19:16] DP, [21:20] AP2,
                         // [23:22] AP1, [26:24] EX, [27] ES, [28] E2, [29] ET,
                         // [30] EE, [31] DT

  uint32_t GetSize               () const { return (0x7FFFFFFF & size); }
  uint32_t GetFI                 () const { return (0x80000000 & size)>>31; }
  uint32_t GetNumSamples         () const { return (0x0000FFFF & format)<<3; }
  uint32_t GetDP                 () const { return (0x000F0000 & format)>>16; }
  uint32_t GetAP2                () const { return (0x00300000 & format)>>20; }
  uint32_t GetAP1                () const { return (0x00C00000 & format)>>22; }
  uint32_t GetEX                 () const { return (0x07000000 & format)>>24; }
  uint32_t GetES                 () const { return (0x08000000 & format)>>27; }
  uint32_t GetE2                 () const { return (0x10000000 & format)>>28; }
  uint32_t GetET                 () const { return (0x20000000 & format)>>29; }
  uint32_t GetEE                 () const { return (0x40000000 & format)>>30; }
  uint32_t GetDT                 () const { return (0x80000000 & format)>>31; }
};

struct CAENBoardHeader
{
  uint32_t size    = 0;   // [27:0] Board Aggregate Size, [31:28] 1010
  uint32_t info    = 0;   // [7:0] Dual Channel Mask, [22:8] Pattern,
                          // [26] BF, [31:27] Board ID
  
  uint32_t counter = 0;   // [22:0] Board Aggregate Counter
  uint32_t timetag = 0;   // [31:0] Board Aggregate Time Tag
  
  uint32_t GetSize               () const { return (0x0FFFFFFF & size); }
  uint32_t GetDualChannelMask    () const { return (0x000000FF & info); }
  uint32_t GetPattern            () const { return (0x003FFF00 & info); }
  uint32_t GetBF                 () const { return (0x04000000 & info)>>26; }
  uint32_t GetBoardID            () const { return (0xF8000000 & info)>>27; }
  uint32_t GetCounter            () const { return (0x007FFFFF & counter); }
  uint64_t GetTimeTag            () const { return timetag; }
};

#endif // CAENFRAMES_H_INCLUDED
