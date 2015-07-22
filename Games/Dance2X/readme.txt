[b]What is it[/b]
It was *meant* to be a [url=http://stepmania.com]Stepmania[/url]/DDR clone for the GP2X. It didn't quite make it so I am releasing what I have so far with the source so someone else can improve, or finish the project.

[bThanks to[/b]

- Vynx for the background graphics
- Rob Miles for supervising this project
- Paeryn for the HW SDL Libs
- The developers at Stepmania for letting me use their graphics set
- The QA team
	- Azure
	- Quiest
	- Phil
- The guys on the #gp2xdev channel

[b]Version 0.1[/b]
- No freeze arrow or BPM change support
- Hardcoded to one song and one difficulty level

[b]Controls[/b]
Both the face buttons and Joystick are in play for the directions in game.
Hold down Select in game to exit back to main menu.

B: Forward/Select in Menus


[b]What next[/b]
I unfortunately don't have any plans to continue this project until at least the Report (that I have to do) is done. Even then I will be planning to redo/redesign the backend of the 2D engine to increase the performence and possibly entering the Community Game Competition with a different game.

However the source is available (for non commercial use) and there hooks are there for loading other songs, it is just lacking a song browser game state at present. Bear in mind that my 2D backend is a chore to use (hence the redesign) but if you need any help, drop me a PM at http://gp32x.com.

[b]Extra Note to developers[/b]
If you have some time to kill, can you look through my code and criticise it to death? I am focusing where the bottlenecks are in the back-end causing the FPS to be so low (note that it is frame capped at present) so if you find something, I will be very grateful.

Finally, if I have missed anything in terms of the LGPL etc, let me know, I will rectify it as soon as I can.

