// EvCode.h		
		
// Shutdowns		
#define EV_VDC_HI_INT		1	
#define EV_VDC_LO_INT		2	
#define EV_IOUT_POS		3	
#define EV_IOUT_NEG		4	
#define EV_VAC_PEAK		5	
#define EV_DRV_FAULT		6	
#define EV_TMR_FAULT		7	
#define EV_RES_COMP_HI		8	
#define EV_BATMON_TIMEOUT	9	
#define EV_VIN_HI_INT		15	
		
// Status-Based Shutdowns		
#define EV_SYSSTATUS		10	
#define EV_GRID_VOLTAGE		11	
#define EV_GRID_FREQ		12	
		
// Slave Shutdowns		
#define EV_SLAVE_IOUT_ERROR	50	
#define EV_SLAVE_THS_ERROR	51	
		
// System		
#define EV_POWER_UP		100	
#define EV_TRAP_ERROR		101	
#define EV_FLASH_ERROR		102	
#define EV_STACK_DUMP		104	
#define EV_SET_SITE_KEY		105	
#define EV_RESET_SITE_KEY	106	
#define EV_ACCESS_SET		107	
#define EV_TIME_SET		108	
		
// Turbine		
#define EV_TURBINE_OVERSPEED	110	
		
// Boot		
#define EV_BOOT_LOAD		151	
#define EV_BOOT_HW_TYPE		152	
#define EV_BOOT_OLDVER		153	
#define EV_BOOT_NEWVER		154	
		
// Control		
#define EV_ENABLE		200	
#define EV_QUIET		201	
		
// Events		
#define EV_LOST_EVENT_RECORD	900	
#define EV_EVENT_POINTERS_RESET	901	
#define EV_TEST_EVENT		1000	
		
		

