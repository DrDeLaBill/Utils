#include "hal_defs.h"

#ifdef USE_HAL_DRIVER


#include "glog.h"


void COREInfo(void);
void FPUInfo(void);
void IDCODEInfo(void);


#if defined(DEBUG) && !defined(NO_CPU_INFO)
static const char CORE_TAG[] = "CORE";
#endif


void SystemInfo(void)
{
#ifndef NO_CPU_INFO
	printTagLog(CORE_TAG, "Core=%lu, %lu MHz", SystemCoreClock, SystemCoreClock / 1000000);
	COREInfo();
	IDCODEInfo();
	FPUInfo();
	printPretty("APB1=%lu\n", HAL_RCC_GetPCLK1Freq());
	printPretty("APB2=%lu\n", HAL_RCC_GetPCLK2Freq());
#endif
}

bool MCUcheck(void)
{
	uint32_t cpuid = SCB->CPUID;
	if ((cpuid & 0xFF000000) != 0x41000000) {
		return false;
	}
#if defined(STM32F1)
	return ((cpuid & 0x0000FFF0) >> 4) == 0xC23;
#elif defined(STM32F4)
	return ((cpuid & 0x0000FFF0) >> 4) == 0xC24;
#else
#   error "Select MCU"
#endif
}

void COREInfo(void)
{
#ifndef NO_CPU_INFO
#   if defined(_DEBUG) || defined(DEBUG) || defined(GBEDUG_FORCE)
	uint32_t cpuid = SCB->CPUID;

	printPretty(
		"CPUID 0x%08X DEVID 0x%03X REVID 0x%04X\n",
		(int)cpuid,
		(int)(DBGMCU->IDCODE & 0xFFF),
		(int)((DBGMCU->IDCODE >> 16) & 0xFFFF)
	);

	uint32_t pat = (cpuid & 0x0000000F);
	uint32_t var = (cpuid & 0x00F00000) >> 20;

	if ((cpuid & 0xFF000000) == 0x41000000) { // ARM
		switch((cpuid & 0x0000FFF0) >> 4) {
		case 0xC20:
			printPretty("Cortex M0 r%lup%lu\n", var, pat);
			break;
		case 0xC60:
			printPretty("Cortex M0+ r%lup%lu\n", var, pat);
			break;
		case 0xC21:
			printPretty("Cortex M1 r%lup%lu\n", var, pat);
			break;
		case 0xC23:
			printPretty("Cortex M3 r%lup%lu\n", var, pat);
			break;
		case 0xC24:
			printPretty("Cortex M4 r%lup%lu\n", var, pat);
			break;
		case 0xC27:
			printPretty("Cortex M7 r%lup%lu\n", var, pat);
			break;
		default:
			printPretty("Unknown CORE\n");
		}
	} else {
		printPretty("Unknown CORE IMPLEMENTER\n");
	}
#   endif
#endif
}

void FPUInfo(void)
{
#ifndef NO_CPU_INFO
	uint32_t mvfr0 = *(volatile uint32_t *)0xE000EF40;

	if (mvfr0) {
		printPretty(
			"0x%08X 0x%08X 0x%08X\n",
			(int)*(volatile uint32_t *)0xE000EF34,   // FPCCR  0xC0000000
			(int)*(volatile uint32_t *)0xE000EF38,   // FPCAR
			(int)*(volatile uint32_t *)0xE000EF3C    // FPDSCR
		);  // MVFR2  0x00000040
		printPretty(
			"0x%08X 0x%08X 0x%08X\n",
			(int)*(volatile uint32_t *)0xE000EF40,   // MVFR0  0x10110021 vs 0x10110221
			(int)*(volatile uint32_t *)0xE000EF44,   // MVFR1  0x11000011 vs 0x12000011
			(int)*(volatile uint32_t *)0xE000EF48
		);
	}

	switch(mvfr0) {
	case 0x00000000:
		printPretty("No FPU\n");
		break;
	case 0x10110021:
		printPretty("FPU-S Single-precision only\n");
		break;
	case 0x10110221:
		printPretty("FPU-D Single-precision and Double-precision\n");
		break;
	default:
		printPretty("Unknown FPU\n");
		break;
	}
#endif
}

void IDCODEInfo(void)
{
#ifndef NO_CPU_INFO
	uint32_t idcode = DBGMCU->IDCODE & 0xFFF;

	printPretty("");
	switch(idcode) {
	case 0x410:
        gprint("STM32F103\n");
        break;
	case 0x411:
        gprint("STM32F457\n");
        break;
	case 0x413:
        gprint("STM32F407\n");
        break;
	case 0x415:
        gprint("STM32L475xx, L476xx or L486xx\n");
        break;
	case 0x417:
        gprint("STM32L0 Cat 3\n");
        break;
	case 0x419:
        gprint("STM32F429 or F439\n");
        break;
	case 0x421:
        gprint("STM32F446\n");
        break;
	case 0x423:
        gprint("STM32F401\n");
        break;
	case 0x431:
        gprint("STM32F411\n");
        break;
	case 0x433:
        gprint("STM32F401\n");
        break;
	case 0x434:
        gprint("STM32F469\n");
        break;
	case 0x435:
        gprint("STM32L43xxx or L44xxx\n");
        break;
	case 0x440:
        gprint("STM32F030x8\n");
        break;
	case 0x441:
        gprint("STM32F412\n");
        break;
	case 0x442:
        gprint("STM32F030xC\n");
        break;
	case 0x444:
        gprint("STM32F030x4 or F030x6\n");
        break;
	case 0x445:
        gprint("STM32F070x6\n");
        break;
	case 0x447:
        gprint("STM32L0 Cat 5\n");
        break;
	case 0x448:
        gprint("STM32F070x8\n");
        break;
	case 0x449:
        gprint("STM32F74xxx or F75xxx\n");
        break;
	case 0x450:
        gprint("STM32H7xx\n");
        break;
	case 0x451:
        gprint("STM32F76xxx or F77xxx\n");
        break;
	case 0x452:
        gprint("STM32F72xxx or F73xxx\n");
        break;
	case 0x457:
        gprint("STM32L011xx\n");
        break;
	case 0x461:
        gprint("STM32L496xx or L4A6xx\n");
        break;
	case 0x462:
        gprint("STM32L45xxx or L46xxx\n");
        break;
	case 0x470:
        gprint("STM32L4Rxxx or L4Sxxx\n");
        break;
	case 0x480:
        gprint("STM32H7Ax or H7Bx\n");
        break;
	default:
        gprint("Unknown STM32 (IDCODE=0x%X)\n", (int)idcode);
		break;
	}
#endif
}


#endif
