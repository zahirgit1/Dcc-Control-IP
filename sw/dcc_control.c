#include "dcc_control.h"




void set_speed_dcc(UINTPTR BaseAddress, u8 address, u8 speed, u8 forward) {
    //Cap speed at 31 (5 bits)
    if (speed > 31) speed = 31;
    u32 word0 = 0;
    u32 word1 = 0;
    // Bit 7:0, Bit 6:1, Bit 5:Direction, Bits 4-0:Speed
    u8 command = 0x40 | ((forward & 0x01) << 5) | (speed & 0x1F);
    // Xor de control
    u8 control = address ^ command;
    // [Preamble (23 bits)] + [Start (1 bit '0')] + [Address (8 bits)]
    word0 = (0x7FFFFF << 9) | (address & 0xFF);

    // Bit 18: Start (0)
    // Bits 17-10: Command
    // Bit 9: Start (0)
    // Bits 8-1: Control
    // Bit 0: Stop (1)
    word1 = ((u32)command << 10) | ((u32)control << 1) | 0x1;

    // 6. Write to the hardware registers
    // RegOffset 0: bits 50 down to 19
    // RegOffset 4: bits 18 down to 0
    IP_CENTRALE_DCC_mWriteReg(BaseAddress, 0, word0);
    IP_CENTRALE_DCC_mWriteReg(BaseAddress, 4, word1);
}
void Function0_4(UINTPTR BaseAddress, u8 address, u8 function) {
    // Bit 7:1, Bit 6-5:0, Bit 4-0
    u8 command = 0x80 |  (function & 0x1F);
    // Xor de control
    u8 control = address ^ command;
    // [Preamble (23 bits)] + [Start (1 bit '0')] + [Address (8 bits)]
    u32 word0 = (0x7FFFFF << 9) | (address & 0xFF);

    // Bit 18: Start (0)
    // Bits 17-10: Command
    // Bit 9: Start (0)
    // Bits 8-1: Control
    // Bit 0: Stop (1)
    u32 word1 = ((u32)command << 10) | ((u32)control << 1) | 0x01;

    // 6. Write to the hardware registers
    // RegOffset 0: bits 50 down to 19
    // RegOffset 4: bits 18 down to 0
    IP_CENTRALE_DCC_mWriteReg(BaseAddress, 0, word0);
    IP_CENTRALE_DCC_mWriteReg(BaseAddress, 4, word1);
}
void Function5_12(UINTPTR BaseAddress, u8 address, u8 function, u8 group) {
    // Bit 7:1, Bit 6:0,bit 5:1 Bit 4:group selection 1 F5-8 0 F9-12, Bits 3-0:function
    u8 command = 0xA0 | ((group & 0x01) << 4) | (function & 0xF);
    // Xor de control
    u8 control = address ^ command;
    // [Preamble (23 bits)] + [Start (1 bit '0')] + [Address (8 bits)]
    u32 word0 = (0x7FFFFF << 9) | (address & 0xFF);

    // Bit 18: Start (0)
    // Bits 17-10: Command
    // Bit 9: Start (0)
    // Bits 8-1: Control
    // Bit 0: Stop (1)
    u32 word1 = ((u32)command << 10) | ((u32)control << 1) | 0x01;

    // 6. Write to the hardware registers
    // RegOffset 0: bits 50 down to 19
    // RegOffset 4: bits 18 down to 0
    IP_CENTRALE_DCC_mWriteReg(BaseAddress, 0, word0);
    IP_CENTRALE_DCC_mWriteReg(BaseAddress, 4, word1);
}
void Function13_20(UINTPTR BaseAddress, u8 address, u8 function){
	//commande 1 : 1101 1110
	u8 command1 = 0xDE;
    u8 command2 = (function & 0xFF );

    // Xor de control
    u8 control = address ^ command1 ^ command2;

    u32 word0 = ((u32)0x3FFF << 18) | ((u32)address << 9) | (u32)command1;

    u32 word1 =  ((u32)command2 << 10) | // Cmd2 (8 bits)
            (0x00 << 9)           | // Start bit (bit 9)
            ((u32)control << 1)   | // Control (8 bits)
            0x01;                   // Stop bit (bit 0) 0x01;

	IP_CENTRALE_DCC_mWriteReg(BaseAddress, 0, word0);
	IP_CENTRALE_DCC_mWriteReg(BaseAddress, 4, word1);
}

