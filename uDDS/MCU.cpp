///////////////////////////////////////////////////////////
//  MCU.cpp
//  Implementation of the Class CMCU
//  Created on:      14-июл-2011 11:34:33
//  Original author: Слава
///////////////////////////////////////////////////////////

#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "MCU.h"


// -=[ Внешние ссылки ]=-


// -=[ Постоянные во флеш-памяти ]=-

// Таблицы сигналов
// sine
#ifdef __ICCAVR__
    __flash uint8_t SineWave[ 256 ] = {
#elif defined __GNUC__
    PROGMEM uint8_t _SineWave[ 256 ] = {
#endif
    0x80, 0x83, 0x86, 0x89, 0x8c, 0x8f, 0x92, 0x95, 0x98, 0x9c, 0x9f, 0xa2, 0xa5, 0xa8, 0xab, 0xae, 
    0xb0, 0xb3, 0xb6, 0xb9, 0xbc, 0xbf, 0xc1, 0xc4, 0xc7, 0xc9, 0xcc, 0xce, 0xd1, 0xd3, 0xd5, 0xd8, 
    0xda, 0xdc, 0xde, 0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xec, 0xed, 0xef, 0xf0, 0xf2, 0xf3, 0xf5, 
    0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfc, 0xfd, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfd, 0xfc, 0xfc, 0xfb, 0xfa, 0xf9, 0xf8, 0xf7, 
    0xf6, 0xf5, 0xf3, 0xf2, 0xf0, 0xef, 0xed, 0xec, 0xea, 0xe8, 0xe6, 0xe4, 0xe2, 0xe0, 0xde, 0xdc, 
    0xda, 0xd8, 0xd5, 0xd3, 0xd1, 0xce, 0xcc, 0xc9, 0xc7, 0xc4, 0xc1, 0xbf, 0xbc, 0xb9, 0xb6, 0xb3, 
    0xb0, 0xae, 0xab, 0xa8, 0xa5, 0xa2, 0x9f, 0x9c, 0x98, 0x95, 0x92, 0x8f, 0x8c, 0x89, 0x86, 0x83, 
    0x80, 0x7c, 0x79, 0x76, 0x73, 0x70, 0x6d, 0x6a, 0x67, 0x63, 0x60, 0x5d, 0x5a, 0x57, 0x54, 0x51, 
    0x4f, 0x4c, 0x49, 0x46, 0x43, 0x40, 0x3e, 0x3b, 0x38, 0x36, 0x33, 0x31, 0x2e, 0x2c, 0x2a, 0x27, 
    0x25, 0x23, 0x21, 0x1f, 0x1d, 0x1b, 0x19, 0x17, 0x15, 0x13, 0x12, 0x10, 0x0f, 0x0d, 0x0c, 0x0a, 
    0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x03, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x03, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 
    0x09, 0x0a, 0x0c, 0x0d, 0x0f, 0x10, 0x12, 0x13, 0x15, 0x17, 0x19, 0x1b, 0x1d, 0x1f, 0x21, 0x23, 
    0x25, 0x27, 0x2a, 0x2c, 0x2e, 0x31, 0x33, 0x36, 0x38, 0x3b, 0x3e, 0x40, 0x43, 0x46, 0x49, 0x4c, 
    0x4f, 0x51, 0x54, 0x57, 0x5a, 0x5d, 0x60, 0x63, 0x67, 0x6a, 0x6d, 0x70, 0x73, 0x76, 0x79, 0x7c  
};
#ifdef __GNUC__
    FU08T_PTR SineWave(_SineWave);
#endif

// square wave
#ifdef __ICCAVR__
    __flash uint8_t SquareWave[ 256 ] = {
#elif defined __GNUC__
    PROGMEM uint8_t _SquareWave[ 256 ] = {
#endif
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff 
};
#ifdef __GNUC__
    FU08T_PTR SquareWave(_SquareWave);
#endif

// sawtooth wave
#ifdef __ICCAVR__
    __flash uint8_t SawToothWave[ 256 ] = {
#elif defined __GNUC__
    PROGMEM uint8_t _SawtoothWave[ 256 ] = {
#endif
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 
    0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 
    0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 
    0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 
    0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff  
};
#ifdef __GNUC__
    FU08T_PTR SawtoothWave(_SawtoothWave);
#endif

// reverse sawtooth wave
#ifdef __ICCAVR__
    __flash uint8_t RewsawToothWave[ 256 ] = {
#elif defined __GNUC__
    PROGMEM uint8_t _RewsawToothWave[ 256 ] = {
#endif
    0xff, 0xfe, 0xfd, 0xfc, 0xfb, 0xfa, 0xf9, 0xf8, 0xf7, 0xf6, 0xf5, 0xf4, 0xf3, 0xf2, 0xf1, 0xf0, 
    0xef, 0xee, 0xed, 0xec, 0xeb, 0xea, 0xe9, 0xe8, 0xe7, 0xe6, 0xe5, 0xe4, 0xe3, 0xe2, 0xe1, 0xe0, 
    0xdf, 0xde, 0xdd, 0xdc, 0xdb, 0xda, 0xd9, 0xd8, 0xd7, 0xd6, 0xd5, 0xd4, 0xd3, 0xd2, 0xd1, 0xd0, 
    0xcf, 0xce, 0xcd, 0xcc, 0xcb, 0xca, 0xc9, 0xc8, 0xc7, 0xc6, 0xc5, 0xc4, 0xc3, 0xc2, 0xc1, 0xc0, 
    0xbf, 0xbe, 0xbd, 0xbc, 0xbb, 0xba, 0xb9, 0xb8, 0xb7, 0xb6, 0xb5, 0xb4, 0xb3, 0xb2, 0xb1, 0xb0, 
    0xaf, 0xae, 0xad, 0xac, 0xab, 0xaa, 0xa9, 0xa8, 0xa7, 0xa6, 0xa5, 0xa4, 0xa3, 0xa2, 0xa1, 0xa0, 
    0x9f, 0x9e, 0x9d, 0x9c, 0x9b, 0x9a, 0x99, 0x98, 0x97, 0x96, 0x95, 0x94, 0x93, 0x92, 0x91, 0x90, 
    0x8f, 0x8e, 0x8d, 0x8c, 0x8b, 0x8a, 0x89, 0x88, 0x87, 0x86, 0x85, 0x84, 0x83, 0x82, 0x81, 0x80, 
    0x7f, 0x7e, 0x7d, 0x7c, 0x7b, 0x7a, 0x79, 0x78, 0x77, 0x76, 0x75, 0x74, 0x73, 0x72, 0x71, 0x70, 
    0x6f, 0x6e, 0x6d, 0x6c, 0x6b, 0x6a, 0x69, 0x68, 0x67, 0x66, 0x65, 0x64, 0x63, 0x62, 0x61, 0x60, 
    0x5f, 0x5e, 0x5d, 0x5c, 0x5b, 0x5a, 0x59, 0x58, 0x57, 0x56, 0x55, 0x54, 0x53, 0x52, 0x51, 0x50, 
    0x4f, 0x4e, 0x4d, 0x4c, 0x4b, 0x4a, 0x49, 0x48, 0x47, 0x46, 0x45, 0x44, 0x43, 0x42, 0x41, 0x40, 
    0x3f, 0x3e, 0x3d, 0x3c, 0x3b, 0x3a, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30, 
    0x2f, 0x2e, 0x2d, 0x2c, 0x2b, 0x2a, 0x29, 0x28, 0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20, 
    0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1a, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 
    0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00
};
#ifdef __GNUC__
    FU08T_PTR RewsawToothWave(_RewsawToothWave);
#endif

// triangle wave
#ifdef __ICCAVR__
    __flash uint8_t TriangleWave[ 256 ] = {
#elif defined __GNUC__
    PROGMEM uint8_t _TriangleWave[ 256 ] = {
#endif
    0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 
    0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 
    0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e, 
    0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76, 0x78, 0x7a, 0x7c, 0x7e, 
    0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e, 0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e, 
    0xa0, 0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe, 
    0xc0, 0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce, 0xd0, 0xd2, 0xd4, 0xd6, 0xd8, 0xda, 0xdc, 0xde, 
    0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xec, 0xee, 0xf0, 0xf2, 0xf4, 0xf6, 0xf8, 0xfa, 0xfc, 0xfe, 
    0xff, 0xfd, 0xfb, 0xf9, 0xf7, 0xf5, 0xf3, 0xf1, 0xef, 0xef, 0xeb, 0xe9, 0xe7, 0xe5, 0xe3, 0xe1, 
    0xdf, 0xdd, 0xdb, 0xd9, 0xd7, 0xd5, 0xd3, 0xd1, 0xcf, 0xcf, 0xcb, 0xc9, 0xc7, 0xc5, 0xc3, 0xc1, 
    0xbf, 0xbd, 0xbb, 0xb9, 0xb7, 0xb5, 0xb3, 0xb1, 0xaf, 0xaf, 0xab, 0xa9, 0xa7, 0xa5, 0xa3, 0xa1, 
    0x9f, 0x9d, 0x9b, 0x99, 0x97, 0x95, 0x93, 0x91, 0x8f, 0x8f, 0x8b, 0x89, 0x87, 0x85, 0x83, 0x81, 
    0x7f, 0x7d, 0x7b, 0x79, 0x77, 0x75, 0x73, 0x71, 0x6f, 0x6f, 0x6b, 0x69, 0x67, 0x65, 0x63, 0x61, 
    0x5f, 0x5d, 0x5b, 0x59, 0x57, 0x55, 0x53, 0x51, 0x4f, 0x4f, 0x4b, 0x49, 0x47, 0x45, 0x43, 0x41, 
    0x3f, 0x3d, 0x3b, 0x39, 0x37, 0x35, 0x33, 0x31, 0x2f, 0x2f, 0x2b, 0x29, 0x27, 0x25, 0x23, 0x21, 
    0x1f, 0x1d, 0x1b, 0x19, 0x17, 0x15, 0x13, 0x11, 0x0f, 0x0f, 0x0b, 0x09, 0x07, 0x05, 0x03, 0x01 
};
#ifdef __GNUC__
    FU08T_PTR TriangleWave(_TriangleWave);
#endif

// ECG wave
#ifdef __ICCAVR__
    __flash uint8_t ECGWave[ 268 ] = {
#elif defined __GNUC__
    PROGMEM uint8_t _ECGWave[ 268 ] = {
#endif
    73,74,75,75,74,73,73,73,73,72,71,69,68,67,67,67,
    68,68,67,65,62,61,59,57,56,55,55,54,54,54,55,55,
    55,55,55,55,54,53,51,50,49,49,52,61,77,101,132,
    169,207,238,255,254,234,198,154,109,68,37,17,5,
    0,1,6,13,20,28,36,45,52,57,61,64,65,66,67,68,68,
    69,70,71,71,71,71,71,71,71,71,72,72,72,73,73,74,
    75,75,76,77,78,79,80,81,82,83,84,86,88,91,93,96,
    98,100,102,104,107,109,112,115,118,121,123,125,
    126,127,127,127,127,127,126,125,124,121,119,116,
    113,109,105,102,98,95,92,89,87,84,81,79,77,76,75,
    74,73,72,70,69,68,67,67,67,68,68,68,69,69,69,69,
    69,69,69,70,71,72,73,73,74,74,75,75,75,75,75,75,
    74,74,73,73,73,73,72,72,72,71,71,71,71,71,71,71,
    70,70,70,69,69,69,69,69,70,70,70,69,68,68,67,67,
    67,67,66,66,66,65,65,65,65,65,65,65,65,64,64,63,
    63,64,64,65,65,65,65,65,65,65,64,64,64,64,64,64,
    64,64,65,65,65,66,67,68,69,71,72,73
};
#ifdef __GNUC__
    FU08T_PTR ECGWave(_ECGWave);
#endif

// -=[ Постоянные в EEPROM ]=-

// Определение адресов eeprom

EEPROMVAR_DECLARE( uint8_t, Mode );
EEPROMVAR_DECLARE( uint8_t, Frequency1 );
EEPROMVAR_DECLARE( uint8_t, Frequency2 );
EEPROMVAR_DECLARE( uint8_t, Frequency3 );
EEPROMVAR_DECLARE( uint8_t, Duty );
EEPROMPTR_DECLARE( uint8_t, Init, ( uint8_t * ) E2END );


// -=[ Переменные в ОЗУ ]=-

// Элементы меню
FLASHSTR_DECLARE( char, szMN000, "      Sine      \0" );
FLASHSTR_DECLARE( char, szMN100, "     Square     \0" );
FLASHSTR_DECLARE( char, szMN200, "    Triangle    \0" );
FLASHSTR_DECLARE( char, szMN300, "    SawTooth    \0" );
FLASHSTR_DECLARE( char, szMN400, "  Rev SawTooth  \0" );
FLASHSTR_DECLARE( char, szMN500, "      ECG       \0" );
FLASHSTR_DECLARE( char, szMN600, "    Freq Step   \0" );
FLASHSTR_DECLARE( char, szMN700, "     Noise      \0" );
FLASHSTR_DECLARE( char, szMN800, "   High Speed   \0" );

// Array of pointers to menu strings stored in flash
//const uint8_t *MENU[]={
//		MN000,	//
//		MN100,	//menu 1 string
//		MN200,	//menu 2 string
//		MN300,	//menu 3 string
//		MN400,	//menu 4 string
//		MN500,	
//		MN600,
//		MN700,
//		MN800
//}; 

// Различные строки
FLASHSTR_DECLARE( char, szMNON,     "ON \0" ); // ON
FLASHSTR_DECLARE( char, szMNOFF,    "OFF\0" ); // OFF
FLASHSTR_DECLARE( char, szNA,       "       NA       \0" ); // Clear freq value
FLASHSTR_DECLARE( char, szCLR,      "                \0" ); // Clear freq value
FLASHSTR_DECLARE( char, szMNClrfreq, "           \0" ); // Clear freq value
FLASHSTR_DECLARE( char, szTOEEPROM, "Saving Settings\0" ); // saving to eeprom
FLASHSTR_DECLARE( char, szONEMHZ,   "      1MHz   \0" ); // saving to eeprom
FLASHSTR_DECLARE( char, szwelcomeln1, "AVR SIGNAL\0" );
FLASHSTR_DECLARE( char, szRND,      "    Random\0" );

// variables to control TDA7313
struct signal {

    volatile uint8_t mode;		    // signal
    volatile uint8_t fr1;		    // Frequency [0..7]
    volatile uint8_t fr2;		    // Frequency [8..15]
    volatile int8_t fr3;            // Frequency [16..31]
    volatile uint32_t freq;         // frequency value
    volatile uint8_t flag;          // if "0"generator is OFF, "1" - ON
    volatile uint32_t acc;          // accumulator
    volatile uint8_t ON;
    volatile uint8_t HSfreq;		// high speed frequency [1...4Mhz]
    volatile uint32_t deltafreq;    // frequency step value

} SG;


/***********************
*  Р Е А Л И З А Ц И Я
*  ~~~~~~~~~~~~~~~~~~~
************************/


/**
 * Главный (основной) поток программы
 */
HRESULT CMCU::MainThreadProcedure(){

    // Разрешаем прерывания
    __enable_interrupt();

    * Init = 'T';

    do {


    } while ( true );

    // Все проверки прошли успешно, объект в рабочем состоянии
    return NO_ERROR;

}


/**
 * Инициализация всей периферии
 */
void CMCU::Initialization(){

    // Схема соединений (разводка выводов) [ATmega16]
    //PortsInit();

    // Настройка АЦП [ATmega16]
    //ADCInit();

    // Настройка таймера/счётчика 0 [ATmega16]
    //Timer0Init();

    // Настройка таймера/счётчика 1 [ATmega16]
    //Timer1Init();

    // Настройка таймера/счётчика 2 [ATmega16]
    //Timer2Init();

    // Настройка внутреннего сторожевого таймера [ATmega16]
    //InternalWDTInit();

    // Настройка внутреннего USART [ATmega16]
    //USARTInit();

    // Настройка последовательного интерфейса TWI [ATmega16]
    //TWIInit();

    // Настройка последовательного интерфейса SPI [ATmega16]
    //SPIInit();

    // Настройка работы с внешними прерываниями [ATmega16]
    //ExternalInterruptsInit();

    // Настройка управляющих регистров контроллера [ATmega16]
    //ControlRegistersInit();

}


/**
 * Настройка управляющих регистров контроллера
 */
void CMCU::ControlRegistersInit(){

    uint8_t temp;

    // MCU Control Register
    // [ Регистр управления микроконтроллером ][ATmega16]
    //          00000000 - Initial Value
    temp = BIN8(00000000);
    //          ||||||||
    //          |||||||+- 0, rw, ISC00: -+ - Interrupt Sense Control 0 Bit 1 and Bit 0
    //          ||||||+-- 1, rw, ISC01: _|
    //          |||||+--- 2, rw, ISC10: -+ - Interrupt Sense Control 1 Bit 1 and Bit 0
    //          ||||+---- 3, rw, ISC11: _|
    //          |||+----- 4, rw, SM0:   -+ - Sleep Mode Select Bits 0, 1
    //          ||+------ 5, rw, SM1:   _|
    //          |+------- 6, rw, SE:       - Sleep Enable
    //          +-------- 7, rw, SM2:      - Sleep Mode Select Bit 2
    // Примечание:
    // Эти конструкции сохраняют младшую тетраду регистра MCUCR от случайного
    // изменения
    //temp &= ( 1 << SM2 ) | ( 1 << SM1 ) | ( 1 << SM0 );
    MCUCR &= ~( ( 1 << SE ) | ( 1 << SM2 ) | ( 1 << SM1 ) | ( 1 << SM0 ) );
    MCUCR |= temp;


    // General Interrupt Control Register
    // [ Общий регистр управлением прерываниями ][ATmega16]
    //          00000000 - Initial Value
    //GICR = BIN8(00000000);
    //          ||||||||
    //          |||||||+- 0, rw, IVCE:  -
    //          ||||||+-- 1, rw, IVSEL: -
    //          |||||+--- 2, r: 0
    //          ||||+---- 3, r: 0
    //          |||+----- 4, r: 0
    //          ||+------ 5, rw, INT2:  - External Interrupt Request 2 Enable
    //          |+------- 6, rw, INT0:  - External Interrupt Request 0 Enable
    //          +-------- 7, rw, INT1:  - External Interrupt Request 1 Enable
    // Примечание:


    // Timer/Counter Interrupt Mask Register
    // [ Регистр маски прерываний от таймеров/счётчиков ][ATmega16]
    //           00000000 - Initial Value
    //TIMSK = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           |||||||+- 0, rw, TOIE0:  - Timer/Counter0 Overflow Interrupt Enable
    //           ||||||+-- 1, rw, OCIE0:  - OCIE0: Timer/Counter0 Output Compare Match Interrupt Enable
    //           |||||+--- 2, rw, TOIE1:  - Timer/Counter1, Overflow Interrupt Enable
    //           ||||+---- 3, rw, OCIE1B: - Timer/Counter1, Output Compare B Match Interrupt Enable
    //           |||+----- 4, rw, OCIE1A: - Timer/Counter1, Output Compare A Match Interrupt Enable
    //           ||+------ 5, rw, TICIE1: - Timer/Counter1, Input Capture Interrupt Enable
    //           |+------- 6, rw, TOIE2:  - Timer/Counter2 Overflow Interrupt Enable
    //           +-------- 7, rw, OCIE2:  - Timer/Counter2 Output Compare Match Interrupt Enable
    // Примечание:

    // Timer/Counter Interrupt Flag Register
    // [ Регистр флагов прерываний таймеров/счётчиков ][ATmega16]
    //          00000000 - Initial Value
    //TIFR = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          |||||||+- 0, rw, TOV0:  - Timer/Counter0 Overflow Flag
    //          ||||||+-- 1, rw, OCF0:  - Output Compare Flag 0
    //          |||||+--- 2, rw, TOV1:  - Timer/Counter1, Overflow Flag
    //          ||||+---- 3, rw, OCF1B: - Timer/Counter1, Output Compare B Match Flag
    //          |||+----- 4, rw, OCF1A: - Timer/Counter1, Output Compare A Match Flag
    //          ||+------ 5, rw, ICF1:  - Timer/Counter1, Input Capture Flag
    //          |+------- 6, rw, TOV2:  - Timer/Counter2 Overflow Flag
    //          +-------- 7, rw, OCF2:  - Output Compare Flag 2
    // Примечание:

    // Special Function IO Register
    // [ Регистр специальных функций ввода/вывода ][ATmega16]
    //           00000000 - Initial Value
    //SFIOR = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           |||||||+- 0, rw, PSR321: - Prescaler Reset Timer/Counter3,
    //           |||||||                  - Timer/Counter2, and Timer/Counter1
    //           ||||||+-- 1, rw, PSR0:   - Prescaler Reset Timer/Counter0
    //           |||||+--- 2, rw, PUD:    - Pull-up disable
    //           ||||+---- 3, rw, ACME:   - Analog Comparator Multiplexer Enable
    //           |||+----- 4, r: 0
    //           ||+------ 5, r: 0
    //           |+------- 6, r: 0
    //           +-------- 7, rw, TSM:    - Timer/Counter Synchronization Mode
    // Примечание:


    // ADC Control and Status Register A – ADCSRA
    // [ ADC Control and Status Register A ]
    //            00000000 - Initial Value
    //ADCSRA = BIN8(10001000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||
    //            |||||||+- 0, rw, ADPS0: -+ - ADC Prescaler Select Bits
    //            ||||||+-- 1, rw, ADPS1:  |
    //            |||||+--- 2, rw, ADPS2: _|
    //            ||||+---- 3, rw, ADIE:     - ADC Interrupt Enable
    //            |||+----- 4, rw, ADIF:     - ADC Interrupt Flag
    //            ||+------ 5, rw, ADATE:    - ADC Auto Trigger Enable
    //            |+------- 6, rw, ADSC:     - ADC Start Conversion
    //            +-------- 7, rw, ADEN:     - ADC Enable
    // Примечание:

}


/**
 * Настройка АЦП
 */
void CMCU::ADCInit(){

    // ADC Multiplexer Selection Register – ADMUX
    // [ ADC Multiplexer Selection Register ]
    //           00000000 - Initial Value
    //ADMUX = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           |||||||+- 0, rw, MUX0:  -+ - Analog Channel Selection Bits
    //           ||||||+-- 1, rw, MUX1:   |
    //           |||||+--- 2, rw, MUX2:   |
    //           ||||+---- 3, rw, MUX3:   |
    //           |||+----- 4, rw: MUX4:  _|
    //           ||+------ 5, rw, ADLAR:    - ADC Left Adjust Result
    //           |+------- 6, rw, REFS0: -+ - Reference Selection Bits
    //           +-------- 7, rw, REFS1: _|
    // Примечание: AVCC


    // Digital Input Disable Register 0 – DIDR0
    // [ Digital Input Disable Register 0 ]
    //           00000000 - Initial Value
    //DIDR0 = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           |||||||+- 0, rw, ADC0D:  -+ - ADC5..0 Digital Input Disable
    //           ||||||+-- 1, rw, ADC1D:   |
    //           |||||+--- 2, rw, ADC2D:   |
    //           ||||+---- 3, rw, ADC3D:   |
    //           |||+----- 4, rw, ADC4D:   |
    //           ||+------ 5, rw, ADC5D:  _|
    //           |+------- 6, r: 0
    //           +-------- 7, r: 0
    // Примечание: Не нужны для ADC7 и ADC6, т.к. их нет.


    // ADC Control and Status Register B – ADCSRB
    // [ ADC Control and Status Register B ]
    //            00000000 - Initial Value
    //ADCSRB = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||
    //            |||||||+- 0, rw, ADTS0: -+ - ADC Auto Trigger Source
    //            ||||||+-- 1, rw, ADTS1:  |
    //            |||||+--- 2, rw, ADTS2: _|
    //            ||||+---- 3, r: 0
    //            |||+----- 4, r: 0
    //            ||+------ 5, r: 0
    //            |+------- 6, rw, ACME:     - (?)
    //            +-------- 7, r: 0
    // Примечание: Free Running mode.

}


/**
 * Настройка таймера/счётчика 0
 */
void CMCU::Timer0Init(){

    // [ATmega16] Table 42, p. 85. Clock Select Bit Description
    // +----+----+----+-----------------------------------------------------------------+
    // |CSn2|CSn1|CSn0| Description                                                     ¦
    // +----+----+----+-----------------------------------------------------------------+
    // | 0  | 0  | 0  | No clock source. (Таймер/счетчик остановлен)                    |
    // | 0  | 0  | 1  | clkT0S/(No prescaling)                                          |
    // | 0  | 1  | 0  | clkT0S/8    (From prescaler)                                    |
    // | 0  | 1  | 1  | clkT0S/64   (From prescaler)                                    |
    // | 1  | 0  | 0  | clkT0S/256  (From prescaler)                                    |
    // | 1  | 0  | 1  | clkT0S/1024 (From prescaler)                                    |
    // | 1  | 1  | 0  | External clock source on T0 pin. Clock on falling edge.         |
    // | 1  | 1  | 1  | External clock source on T0 pin. Clock on rising edge.          |
    // +----+----+----+-----------------------------------------------------------------+

    // Timer/Counter 0 Control Register
    // [ Регистр управления Таймером/Счётчиком 0 ]
    //           00000000 - Initial Value
    //TCCR0 = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           |||||||+- 0, rw, CS00:  -+ - Управление тактовым сигналом
    //           ||||||+-- 1, rw, CS01:   |
    //           |||||+--- 2, rw, CS02:  _|
    //           ||||+---- 3, rw, WGM01: -+ - Waveform Generation Mode
    //           |||+----- 4, rw, COM00: -+ - Compare Match Output Mode
    //           ||+------ 5, rw: COM01  _|
    //           |+------- 6, rw, WGM00:    - Waveform Generation Mode
    //           +-------- 7, w, FOC0:      - Force Output Compare
    // Примечание:

    // Устанавливаем значения для счётного регистра
    //TCNT0 = 0xFF - F_CPU / 64000UL;

    // Timer/Counter0 Output Compare Register
    //OCR0 = 0x00;

    // Timer/Counter0 Asynchronous Status Register
    // [ Регистр ... ][ATmega16]
    //          00000000 - Initial Value
    //ASSR = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          |||||||+- 0, r, TCR0UB: - Timer/Counter Control Register0 Update Busy
    //          ||||||+-- 1, r, OCR0UB: - Output Compare Register0 Update Busy
    //          |||||+--- 2, r, TCN0UB: - Timer/Counter0 Update Busy
    //          ||||+---- 3, rw, AS0:   - Asynchronous Timer/Counter0
    //          |||+----- 4, r: 0
    //          ||+------ 5, r: 0
    //          |+------- 6, r: 0
    //          +-------- 7, r: 0
    // Примечание:

}


/**
 * Настройка таймера/счётчика 1
 */
void CMCU::Timer1Init(){

    // [ATmega16] Table 62. Clock Select Bit Description
    // +----+----+----+-----------------------------------------------------------------+
    // |CSn2|CSn1|CSn0| Description                                                     ¦
    // +----+----+----+-----------------------------------------------------------------+
    // | 0  | 0  | 0  | No clock source. (Таймер/счетчик остановлен)                    |
    // | 0  | 0  | 1  | clkI/O/1    (No prescaling)                                     |
    // | 0  | 1  | 0  | clkI/O/8    (From prescaler)                                    |
    // | 0  | 1  | 1  | clkI/O/64   (From prescaler)                                    |
    // | 1  | 0  | 0  | clkI/O/256  (From prescaler)                                    |
    // | 1  | 0  | 1  | clkI/O/1024 (From prescaler)                                    |
    // | 1  | 1  | 0  | Вывод Тn, счет осуществляется по спадающему фронту импульсов    |
    // | 1  | 1  | 1  | Вывод Тn, счет осуществляется по нарастающему фронту импульсов  |
    // +----+----+----+-----------------------------------------------------------------+

    // Timer/Counter1 Control Register B
    // [ Регистр управления B Таймером/Счётчиком 1 ][ATmega16]
    //            00000000 - Initial Value
    //TCCR1B = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||
    //            |||||||+- 0, rw, CS10:  -+ - Управление тактовым сигналом
    //            ||||||+-- 1, rw, CS11:   | 
    //            |||||+--- 2, rw, CS12:  _|
    //            ||||+---- 3, rw, WGM12: -+ - Режим работы таймера/счетчика
    //            |||+----- 4, rw, WGM13: _|
    //            ||+------ 5, r: 0          - зарезервирован, должен быть установлен в 0 при записи
    //            |+------- 6, rw, ICES1:    - Выбор активного фронта сигнала захвата
    //            +-------- 7, rw, ICNC1:    - Управление схемой подавления помех блока захвата
    // Примечание:

    // Устанавливаем значения для счётных регистров
    //TCNT1H = 0x00; // ( 0xFFFF - Delay * F_CPU / PrescaleValue ) >> 8
    //TCNT1L = 0x00; // ( 0xFFFF - Delay * F_CPU / PrescaleValue )
    /*
    OCR1AH = 0x00;
    OCR1AL = 0x39;

    OCR1BH = 0x00;
    OCR1BL = 0x39;

    OCR1CH = 0x00;
    OCR1CL = 0x39;

    ICR1H  = 0x00;
    ICR1L  = 0x39;
    */
    // Timer/Counter1 Control Register A
    // [ Регистр управления A Таймером/Счётчиком 1 ][ATmega16]
    //            00000000 - Initial Value
    //TCCR1A = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||
    //            |||||||+- 0, rw, WGM10:  -+ - Режим работы таймера/счетчика
    //            ||||||+-- 1, rw, WGM11:  _|
    //            |||||+--- 2, rw, COM1C0: -+ - Режим работы канала сравнения C
    //            ||||+---- 3, rw, COM1C1: _|
    //            |||+----- 4, rw, COM1B0: -+ - Режим работы канала сравнения B
    //            ||+------ 5, rw, COM1B1: _|
    //            |+------- 6, rw, COM1A0: -+ - Режим работы канала сравнения A
    //            +-------- 7, rw, COM1A1: _|
    // Примечание: Установлен режим работы ...

    //TCCR1B = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||
    //            |||||||+- 0, rw, CS10:  -+ - Управление тактовым сигналом
    //            ||||||+-- 1, rw, CS11:   | 
    //            |||||+--- 2, rw, CS12:  _|
    //            ||||+---- 3, rw, WGM12: -+ - Режим работы таймера/счетчика
    //            |||+----- 4, rw, WGM13: _|
    //            ||+------ 5, r: 0          - зарезервирован, должен быть установлен в 0 при записи
    //            |+------- 6, rw, ICES1:    - Выбор активного фронта сигнала захвата
    //            +-------- 7, rw, ICNC1:    - Управление схемой подавления помех блока захвата
    // Примечание: Коэффициент деления тактовой частоты установлен равным ...
    // Режим работы: ...

}


/**
 * Настройка таймера/счётчика 2
 */
void CMCU::Timer2Init(){

    // Timer/Counter2 Control Register
    // [ Регистр управления Таймером/Счётчиком 2 ][ATmega16]
    //           00000000 - Initial Value
    //TCCR2 = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           |||||||+- 0, rw, CS20:  -+ - Clock Select
    //           ||||||+-- 1, rw, CS21:   |
    //           |||||+--- 2, rw, CS22:  _|
    //           ||||+---- 3, rw, WGM21:    - Waveform Generation Mode
    //           |||+----- 4, rw, COM20: -+ - Compare Match Output Mode
    //           ||+------ 5, rw, COM21: _|
    //           |+------- 6, rw, WGM20:    - Waveform Generation Mode
    //           +-------- 7, w,  FOC2:     - Force Output Compare
    // Примечание:

    // Устанавливаем значения для счётного регистра
    //TCNT2 =  0xFF - F_CPU / 64000UL;

    // Timer/Counter2 Output Compare Register
    //OCR2 = 0x00;

    // Timer/Counter2 Asynchronous Status Register
    // [ Регистр ... ][ATmega16]
    //          00000000 - Initial Value
    //ASSR = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          |||||||+- 0, r, TCR2UB: - Timer/Counter Control Register2 Update Busy
    //          ||||||+-- 1, r, OCR2UB: - Output Compare Register2 Update Busy
    //          |||||+--- 2, r, TCN2UB: - Timer/Counter2 Update Busy
    //          ||||+---- 3, rw, AS2:   - Asynchronous Timer/Counter2
    //          |||+----- 4, r: 0
    //          ||+------ 5, r: 0
    //          |+------- 6, r: 0
    //          +-------- 7, r: 0
    // Примечание:

}


/**
 * Настройка последовательного интерфейса SPI
 */
void CMCU::SPIInit(){

    // SPI Control Register
    // [ Регистр управления SPI ][ATmega16]
    //          00000000 - Initial Value
    //SPCR = BIN8(0000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          |||||||+- 0, rw, SPR0: -+ - Скорость передачи
    //          ||||||+-- 1, rw, SPR1: _|
    //          |||||+--- 2, rw, CPHA:    - Фаза тактового сигнала
    //          ||||+---- 3, rw, CPOL:    - Полярность тактового сигнала
    //          |||+----- 4, rw, MSTR:    - Выбор режима работы (Master/Slave)
    //          ||+------ 5, rw, DORD:    - Порядок передачи данных
    //          |+------- 6, rw, SPE:     - Включение/выключение SPI
    //          +-------- 7, rw, SPIE:    - Разрешение прерывания от SPI
    // Примечание:

    // SPI Status Register
    // [ Регистр статуса SPI ][ATmega16]
    //          00000000 - Initial Value
    //SPSR = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          |||||||+- 0, rw, SPI2X:    - Double SPI Speed Bit
    //          ||||||+-- 1, r, 0       -+ - зарезервированы
    //          |||||+--- 2, r, 0        |
    //          ||||+---- 3, r, 0        | 
    //          |||+----- 4, r, 0        |
    //          ||+------ 5, r, 0       _|
    //          |+------- 6, r, WCOL:      - Write COLlision flag
    //          +-------- 7, r, SPIF:      - SPI Interrupt Flag
    // Примечание:

}


/**
 * Настройка последовательного интерфейса TWI
 */
void CMCU::TWIInit(){

    // TWI Bit Rate Register
    TWBR = 1; // TODO: Написать формулу для настройки скорости

    // TWI Control Register
    // [ Регистр управления TWI ][ATmega16]
    //          00000000 - Initial Value
    TWCR = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          |||||||+- 0, rw, TWIE:  - TWI Interrupt Enable
    //          ||||||+-- 1, r:         - reserved (will always read as zero)
    //          |||||+--- 2, rw, TWEN:  - TWI Enable Bit
    //          ||||+---- 3, r,  TWWC:  - TWI Write Collision Flag
    //          |||+----- 4, rw, TWSTO: - TWI STOP Condition Bit
    //          ||+------ 5, rw, TWSTA: - TWI START Condition Bit
    //          |+------- 6, rw, TWEA:  - TWI Enable Acknowledge Bit
    //          +-------- 7, rw, TWINT: - TWI Interrupt Flag
    // Примечание:

    // TWI Status Register
    // [ TWI статус регистр ][ATmega16]
    //          11111000 - Initial Value
    TWSR = BIN8(11111000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          |||||||+- 0, rw, TWPS0: -+ - TWI Prescaler Bits
    //          ||||||+-- 1, rw, TWPS1: _|
    //          |||||+--- 2, r:            - reserved (will always read as zero)
    //          ||||+---- 3, r, TWS3:   -+ - TWI Status
    //          |||+----- 4, r, TWS4:    |
    //          ||+------ 5, r, TWS5:    | 
    //          |+------- 6, r, TWS6:    |
    //          +-------- 7, r, TWS7:   _|
    // Примечание:

    // TWI (Slave) Address Register
    // Иметь в виду, что регистр содержит бит TWGCE (TWI General Call Recognition Enable Bit)
    TWAR = 0xFE;

}


/**
 * Настройка внутреннего USART
 */
void CMCU::USARTInit(){

    // USART Control and Status Register A
    // [ Регистр управления UCSRA ][ATmega16]
    //           00100000 - Initial Value
    //UCSRA = BIN8(00100000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           |||||||+- 0, rw, MPCM: - Multi-processor Communication Mode
    //           ||||||+-- 1, rw, U2X:  - Double the USART Transmission Speed
    //           |||||+--- 2, r, PE:    - Parity Error
    //           ||||+---- 3, r, DOR:   - Data OverRun
    //           |||+----- 4, r, FE:    - Frame Error
    //           ||+------ 5, r, UDRE:  - USART Data Register Empty
    //           |+------- 6, rw, TXC:  - USART Transmit Complete
    //           +-------- 7, r, RXC:   - USART Receive Complete
    // Примечание:

    //UCSRB = 0x00; // отключаем, пока настраиваем скорость

    // Определение BAUD см. в файле: "Configuration.h"
    //UBRRL = ( uint8_t ) ( F_CPU / ( 16UL * BAUD ) - 1UL ); // устанавливаем скорость
    //UBRRH = ( uint8_t ) ( ( F_CPU / ( 16UL * BAUD ) - 1UL ) >> 8 );

    // USART Control and Status Register B
    // [ Регистр управления UCSRB ][ATmega16]
    //           00000000 - Initial Value
    //UCSRB = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           |||||||+- 0, rw, TXB8:  - Transmit Data Bit 8
    //           ||||||+-- 1, r,  RXB8:  - Receive Data Bit 8
    //           |||||+--- 2, rw, UCSZ2: - Character Size
    //           ||||+---- 3, rw, TXEN:  - Transmitter Enable
    //           |||+----- 4, rw, RXEN:  - Receiver Enable
    //           ||+------ 5, rw, UDRIE: - USART Data Register Empty Interrupt Enable
    //           |+------- 6, rw, TXCIE: - TX Complete Interrupt Enable
    //           +-------- 7, rw, RXCIE: - RX Complete Interrupt Enable
    // Примечание:

    // USART Control and Status Register C
    // [ Регистр управления UCSRC ][ATmega16]
    //           10000110 - Initial Value
    //UCSRC = BIN8(10000110); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           |||||||+- 0, rw, UCPOL:    - Clock Polarity
    //           ||||||+-- 1, rw, UCSZ0: -+ - Character Size
    //           |||||+--- 2, rw, UCSZ1: _|
    //           ||||+---- 3, rw, USBS:     - Stop Bit Select
    //           |||+----- 4, rw, UPM0:  -+ - Parity Mode
    //           ||+------ 5, rw, UPM1:  _|
    //           |+------- 6, rw, UMSEL:    - USART Mode Select
    //           +-------- 7, rw: URSEL     - Register Select
    // Примечание:

}


/**
 * -=[ Схема соединений (разводка выводов) MCU ATmega16 ]=-
 */
void CMCU::PortsInit(){

    // Параметры проекта:
    // Имя проекта: Шаблон для ATmega16
    // Файл схемы (URI): file:///
    // Хеш файла схемы (MD5):
    // Код:
    // Дата: 17.11.2012
    // Координаты разработчика:
    //  mailto:unihomelab@ya.ru
    //  skype: viacheslavmezentsev

//                 [ATmega16]                                                   TQFP/QFN/MLF
//
//                                                                                  +----------[39]- GND
//                                                     (XCK/T0) PB0 -[40]---------+ | +--------[38]- VCC
//                         PDIP                            (T1) PB1 -[41]-------+ | | | +------[37]- PA0 (ADC0)
//                   +--------------+               (AIN0/INT2) PB2 -[42]-----+ | | | | | +----[36]- PA1 (ADC1)
//     (XCK/T0) PB0 -|  1        40 |- PA0 (ADC0)    (AIN1/OC0) PB3 -[43]---+ | | | | | | | +--[35]- PA2 (ADC2)
//         (T1) PB1 -|  2        39 |- PA1 (ADC1)         (~SS) PB4 -[44]-+ | | | | | | | | | +[34]- PA3 (ADC3)
//  (INT2/AIN0) PB2 -|  3        38 |- PA2 (ADC2)                         | | | | | | | | | | |
//   (OC0/AIN1) PB3 -|  4        37 |- PA3 (ADC3)                    +----------------------------+
//        (~SS) PB4 -|  5        36 |- PA4 (ADC4)                    |O  44  42  40  38  36  34   |
//       (MOSI) PB5 -|  6        35 |- PA5 (ADC5)                    |     43  41  39  37  35     |
//       (MISO) PB6 -|  7        34 |- PA6 (ADC6)        (MOSI) PB5 -|  1 +------------------+ 33 |- PA4 (ADC4)
//        (SCK) PB7 -|  8        33 |- PA7 (ADC7)        (MISO) PB6 -|  2 |                  | 32 |- PA5 (ADC5)
//           ~RESET -|  9        32 |- AREF               (SCK) PB7 -|  3 |                  | 31 |- PA6 (ADC6)
//              VCC -| 10        31 |- GND                   ~RESET -|  4 |                  | 30 |- PA7 (ADC7)
//              GND -| 11        30 |- AVCC                     VCC -|  5 |                  | 29 |- AREF
//            XTAL2 -| 12        29 |- PC7 (TOSC2)              GND -|  6 |                  | 28 |- GND
//            XTAL1 -| 13        28 |- PC6 (TOSC1)            XTAL2 -|  7 |                  | 27 |- AVCC
//        (RXD) PD0 -| 14        27 |- PC5 (TDI)              XTAL1 -|  8 |                  | 26 |- PC7 (TOSC2)
//        (TXD) PD1 -| 15        26 |- PC4 (TDO)          (RXD) PD0 -|  9 |                  | 25 |- PC6 (TOSC1)
//       (INT0) PD2 -| 16        25 |- PC3 (TMS)          (TXD) PD1 -| 10 |                  | 24 |- PC5 (TDI)
//       (INT1) PD3 -| 17        24 |- PC2 (TCK)         (INT0) PD2 -| 11 +------------------+ 23 |- PC4 (TDO)
//       (OC1B) PD4 -| 18        23 |- PC1 (SDA)                     |     13  15  17  19  21     |
//       (OC1A) PD5 -| 19        22 |- PC0 (SCL)                     |   12  14  16  18  20  22   |
//       (ICP1) PD6 -| 20        21 |- PD7 (OC2)                     +----------------------------+
//                   +--------------+                                     | | | | | | | | | | |
//                                                       (INT1) PD3 -[12]-+ | | | | | | | | | +[22]- PC3 (TMS)
//                                                       (OC1B) PD4 -[13]---+ | | | | | | | +--[21]- PC2 (TCK)
//                                                       (OC1A) PD5 -[14]-----+ | | | | | +----[20]- PC1 (SDA)
//                                                       (ICP1) PD6 -[15]-------+ | | | +------[19]- PC0 (SCL)
//                                                        (OC2) PD7 -[16]---------+ | +--------[18]- GND
//                                                                                  +----------[17]- VCC


    // Table 25. Port Pin Configurations
    //+-----+------+----------+--------+---------+--------------------------------------------+
    //| DDxn|PORTxn|   PUD    |  I/O   | Pull-up |   Comment                                  |
    //|     |      |(in SFIOR)|        |         |                                            |
    //+-----+------+----------+--------+---------+--------------------------------------------+
    //|  0  |  0   |    X     | Input  |   No    | Tri-state (Hi-Z)                           |
    //|  0  |  1   |    0     | Input  |   Yes   | Pxn will source current if ext. pulled low.|
    //|  0  |  1   |    1     | Input  |   No    | Tri-state (Hi-Z)                           |
    //|  1  |  0   |    X     | Output |   No    | Output Low (Sink)                          |
    //|  1  |  1   |    X     | Output |   No    | Output High (Source)                       |
    //+-----+------+----------+--------+---------+--------------------------------------------+
    // Если же бит PUD установлен в 1, подтягивающие
    // резисторы отключаются от всех выводов микроконтроллера.

    //sbi( SFIOR, PUD );
    cbi( SFIOR, PUD );

    // Настройка портов: A, B, C, D. Начальная инициализация уровней

    // Port A Data Direction Register
    // [ Регистр направления порта A ][ATmega16]
    //          00000000 - Initial Value
    DDRA = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          |||||||+- 0, rw, DDA0: (ADC0) -
    //          ||||||+-- 1, rw, DDA1: (ADC1) -
    //          |||||+--- 2, rw, DDA2: (ADC2) -
    //          ||||+---- 3, rw, DDA3: (ADC3) -
    //          |||+----- 4, rw, DDA4: (ADC4) -
    //          ||+------ 5, rw, DDA5: (ADC5) -
    //          |+------- 6, rw, DDA6: (ADC6) -
    //          +-------- 7, rw, DDA7: (ADC7) -
    // Примечание:

    // Port B Data Direction Register
    // [ Регистр направления порта B ][ATmega16]
    //          00000000 - Initial Value
    DDRB = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          |||||||+- 0, rw, DDB0: (XCK/T0)    -
    //          ||||||+-- 1, rw, DDB1: (T1)        -
    //          |||||+--- 2, rw, DDB2: (INT2/AIN0) -
    //          ||||+---- 3, rw, DDB3: (OC0/AIN1)  -
    //          |||+----- 4, rw, DDB4: (~SS)       -
    //          ||+------ 5, rw, DDB5: (MOSI)      -
    //          |+------- 6, rw, DDB6: (MISO)      -
    //          +-------- 7, rw, DDB7: (SCK)       -
    // Примечание:

    // Port C Data Direction Register
    // [ Регистр направления порта C ][ATmega16]
    //          00000000 - Initial Value
    DDRC = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          |||||||+- 0, rw, DDC0: (SCL)   -
    //          ||||||+-- 1, rw, DDC1: (SDA)   -
    //          |||||+--- 2, rw, DDC2: (TCK)   -
    //          ||||+---- 3, rw, DDC3: (TMS)   -
    //          |||+----- 4, rw, DDC4: (TDO)   -
    //          ||+------ 5, rw, DDC5: (TDI)   -
    //          |+------- 6, rw, DDC6: (TOSC1) -
    //          +-------- 7, rw, DDC7: (TOSC2) -
    // Примечание:

    // Port D Data Direction Register
    // [ Регистр направления порта D ][ATmega16]
    //          00000000 - Initial Value
    DDRD = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          |||||||+- 0, rw, DDD0: (RXD)  -
    //          ||||||+-- 1, rw, DDD1: (TXD)  -
    //          |||||+--- 2, rw, DDD2: (INT0) -
    //          ||||+---- 3, rw, DDD3: (INT1) -
    //          |||+----- 4, rw, DDD4: (OC1B) -
    //          ||+------ 5, rw, DDD5: (OC1A) -
    //          |+------- 6, rw, DDD6: (ICP1) -
    //          +-------- 7, rw, DDD7: (OC2)  -
    // Примечание:


    // Port A Data Register
    // [ Регистр данных порта A ][ATmega16]
    //           00000000 - Initial Value
    PORTA = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           |||||||+- 0, rw, PORTA0: (ADC0) -
    //           ||||||+-- 1, rw, PORTA1: (ADC1) -
    //           |||||+--- 2, rw, PORTA2: (ADC2) -
    //           ||||+---- 3, rw, PORTA3: (ADC3) -
    //           |||+----- 4, rw, PORTA4: (ADC4) -
    //           ||+------ 5, rw, PORTA5: (ADC5) -
    //           |+------- 6, rw, PORTA6: (ADC6) -
    //           +-------- 7, rw, PORTA7: (ADC7) -
    // Примечание:

    // Port B Data Register
    // [ Регистр данных порта B ][ATmega16]
    //           00000000 - Initial Value
    PORTB = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           |||||||+- 0, rw, PORTB0: (XCK/T0)    -
    //           ||||||+-- 1, rw, PORTB1: (T1)        -
    //           |||||+--- 2, rw, PORTB2: (INT2/AIN0) -
    //           ||||+---- 3, rw, PORTB3: (OC0/AIN1)  -
    //           |||+----- 4, rw, PORTB4: (~SS)       -
    //           ||+------ 5, rw, PORTB5: (MOSI)      -
    //           |+------- 6, rw, PORTB6: (MISO)      -
    //           +-------- 7, rw, PORTB7: (SCK)       -
    // Примечание:

    // Port C Data Register
    // [ Регистр данных порта C ][ATmega16]
    //           00000000 - Initial Value
    PORTC = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           |||||||+- 0, rw, PORTC0: (SCL)   -
    //           ||||||+-- 1, rw, PORTC1: (SDA)   -
    //           |||||+--- 2, rw, PORTC2: (TCK)   -
    //           ||||+---- 3, rw, PORTC3: (TMS)   -
    //           |||+----- 4, rw, PORTC4: (TDO)   -
    //           ||+------ 5, rw, PORTC5: (TDI)   -
    //           |+------- 6, rw, PORTC6: (TOSC1) -
    //           +-------- 7, rw, PORTC7: (TOSC2) -
    // Примечание:

    // Port D Data Register
    // [ Регистр данных порта D ][ATmega16]
    //           00000000 - Initial Value
    PORTD = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           |||||||+- 0, rw, PORTD0: (RXD)  -
    //           ||||||+-- 1, rw, PORTD1: (TXD)  -
    //           |||||+--- 2, rw, PORTD2: (INT0) -
    //           ||||+---- 3, rw, PORTD3: (INT1) -
    //           |||+----- 4, rw, PORTD4: (OC1B) -
    //           ||+------ 5, rw, PORTD5: (OC1A) -
    //           |+------- 6, rw, PORTD6: (ICP1) -
    //           +-------- 7, rw, PORTD7: (OC2)  -
    // Примечание:

}


/**
 * Настройка внутреннего сторожевого таймера
 */
void CMCU::InternalWDTInit(){

    // [ATmega16] Table 22. Watchdog Timer Prescale Select
    // +----+----+----+------------------+---------+---------+
    // |WDP2|WDP1|WDP0|OscCycles         | VCC3.0V | VCC5.0V |
    // +----+----+----+------------------+---------+---------+
    // | 0  | 0  | 0  |16K (16,384)      | 14.8 ms | 14.0 ms |
    // | 0  | 0  | 1  |32K (32,768)      | 29.6 ms | 28.1 ms |
    // | 0  | 1  | 0  |64K (65,536)      | 59.1 ms | 56.2 ms |
    // | 0  | 1  | 1  |128K (131,072)    | 0.12 s  | 0.11 s  |
    // | 1  | 0  | 0  |256K (262,144)    | 0.24 s  | 0.22 s  |
    // | 1  | 0  | 1  |512K (524,288)    | 0.47 s  | 0.45 s  |
    // | 1  | 1  | 0  |1,024K (1,048,576)| 0.95 s  | 0.9 s   |
    // | 1  | 1  | 1  |2,048K (2,097,152)| 1.9 s   | 1.8 s   |
    // +----+----+----+------------------+---------+---------+

    // Watchdog Timer ControlRegister
    // [ Регистр управления сторожевого таймера ][ATmega16]
    //           00000000 - Initial Value
    WDTCR = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           |||||||+- 0, rw, WDP0: -+
    //           ||||||+-- 1, rw, WDP1:  |- Коэффициент деления предделителя сторожевого таймера
    //           |||||+--- 2, rw, WDP2: _|
    //           ||||+---- 3, rw, WDE:  Разрешение сброса по тайм-ауту сторожевого таймера
    //           |||+----- 4, rw, WDCE: Разрешение изменения конфигурации сторожевого таймера
    //           ||+------ 5, r, -+
    //           |+------- 6, r,  |- Reserved Bits
    //           +-------- 7, r, _|
    // Примечание:
    // Включен сторожевой таймер. Далее в коде его необходимо сбрасывать
    // через промежутки времени меньшие периода его генератора с учётом
    // коэффициентов деления.

}


/**
 * Настройка работы с внешними прерываниями
 */
void CMCU::ExternalInterruptsInit(){

    uint8_t temp;

    // Table 48. Interrupt Sense Control
    //+-----+------+----------+--------+---------+--------------------------------------------+
    //|ISCn1|ISCn0 | Description                                                              |
    //+-----+------+----------+--------+---------+--------------------------------------------+
    //|  0  |  0   | The low level of INTn generates an interrupt request.                    |
    //|  0  |  1   | Reserved                                                                 |
    //|  1  |  0   | The falling edge of INTn generates asynchronously an interrupt request.  |
    //|  1  |  1   | The rising edge of INTn generates asynchronously an interrupt request.   |
    //+-----+------+----------+--------+---------+--------------------------------------------+

    // Запрещаем внешние прерывания
    GICR &= ~( ( 1 << INT0 ) | ( 1 << INT1 ) | ( 1 << INT2 ) );

    // Настройка прерываний INT0 и INT1
    // MCU Control Register
    // [ Регистр управления микроконтроллером ]
    //          ****0000 - Initial Value
    temp = BIN8(00000000);
    //          ||||||||
    //          |||||||+- 0, rw, ISC00: -+ - Interrupt Sense Control 0 Bit 1 and Bit 0
    //          ||||||+-- 1, rw, ISC01: _|
    //          |||||+--- 2, rw, ISC10: -+ - Interrupt Sense Control 1 Bit 1 and Bit 0
    //          ||||+---- 3, rw, ISC11: _|
    //          |||+----- 4, rw, SM0:      -
    //          ||+------ 5, rw, SM1:      -
    //          |+------- 6, rw, SE:       -
    //          +-------- 7, rw, SM2:      -
    // Примечание:
    // Эти конструкции сохраняют старшую тетраду регистра MCUCR от случайного
    // изменения
    temp &= ( 1 << ISC11 ) | ( 1 << ISC01 );
    MCUCR &= ~( ( 1 << ISC11 ) | ( 1 << ISC10 ) | ( 1 << ISC01 ) | ( 1 << ISC00 ) );
    MCUCR |= temp;

    // Настройка прерывания INT2
    // If ISC2 is written to zero, a falling edge on INT2 activates the
    // interrupt. If ISC2 is written to one, a rising edge on INT2
    // activates the interrupt.

    // MCU Control and Status Register
    // [ Регистр управления микроконтроллером ]
    //          *0****** - Initial Value
    temp = BIN8(00000000);
    //          ||||||||
    //          |||||||+- 0, rw, PORF:  -
    //          ||||||+-- 1, rw, EXTRF: -
    //          |||||+--- 2, rw, BORF:  -
    //          ||||+---- 3, rw, WDRF:  -
    //          |||+----- 4, rw, JTRF:  -
    //          ||+------ 5, r: 0
    //          |+------- 6, rw, ISC2:  - Interrupt Sense Control 2
    //          +-------- 7, rw, JTD:   -
    // Примечание:
    // Эти конструкции сохраняют другие разряды регистра MCUCSR от случайного
    // изменения
    //temp &= ( 1 << ISC2 );
    MCUCSR &= ~( 1 << ISC2 );
    MCUCSR |= temp;

}


// -=[ Обработчики прерываний ]=-

/**
 * External Interrupt Request 0
 */
void CMCU::OnExternalInterruptRequest0(){

}


/**
 * External Interrupt Request 1
 */
void CMCU::OnExternalInterruptRequest1(){

}


/**
 * External Interrupt Request 2
 */
void CMCU::OnExternalInterruptRequest2(){

}


/**
 * Timer/Counter2 Compare Match
 */
void CMCU::OnTimerCounter2CompareMatch(){

}


/**
 * Timer/Counter2 Overflow
 */
void CMCU::OnTimerCounter2Overflow(){

}


/**
 * Timer/Counter1 Capture Event
 */
void CMCU::OnTimerCounter1CaptureEvent(){

}


/**
 * Timer/Counter1 Compare Match A
 */
void CMCU::OnTimerCounter1CompareMatchA(){

}


/**
 * Timer/Counter Compare Match B
 */
void CMCU::OnTimerCounter1CompareMatchB(){

}


/**
 * Timer/Counter1 Overflow
 */
void CMCU::OnTimerCounter1Overflow(){

}


/**
 * Timer/Counter0 Compare Match
 */
void CMCU::OnTimerCounter0CompareMatch(){

}


/**
 * Timer/Counter0 Overflow
 */
void CMCU::OnTimerCounter0Overflow(){

}


/**
 * SPI Serial Transfer Complete
 */
void CMCU::OnSPISerialTransferComplete(){

}


/**
 * USART, Rx Complete
 */
void CMCU::OnUSARTRxComplete( uint8_t data ){

}


/**
 * USART Data Register Empty
 */
void CMCU::OnUSARTDataRegisterEmpty(){


}


/**
 * USART, Tx Complete
 */
void CMCU::OnUSARTTxComplete(){


}


/**
 * ADC Conversion Complete
 */
void CMCU::OnADCConversionComplete(){

}


/**
 * EEPROM Ready
 */
void CMCU::OnEEPROMReady(){

}


/**
 * Analog Comparator
 */
void CMCU::OnAnalogComparator(){

}


/**
 * 2-wire Serial Interface
 */
void CMCU::OnTWISerialInterface(){

}


/**
 * Store Program Memory Read
 */
void CMCU::OnStoreProgramMemoryRead(){

}
