#include "xgpio.h"
#include "xparameters.h"
#include "IP_Centrale_DCC.h"
#include "segment_write.h"
#include "dcc_control.h"

typedef struct {
    u8 address;   // Train address (e.g., 0x01)
    u8 speed;     // Speed value (0 to 31)
    u8 forward;   // Direction: 1 for forward, 0 for reverse
} SpeedSelect;
typedef struct {
    u8 address;     // Train address
    u8 type;        // 0 for F0-F4, 1 for F5-F12 ,2 For F13-F20
    u8 command; // The bitmask for the functions
    u8 group;       // specific for the F5-F12 function
} FunctionSelect;
typedef struct{
	u8 FLAG;
	u8 id;
	u8 pressing;
}Buton;
void run_function(int id,SpeedSelect *train, FunctionSelect *func){
	if(id == 1)
	{
		set_speed_dcc(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR, train->address, train->speed, train->forward);
	}
	else
	{
		if(func->type== 0)
		{
			Function0_4(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR, func-> address, func->command);
		}
		if(func->type== 1)
		{
			Function5_12(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR, func-> address, func->command,func -> group);
		}
		if(func->type== 2)
		{
			Function13_20(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR, func -> address, func->command);
		}
	}
}
void is_pressed(Buton * but, u32 gpio_read){
	u32 data = (gpio_read & (1<<but->id))>> but->id;
    if((data == 1 )&( but-> pressing == 0)){
    	but-> pressing =1;
    }
    if((data == 0 )& (but->pressing == 1)){
    	but ->FLAG = 1;
    	but->pressing =0;
    }
}

int main (){
	XGpio led_sw,sw,segment,butons;
	u16 chenillard;
	u32  counter, data,check, forward,speed ;
	int count =0;
	// buton control
	u32 buton,buton_down,buton_up,buton_left,buton_right,buton_center=0;

	Buton down ={0,0,0};Buton up ={0,1,0};Buton left ={0,2,0};Buton right ={0,3,0};Buton center ={0,4,0};
	// control vitesse
	SpeedSelect sp={0,0,0};
	FunctionSelect function={0,0,0,0};
	chenillard =0x1;
	counter= 4000;
	check =0;
	//IP_CENTRALE_DCC_mWriteReg(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR, 0, counter);
	XGpio_Initialize(&led_sw,XPAR_GPIO_1_DEVICE_ID);
	XGpio_Initialize(&segment,XPAR_GPIO_2_DEVICE_ID);
	XGpio_Initialize(&butons,XPAR_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&led_sw,2,0);
	XGpio_SetDataDirection(&segment,1,0);
	XGpio_SetDataDirection(&led_sw,1,0);

	while(1){
			data= XGpio_DiscreteRead(&led_sw,1);
			speed = data & 0xF;
			forward = (data & (1<<4))>>4;
			buton = XGpio_DiscreteRead(&butons,1);
			is_pressed(&down,buton);
			is_pressed(&up,buton);
			is_pressed(&left,buton);
			is_pressed(&right,buton);



			//down 0 up 1 left 2 right 3 middle 4

			//Function13_20(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR,function.address,1);
			//set_speed_dcc(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR, 1, 0, 0);
			// test Speed set
			//set_speed_dcc(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR, 1, 31, forward);
			// test F0 to F4
			//sFunction0_4(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR, 1, speed);
			//test F5 to F12
			//Function5_12(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR, 1, speed,forward);
			//write_number(&segment,21,10);
			if(left.FLAG){
				data= XGpio_DiscreteRead(&led_sw,1);
				is_pressed(&center,buton);
				if(center.FLAG){
					center.FLAG=0;
					left.FLAG=0;
					up.FLAG=0;
					sp.address = data;
					function.address = data;

				}
				else{

					write_letter_num(&segment, 11, data,70);
					continue;
				}
			}
			if (down.FLAG){

				if(up.FLAG) {
					count ++;
					if (count >20){
						count = 0;
					}
				}
				if(right.FLAG) {
					count --;
					if (count <0){
						count = 20;
					}
				}


				write_letter_num(&segment, 16, count ,70);

				up.FLAG=0;
				right.FLAG =0;
				is_pressed(&center,buton);
				if(center.FLAG){

					center.FLAG=0;
					down.FLAG=0;
					up.FLAG=0;
					if(count<5){
						function.type=0;
						function.command=1<<(4 -count);

					}
					else if(count < 13 && count >=5){
						function.type = 1;
						if(count <9){
							function.group = 1;
							function.command = 1<<(count - 5) ;
						}
						else {
							function.group = 0;
							function.command = 1<<(count - 9) ;
							}
					}
					else{
						function.type=2;
						function.command=1<<(count-13);


					}
					count =0;

				}
				//write_letter_num(&segment, 16, check_type*(0X64) ,70);
				continue;


			}
			if(up.FLAG){
				is_pressed(&center,buton);
				data= XGpio_DiscreteRead(&led_sw,1);
				forward = (data & (1<<5))>>5;
				if(center.FLAG){
					center.FLAG=0;
					left.FLAG=0;
					up.FLAG=0;
					sp.speed = (data&0X1F);
					sp.forward = forward;
					set_speed_dcc(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR, sp.address, sp.speed, sp.forward);
				}
				else{
					write_letter_num(&segment, 17, (data&0X1F)+forward*(0X64),70);
					continue;
				}
			}
			if(right.FLAG){

				Function0_4(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR,function.address,0);
				usleep(30000);
				Function5_12(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR,function.address,0,0);
				usleep(30000);
				Function5_12(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR,function.address,0,1);
				usleep(30000);
				Function13_20(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR,function.address,0);
				usleep(30000);

				if(function.type == 0){
					Function0_4(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR,function.address,function.command);
				}
				if(function.type == 1){
					Function5_12(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR,function.address,function.command,function.group);
				}
				if(function.type == 2){
					Function13_20(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR,function.address,function.command);
				}
				//Function13_20(XPAR_IP_CENTRALE_DCC_0_S00_AXI_BASEADDR,function.address,1);
				right.FLAG=0;



			}


			write_letter_num(&segment, 12, check,70);



	}
	return 0;
}
