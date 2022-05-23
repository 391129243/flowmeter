#ifndef KEYBORD_H
#define KEYBORD_H

#define On1_Status 0x01
#define On2_Status 0x02
#define On3_Status 0x04

#define LongPress_DaysCount 15

void Init_Keybord(void);
void Enable_Keybord(void);
void doKeyPressL(void);

#endif