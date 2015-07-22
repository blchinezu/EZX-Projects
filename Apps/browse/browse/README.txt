AUTHOR:
	BruceLee [www.motorolafans.com]


DETAILS:
	This is an application built from the filebrowser library present DownMan.
	It's main purpose is to show a window and output the selected item in terminal (stdout).


MODES:
	The application can run 3 modes:

	-f   Can select files/directoies
	-d   Can select only directories
	-s   A saving path window.

	
SYNTAX:
	browse [-f|-d|-s] <path>
	
	You can either use 2 arguments (EX: "browse -f /mmc/mmca1") or none (EX: "browse").
	Using only one argument is not possible (EX: "browse -f"  OR  "browse /mmc/mmca1")
	
	
DEFAULTS:
	If there is no argument it assumes "-f" with the root directory "/".
	