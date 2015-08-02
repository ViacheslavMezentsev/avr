//-----------------------------------------------------------------------
// PFF - Low level disk control module for ATmega32
//-----------------------------------------------------------------------

#include "Defines.h"
#include "Configuration.h"
#include "MMC.h"


//-----------------------------------------------------------------------
//   Module Private Function
//-----------------------------------------------------------------------
BYTE CMMC::CardType = CMMC::CT_NONE;


//-----------------------------------------------------------------------
// SPI functions
//-----------------------------------------------------------------------
// Send a byte
void CMMC::SPIWrite( BYTE Data ) {

    BYTE i;

    for ( i = 0; i < 8; i++ ) {

        if ( ( Data & 0x80 ) == 0x00 ) PORTB &= ~ _BV( SD_DI );
        else PORTB |= _BV( SD_DI );

        Data = Data << 1;

        PORTB |= _BV( SD_CLK );
        nop();
        PORTB &= ~_BV( SD_CLK );

    }

}


// Send 0xFF and receive a byte
BYTE CMMC::SPIRead() {

    BYTE i, res = 0;

    PORTB |= _BV( SD_DI );

    for ( i = 0; i < 8; i++ ) {

        PORTB |= _BV( SD_CLK );

        res = res << 1;

        if ( ( PINB & _BV( SD_DO ) ) != 0x00 ) res = res | 0x01;

        PORTB &= ~ _BV( SD_CLK );

        nop();

    }

    return res;

}	/* Send 0xFF and receive a byte */


//-----------------------------------------------------------------------
// Deselect the card and release SPI bus
//-----------------------------------------------------------------------
void CMMC::SPIRelease() {

	SPIRead();
}


//-----------------------------------------------------------------------
// Send a command packet to MMC
//-----------------------------------------------------------------------
// Command byte
// Argument
BYTE CMMC::SendCommand( BYTE Command, DWORD Argument ) {

	BYTE n, res;

	// ACMD<n> is the command sequense of CMD55-CMD<n>
    if ( Command & 0x80 ) {	
		
        Command &= 0x7F;
		res = SendCommand( CMD55, 0 );
		if ( res > 1 ) return res;

	}

	// Select the card
	DESELECT();
	SPIRead();
	SELECT();
	SPIRead();

	// Send a command packet
	SPIWrite( Command );						// Start + Command index
	SPIWrite( ( BYTE )( Argument >> 24 ) );		// Argument[31..24]
	SPIWrite( ( BYTE )( Argument >> 16 ) );		// Argument[23..16]
	SPIWrite( ( BYTE )( Argument >> 8 ) );		// Argument[15..8]
	SPIWrite( ( BYTE ) Argument );				// Argument[7..0]
	
    n = 0x01;							// Dummy CRC + Stop
	
    if ( Command == CMD0 ) n = 0x95;			// Valid CRC for CMD0(0)
	if ( Command == CMD8 ) n = 0x87;			// Valid CRC for CMD8(0x1AA)
	
    SPIWrite(n);

	// Receive a command response
	n = 10;								// Wait for a valid response in timeout of 10 attempts

    do {
		
        res = SPIRead();

	} while ( ( res & 0x80 ) && --n );

	// Return with the response value
    return res;			

}

//--------------------------------------------------------------------------
//
//   Public Functions
//
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Initialize Disk Drive
//--------------------------------------------------------------------------
DSTATUS CMMC::Initialize() {

	BYTE n, cmd, ty, ocr[4];
	WORD tmr;

	INIT_SPI();

	if ( ( PINB & _BV( SD_INS ) ) != 0x00 ) return STA_NOINIT;

#if _WRITE_FUNC
	// Finalize write process if it is in progress
    if ( MMC_SEL ) Write( ( const BYTE * ) 0, 0 );
#endif

	// Dummy clocks
    for ( n = 100; n; n-- ) SPIRead();	

	ty = 0;

	// Enter Idle state
    if ( SendCommand( CMD0, 0 ) == 1 ) {

		// SDv2
        if ( SendCommand( CMD8, 0x1AA ) == 1 ) {	

			// Get trailing return value of R7 resp
            for ( n = 0; n < 4; n++ ) ocr[n] = SPIRead();		

			// The card can work at vdd range of 2.7-3.6V
            if ( ocr[2] == 0x01 && ocr[3] == 0xAA ) {		

				// Wait for leaving idle state (ACMD41 with HCS bit)
                for ( tmr = 12000; tmr && SendCommand( ACMD41, 1UL << 30 ); tmr-- ) ;	
				
                // Check CCS bit in the OCR
                if ( tmr && SendCommand( CMD58, 0 ) == 0 ) {		

					for ( n = 0; n < 4; n++ ) ocr[n] = SPIRead();

					// SDv2 (HC or SC)
                    ty = ( ocr[0] & 0x40 ) ? CT_SD2 | CT_BLOCK : CT_SD2;	

				}

			}

		// SDv1 or MMCv3
        } else {		

			if ( SendCommand( ACMD41, 0 ) <= 1 ) {

				ty = CT_SD1;
                cmd = ACMD41;	// SDv1

			} else {

				ty = CT_MMC;
                cmd = CMD1;	// MMCv3
			}

			// Wait for leaving idle state
            for ( tmr = 25000; tmr && SendCommand( cmd, 0 ); tmr-- ) ;	

			// Set R/W block length to 512
            if ( !tmr || SendCommand( CMD16, 512 ) != 0 ) ty = 0;

		}

	}

	CardType = ty;

	SPIRelease();

	return ( ty != CT_NONE ) ? 0 : STA_NOINIT;

}


//-----------------------------------------------------------------------
// Read partial sector
//-----------------------------------------------------------------------

DRESULT CMMC::Read (
	BYTE * buff,	// Pointer to the read buffer (NULL:Read bytes are forwarded to the stream)
	DWORD lba,		// Sector number (LBA)
	WORD ofs,		// Byte offset to read from (0..511)
	WORD cnt		// Number of bytes to read (ofs + cnt mus be <= 512)
    ) {

    DRESULT res;
	BYTE rc;
	WORD bc;

	if ( ( PINB & _BV( SD_INS ) ) != 0x00 ) return RES_ERROR;

	// Convert to byte address if needed
    if ( !( CardType & CT_BLOCK ) ) lba *= 512;		

	res = RES_ERROR;

	// READ_SINGLE_BLOCK
    if ( SendCommand( CMD17, lba ) == 0 ) {

		bc = 30000;

		// Wait for data packet in timeout of 100ms
        do {							

			rc = SPIRead();

		} while ( rc == 0xFF && --bc );

		// A data packet arrived
        if ( rc == 0xFE ) {				
			
            bc = 514 - ofs - cnt;

			// Skip leading bytes
			if ( ofs ) {

				do SPIRead(); while ( --ofs );
			}

			// Receive a part of the sector
			// Store data to the memory
            if ( buff ) {	
				do
					* buff++ = SPIRead();

				while ( --cnt );

			// Forward data to the outgoing stream (depends on the project)
            } else {	

                do {

                    // (Console output)
                    //uart_transmit(SPIRead());

                } while ( --cnt );

			}

			// Skip trailing bytes and CRC
			do SPIRead(); while (--bc);

			res = RES_OK;

		}

	}

	SPIRelease();

	return res;

}


//-----------------------------------------------------------------------
// Write partial sector
//-----------------------------------------------------------------------
#if _WRITE_FUNC

// Pointer to the bytes to be written (NULL:Initiate/Finalize sector write)
// Number of bytes to send, Sector number (LBA) or zero
DRESULT CMMC::Write( const BYTE * buff,	DWORD sa ) {

	DRESULT res;
	WORD bc;
	static WORD wc;

	if ( ( PINB & _BV( SD_INS ) ) != 0x00 ) return RES_ERROR;
	if ( ( PINB & _BV( SD_WP ) ) != 0x00 ) return RES_ERROR;

	res = RES_ERROR;

	// Send data bytes
    if ( buff ) {		

		bc = ( WORD ) sa;
		
        // Send data bytes to the card
        while ( bc && wc ) {

			SPIWrite( * buff++ );
			wc--;
            bc--;

		}

		res = RES_OK;

	} else {

		// Initiate sector write process
        if ( sa ) {	

			// Convert to byte address if needed
            if ( !( CardType & CT_BLOCK ) ) sa *= 512;	

			// WRITE_SINGLE_BLOCK
            if ( SendCommand( CMD24, sa ) == 0) {			

				// Data block header
                SPIWrite( 0xFF );
                SPIWrite( 0xFE );		
				
                // Set byte counter
                wc = 512;							
				res = RES_OK;

			}

		// Finalize sector write process
        } else {	

			bc = wc + 2;

			// Fill left bytes and CRC with zeros
            while ( bc-- ) SPIWrite(0);	

			// Receive data resp and wait for end of write process in timeout of 300ms
            if ( ( SPIRead() & 0x1F ) == 0x05 ) {

				// Wait ready
                for ( bc = 65000; SPIRead() != 0xFF && bc; bc-- );

				if ( bc ) res = RES_OK;

			}

			SPIRelease();

		}

	}

	return res;

}


DRESULT CMMC::Write( FCHAR_PTR buff, DWORD sa ) {

	DRESULT res;
	WORD bc;
	static WORD wc;

	if ( ( PINB & _BV( SD_INS ) ) != 0x00 ) return RES_ERROR;
	if ( ( PINB & _BV( SD_WP ) ) != 0x00 ) return RES_ERROR;

	res = RES_ERROR;

	// Send data bytes
    if ( buff != NULL ) {		

		bc = ( WORD ) sa;
		
        // Send data bytes to the card
        while ( bc && wc ) {

			SPIWrite( * buff++ );
			wc--;
            bc--;

		}

		res = RES_OK;

	} else {

		// Initiate sector write process
        if ( sa ) {	

			// Convert to byte address if needed
            if ( !( CardType & CT_BLOCK ) ) sa *= 512;	

			// WRITE_SINGLE_BLOCK
            if ( SendCommand( CMD24, sa ) == 0) {			

				// Data block header
                SPIWrite( 0xFF );
                SPIWrite( 0xFE );		
				
                // Set byte counter
                wc = 512;							
				res = RES_OK;

			}

		// Finalize sector write process
        } else {	

			bc = wc + 2;

			// Fill left bytes and CRC with zeros
            while ( bc-- ) SPIWrite(0);	

			// Receive data resp and wait for end of write process in timeout of 300ms
            if ( ( SPIRead() & 0x1F ) == 0x05 ) {

				// Wait ready
                for ( bc = 65000; SPIRead() != 0xFF && bc; bc-- );

				if ( bc ) res = RES_OK;

			}

			SPIRelease();

		}

	}

	return res;

}

#endif
