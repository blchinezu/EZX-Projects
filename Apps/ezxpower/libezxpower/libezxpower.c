#include <stdio.h>

void _ZN15PHONE_MsgClient16notifyPmPowerOffEv(void *this, void *event)
{
  (void)this;
  (void)event;
system("/usr/SYSqtapp/power/ezxpower");
}

