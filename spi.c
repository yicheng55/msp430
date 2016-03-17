/*
* Copyright (c) 2012, Alexander I. Mykyta
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
*    list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*==================================================================================================
* File History:
* NAME          DATE         COMMENTS
* Alex M.       2011-04-07   born
* Alex M.       2013-08-10   Added uninit routine
*
*=================================================================================================*/

/**
* \addtogroup MOD_SPI
* \{
**/

/**
* \file
* \brief Code for \ref MOD_SPI "SPI Bus"
* \author Alex Mykyta
**/

#include <stdint.h>

//#include "msp430_xc.h"
#include <msp430.h>
#include "spi.h"
#include "spi_internal.h"


//--------------------------------------------------------------------------------------------------
void spiInit(uint8_t spi_mode)
{
	P4SEL |= SOMI + SIMO + SCLK; 	// P4.2,1,3 use special function
	P4DIR |= CE; 					//Set P4.0 Gpio as chip enable
	P4OUT |= CE;

    SPI_UCCTL1 |= UCSWRST;
    SPI_UCCTL0 = spi_mode + UCMSB + UCMST + UCSYNC;
    SPI_UCCTL1 = (SPI_CLK_SRC << 6) + UCSWRST;
    SPI_UCBR = SPI_CLK_DIV;
#if(SPI_USE_USCI < 4)
    SPI_UCMCTL = 0;
#endif
    SPI_UCCTL1 &= ~UCSWRST;
}

//--------------------------------------------------------------------------------------------------
void spiUninit(void)
{
    SPI_UCCTL1 |= UCSWRST;
}

//--------------------------------------------------------------------------------------------------
uint8_t spiSendByte(uint8_t data)
{
    SPI_UCTXBUF = data;    // write
    while ((SPI_UCIFG & UCRXIFG) == 0); // wait for transfer to complete
    return(SPI_UCRXBUF);
}

//--------------------------------------------------------------------------------------------------
void spiReadFrame(uint8_t* pBuffer, uint16_t size)
{
    uint16_t i = 0;
    for (i = 0; i < size; i++)
    {
        SPI_UCTXBUF = DUMMY_CHAR;     // dummy write
        while ((SPI_UCIFG & UCRXIFG) == 0); // wait for transfer to complete
        pBuffer[i] = SPI_UCRXBUF;
    }
}

//--------------------------------------------------------------------------------------------------
void spiSendFrame(const uint8_t* pBuffer, uint16_t size)
{
    uint16_t i = 0;
    volatile uint8_t x;
    for (i = 0; i < size; i++)
    {
        SPI_UCTXBUF = pBuffer[i];
        while ((SPI_UCIFG & UCTXIFG) == 0); // wait for tx buffer to be ready
    }
    while ((SPI_UCSTAT & UCBUSY) == 0); // wait for transfer to complete
    x = SPI_UCRXBUF; // dummy read to clear the rx flag
}

///\}
