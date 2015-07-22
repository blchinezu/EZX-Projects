#!/bin/bash
if [ "$(id -u)" != "0" -a -z "$1" ]
then
  exec /sbin/start-stop-daemon -S -c root:root -x "$0" -- restart
  exit 1
fi

myfile=`basename $0`
mypath="/mmc/mmca1/.system/QTDownLoad/space/"
pausefile="/ram/var/run/pause"
startflag="/ram/var/run/spcstarted"
stopflag="/ram/var/run/spcSTOP"
cd $mypath

. /home/native/.profile 2>/dev/null 1>/dev/null

echo 'Info: delayedstart.sh begin'

while [ -f $pausefile ]; do
	time=`cat ${pausefile}`
	if [ "$time" -gt "0" ]; then
		echo "Info: delayedstart.sh time: $time"
		echo "Info: delayedstart.sh sleep 60s"
		sleep 60s
		time=`cat ${pausefile}`
		time=$(( ${time}-1 ))
		echo "Info: delayedstart.sh new time written: $time"
		echo "$time" > ${pausefile}
	else
		rm -f ${pausefile}
		echo "Info: delayedstart.sh pausefile delete (time: $time)"
	fi
	
	if [ -f $startflag ]; then
		rm -f $pausefile
		echo "Info: delayedstart.sh Startflag detected, exiting"
		exit
	fi
	
	if [ -f $stopflag ]; then
		rm -f $pausefile
		rm -f $startflag
		rm -f $stopflag
		echo "Info: delayedstart.sh Stopflag detected, exiting"
		exit
	fi
done

echo "Info: delayedstart.sh Successful delay, starting the monitoring process"

./space.sh -start &

exit
