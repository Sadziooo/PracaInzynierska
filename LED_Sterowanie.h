
#ifndef LED_INITIALIZATION_H_
#define LED_INITIALIZATION_H_

#include "main.h"

#define def_readcoil	(VAR_ReceiverBufor[1]==0x01)

#define def_zawor_1		((VAR_ReceiverBufor[3]&0b00000001)==1)
#define def_zawor_2		((VAR_ReceiverBufor[3]&0b00000010)==2)
#define def_zawor_3		((VAR_ReceiverBufor[3]&0b00000100)==4)
#define def_zawor_4		((VAR_ReceiverBufor[3]&0b00001000)==8)
#define def_zawor_5		((VAR_ReceiverBufor[3]&0b00010000)==16)
#define def_zawor_6		((VAR_ReceiverBufor[3]&0b00100000)==32)

#define def_mieszadlo	((VAR_ReceiverBufor[3]&0b01000000)==64)
#define def_podajnik	((VAR_ReceiverBufor[3]&0b10000000)==128)

#define def_error		((VAR_ReceiverBufor[4]&0b00000001)==1)
#define def_reset		((VAR_ReceiverBufor[4]&0b00000010)==2)
#define def_start		((VAR_ReceiverBufor[4]&0b00000100)==4)
#define def_pause		((VAR_ReceiverBufor[4]&0b00001000)==8)

#define def_ZB1_git		((VAR_ReceiverBufor[4]&0b00010000)==16)
#define def_ZB2_git		((VAR_ReceiverBufor[4]&0b00100000)==32)
#define def_ZB3_nD_git	((VAR_ReceiverBufor[4]&0b01000000)==64)
#define def_ZB3_git		((VAR_ReceiverBufor[4]&0b10000000)==128)
#define def_ZB4_git		((VAR_ReceiverBufor[5]&0b00000001)==1)

#define def_ZB1_0		((VAR_ReceiverBufor[5]&0b00000010)==2)
#define def_ZB2_0		((VAR_ReceiverBufor[5]&0b00000100)==4)
#define def_ZB3_0		((VAR_ReceiverBufor[5]&0b00001000)==8)
#define def_ZB4_0		((VAR_ReceiverBufor[5]&0b00010000)==16)

void Fun_LED_Init(void);

void Fun_CoilsChecking(void);

#endif /* LED_INITIALIZATION_H_ */
