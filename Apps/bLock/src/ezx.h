#ifndef _CEZX_H_
#define _CEZX_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>
#include <linux/fb.h>
#include <string.h>

#include "SETUP_SET_Manager.h"
#include "SETUP_Utility.h"
#include <ezxpm.h>
//#include "pm_app_interface.h"

#include "main.h"



// Vibration
#define VIB_DEV				"/dev/vibrator"
#define VIB_MAJOR			108
#define VIB_IOCTL_BASE		0xbb
#define VIB_ENABLE			_IOW (VIB_IOCTL_BASE,1,int)
#define VIB_DISABLE	_IO		(VIB_IOCTL_BASE,2)
// Key Light
#define KEYLIGHT_DEV		"/dev/keylight"
#define KEYLIGHT_MAIN_ON    0xf0
#define KEYLIGHT_MAIN_OFF   0xf2
// Backlight/Brightness
#define FRAMEBUFFER_DEV		"/dev/fb0"
#define FBIOSETBKLIGHT      0x4619
#define FBIOGETBKLIGHT      0x461A
#define FBIOSETBRIGHTNESS   0x461B
#define FBIOGETBRIGHTNESS   0x461C
#define BKLIGHT_OFF         0
#define BKLIGHT_ON          1
#define MIN_BRIGHTNESS		0
#define MAX_BRIGHTNESS		100
#define FILE_SYS_CFG		"/ezx_user/download/appwrite/setup/ezx_system.cfg"
#define FILE_THEME_CFG		"/ezx_user/download/appwrite/setup/ezx_theme.cfg"

// Usb info file
#define USB_INFO_FILE		"/proc/usbd-switch"

 
// Class
class CEZX {
	private:
		int		fb, kl;
		
	public:
		int		current_br, normal_br,
				LCDOffSeconds;
		bool	current_bl, current_kl;
		
	public:
		CEZX();
		
		void	vibrate				(int);
		int		get_brightness		();
		void	set_brightness		(int);
		int		get_LCDOffSeconds	();
		void	set_LCDOffSeconds	(int);
		void	get_wallpaper		(char*);
		void	backlight			(bool);
		void	keyslight			(bool);
		void	power_saving		(bool);
		void	usb_status			(char*, char*, int*, int*);
		void	fadein_screen		(int);
		void	fadeout_screen		(int);
};



#endif /* _CEZX_H_ */

