#include "ezx.h"

CEZX::CEZX() {
	current_br = normal_br = 5;
	LCDOffSeconds = 30;
	current_bl = current_kl = true;
};

void CEZX::get_wallpaper(char* ret) {
	QString wall;
	SETUP_SET_Manager mgr2(FILE_THEME_CFG, false);
	mgr2.getSettingInfo("THEME_CONFIG_TABLE", "WallPaper", wall, "", true);
	sprintf(ret, "%s", (const char*)wall);
}

void CEZX::vibrate(int time) {
	int arg=1, argg=0, vib=0;
	vib = open(VIB_DEV,  O_RDWR);	
	ioctl(vib, VIB_ENABLE, &arg);
	usleep(time*1000);
	ioctl(vib, VIB_DISABLE, &argg);
	close(vib);
}

void CEZX::set_brightness(int value) {
	#ifdef DEBUG
	printf("Set brightness: %d\n", value);
	#endif
    PM_setupBklightBrightness(value);
	current_br = value;
}

int CEZX::get_brightness() {
	int bright;
    SETUP_Utility su;
    su.getBackLightValue(bright);
	return bright;
}

void CEZX::backlight(bool light) {
	current_bl = light;
	fb = open(FRAMEBUFFER_DEV, O_RDONLY);
	if (light)	ioctl(fb, FBIOSETBKLIGHT, BKLIGHT_ON);
	else		ioctl(fb, FBIOSETBKLIGHT, BKLIGHT_OFF);
	close(fb);
}

void CEZX::keyslight(bool light) {
	current_kl = light;
	kl = open(KEYLIGHT_DEV, O_RDONLY | O_NONBLOCK);
	if (light)	ioctl(kl, KEYLIGHT_MAIN_ON, 0);
	else		ioctl(kl, KEYLIGHT_MAIN_OFF, 0);
	close(kl);
}

void CEZX::set_LCDOffSeconds(int sec) {
	PM_setupLcdSleepTime(sec);
    PM_setupBklightBrightness(current_br);
}

int CEZX::get_LCDOffSeconds()
{
	return 30;
}

void CEZX::power_saving(bool v) {
	#ifdef DEBUG
	v=true; // warning related... useless
	#endif
}

void CEZX::usb_status(char *mode, char *ip, int *cable, int *charging) {
	bool ok = false;
	FILE *fd = fopen(USB_INFO_FILE, "r");
	if (fd) {
		if (fscanf(fd, "%s\nIP=%s\nCABLE=%d\nCHARGING=%d",
			mode, ip, cable , charging) != 4) {
			dbg(WARNING,"ezx.usb_status(): Problem while reading: ", USB_INFO_FILE);
		} else {
			ok = true;
			if(!strcmp(mode,"MotNone"))	sprintf(mode, "None");			else
			if(!strcmp(mode,"MotNet"))	sprintf(mode, "Network");		else
			if(!strcmp(mode,"MotCdc"))	sprintf(mode, "WebCam");		else
			if(!strcmp(mode,"MotACM"))	sprintf(mode, "Modem");			else
			if(!strcmp(mode,"MotMas"))	sprintf(mode, "Storage");		else
			if(!strcmp(mode,"MotPst"))	sprintf(mode, "Product Support");
		}
	} else {
		 dbg(WARNING,"ezx.usb_status(): Could not read: ", USB_INFO_FILE);
	}
	if(fd) fclose(fd);
	if(!ok) {
		sprintf(mode, "None");
		sprintf(ip, "0.0.0.0");
		*cable = *charging = 0;
	}
}

void CEZX::fadein_screen(int duration) {
	keyslight(true);
	backlight(true);
	if (current_br < normal_br) {
		int fade_steps = FADE_STEPS,
			tm = duration/fade_steps;
		if (fade_steps > normal_br) fade_steps = normal_br;
		for (float i=current_br; i<=normal_br; i+=(float)normal_br/fade_steps) {
			set_brightness((int)round(i));
			usleep(tm*1000);
		}
	} else {
		set_brightness(normal_br);
	}
}

void CEZX::fadeout_screen(int duration) {
	if (current_br == normal_br) {
		int fade_steps = FADE_STEPS,
			tm = duration/fade_steps;
		if (fade_steps > normal_br) fade_steps = normal_br;
		for (float i=current_br; i>=MIN_BRIGHTNESS; i-=(float)normal_br/fade_steps) {
			set_brightness((int)round(i));
			usleep(tm*1000);
		}
	} else {
		set_brightness(0);
	}
	backlight(false);
	keyslight(false);
}

