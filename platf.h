#ifndef _PLATF_H
#define _PLATF_H
/****** Platform-specific code and defines
 */

/* (c) copyright fenugrec 2016
 * GPLv3
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/****** kernel customization ******/
#define SCI_DEFAULTDIV 9	//default value for BRR reg. Speed (kbps) = (20 * 1000) / (32 * (BRR + 1))

/* Uncomment to enable verification of succesful block erase . Adds 128B for the block descriptors + ~ 44B of code */
//#define POSTERASE_VERIFY

/* Uncomment to add diag function for atomic u16 reads */
//#define DIAG_U16READ

/* Uncomment to taint WDT pulse for debug use */
//#define DIAG_TAINTWDT



#include <stdbool.h>


#if defined(SH7058)

#define RAM_MIN	0xFFFF0000
#define RAM_MAX 	0xFFFFBFFF

#elif defined(SH7055_18)

#define RAM_MIN	0xFFFF6000
#define RAM_MAX	0xFFFFDFFF

#elif defined(SH7055_35)

#define RAM_MIN	0xFFFF6000
#define RAM_MAX	0xFFFFDFFF

#else
#error No target specified !
#endif

/* where the pre-ramjump metadata is stored (wdt pin, s36k2, etc) */
#define RAMJUMP_PRELOAD_META 0xffff8000

/*** WDT and master clock stuff */
#define WDT_PER_MS	2
	/* somehow shc sucks at reducing the following :
	 * u16 WDT_MAXCNT = WDT_PER_MS / 1000 * (40*1000*1000ULL / 64))
	 */
//#define WDT_MAXCNT WDT_PER_MS * 40*1000*1000UL / 64 / 1000
#define WDT_MAXCNT 1250

#define MCLK_GETMS(x) ((x) * 16 / 10000)	/* convert ticks to milliseconds */
#define MCLK_GETTS(x) ((x) * 10000 / 16) /* convert millisec to ticks */
#define MCLK_MAXSPAN	10000	/* arbitrary limit (in ms) for time spans measured by MCLK */
/** Get current timestamp from free-running counter */
//uint32_t get_mclk_ts(void);
#define get_mclk_ts(x) (ATU0.TCNT)


/** Ret 1 if ok
 *
 * sets *err to a negative response code if failed
 */
bool platf_flash_init(u8 *err);


/** Enable modification (erase/write) to flash.
 *
 * If this is not called after platf_flash_init(), the actual calls to erase / write flash are skipped
 */
void platf_flash_unprotect(void);

/** Erase block, see definition of blocks in DS.
 *
 * ret 0 if ok
 */
uint32_t platf_flash_eb(unsigned blockno);

/** Write block of data. len must be multiple of SIDFL_WB_DLEN
 *
 *
 * @return 0 if ok , response code ( > 0x80) if failed.
 *
 * Note : the implementation must not assume that the src address will be 4-byte aligned !
 */
uint32_t platf_flash_wb(uint32_t dest, uint32_t src, uint32_t len);

/***** Init funcs ****/


/** init platform-specific stuff : SCI, clocks, interrupts */
void init_platf(void);

/** init necessary timers & shit for WDT interrupt !
 */
void init_wdt(void);


/** spinloop for X millisecs */
//void waitms(unsigned ms);

#endif

