/**
* \addtogroup MOD_SPI
* \{
**/

/**
* \file
* \brief Configuration include file for \ref MOD_SPI "SPI Bus"
* \author Alex Mykyta
**/

#ifndef _SPI_CONFIG_H_
#define _SPI_CONFIG_H_

//==================================================================================================
/// \name Configuration
/// Configuration defines for the \ref MOD_SPI module
/// \{
//==================================================================================================

//  ===================================================
//  = NOTE: Actual ports must be configured manually! =
//  ===================================================


/// Select which USCI module to use
#define SPI_USE_USCI    5	 ///< \hideinitializer
/**<    0 = USCIA0 \n
*         1 = USCIA1 \n
*         2 = USCIA2 \n
*         3 = USCIA3 \n
*         4 = USCIB0 \n
*         5 = USCIB1 \n
*         6 = USCIB2 \n
*         7 = USCIB3
**/

/// Select which clock source to use
#define SPI_CLK_SRC        2 ///< \hideinitializer
/**<    1 = ACLK    \n
*        2 = SMCLK
**/

/// SPI Clock division. Must be 4 or greater
#define SPI_CLK_DIV        2 //,4 ///< \hideinitializer

/// Byte that is transmitted during read operations
#define DUMMY_CHAR    (0xFF)


#define SCLK BIT3 	//P4.3 master Clock out
#define SOMI BIT2 	//P4.2 master in
#define SIMO BIT1 	//P4.1 master out
#define CE   BIT0 	//P4.0 Chip Enable line


///\}

#endif

///\}
