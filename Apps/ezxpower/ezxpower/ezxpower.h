// Copyright 2010, Abrek
// Modified by BruceLee @ MororolaFans.com

#ifndef EZXPOWER_H
#define EZXPOWER_H

#define WAIT_TILL_EXIT 20
//#define WITH_HIDE_BUTTON //XXX: Useless anyway

#include <stdio.h>

#include <ZApplication.h>
#include <ZPushButton.h>
#include <ZMainWidget.h>
#include <ZBaseDialog.h>

extern bool glob_hide_press;

class Power : public ZMainWidget
{
	Q_OBJECT
public:
	Power(QWidget *parent = 0,const char *name=0, uint f=0);
	
private slots:
	void reboot();
	void freboot();
	void shutdown();
	void fshutdown();
	void smessage();
	void updateTimer();
	void slot_hide();
	
private:
	int ttime;
	ZMainWidget *w;
	ZPushButton *bt, *bt1, *bt2, *bt3, *bt4;
	QTranslator *ts;
	QString language, language_string_path, msg, FileName;

	void updateButtonText();
};

#endif

