#!/bin/bash
if [ "$(id -u)" != "0" -a -z "$1" ]
then
  exec /sbin/start-stop-daemon -S -c root:root -x "$0" -- restart
  exit 1
fi

myfile=`basename $0`
mypath=`echo $0 | sed -e "s|$myfile||g" | sed -e "s|/$||g"`
mypath=/mmc/mmca1/.system/QTDownLoad/space/
cd $mypath
export PATH=$PATH:$mypath
export QTDIR=/usr/lib/ezx

ram="/ram/var/run/"
daemonpid="${ram}spc.pid"
startflag="${ram}spcstarted"
stopflag="${ram}spcSTOP"
spcsize="${ram}spcsize"
pausefile="${ram}pause"
logfile="/mmc/mmca1/spacem.log"
lastrefresh="${mypath}stat/last.refresh"
lastalarmdate="${mypath}stat/last.alarm.date"
lastalarmtime="${mypath}stat/last.alarm.time"
alarms="${mypath}stat/nr.alarms"
smonitoring="${mypath}stat/started.mon"
alarm_sound="${mypath}settings/alarm.sound"
soundalert='/usr/SYSqtapp/phone/alertprocess'

appname='Space Monitor'
version='1.1'

. /home/native/.profile 2>/dev/null 1>/dev/null

init ()
{
	lasttime="1"
	lastsize="1"
	timezone="0"
	if [ -f "${mypath}settings/last.time" ]; then
		lasttime=`cat ${mypath}settings/last.time`
	fi
	if [ -f "${mypath}settings/last.size" ]; then
	lastsize=`cat ${mypath}settings/last.size`
	fi
	if [ -f "${mypath}settings/timezone" ]; then
	timezone=`cat ${mypath}settings/timezone`
	fi

	if [ ! -d "${mypath}settings" ]; then
		mkdir ${mypath}settings
		echo "Info: Settings folder created"
	fi
	if [ ! -d "${mypath}stat" ]; then
		mkdir ${mypath}stat
		echo "Info: Statistics folder created"
	fi
}

set_timezone ()
{
	if [ -f "${mypath}settings/timezone" ]; then
		default=`cat ${mypath}settings/timezone`
	else
		default="0"
	fi
	size=`${mypath}bin/inputbox -t "<h3>Set your timezone</h3>" -l "Min: -12 Max: 12" -getinteger -d "$default" -n "-12" -x "12" | sed -n s/^Integer=//gp`
	if [ "$size" = "" ]; then
		if [ ! -f "${mypath}settings/timezone" ];then
			set_timezone
		fi
	else
		if [ "$size" -gt "-13" ] && [ "$size" -lt "13" ];then
			echo "Settings: Timezone [GMT ${size}]"
			echo "$size" > ${mypath}settings/timezone
		else
			set_timezone
		fi
	fi
}


set_monitor ()
{
	if [ -z "$1" ]; then
		echo 'ERROR: set_monitor() has no "$1" variable defined' # ex: $1 = ezxuser [string]
		exit
	fi
	mmsg=`${mypath}bin/showQ -u -1 "Yes" -2 "No" -g "${mypath}msg.png" -m "Do you want to monitor [$1]?"`
	case "$mmsg" in
	0)
		echo "Settings: Do monitor [$1]"
		echo "1" > ${mypath}settings/$1.monitor
		;;
	1)
		echo "Settings: Do not monitor [$1]"
		echo "0" > ${mypath}settings/$1.monitor
		;;
	*)	echo 'ERROR: set_monitor() "case" statement got wrong value:'
		echo "$mmsg"
		;;
	esac
}


set_size_type ()
{
	if [ -z "$1" ]; then
		echo 'ERROR: set_size_type() has no "$1" variable defined' # ex: $1 = ezxuser [string]
		exit
	fi

	if [ -f "${mypath}settings/$1.type" ]; then
		current_type=`cat ${mypath}settings/$1.type`
		add_text="<br><br>Current: ${current_type}"
	else
		add_text=""
	fi
	mmsg=`${mypath}bin/showQ -u -1 "KB" -2 "MB" -3 "Cancel" -g "${mypath}msg.png" -m "What size type to use for [$1]?${add_text}"`
	case "$mmsg" in
	0)
		echo "Settings: [$1] size type: KB"
		echo "KB" > ${mypath}settings/$1.type
		;;
	1)
		echo "Settings: [$1] size type: MB"
		echo "MB" > ${mypath}settings/$1.type
		;;
	*)	if [ "$mmsg" = "2" ]; then
			echo "INFO: [$1] Size type menu cancelled"
		else
			echo 'ERROR: set_size_type() "case" statement got wrong value:'
			echo "$mmsg"
			if [ ! -f "${mypath}settings/$1.type" ]; then
				set_size_type $@
			fi
		fi
	;;
	esac
}


set_limit()
{
	if [ -f "${mypath}settings/$1.monitor" ]; then
		monitor=`cat ${mypath}settings/$1.monitor`
	else
		monitor="0"
	fi
	if [ "$monitor" = "1" ]; then
		if [ -z "$1" ]; then
			echo 'ERROR: set_limit() has no "$1" variable defined' # ex: $1 = ezxuser [string]
			exit
		fi
		if [ -z "$2" ]; then
			echo 'ERROR: set_limit() has no "$2" variable defined' # ex: $2 = ezxusersize [int]
			exit
		fi
		if [ -f "${mypath}settings/$1.type" ]; then
			sizetype=`cat ${mypath}settings/$1.type`
			if [ -f "${mypath}settings/$1.limit" ]; then
				default=`cat ${mypath}settings/$1.limit`
				if [ "$sizetype" = "MB" ]; then
					default="$(( $default / 1024 ))"
				fi
			else
				default="1"
			fi
			if [ "$sizetype" = "MB" ]; then
				max_size="$(( $2 / 1024 ))"
			else
				max_size="$2"
			fi
			size=`${mypath}bin/inputbox -t "<h3>$1</h3>" -l "Alarm level (max ${max_size}${sizetype}):" -getinteger -d "$default" -n "1" -x "${max_size}" | sed -n s/^Integer=//gp`
			if [ "$size" = "" ]; then
				if [ ! -f "${mypath}settings/$1.limit" ]; then
					set_limit $@
				fi
			else
				echo "Settings: [$1] Alarm level: ${size}${sizetype}"
				if [ "$sizetype" = "MB" ]; then
					size="$(( $size * 1024 ))"
				fi
				echo "$size" > ${mypath}settings/$1.limit
				echo "$size" > ${mypath}settings/last.size
			fi
		else
			set_size_type $@
			set_limit $@
		fi
	else
		set_monitor $@
		if [ -f "${mypath}settings/$1.monitor" ]; then
			monitor=`cat ${mypath}settings/$1.monitor`
		else
			monitor="0"
		fi
		if [ "$monitor" = "1" ]; then
			set_limit $@
		fi
	fi
}


set_sound ()
{
	mmsg=`${mypath}bin/showQ -u -1 "Yes" -2 "No" -g "${mypath}msg.png" -m "Do you want to activate sound?"`
	case "$mmsg" in
	0)
		echo "Settings: Sound enabled"
		echo "1" > ${mypath}settings/sound.active
		;;
	1)
		echo "Settings: Sound disabled"
		echo "0" > ${mypath}settings/sound.active
		;;
	*)	echo 'ERROR: set_sound() "case" statement got wrong value:'
		echo "$mmsg"
		;;
	esac
}


set_volume ()
{
	if [ -f "${mypath}settings/sound.volume" ]; then
		default=`cat ${mypath}settings/sound.volume`
	else
		default="3"
	fi
	vol=`${mypath}bin/inputbox -t "<h3>${appname} v${version}</h3>" -l "Sound volume (1>7):" -getinteger -d "$default" -n "1" -x "7" | sed -n s/^Integer=//gp`
	if [ "$vol" = "" ]; then
		if [ ! -f "${mypath}settings/sound.volume" ]; then
			set_volume
		fi
	else
		echo "Settings: Sound volume: $vol [min.1 - max.7]"
		echo "$vol" > ${mypath}settings/sound.volume
	fi
}


set_test_delay ()
{
	if [ -f "${mypath}settings/test.delay" ]; then
		default=`cat ${mypath}settings/test.delay`
	else
		default="5"
	fi
	time=`${mypath}bin/inputbox -t "<h3>${appname} v${version}</h3>" -l "Refresh rate (minutes):" -getinteger -d "5" -n "1" | sed -n s/^Integer=//gp`
	if [ "$time" = "" ]; then
		if [ ! -f "${mypath}settings/test.delay" ]; then
			set_test_delay
		fi
	else
		echo "Settings: Space check refresh rate: $time"
		echo "$time" > ${mypath}settings/test.delay
		echo "$time" > ${mypath}settings/last.time
	fi
}


write_time ()
{
	if [ -z "$1" ]; then
		echo 'ERROR: write_time() has no "$1" variable defined' # $1 output file
	else
		timezone=`cat ${mypath}settings/timezone`
		ora=`date +"%H"`
		ora=$(( $ora + $timezone ))
		if [ "$ora" -lt "0" ]; then
			ora=$(( $ora + 24 ))
		fi
		if [ "$ora" -gt "24" ]; then
			ora=$(( $ora - 24 ))
		fi
		min=`date +":%M:%S"`
		echo "${ora}${min}" > $1
	fi
}


write_date ()
{
	if [ -z "$1" ]; then
		echo 'ERROR: write_date() has no "$1" variable defined' # $1 output file
	else
		timezone=`cat ${mypath}settings/timezone`
		ora=`date +"%H"`
		ora=$(( $ora + $timezone ))
		data=`date +%d/%m/%y`
		if [ "$ora" -lt "0" ]; then
			data=`date -d "1 day ago" +%d/%m/%y`
		fi
		if [ "$ora" -gt "24" ]; then
			data=`date -d "next day" +%d/%m/%y`
		fi
		echo "${data}" > $1
	fi
}


get_current_status ()
{
	# TIMEZONE
	if [ ! -f "${mypath}settings/timezone" ]; then
		set_timezone
	fi
	
	# TIME
	if [ ! -f "${mypath}settings/test.delay" ]; then
		set_test_delay
	fi
	
	# SOUND
	if [ ! -f "${mypath}settings/sound.active" ]; then
		set_sound
	fi
	soundactive=`cat ${mypath}settings/sound.active`
	if [ "$soundactive" = "1" ]; then
		if [ ! -f "${mypath}settings/sound.volume" ]; then
			set_volume
		fi
		soundvolume=`cat ${mypath}settings/sound.volume`
		if [ ! -f "$alarm_sound" ]; then
			set_sound_file
		fi
		dingsound=`cat $alarm_sound`
	fi
	
	# EZX_USER
	if [ ! -f "${mypath}settings/ezxuser.monitor" ]; then
		set_monitor ezxuser
	fi
	ezxusermonitor=`cat ${mypath}settings/ezxuser.monitor`
	if [ "$ezxusermonitor" = "1" ]; then
		ezxusersize=$((`df | grep ezx_user | sed 's/  */ /g' | sed 's/\([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\)/\2/'`))
		ezxuserfree=$((`df | grep ezx_user | sed 's/  */ /g' | sed 's/\([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\)/\4/'`))
		if [ ! -f "${mypath}settings/ezxuser.limit" ]; then
			set_limit ezxuser $ezxusersize
		fi
		ezxuseralarm=`cat ${mypath}settings/ezxuser.limit`
	fi

	# EZXLOCAL
	if [ ! -f "${mypath}settings/ezxlocal.monitor" ]; then
		set_monitor ezxlocal
	fi
	ezxlocalmonitor=`cat ${mypath}settings/ezxlocal.monitor`
	if [ "$ezxlocalmonitor" = "1" ]; then
		ezxlocalsize=$((`df | grep ezxlocal | sed 's/  */ /g' | sed 's/\([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\)/\2/'`))
		ezxlocalfree=$((`df | grep ezxlocal | sed 's/  */ /g' | sed 's/\([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\)/\4/'`))
		if [ ! -f "${mypath}settings/ezxlocal.limit" ]; then
			set_limit ezxlocal $ezxlocalsize
		fi
		ezxlocalalarm=`cat ${mypath}settings/ezxlocal.limit`
	fi
	
	# MMC
	if [ ! -f "${mypath}settings/mmc.monitor" ]; then
		set_monitor mmc
	fi
	mmcmonitor=`cat ${mypath}settings/mmc.monitor`
	if [ "$mmcmonitor" = "1" ]; then
		mmcsize=$((`df | grep "/mmc/mmca1$" | sed 's/  */ /g' | sed 's/\([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\)/\2/'`))
		mmcfree=$((`df | grep "/mmc/mmca1$" | sed 's/  */ /g' | sed 's/\([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\)/\4/'`))
		if [ ! -f "${mypath}settings/mmc.limit" ]; then
			set_limit mmc $mmcsize
		fi
		mmcalarm=`cat ${mypath}settings/mmc.limit`
	fi
}


alarm_check_proc ()
{
# Arg - example meaning
# ---------------------
# $1  - ezxuser
# $2  - $ezxusermonitor
# $3  - $ezxuseralarm
# $4  - $ezxuserfree
# $5  - $ezxusersize
# $6  - $soundactive
# $7  - $soundvolume
	if [ "$2" = "1" ]; then
		if [ "$3" -ge "$4" ]; then
			sizetype=`cat ${mypath}settings/$1.type`
			if [ "$sizetype" = "MB" ]; then
				alarm="$(( $3 / 1024 ))"
				free="$(( $4 / 1024 ))"
				size="$(( $5 / 1024 ))"
			else
				alarm="$3"
				free="$4"
				size="$5"
			fi
			if [ "$6" = "1" ]; then
				"$soundalert" -playvol "$7" -playfile "$dingsound" &
				soundplayer="$!"
			fi
			write_time ${lastalarmtime}
			write_date ${lastalarmdate}
			if [ ! -f "$alarms" ]; then
				echo "1" > $alarms
			else
				al=`cat $alarms`
				echo "$(( $al + 1 ))" > $alarms
			fi
			${mypath}bin/showQ -t250 -1 "Ok" -2 "" -g "${mypath}msg.png" -m "<h3><color=red>!!! ALARM !!!</color></h3>Low space @ <b>$1</b>:
<br>Free: ${free}${sizetype} ($((${free}*100/${size}))%)
<br>Alarm: ${alarm}${sizetype} ($((${alarm}*100/${size}))%)
<br>Total: ${size}${sizetype} (100%)"
			if [ "$6" = "1" ]; then
				start-stop-daemon --start --exec /bin/kill $soundplayer
			fi
		fi
	fi
}


alarm_check ()
{
	echo "Info: Starting alarm check"
	if [ -f "${mypath}settings/sound.active" ]; then
		soundactive=`cat ${mypath}settings/sound.active`
		if [ -f "${mypath}settings/sound.volume" ]; then
			soundvolume=`cat ${mypath}settings/sound.volume`
			dingsound=`cat $alarm_sound`
		else
			soundvolume="0"
		fi
	else
		soundactive="0"
	fi
	write_time ${lastrefresh}
	alarm_check_proc ezxuser $ezxusermonitor $ezxuseralarm $ezxuserfree $ezxusersize $soundactive $soundvolume
	alarm_check_proc ezxlocal $ezxlocalmonitor $ezxlocalalarm $ezxlocalfree $ezxlocalsize $soundactive $soundvolume
	alarm_check_proc mmc $mmcmonitor $mmcalarm $mmcfree $mmcsize $soundactive $soundvolume
	echo "Info: "
}


add_to_startup_dir ()
{
	if [ -z "$1" ]; then
		echo 'ERROR: add_to_startup_dir() has no "$1" variable defined' # $1 output file
	else
		rez=`${mypath}bin/inputbox -t "<h3>${appname}</h3>" -l "Write the folder path" -getitemtext "$@" | sed -n s/^ItemText=//gp`
		if [ "$rez" != "" ]; then
			folder="${rez}"
			if [ ! -d "$folder" ]; then
				mmsg=`${mypath}bin/showQ -u -1 "Ok" -2 "" -g "${mypath}msg.png" -m "<h3><color=red>! ERROR !</color></h3>There is no:<br>$folder"`
				echo 'ERROR: add_to_startup() there is no such folder'
				add_to_startup
			else
				echo "#!/bin/bash

" > $folder/space_mon.lin
				echo "$text" >> $folder/space_mon.lin
				echo "

exit" >> $folder/space_mon.lin
				echo "System: ${appname} added to startup [$folder]"
				${mypath}bin/showQ -u -1 "Ok" -2 "" -g "${mypath}msg.png" -m "${appname} v${version} was successfully added to the startup script:<br>$folder"
			fi
		fi
	fi
}


add_to_startup_file ()
{
	if [ -z "$1" ]; then
		echo 'ERROR: add_to_startup_file() has no "$1" variable defined' # $1 output file
	else
		rez=`${mypath}bin/inputbox -t "<h3>${appname}</h3>" -l "Target file path" -getitemtext "$@" | sed -n s/^ItemText=//gp`
		if [ "$rez" != "" ]; then
			file="${rez}"
			if [ ! -f "$file" ]; then
				${mypath}bin/showQ -u -1 "Ok" -2 "" -g "${mypath}msg.png" -m "<h3><color=red>! ERROR !</color></h3>There is no:<br>$file"
				echo 'ERROR: add_to_startup() there is no such file'
				add_to_startup
			else
				bak=`cat $file`
				echo $bak > ${file}.bak
				echo "$text" >> $file
				echo "System: ${appname} added to startup [$file]"
				${mypath}bin/showQ -u -1 "Ok" -2 "" -g "${mypath}msg.png" -m "${appname} v${version} was successfully added to the startup script:<br>$file"
			fi
		fi
	fi
}


add_to_startup ()
{
	added="0"
	if [ ! -f "${mypath}add2startup.script" ]; then
		echo 'ERROR: add_to_startup() could not find: "add2startup.script"'
		exit
	fi
	text=`cat ${mypath}add2startup.script`
	opt=`${mypath}bin/showQ -u -1 "File" -2 "Folder" -3 "Cancel" -g "${mypath}msg.png" -m "What kind?"`
	case "$opt" in
	0)
		add_to_startup_file -item /mmc/mmca1/startup1.txt -item /mmc/mmca1/startup.txt -item /mmc/mmca1/startup.lin -item /mmc/mmca1/autorun.txt -item /mmc/mmca1/autorun1.txt -item /mmc/mmca1/.system/QTDownLoad/RunScript/autorun.lin
		;;
	1)
		add_to_startup_dir -item /mmc/mmca1/startup -item /mmc/mmca1/autorun
		;;
	*)
		echo "Info: Cancelled"
		;;
	esac
	menu
}


start_monitoring ()
{
	echo "1" > $startflag
	rm -f $pausefile
	cp -f ${mypath}startup.sh ${ram}spcstartup.sh
	${ram}spcstartup.sh &
	echo "$daemon" > $daemonpid
	if [ ! -f "$smonitoring" ]; then
		echo "1" > $smonitoring
	else
		count=`cat $smonitoring`
		echo "$(( $count + 1 ))" > $smonitoring
	fi
	echo 'Info: Monitoring started'
}


stop_monitoring ()
{			
	echo "1" > $stopflag
	rm -f $startflag
	rm -f $pausefile
	start-stop-daemon --start --exec /bin/kill ` pidof spcstartup.sh `
	echo 'Info: Monitoring stopped'
}


statistics ()
{
	if [ -f "$lastrefresh" ]; then
		lrefr=`cat ${lastrefresh}`
	else
		lrefr="none"
	fi
	if [ -f "$lastalarmdate" ]; then
		lalx=`cat ${lastalarmdate}`
		laly=`cat ${lastalarmtime}`
		lal="${lalx}<br>${laly}"
	else
		lal="none"
	fi
	if [ -f "$alarms" ]; then
		sal=`cat ${alarms}`
	else
		sal="0"
	fi
	if [ -f "$smonitoring" ]; then
		smon=`cat ${smonitoring}`
		if [ "$smon" -gt "1" ]; then
			tt="times"
		else
			tt="time"
		fi
	else
		smon="0"
		tt="[never monitored]"
	fi
	mmsg=`${mypath}bin/showQ -t250 -1 "OK" -2 "Reset" -m "<h3>Statistics</h3>
Last Refresh Time:<br>${lrefr}<br>Last Alarm Date/Time:<br>${lal}<br>Alarms Encountered:<br>${sal}<br>Started Monitorings:<br>${smon} ${tt}"`
	if [ "$mmsg" = "1" ]; then
		rm -f $lastrefresh
		rm -f $lastalarmdate
		rm -f $lastalarmtime
		rm -f $alarms
		rm -f $smonitoring
	fi
}


set_sound_file ()
{
	temp="${ram}spc.tmp"
	soundspath="${mypath}sounds/"
	rm -f $temp
	echo "Info: Ignore these errors (max 4):"
	echo "----------------------------------"
	ls ${soundspath}*.mid >> $temp
	ls ${soundspath}*.MID >> $temp
	ls ${soundspath}*.mp3 >> $temp
	ls ${soundspath}*.MP3 >> $temp
	echo "----------------------------------"
	cat ${temp} | sed "s|${soundspath}||g" > $temp
	cat ${temp} | sed -e 's/^/ -item /g' -e 's/$//g' > $temp
	cat ${temp} | sed -e ':a;$!N;s/\n//;ta;' -e 's/$//g' > $temp
	itemlist=`cat $temp`
	if [ -z "$itemlist" ]; then
		echo "ERROR: No sounds found"
		${mypath}bin/showQ -t250 -1 "OK" -2 "" -m "<b>ERROR:</b><br><br>No sounds found."
	else
		sounds=`${mypath}bin/inputbox -t "<h3>${appname}</h3>" -l "Choose sound" -getitemtext $itemlist | sed -n s/^ItemText=//gp`
		if [ "$sounds" = "" ]
		then
			rm -f $temp
		else
			rm -f $temp
			file="${soundspath}${sounds}"
			if [ -f "$file" ]; then
				echo "$file" > $alarm_sound
				echo "Settings: Setted sound: $file"
				${mypath}bin/showQ -t250 -1 "OK" -2 "" -m "Sound [${sounds}] activated."
			else
				echo 'ERROR: set_sound_file() sound not found:'
				echo "$file"
				${mypath}bin/showQ -t250 -1 "OK" -2 "" -m "<b>ERROR:</b><br><br>Sound not found."
			fi
		fi
	fi
}


settings ()
{
	monitored="0"
	ezxusermonitor=`cat ${mypath}settings/ezxuser.monitor`
	ezxlocalmonitor=`cat ${mypath}settings/ezxlocal.monitor`
	mmcmonitor=`cat ${mypath}settings/mmc.monitor`
	time=`cat ${mypath}settings/test.delay`
	if [ -f "${mypath}settings/last.size" ]; then
		lastsize=`cat ${mypath}settings/last.size`
	fi
	soundactive=`cat ${mypath}settings/sound.active`
	if [ -f "${mypath}settings/sound.volume" ]; then
		soundvolume=`cat ${mypath}settings/sound.volume`
	fi
	timezone=`cat ${mypath}settings/timezone`
	if [ "$ezxusermonitor" = "1" ]; then
		monitored=$(($monitored+1))
		ezxusermonitorstr="Yes"
	else
		ezxusermonitorstr="No"
	fi
	if [ "$ezxlocalmonitor" = "1" ]; then
		monitored=$(($monitored+1))
		ezxlocalmonitorstr="Yes"
	else
		ezxlocalmonitorstr="No"
	fi
	if [ "$mmcmonitor" = "1" ]; then
		monitored=$(($monitored+1))
		mmcmonitorstr="Yes"
	else
		mmcmonitorstr="No"
	fi
	if [ "$soundactive" = "1" ]; then
		soundstatus="ON"
	else
		soundstatus="OFF"
	fi
	choice=`${mypath}bin/inputbox -t "<h3>Settings</h3>" -l "Choose option:" -select -item "Timezone [GMT${timezone}]" -item "Refresh rate (${time}min)" -item "Monitored locations (${monitored}/3)" -item "Alert levels" -item "Size types" -item "Sound [$soundstatus]" | sed -n s/^Select=//gp`
	case "$choice" in
	0)
		echo "Menu: Timezone"
		set_timezone
		get_current_status
		settings
		;;
	1)
		echo "Menu: Refresh rate"
		set_test_delay
		get_current_status
		settings
		;;
	2)
		elem=`${mypath}bin/inputbox -t "<h3>Monitored</h3>" -l "Choose location:" -select -item "ezxuser [${ezxusermonitorstr}]" -item "ezxlocal [${ezxlocalmonitorstr}]" -item "mmc [${mmcmonitorstr}]" | sed -n s/^Select=//gp`
		case "$elem" in
		0)
			echo "Menu: ezxuser"
			set_monitor ezxuser
			;;
		1)
			echo "Menu: ezxlocal"
			set_monitor ezxlocal
			;;
		2)
			echo "Menu: mmc"
			set_monitor mmc
			;;
		esac
		get_current_status
		settings
		;;
	3)
		if [ -f "${mypath}settings/ezxuser.limit" ]; then
			ezxuseralarm=`cat ${mypath}settings/ezxuser.limit`
		else
			ezxuseralarm="none"
		fi
		if [ -f "${mypath}settings/ezxlocal.limit" ]; then
			ezxlocalalarm=`cat ${mypath}settings/ezxlocal.limit`
		else
			ezxlocalalarm="none"
		fi
		if [ -f "${mypath}settings/mmc.limit" ]; then
			mmcalarm=`cat ${mypath}settings/mmc.limit`
		else
			mmcalarm="none"

		fi
		if [ -f "${mypath}settings/ezxuser.type" ]; then
			ezxusertype=`cat ${mypath}settings/ezxuser.type`
			if [ "$ezxusertype" = "MB" ]; then
				ezxuseralarm=$(( ${ezxuseralarm} / 1024 ))
			fi
		else
			ezxusertype=""
		fi
		if [ -f "${mypath}settings/ezxlocal.type" ]; then
			ezxlocaltype=`cat ${mypath}settings/ezxlocal.type`
			if [ "$ezxlocaltype" = "MB" ]; then
				ezxlocalalarm=$(( ${ezxlocalalarm} / 1024 ))
			fi
		else
			ezxlocaltype=""
		fi
		if [ -f "${mypath}settings/mmc.type" ]; then
			mmctype=`cat ${mypath}settings/mmc.type`
			if [ "$mmctype" = "MB" ]; then
				mmcalarm=$(( ${mmcalarm} / 1024 ))
			fi
		else
			mmctype=""
		fi
		sel=`${mypath}bin/inputbox -t "<h3>Alert Levels</h3>" -l "Choose location:" -select -item "ezxuser [${ezxuseralarm}${ezxusertype}]" -item "ezxlocal [${ezxlocalalarm}${ezxlocaltype}]" -item "mmc [${mmcalarm}${mmctype}]" | sed -n s/^Select=//gp`
		case "$sel" in
		0)
			echo "Menu: ezxuser limit"
			ezxusersize=$((`df | grep ezx_user | sed 's/  */ /g' | sed 's/\([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\)/\2/'`))
			set_limit ezxuser $ezxusersize
			;;
		1)
			echo "Menu: ezxlocal limit"
			ezxlocalsize=$((`df | grep ezxlocal | sed 's/  */ /g' | sed 's/\([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\)/\2/'`))

			set_limit ezxlocal $ezxlocalsize
			;;
		2)
			echo "Menu: mmc limit"
			mmcsize=$((`df | grep "/mmc/mmca1$" | sed 's/  */ /g' | sed 's/\([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\)/\2/'`))
			set_limit mmc $mmcsize
			;;
		esac
		get_current_status
		settings
		;;
	4)
		ezxusertype=`cat ${mypath}settings/ezxuser.type`
		ezxlocaltype=`cat ${mypath}settings/ezxlocal.type`
		mmctype=`cat ${mypath}settings/mmc.type`
		elem=`${mypath}bin/inputbox -t "<h3>Size type</h3>" -l "Choose location:" -select -item "ezxuser [${ezxusertype}]" -item "ezxlocal [${ezxlocaltype}]" -item "mmc [${mmctype}]" | sed -n s/^Select=//gp`
		case "$elem" in
		0)
			echo "Menu: ezxuser"
			set_size_type ezxuser
			;;
		1)
			echo "Menu: ezxlocal"
			set_size_type ezxlocal
			;;
		2)
			echo "Menu: mmc"
			set_size_type mmc
			;;
		esac
		get_current_status
		settings
		;;
	5)
		if [ -f ${mypath}settings/sound.active ]; then
			soundactive=`cat ${mypath}settings/sound.active`
		else
			echo "0" > ${mypath}settings/sound.active
			soundactive="0"
		fi
		if [ "$soundactive" = "1" ]; then
			soundstatus="Sound [ON]"
		else
			soundstatus="Sound [OFF]"
		fi
		if [ -f ${mypath}settings/sound.volume ]; then
			soundvolume=`cat ${mypath}settings/sound.volume`
		else
			echo "3" > ${mypath}settings/sound.volume
			soundvolume="3"
		fi
		sel=`${mypath}bin/inputbox -t "<h3>Sound</h3>" -l "Choose option:" -select -item "$soundstatus" -item "Volume [$soundvolume]" -item "Change Sound"| sed -n s/^Select=//gp`
		case "$sel" in
		0)
			set_sound
			;;
		1)
			set_volume
			;;
		2)
			set_sound_file
			;;
		esac
		get_current_status
		settings
		;;
	esac
}


delay_start ()
{
	if [ -f $pausefile ]; then
		time=`cat $pausefile`
		mmsg=`${mypath}bin/showQ -u -1 "Stop" -2 "Time" -3 "Cancel" -g "${mypath}msg.png" -m "Starting in: ${time}min<br>So what about the delayed start?"`
		case "$mmsg" in
		"0")
			rm -f $pausefile
			rm -f $startflag
			rm -f $stopflag
			echo "Menu: Stop delayed start"
			;;
		"1")
			lasttime="$time"
			time=`${mypath}bin/inputbox -t "<h3>Delay</h3>" -l "Set the delay (minutes):" -getinteger -d "$lasttime" -n "1" | sed -n s/^Integer=//gp`
			if [ "$time" -gt "0" ]; then
				echo "$time" > $pausefile
				echo "$time" > ${mypath}settings/last.time
				echo "Info: Delayed start time changed from ${lasttime}min to ${time}min"
			fi
			;;
		"2")
			echo "Menu: Cancel"
			;;
		esac
	else
		time=`${mypath}bin/inputbox -t "<h3>Delay</h3>" -l "Set the delay (minutes):" -getinteger -d "$lasttime" -n "1" | sed -n s/^Integer=//gp`
		if [ "$time" -gt "0" ]; then
			echo "$time" > $pausefile
			echo "$time" > ${mypath}settings/last.time
			./delayedstart.sh &
			echo "Info: Delayed start time: ${time}min"
		fi
	fi
}


pause_monitoring ()
{
	if [ -f $pausefile ]; then
		time=`cat $pausefile`
		mmsg=`${mypath}bin/showQ -u -1 "Stop" -2 "Time" -3 "Cancel" -g "${mypath}msg.png" -m "Starting in: ${time}min<br>So what about the paused time?"`
		case "$mmsg" in
		"0")
			rm -f $pausefile
			echo "Menu: Ended pause time"
			;;
		"1")
			lasttime="$time"
			time=`${mypath}bin/inputbox -t "<h3>Pause</h3>" -l "Set the pause time (minutes):" -getinteger -d "$lasttime" -n "1" | sed -n s/^Integer=//gp`
			if [ "$time" -gt "0" ]; then
				echo "$time" > $pausefile
				echo "$time" > ${mypath}settings/last.time
				echo "Info: Pause time changed from ${lasttime}min to ${time}min"
			fi
			;;
		"2")
			echo "Menu: Cancel"
			;;
		esac
	else
		time=`${mypath}bin/inputbox -t "<h3>Pause</h3>" -l "Set the pause time (minutes):" -getinteger -d "$lasttime" -n "1" | sed -n s/^Integer=//gp`
		if [ "$time" -gt "0" ]; then
			echo "$time" > $pausefile
			echo "$time" > ${mypath}settings/last.time
			echo "Info: Pause time: ${time}min"
		fi
	fi
}


menu ()
{
	if [ -f "$startflag" ]; then
		startstopstr="Stop Monitoring"
		if [ -f $pausefile ]; then
			time=`cat $pausefile`
			pausestr="Paused: ${time}min"
		else
			pausestr="Pause Monitoring"
		fi
	else
		startstopstr="Start Monitoring"
		if [ -f $pausefile ]; then
			time=`cat $pausefile`
			pausestr="Delayed: ${time}min"
		else
			pausestr="Delayed Start"
		fi
	fi
	opt=`${mypath}bin/inputbox -t "<h3>${appname} v${version}</h3>" -l "Choose option:" -select -item "Check  Now" -item "${startstopstr}" -item "${pausestr}" -item "Change Settings" -item "Add to startup" -item "Show Statistics" | sed -n s/^Select=//gp`
		case "$opt" in
		0)
			echo "Menu: Check now"
			get_current_status
			alarm_check
			menu
			;;
		1)
			if [ -f "$startflag" ]; then
				echo "Menu: Stop Monitoring"
				stop_monitoring
			else
				echo "Menu: Start Monitoring"
				start_monitoring
			fi
			get_current_status
			menu
			;;
		2)
			if [ -f "$startflag" ]; then
				echo "Menu: Pause Monitoring"
				pause_monitoring
			else
				echo "Menu: Delayed start"
				delay_start
			fi
			get_current_status
			menu
			;;
		3)
			echo "Menu: Change Settings"
			settings
			get_current_status
			menu
			;;
		4)
			echo "Menu: Add to startup"
			add_to_startup
			menu
			;;
		5)
			echo "Menu: Show statistics"
			statistics
			menu
			;;
	esac
	exit
}




console_help ()
{
	echo "
============ HELP ============
 ${appname} v${version}

 -help      Show this
 -log       Output mmc/sm.log
 -check     Just check space
 -start     Start monitoring
 -stop      Stop monitoring
 -settings  Get GUI settings
 -menu      Get GUI menu
 No arg     Get GUI menu

==============================

"
}




main_sec ()
{
	init
	case "$1" in
	"-check")
		get_current_status
		alarm_check
		;;
	"-menu")
		get_current_status
		menu
		;;
	"-settings")
		get_current_status
		settings
		;;
	"-start")
		start_monitoring
		;;
	"-stop")
		stop_monitoring
		;;
	"-help")
		console_help
		;;
	*)
		get_current_status
		menu
		;;
	esac
	echo "[space.sh] exiting"
	exit
}




main ()
{
	log="0"
	if [ "$1" = "-log" ]; then
		main_sec $2 > $logfile
	elif [ "$2" = "-log" ]; then
		main_sec $1 > $logfile
	else
		main_sec $@
	fi
}

main $@

exit
