/*
    ruthfox(foxe6) add
    2008.1.6

    EMail: ruthfox@163.com

    未完成
*/

#ifndef _SETUP_UTILITY_H_
#define _SETUP_UTILITY_H_

//#include SETUP_UTIL_Define.h
//参考 Setup 程序
enum ENUM_INVL_T
{
    ENUM_INVL_BLOG,  
    ENUM_INVL_PIC,   
    ENUM_INVL_AUD,   
    ENUM_INVL_VID,   
    ENUM_INVL_GAME   
};

enum ENUM_TALKINGPHONE_READOUT_TYPE
{
    CALLER_ID_EV,
    MENU_EV,
    CONTACTS_EV,
    DIGITS_EV,
    EMAIL_EV,
    MESSAGE_EV
}; 




class SETUP_Utility
{
public:
	int addInvLProfile(ENUM_INVL_T,QString);
	int copyFile(QString,QString);
	int dcAlertStyleInit(void);
	int dcDSMInit(void);
	int dcSetupInit(void);
	int dcSkinInit(void);
	int dcThemeInit(void);
	int defaultAniDir(void);
	int defaultPictureDir(void);
	int defaultPictureFile(void);
	int defaultPowerOffFile(void);
	int defaultPowerOnFile(void);
	int defaultSoundDir(void);
	int defaultSoundFile(void);
	int delInvLProfile(ENUM_INVL_T,QString);
	int dynamicIdleInit(void);
	int dynamicIdleMakeDir(QString  const&);
//	int getAlertStatus(SETUP_SOUND_ALERT_TYPE_T,bool &);
	int getAlertStyleTimestampFileName(void);
	int getAppOfCustomisedKey(void);
	int getAutoPowerOffStatus(bool &);
//	int getAutoPowerOffTime(QTime &);
	int getBackLightValue(int &);
	int getBlendingCST(int &);
	int getBlendingColor(uint &);
	int getBlendingGNB(int &);
	int getBlendingPercentage(int &);
	int getBlendingScrollBar(int &);
	int getBlendingStatusBar(int &);
	int getBluetoothName(QString &);
	int getBluetoothNotify(bool &);
	int getBluetoothStatus(bool &);
	
//	int getBrowserFeatureBlock(BROWSER_FEATURE_BLOCK_T,uchar *&,uchar &);
//	int getBrowserFeatureState(BROWSER_FEATURE_STATE_T,uint &);
//	int getBrowserFeatureString(BROWSER_FEATURE_STRING_T,QString &);
//	int getBrowserFeatureValue(BROWSER_FEATURE_VALUE_T,uint &);
//	int getDataByInvLProfile(ENUM_INVL_T,QString,INVL_DATA_T &);
//	int getFlexBit(SETUP_FLEX_ID_T,bool &);
//	int getFlexBitFileName(SETUP_FLEX_ID_T);
	
	int getFlexBitFromBP(int *,uchar,uchar *);
	int getImsiChangedFlag(void);
	int getInstantPhoneLockStatus(bool &);
	int getInvLProfiles(ENUM_INVL_T);
	int getIrdaStatus(bool &);
	int getJavaHeapSize(int &);
	int getJavaOTATimeout(int &);
//	int getJavaSoundType(SETUP_JAVASOUND_TYPE_T &);
	int getKeyLockStatus(bool &);
	int getKeyLockTime(int &);
	int getKeyPadType(int &);
//	int getLcdSleepTime(SETUP_LCD_SLEEP_TIME_T &);
	int getLcdSleepTime(int &);
	int getMaxBrightnessRange(int &);
	int getMinBrightnessRange(int &);
	int getMinUnlockCodeLength(uint &);
	int getPhoneAutoLockStatus(bool &);
	int getPhoneAutoLockTime(int &);
	int getPhoneLockFlags(bool &,bool &);
	int getPowerDownImage(QString &);
	int getPowerUpImage(QString &);
	int getPreloadAppsPath(void);
//	int getRingToneType(RING_TONE_TYPE_T &);
//	int getServerAddrByInvLProfile(ENUM_INVL_T,QString);
	int getShortcutFileName(void);
//	int getSoundFile(SETUP_SOUND_ALERT_TYPE_T,QString &);
//	int getSoundVolume(SETUP_SOUND_ALERT_TYPE_T,int &);
//	int getTalkingPhoneStatus(ENUM_TALKINGPHONE_READOUT_TYPE,int &);
	int getThemeTimestampFileName(void);
//	int getTimeout(ENUM_TIMER_TYPE,int &);
	int getTimestampFileName(void);
	int getTouchScreenType(int &);
	int getUSBStatus(int &);
	int getUrlByInvLProfile(ENUM_INVL_T,QString);
	int getUserDefinedLanguageType(QString &);
	int getVRCommandTone(bool &);
	int getVRHelpFlex(bool &);
	int getVRTalkingPhone(bool &);
	int getVRTimeFormat(int &);
	int getVoiceMailboxName(int,QString &);
	int getVoiceMailboxNumber(int,QString &);
	int getWallpaperFileName(QString &);
	int getWallpaperTransparency(bool &);
	int initFlexbitFromBP(void);
//	int isFeatureAvailable(SETUP_FLEX_ID_T);
	int isSupportVR(bool &);
	int md5Encrypt(uchar *,ushort,char *);
	int renInvLProfile(ENUM_INVL_T,QString,QString);
//	int setAlertStatus(SETUP_SOUND_ALERT_TYPE_T,bool);
	int setAppOfCustomisedKey(QString  const&);
	int setAsPowerDownImage(QString);
	int setAsPowerUpImage(QString);
	int setAsRingTone(QString);
	int setAsWallpaper(QString);
	int setBacklightValue(void);
	int setBlendingCST(int);
	int setBlendingColor(uint);
	int setBlendingGNB(int);
	int setBlendingPercentage(int);
	int setBlendingScrollBar(int);
	int setBlendingStatusBar(int);
	int setBluetoothName(QString);
	int setBluetoothNotify(bool);
	int setBluetoothStatus(bool);
//	int setBrowserFeatureBlock(BROWSER_FEATURE_BLOCK_T,uchar *,uchar);
//	int setBrowserFeatureState(BROWSER_FEATURE_STATE_T,uint);
//	int setBrowserFeatureString(BROWSER_FEATURE_STRING_T,QString);
//	int setBrowserFeatureValue(BROWSER_FEATURE_VALUE_T,uint);
//	int setDataByInvLProfile(ENUM_INVL_T,QString,INVL_DATA_T  const&);
	int setImsiChangedFlag(bool);
	int setInstantPhoneLockStatus(bool);
	int setIrdaStatus(bool);
	int setKeyLockTime(int);
	int setLanguageType(QString);
//	int setLcdSleepTime(SETUP_LCD_SLEEP_TIME_T);
	int setMinUnlockCodeLength(uint);
//	int setPhoneAutoLockTime(SETUP_PHONELOCK_TYPE_T);
	int setPhoneLockCode(QString  const&);
	int setPhoneLockFlag(bool);
	int setPhoneLockTimeOutFlag(bool);
//	int setRingToneType(RING_TONE_TYPE_T);
	int setServerAddrByInvLProfile(ENUM_INVL_T,QString,QString);
//	int setSoundVolume(SETUP_SOUND_ALERT_TYPE_T,int);
//	int setTalkingPhoneStatus(ENUM_TALKINGPHONE_READOUT_TYPE,bool);
	int setTouchScreenType(int);
	int setUSBStatus(int);
	int setUrlByInvLProfile(ENUM_INVL_T,QString,QString);
	int setVRCommandTone(bool);
	int setVRHelpFlex(bool);
	int setVRTalkingPhone(bool);
	int setVoiceMailboxName(int,QString);
	int setVoiceMailboxNumber(int,QString);
	int setWallpaperTransparency(bool);
	int setupInitialize(void);
//	int shortcutCreateRecord(SHORTCUT_RECORD);
	int shortcutDeleteAllRecords(void);
	int shortcutDeleteRecord(QString);
//	int shortcutGetAllRecords(uchar &,SHORTCUT_RECORD *&);
	int systemDataInit(void);
	int verifyAutoLanguageCode(int);
	int verifySecurityCode(QString  const&,bool &);
	int verifyUnlockCode(QString  const&,bool &);

    SETUP_Utility(void);
    ~SETUP_Utility(void);
};

#endif
