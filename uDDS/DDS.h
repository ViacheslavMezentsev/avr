///////////////////////////////////////////////////////////
//  DDS.h
//  Implementation of the Class CDDS
//  Created on:      06-окт-2013 23:23:27
//  Original author: Слава
///////////////////////////////////////////////////////////

#if !defined(EA_4E769F17_3091_4b41_AEDE_D783CC2DD2FF__INCLUDED_)
#define EA_4E769F17_3091_4b41_AEDE_D783CC2DD2FF__INCLUDED_

enum EnSignals {

    sigSineWave = 0,
    sigSquareWave,
    sigTriangleWave,
    sigSawToothWave,
    sigRewsawToothWave,
    sigECGWave
};


typedef struct _SETTINGS {

    // Signal.
    volatile EnSignals Mode;		

    // Frequency [0..7].
    volatile uint8_t Frequency1;		

    // Frequency [8..15].
    volatile uint8_t Frequency2;

    // Frequency [16..31].
    volatile uint8_t Frequency3;		

    // Frequency value.
    volatile uint32_t Frequency;		

    // if "0" generator is OFF, "1" - ON.
    volatile uint8_t Flag;		

    // Accumulator.
    volatile uint32_t Accumulator;		

    volatile uint8_t On;

    // high speed frequency [1...4Mhz].
    volatile uint8_t HSFrequency;		

    // Frequency step value.
    volatile uint32_t DeltaFreq;	

} SETTINGS;

class CDDS {

public:

    static void Initialization();
    static void Run( PGM_P signal, uint8_t ad2, uint8_t ad1, uint8_t ad0 ); 

};

#endif // !defined(EA_4E769F17_3091_4b41_AEDE_D783CC2DD2FF__INCLUDED_)
