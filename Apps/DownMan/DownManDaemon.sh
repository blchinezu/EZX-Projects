#!/bin/bash

# Make sure this is runned as root
if [ "$(id -u)" != "0" -a -z "$1" ]; then
  exec /sbin/start-stop-daemon -S -c root:root -x "$0" -- restart
  exit 1
fi

# Variables you can modify
debug="yes"
check_time=2  # seconds between 2 checks
running_flag="/tmp/DownManDaemon"
this=`basename $0`

debug() { if [ $debug = "yes" ]; then echo " $this@$$ > $@"; fi; }

# Check if daemon is already running
if [ -f "$running_flag" ]; then
	# If already running kill the previous instance
	pid=`cat "$running_flag"`
	debug "Killing another running instance ($pid)"
	if [ "`ps ax|grep -v grep|grep $pid`" != "" ]; then
		kill -9 $pid
		sleep 1
	fi
	rm -f "$running_flag"
fi

# Exit if the daemon was not killed
if [ -f "$running_flag" ]; then
	debug "Could not kill the other instance. Exiting."
	exit
fi

if [ "$1" = "--kill" ] || [ "$1" = "-k" ]; then
	umount /usr/SYSqtapp/opera/brwdaemon
	umount /usr/SYSqtapp/opera/opera
	debug "Done"; debug "Exiting"
	exit
fi

# Put running flag
printf "$$" > "$running_flag"

# Mount foo over the opera binary so that it won't be launched
debug "Mounting foo over the opera binary"
mount --bind "$running_flag" /usr/SYSqtapp/opera/opera

# Start loop
debug "Entering the daemon loop"
while [ 1 = 1 ]; do
	# Check if the opera daemon was launched
	proc=`ps axo "%a" | grep -v grep | grep -i brwdaemon`

	if [ "$proc" != "" ]; then
		# If yes then try stealing the link from it
		link=`printf "$proc" | tr '\n' '|' | awk -F'|' '{printf $NR}' | awk -F' -d ' '{printf $NF}'`
		# If there is a link pass it to the download manager
		if [ "$link" != "" ] && [ "$link" != "/usr/SYSqtapp/opera/brwdaemon" ]; then
			debug "Got link: $link"
		fi
		
		# Mount foo over the opera daemon so that it won't be auto-reloaded when killed
		mount --bind "$running_flag" /usr/SYSqtapp/opera/brwdaemon
		# Kill opera daemon
		kill -9 `pidof brwdaemon`
		sleep 0.7
		# Restore normal opera daemon binary
		umount "/usr/SYSqtapp/opera/brwdaemon"
	fi
	
	# Delay until the next check
	sleep $check_time
done

# After killing this you should do the following commands with an external script:
# umount "/usr/SYSqtapp/opera/opera"
# umount "/usr/SYSqtapp/opera/brwdaemon"
# Otherwise you won't be able to use native opera untill reboot

