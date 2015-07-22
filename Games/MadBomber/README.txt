README for Mad Bomber

by Bill Kendrick
bill@newbreedsoftware.com
http://www.newbreedsoftware.com/madbomber/

Version 0.2.5

March 16, 2003


DESCRIPTION
-----------
  "Mad Bomber" is a clone of Activision's classic Atari 2600 console
  game, "Kaboom!," by Larry Kaplan, with spruced-up graphics and
  sound effects, and music.


STORY
-----
  The Mad Bomber is loose in the city and he's dropping bombs everywhere!
  It's your job to catch them before they hit the ground and explode.
  Luckily, you have a set of trusty buckets to extinguish the bombs with.


DOCUMENTATION
-------------
  Important documentation for "Mad Bomber" is contained in multiple files.
  Please see them:

    AUTHORS.txt    - Credits for who made this game.
    CHANGES.txt    - Changes since the previous versions of "Mad Bomber."
    COPYING.txt    - The GNU Public License, which "Mad Bomber" is under.
    INSTALL.txt    - Instructions on requirements, compiling and installing.
    README.txt     - (This file.)  Game story, usage, rules and controls.
    TODO.txt       - A wish-list for this version of "Mad Bomber."


RUNNING THE GAME
----------------
  LINUX
  -----
    Just run the program "madbomber".

  WIN32
  -----
    Go into the directory (folder) containing "madbomber.exe" and the
    "data" directory, and run the "madbomber.exe" executable.
    (You can do this by double-clicking its icon.)

  ZAURUS
  ------
    Install the IPK release of Mad Bomber.  Click the "Mad Bomber" icon
    in the 'Games' tab of the Launcher.

  OPTIONS
  -------
    The program accepts some options:

      --disable-sound     - If sound support was compiled in, this will
                            disable it for this session of the game.

      --fullscreen        - Run in fullscreen mode.
                            (Note: Under Linux, if this doesn't work, 
                            you will need to run the game as root.)

      --help              - Display a help message summarizing command-line
                            options, copyright, and game controls.

      --usage             - Display a brief message summarizing command-line
                            options.

  A screen will appear stating that the game is loading.  A meter shows
  the progress as it loads.


TITLE SCREEN
------------
  At the title screen, you can choose between the three game types
  that are available ("ONE PLAYER", "TWO PLAYERS", and "TWO PLAYER VS."),
  get to the OPTIONS screen, or EXIT the program.

  Use the [ UP ARROW ] and [ DOWN ARROW ] keys to choose an option, then
  press [ SPACE ] or [ ENTER / RETURN ].

  You can also use the mouse pointer, if available, to click the item
  you wish to choose.


GAME VARIATIONS
---------------
  ONE PLAYER

    The Mad Bomber drops bombs, and you catch them.


  TWO PLAYERS

    Two people take turns catching bombs that the Mad Bomber is dropping.


  TWO PLAYER VS.

    One player plays as the Mad Bomber, and the other catches the bombs.


OPTIONS
-------
  Use the [ UP ARROW ] and [ DOWN ARROW ] to choose a setting in the options
  menu, then press [ SPACE ], [ ENTER / RETURN ], [ LEFT ARROW ] or
  [ RIGHT ARROW ] to toggle options which can be toggled.

  DETAIL

    The default mode, "NORMAL," provides a game background with a
    photo-realistic background.  The "ZEN" option provides a plain,
    Kaboom!-style background.

  DIFFICULTY  (Player One and Player Two)

    The default difficulty, "NORMAL," gives the player a wide bucket.
    The "HARD" option gives the player a narrow bucket.

  VOLUME  (Effects and Music)

    Choose between 9 sound settings for both sound effects and music.
    The lowest setting is "off," the highest is maximum volume.

    (Note: Use the [ LEFT ARROW ] and [ RIGHT ARROW ] keys to change the
    sound settings.  Since they have more than two settings, the
    [ SPACE ] and [ ENTER / RETURN ] keys do not affect these settings.)

    (Note: If sound support was not compiled in, or you disabled it with
    the "--disable-sound" flag, the volume controls will not be available.)

  OK

    Accept these changes and return to the main title screen menu.
    (The [ ESCAPE ] key also does this.)

  You can also use the mouse pointer, if available, to click the option
  you wish to change/select.


GAME CONTROLS
-------------
  The following controls are available:

  [ LEFT ARROW KEY ]  -- Move the bucket left.

  [ RIGHT ARROW KEY ] -- Move the bucket right.

  [ MOUSE MOTION ]    -- Move the bucket (follows mouse pointer).

  [ SPACE ]           -- Fire.
  [ ENTER / RETURN ]     (Cause the Mad Bomber to drop bombs.)
  [ MOUSE CLICK ]

  [ TAB ]             -- Pause / unpause.
  [ P ]

  [ ESCAPE ]          -- Quit the game, return to the title menu.

  [ 1 ]               -- Move Mad Bomber left, quickly.

  [ 2 ]               -- Move Mad Bomber left, slowly.

  [ 3 ]               -- Move Mad Bomber right, slowly.

  [ 4 ]               -- Move Mad Bomber right, quickly.


PLAYING THE GAME
----------------
  When you begin the game, the Mad Bomber is sitting at the top of
  the screen, and you have three buckets at the bottom.

  Press Fire ([ SPACE ], [ ENTER / RETURN ] or [ MOUSE CLICK ]) and
  the Mad Bomber begins moving left and right, dropping bombs.
  (Note, in "TWO PLAYER VS." mode, the second player must use the
  [ 1 ], [ 2 ], [ 3 ], and [ 4 ] keys to move the Mad Bomber.
  Player one still causes the bombs to start dropping.)

  Move your buckets left and right to position them under the bombs
  and catch them.

  When the Mad Bomber is done dropping bombs for this group (see
  "SCORING AND BOMB GROUPS", below), he stops.  Once you've caught
  all of the bombs in the group, press Fire again to begin the next,
  faster group...

  If a bomb hits the bottom of the screen, all of the bombs explode
  and you lose one of your three buckets.  You are sent back to the
  previous bomb group, but only one half of the bombs are dropped.


SCORING AND BOMB GROUPS
-----------------------
  As each level progresses, more bombs are dropped, and they are dropped
  more quickly.  You also get more points per bomb.

  Group      Score per Bomb       Bombs Dropped
  ---------------------------------------------
    1              1                  10
    2              2                  20
    3              3                  30
    4              4                  40
    5              5                  50
    6              6                  75
    7              7                  100
    8              8                  150

  Each time you get 1000 points, you receive a bonus bucket.  (If you
  already have three buckets, nothing changes.)  You will hear a sound
  indicating that you've passed a 1000-point mark.

  After you beat the 8th bomb group, you remain in the 8th bomb group
  until you let a bomb explode and are sent back one level.
  (The first time you beat the 8th bomb group, a congradulational sound
  is played indicating that you've gotten so far.)

  Note: When you enter a complete a bomb group and move on to the next
  level, the new level's number appears on the screen, flying towards you.
  If you drop a bomb and are sent back to the previous group, that level's
  number appears on the screen, flying away from you.


HIGH SCORE
----------
  If you beat the current high score, the words "HIGH SCORE" will appear
  next to your score, and you will hear a tune.  Note: In Two Players mode,
  you can lose the high score to your opponent if they surpass your score
  during their turn!


10,000 POINTS
-------------
  If get past 10,000 points, you'll amaze the Mad Bomber.


SETTINGS FILE
-------------
  When you first run "Mad Bomber," you have no settings file.  Defaults
  are chosen (this includes default music volumes, default difficulty
  settings, the high score, etc.).

  LINUX
  -----
    When you quit the program, it attempts to save your settings to a
    ".madbomber" file in your $(HOME) directory.  (If $(HOME) is undefined,
    it saves it in the current directory.)

  WINDOWS
  -------
    When you quit the program, it attempts to save your settings to a
    "madbomber.dat" file in the current directory.

  The next time you load the game, it will try to load your settings
  file.  (If it can't, it will use the defaults.)  This means whatever
  settings you had when you quit the program last time, will be active
  the next time you start the game up!

  Normally you would not need to edit this file, but you can.  When it
  is saved (when you quit the program), descriptive comments are saved
  within the file explaining what each line in the settings file is
  for and what values are accepted.


IF YOU LIKE THIS GAME
---------------------
  If you like Mad Bomber, please feel free to give your friends copies
  of the original .tar.gz or .zip file!  Or just point them to the
  Mad Bomber web page:

    http://www.newbreedsoftware.com/madbomber/

  If you want, you can send a small donation, a post card, or just a
  simple note saying "thanks" to:

    New Breed Software
    c/o Bill Kendrick
    675 Alvarado Ave #27
    Davis, CA 95616-0620     (current as of April 2002)

  Or e-mail us to say "hi!"

    madbomber@newbreedsoftware.com


THANKS!
