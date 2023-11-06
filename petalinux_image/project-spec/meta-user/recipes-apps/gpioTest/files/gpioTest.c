/*****************************************************************************/
/*                         I N C L U D E   F I L E S                         */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>


/*****************************************************************************/
/*                            D E F I N E S                                  */
/*****************************************************************************/


#define BUTTONS_BASE_ADDR 	0x41200000
#define LEDS_BASE_ADDR  	0x41210000


/*****************************************************************************/
/*                                M A I N                                    */
/*****************************************************************************/

int main(void){
	
	int 		i, Data, pre_Data;
	int 		buttons_value = 0, pre_buttons_value = 16;
	int 		fd;
	void 		*ptr_buttons,*ptr_led;
	unsigned 	page_addr_buttons,page_addr_led;
	unsigned	page_offset_buttons,page_offset_led;
	unsigned 	page_size = sysconf(_SC_PAGESIZE);
	
	fd = open("/dev/mem",O_RDWR);

	page_addr_buttons	= (BUTTONS_BASE_ADDR & ~(page_size-1));
	page_offset_buttons	= BUTTONS_BASE_ADDR - page_addr_buttons;

	page_addr_led		= (LEDS_BASE_ADDR & ~(page_size-1));
	page_offset_led		= LEDS_BASE_ADDR - page_addr_led;

	
	ptr_buttons = mmap(NULL,page_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(BUTTONS_BASE_ADDR & ~(page_size-1)));
	ptr_led 	= mmap(NULL,page_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(LEDS_BASE_ADDR & ~(page_size-1)));
	
	pre_Data = 4;
	fprintf(stdout,"\n GPIO Test !! \n");
	while (1)
   	{ 
		buttons_value = *((unsigned *)(ptr_buttons+page_offset_buttons));	
		if((buttons_value != pre_buttons_value) && (buttons_value))
		{	
			fprintf(stdout,"Buttons Value %d\n", buttons_value);

			if(buttons_value == 1)
			  	fprintf(stdout,"Center Button!\n");
			else if(buttons_value == 2)
		  	  	fprintf(stdout,"Down Button!\n");
		  	else if(buttons_value == 4)
		  	  	fprintf(stdout,"Left Button!\n");
		  	else if(buttons_value == 8)
		  	  	fprintf(stdout,"Right Button!\n");
			else if(buttons_value == 16)
		  	 	fprintf(stdout,"Up Button!\n");
			else	fprintf(stdout,"No Button\n");

		}
		pre_buttons_value = buttons_value;

		if(buttons_value == 0)
			  	Data = 0;
		else if(buttons_value == 2)
		  	  	Data = 1;
		else if(buttons_value == 16)
		  	 	Data = 2;
		else	Data = 3;
		
		if(Data != pre_Data)
		{
			fprintf(stdout,"Data: %d\n", Data);
		}
		pre_Data = Data;

		//fprintf(stdout,"Counter Status %d\n", counter);
		//*((unsigned *)(ptr_led+page_offset_led)) = counter;


		for (i=0; i<9999999; i++);
   }	
	
	return 0;
}

