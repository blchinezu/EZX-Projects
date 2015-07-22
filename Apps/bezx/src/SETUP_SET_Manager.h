#ifndef _SETUPSETMANAGER_H_
#define _SETUPSETMANAGER_H_

#include <ZConfig.h>

class SETUP_SET_Manager: public ZConfig
{
public:
    SETUP_SET_Manager(QString, bool);

    virtual int clearGroup(QString);
    virtual int removeEntry(QString, QString);
    int getSettingInfo(QString Section, QString Name, QString& Value, QString Default, bool);
    int getSettingInfo(QString Section, QString Name, bool& Value, bool Default);
    int getSettingInfo(QString Section, QString Name, int& Value, int Default);
    int getSettingInfo(QString Section, QString Name, unsigned& Value, unsigned Default);
    int getSettingInfo(QString Section, QString Name, unsigned short& Value, unsigned short Default);
    int setSettingInfo(QString Section, QString Name, QString, bool);
    int setSettingInfo(QString Section, QString Name, bool);
    int setSettingInfo(QString Section, QString Name, int);
    int setSettingInfo(QString Section, QString Name, unsigned);
    int setSettingInfo(QString Section, QString Name, unsigned short);
    int getEntryKeyList(QString);
    virtual bool flush(void);

    virtual ~SETUP_SET_Manager();
protected:

private:
};


#endif
