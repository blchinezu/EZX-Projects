#!/bin/bash
if [ "$(id -u)" != "0" -a -z "$1" ]
then
  exec /sbin/start-stop-daemon -S -c root:root -x "$0" -- restart
  exit 1
fi

myfile=`basename $0`
mypath='/mmc/mmca1/.system/QTDownLoad/space/'
cd $mypath

startflag="/ram/var/run/spcstarted"
stopflag="/ram/var/run/spcSTOP"
pausefile="/ram/var/run/pause"
delayfile="${mypath}settings/test.delay"
lastrefresh="${mypath}settings/last.refresh"

. /home/native/.profile 2>/dev/null 1>/dev/null

 echo "Info: startup.sh begin"

echo "0" > $startflag
while [ "0" = "0" ]; do
	if [ -f "$stopflag" ]; then
		rm -f $stopflag
		rm -f $startflag
		rm -f $pausefile
		echo "Info: startup.sh Stopflag detected, exiting"
		exit
	else
		if [ -f ${pausefile} ]; then
			echo "Info: startup.sh Pausefile detected"
			time=`cat ${pausefile}`
			echo "Info: startup.sh time: $time"
			if [ "$time" -gt "0" ]; then
				time=$(( ${time}-1 ))
				echo "Info: startup.sh sleep 60s"
				sleep 60s
				echo "Info: startup.sh new time written to pausefile: $time"
				echo "$time" > ${pausefile}
			else
				rm -f ${pausefile}
				echo "Info: startup.sh pausefile removed"
				echo "Info: startup.sh checking space ( ./space.sh -check )"
				./space.sh -check
				time=`cat ${delayfile}`
				echo "Info: startup.sh read wait time between checks: ${time}min"
				echo "Info: startup.sh sleep $((60*$time))seconds"
				sleep $((60*${time}))
			fi
		else
			echo "Info: startup.sh checking space ( ./space.sh -check )"
			./space.sh -check
			time=`cat ${delayfile}`
			echo "Info: startup.sh read wait time between checks: ${time}min"
			sleep $((60*${time}))
			echo "Info: startup.sh sleep $((60*$time))seconds"
		fi
	fi
done

echo "Info: startup.sh exiting"

exit
