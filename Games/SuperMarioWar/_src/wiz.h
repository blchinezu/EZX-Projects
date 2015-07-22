#ifndef _WIZ_H
#define _WIZ_H

#define VOLUME_MIN 0
#define VOLUME_MAX 128
#define VOLUME_CHANGE_RATE 10
#define VOLUME_NOCHG 0
#define VOLUME_DOWN 1
#define VOLUME_UP 2

#define GP2X_BUTTON_UP              (0)
#define GP2X_BUTTON_DOWN            (4)
#define GP2X_BUTTON_LEFT            (2)
#define GP2X_BUTTON_RIGHT           (6)
#define GP2X_BUTTON_UPLEFT          (1)
#define GP2X_BUTTON_UPRIGHT         (7)
#define GP2X_BUTTON_DOWNLEFT        (3)
#define GP2X_BUTTON_DOWNRIGHT       (5)
#define GP2X_BUTTON_CLICK           (18)
#define GP2X_BUTTON_A               (12)
#define GP2X_BUTTON_B               (13)
#define GP2X_BUTTON_X               (14)
#define GP2X_BUTTON_Y               (15)
#define GP2X_BUTTON_L               (10)
#define GP2X_BUTTON_R               (11)
#define GP2X_BUTTON_START           (8)
#define GP2X_BUTTON_SELECT          (9)
#define GP2X_BUTTON_VOLUP           (16)
#define GP2X_BUTTON_VOLDOWN         (17)

#define EZX_TopLeft                 (70)
#define EZX_BottomLeft              (71)
#define EZX_TopRight                (72)
#define EZX_BottomRight             (73)

#define TopLeft 0
#define BottomLeft 1
#define TopRight 2
#define BottomRight 3

void EZX_init_vol();
void EZX_set_vol(int vol);
void EZX_quit_vol();

char EZX_ClickedOver(int cx, int cy, int x, int y, int w, int h);
char EZX_TouchState(int place, int cx, int cy);
void EZX_ProcessTouch(int cx, int cy, int value);

void WIZ_AdjustVolume( int direction ); 
void WIZ_KeyboardEmulation( int button, int value );

#endif
