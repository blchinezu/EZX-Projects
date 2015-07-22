/*
 *  Copyright (C) 2006 Ludovic Jacomme (ludovic.jacomme@gmail.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

# ifndef _KBD_H_
# define _KBD_H_

# define PSP_ALL_BUTTON_MASK GP2X_CTRL_MASK
# define GP2X_ALL_BUTTON_MASK GP2X_CTRL_MASK

 enum msx_keys_emum {

   MSXK_UNDERSCORE, 
   MSXK_1,          
   MSXK_2,          
   MSXK_3,          
   MSXK_4,          
   MSXK_5,          
   MSXK_6,          
   MSXK_7,          
   MSXK_8,          
   MSXK_9,          
   MSXK_0,          
   MSXK_SEMICOLON,  
   MSXK_MINUS    ,  
   MSXK_DELETE,     
   MSXK_POUND,      
   MSXK_EXCLAMATN,  
   MSXK_DBLQUOTE,   
   MSXK_HASH,       
   MSXK_DOLLAR,     
   MSXK_PERCENT,    
   MSXK_AMPERSAND,  
   MSXK_QUOTE,      
   MSXK_LEFTPAREN,  
   MSXK_RIGHTPAREN, 
   MSXK_PLUS,       
   MSXK_EQUAL,      
   MSXK_TAB,        
   MSXK_a,          
   MSXK_b,          
   MSXK_c,          
   MSXK_d,          
   MSXK_e,          
   MSXK_f,          
   MSXK_g,          
   MSXK_h,          
   MSXK_i,          
   MSXK_j,          
   MSXK_k,          
   MSXK_l,          
   MSXK_m,          
   MSXK_n,          
   MSXK_o,          
   MSXK_p,          
   MSXK_q,          
   MSXK_r,          
   MSXK_s,          
   MSXK_t,          
   MSXK_u,          
   MSXK_v,          
   MSXK_w,          
   MSXK_x,          
   MSXK_y,          
   MSXK_z,          
   MSXK_A,          
   MSXK_B,          
   MSXK_C,          
   MSXK_D,          
   MSXK_E,          
   MSXK_F,          
   MSXK_G,          
   MSXK_H,          
   MSXK_I,          
   MSXK_J,          
   MSXK_K,          
   MSXK_L,          
   MSXK_M,          
   MSXK_N,          
   MSXK_O,          
   MSXK_P,          
   MSXK_Q,          
   MSXK_R,          
   MSXK_S,          
   MSXK_T,          
   MSXK_U,          
   MSXK_V,          
   MSXK_W,          
   MSXK_X,          
   MSXK_Y,          
   MSXK_Z,          
   MSXK_RETURN,     
   MSXK_CTRL_L,     
   MSXK_CTRL_R,     
   MSXK_SHIFT,      
   MSXK_CAPSLOCK,   
   MSXK_ESC,        
   MSXK_SPACE,      
   MSXK_LEFT,       
   MSXK_UP,         
   MSXK_RIGHT,      
   MSXK_DOWN,       
   MSXK_F0,         
   MSXK_F1,         
   MSXK_F2,         
   MSXK_F3,         
   MSXK_F4,         
   MSXK_F5,         
   MSXK_F6,         
   MSXK_F7,         
   MSXK_F8,         
   MSXK_F9,         
   MSXK_F10,
   MSXK_AT,         
   MSXK_COLON,      
   MSXK_COMMA,      
   MSXK_PERIOD,     
   MSXK_SLASH,      
   MSXK_ASTERISK,   
   MSXK_LESS,       
   MSXK_GREATER,    
   MSXK_QUESTION,   
   MSXK_PIPE,       
   MSXK_RCBRACE,    
   MSXK_RBRACKET,   
   MSXK_LBRACKET,   
   MSXK_LCBRACE,    
   MSXK_TILDA    ,  
   MSXK_BACKSLASH,  
   MSXK_POWER,      
   MSXK_SUPPR,      
   MSXK_SELECT,
   MSXK_GRAPH,
   MSXK_STOP,
   MSXK_HOME,  
   MSXK_JOY_UP,     
   MSXK_JOY_DOWN,   
   MSXK_JOY_LEFT,   
   MSXK_JOY_RIGHT,  
   MSXK_JOY_FIRE1,  
   MSXK_JOY_FIRE2,  
   MSXK_C_FPS,
   MSXK_C_JOY,
   MSXK_C_RENDER,
   MSXK_C_LOAD,
   MSXK_C_SAVE,
   MSXK_C_RESET,
   MSXK_C_AUTOFIRE,
   MSXK_C_INCFIRE,
   MSXK_C_DECFIRE,
   MSXK_C_SCREEN,

   MSXK_MAX_KEY      
  };

# define KBD_UP           0
# define KBD_RIGHT        1
# define KBD_DOWN         2
# define KBD_LEFT         3
# define KBD_TRIANGLE     4
# define KBD_CIRCLE       5
# define KBD_CROSS        6
# define KBD_SQUARE       7
# define KBD_SELECT       8
# define KBD_START        9
# define KBD_LTRIGGER    10
# define KBD_RTRIGGER    11
# define KBD_FIRE        12

# define KBD_MAX_BUTTONS 13

# define KBD_JOY_UP      13
# define KBD_JOY_RIGHT   14
# define KBD_JOY_DOWN    15
# define KBD_JOY_LEFT    16

# define KBD_ALL_BUTTONS 17

# define KBD_UNASSIGNED         -1

# define KBD_LTRIGGER_MAPPING   -2
# define KBD_RTRIGGER_MAPPING   -3
# define KBD_NORMAL_MAPPING     -1

 struct msx_key_trans {
   int  key;
   int  index;
   int  bit_mask;
   int  shift;
   char name[10];
 };
  

  extern int psp_screenshot_mode;
  extern int psp_kbd_mapping[ KBD_ALL_BUTTONS ];
  extern int psp_kbd_mapping_L[ KBD_ALL_BUTTONS ];
  extern int psp_kbd_mapping_R[ KBD_ALL_BUTTONS ];
  extern int psp_kbd_presses[ KBD_ALL_BUTTONS ];
  extern int kbd_ltrigger_mapping_active;
  extern int kbd_rtrigger_mapping_active;

  extern struct msx_key_trans psp_msx_key_info[MSXK_MAX_KEY];

  extern int  psp_update_keys(void);
  extern void kbd_wait_start(void);
  extern void psp_init_keyboard(void);
  extern void psp_kbd_wait_no_button(void);
  extern int  psp_kbd_is_danzeff_mode(void);
  extern void psp_kbd_display_active_mapping(void);
  extern int psp_kbd_load_mapping(char *kbd_filename);
# endif
