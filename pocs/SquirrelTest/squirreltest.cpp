#include "global.h"
#include "pci.h"
#include "common_chipset.h"
#include "amd_chipset.h"
#include "Util.h"
#include "physmem.h"
#include "msr.h"
#include "smi.h"
#include "smm_communicate.h"

#ifdef __linux__
#include <sched.h>
#include <unistd.h>
#endif

// Find through `lspci` (look for "Ethernet controller: Xilinx Corporation Device 0666 (rev 02)")
#define SQUIRREL_BUS 4
#define SQUIRREL_DEV 0
#define SQUIRREL_FNC 0

// Find through `lspci -vvv -s $BUS:$DEV.$FUNC`
#define SQUIRREL_BAR 0xD0500000

int main(int argc, char** argv)
{
	enable_debug_mode();

	open_platbox_device();

	// Enable Squirrel
	pci_enable_memory_space(SQUIRREL_BUS, SQUIRREL_DEV, SQUIRREL_FNC);
	pci_enable_bus_master(SQUIRREL_BUS, SQUIRREL_DEV, SQUIRREL_FNC);

	// Clear BAR buffer (TODO: why?)
	void* dma_buf_page = map_physical_memory(SQUIRREL_BAR, PAGE_SIZE);
	// memset(dma_buf_page, 0x00, PAGE_SIZE);
	// print_memory(SQUIRREL_BAR, (char *) dma_buf_page, 0x100);
	// getchar();

	// UINT32 *squirrel = (UINT32 *) SQUIRREL_BAR;
	UINT32 *squirrel = (UINT32*)dma_buf_page;
	squirrel[32] = 0x42424242;

	unmap_physical_memory(dma_buf_page, PAGE_SIZE);
	close_platbox_device();

	return 0;
}
