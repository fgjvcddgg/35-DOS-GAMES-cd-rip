

  Robert J. Deutsch
  1152 N. 14th St.
  Allentown, PA              
  18102

                   

           *** WELCOME TO THE GREAT ULTIZURKIAN UNDERLAND ***                  
                 A World-Simulation RPG by Dr. Dungeon

                       
                      SYSTEM NOTES, PLAYER'S GUIDE
          For: The Great Ultizurkian Underland (Version 5.5)

       (Be sure to see the addendum at the end to read of the many
        new improvements in V5.5 over 5.0)        

                        
                        SYSTEM REQUIREMENTS:

  You will need at least a 386SX PC compatible machine with 16mhz speed,
MS-DOS 5.0, and 2 megabytes RAM. Recommended is a 486+ but it
is alright on a 386SX/16. Installing a drive CACHE is also recommended.

      ************ FINE TUNING FOR YOUR SYSTEM  *********************
  You can adjust several speed factors such as how fast monsters and
people walk when the Guildmaster is walking or standing still as well
as the speed at which torches flicker and clocks tick, etc.
  
  This feature has been added so you can fine-tune the speed to any
machine to insure maximum realism. None of these entries need to be
exact. You can experiment to optimize but here are some samples.

  When in the game, press SHIFT plus the _ underline key.
  You will see three numbers, like maybe 1100,2,4  You can enter
  new ones. Here are a few examples:

Good numbers for a 386SX/16 are:  1100,2,4       (Enter with commas)
Good numbers for a 486DX/33 are:  9400,12,10     (Enter with commas)
Good numbers for a 486SX/50 are:  2000,8,6       (Enter with commas)

  The first number is a ratio that controls how fast people and monsters
will move in RELATION to your character -- when he is walking as opposed to
standing still. The goal is to enter some number between about 1-9999
so that people walk at the same speed whether you are walking or standing
still.

   You can determine an accurate value by watching as NPC's
walk nearby. Walk a bit, stop, walk again. The NPC's should continue at
the same pace, whether you are walking or standing still. If not, try
a higher or lower value. Lower values speed them up. Higher values
slow them down.

   The second number controls the speed of moving tiles, like torches,
waterfalls, blinking things, etc.  Use the above reference to
modify it, probably a value between 2 and 12.

  The third number is for the various grandfather clocks you will find.
Again, experiment with values between 2-12. They will not affect the actual
time in the game, but merely adjust the realism of the tick and pendulum
for different machines. This number is usually close to the tile number.
Walk near a clock and watch it's tick. Try different numbers until it
goes at a realistic speed.

  The installed program will require about 2.5 megabytes of hard drive space.
It has been tested with everything except hard disk compression
utilities. Although it might work, we would recommend not to use a disk
compression utility. It will also help speed the game. You should also
exit WINDOWS because sometimes the game can conflict with Windows.
Run it from the DOS prompt, exited from windows.


           GREAT ULTIZURKIAN UNDERLAND -- The Story so far...

  After an original visit to the land of King Eldor, chronicled in:
ULTIZURK I: The Grandmaster's Quest, the great Grandmaster Adventurer
found himself on a strange world in the Arcturian star system,
in a bizarre contest with another Grandmaster.

  That tale was chronicled in ULTIZURK II: The Shadow Master.
  Our hero emerged victorious, after helping the residents of the planet
with their water problem. Now he was home again, promoted to Guildmaster,
the highest honor and level an adventurer can achieve.
  And so, you went to your quarters to rest, but found a mysterious old
tome upon the floor. You picked it up, read it, then feel asleep.
  You do not know if perhaps you yet dream, for you've found yourself in a
fantastic underground world of huge proportions.
  And there is a terrible problem here. But no one knows what it is!
Your vacation is over.

                          *** PLAYGUIDE ***

            ***MAKE SURE THE CAPS AND NUMLOCK KEYS ARE ON!***

  Just run the file ULTIZURK to begin the game!  You'll see other EXE's in
the list, but it'll crash if you don't run only ULTIZURK to play. You'll
need to be in the right directory, for example:
CD C:\AOL\DOWNLOAD\GUZU (ENTER) then type
ULTIZURK

  When you are done looking at the title screen, tap any key. You will be
taken to the main menu. When first running the game, you should select V for
view introduction. It tells about the Underland and how you got here. When
it is done playing, you will be back at the title screen. ESC again
returns you to the main menu. Now select S to start a new game. If
you type Q for quest onward by mistake, it could crash until you have a
SAVEd game.

  After the game loads, you will see a viewport where the Guildmaster has
arrived. Everything happens in this viewport. Underneath is his statistics
and a quick-list of commands. The stats line contains the following:

  LEV:1   HP:65   HM:65   EXP:0   MANA:15   AC:0   ATC:0   AMMO:0   GOLD:5

 These are abbreviations for: Level, Hit Points, Hitpoints Maximum,
Experience, Mana Points, Armor Class, Attack Class, Amount of Ammo,
and Amount of Gold.
  
  Level is an over-all indicator of how powerful you are. It also controls
how many hitpoints you have, as well as mana.
   
  Hit points are your current energy. It will go down in combat, but can be
restored by sleeping (if you have food in your pack), or by magical means.
You can also restore a bit by eating. In the jungle dungeon area, you can
eat as much as you want from the trees. You will also find nourishing
berries in the game.

  Hitmax is controlled by level. It shows the maximum hit points you can
have. This also can be modified by magical means, and will rise as your
level rises. You will discover early in the game how to move up levels.

  Experience is an over-all measure of how much you have accomplished.

  Mana is like hit points, but determines how much magical energy you
have for casting spells. It will go up when you rest to a maximum
determined by level, or can be adjusted with certain herbs you will find
in your quest.  It will always restore to full when you sleep at an Inn.

  Armor Class is a value from 0-15, with 15 the strongest. It determines
how much you will resist enemy blows, and is determined by what type of
helm, shield and armor you are wearing. Some people claim there is magical
armor, but most Arms Masters regard this as hogwash, since it supposedly
raises one's AC to 18.

  Attack Class determines how powerful your current weapon is, and gives an
indication of how hard you will hit an enemy. Many variables in combat can
modify this somewhat.

  Ammunition shows how many projectiles you have for a ranged weapon. These
are kept in a seperate quiver, and will not clutter your inventory pack.

  Gold is how many gold pieces you have in your purse, also seperate from
the main inventory pack.


                          *** COMMANDS ***

  The keyboard inputs are very intuitive and easy to use.

  Please remember to keep NUM and CAPS lock ON. At every step of
every command, a display line will appear at the bottom of the screen
listing available options. We firmly believe the player should have it easy
with inputting commands, and save his problem solving for the quest!

  The next game, Ultizurk III, has full mouse support. We feel,
however, that you will find our keyboard "hotkey" method in the current
game to be extremely intuitive and easy to use.

  Movement is controlled with the KEYPAD arrow keys. For now, you can move
in four directions. In COMBAT, however, you can ATTACK in all eight, using
keys 7,9,1 and 3 as well as 4,6,8 and 2.

  L-Look: When tapped, you'll see a red cursor. Move it with the same arrows
as for walking. The cursor will move all over the screen. Tap L again to get
a description of anything in the land.

  Remember to LOOK in open chests, and BLOODSPLATS if you kill a monster.
Many neat items are found in this manner!

  G-Get: When tapped, you'll be asked to enter a direction. Simply tap an
arrow keypad key. If you have room in your inventory, and are next to
the item, you'll pick it up and it will be in your inventory.
  This is also true of getting things from open chests, cabinets, and other
containers, as well as any holes you might dig with your shovel.

  T-Talk: This works just like get, but move next to a person beforehand.
You will enter talk mode and see a nice portrait of the person.

    Ask his NAME or JOB, and BYE to exit. Tapping ENTER with no
input will work the same as BYE.
    
  Many things will be seen in red highlight, and later in your quest
you'll find even more things that the people can talk about. Just type
what you see hilighted in RED, or any special topics you might learn
from others. NPC interaction is an important part of this game. For
example, all characters respond to NAME or JOB, but someone may say:

"Be sure to ask Hector about the GEM". (gem will be in red) - Later, if
you meet Hector, you could ask him about GEM even if it is not
highlighted in red. (Perhaps it's a secret clue or something!)

   U-Use: This is the do it all command. You can open chests, close them,
open and close doors, sit on certain chairs, throw switches on and off,
pick fruit from trees and bushes, get bones from a skeleton, dig holes,
read books and scrolls, and even climb up glaciers! (With the right tool
readied.) Using items on land asks you to then enter a direction.  In your
inventory, some items, like a watch or sextant, get used right away.

  But if an item in your inventory needs to act upon something on the land,
you will be prompted. We made it easy to do! Just tap the direction of
what you want to USE it on when asked.

  Items in hand, like a shovel, are used immediately, without needing
to open your pack. For example, if you have a SHOVEL armed in your hand
where you usually hold a weapon, you can USE the ground in front of you
to dig a hole, then LOOK inside! It's very realistic! The 'use' command
will always attempt to use what ever you have in hand. If nothing is in
your hand, it means bare hands. See inventory about putting things on.

   A-Attack: Weapon readied in hand, you approach a monster, or he
you. Hit A for Attack. You'll see a printout on the screen bottom as to what
you are attacking with. Just tap in a direction of which way you're
attacking, and whamo! Your Guildmasterly senses will also detect how many
hit points the monster has. Remember, you can attack in all eight directions,
and there are also some ranged weapons. (Just tap in the direction of the
distant monster.) And don't forget that these monsters fight back! If the
screen flashes red, it means you're going critical, so move away quick!
                               
   I-Inventory Control:
  When tapped, your inventory will open, graphically displaying your items.
A little pointer will appear, which looks much like a professor at a school
would use. Again using the arrow keypad, move it around. You will also see
a new list of options appear at the screen bottom. Move the rod over an
item to select it. The SELECTED item is what the following commands will
act upon. You'll see this list when in inventory:

      L-Look: Describes the item currently pointed to.

      U-Uses the item. (Watches, sextants, keys, read book, etc.)

      D-Drops the item. Get near any open chest. (Baldwin doesn't like
                        to litter!) You'll need to be east, west, north
                        or south of the chest to drop things, and the
                        chest must be empty. I guess Lord Baldwin could
                        only afford small chests! <G>

      P-Put on items -- When selected, you'll be asked Helm, Shield
                        Weapon, Armor. Just tap in the first letter.
                        If you already have something there being
                        worn, the items will automatically switch
                        places. Really! Remember, the item going ON
                        is the one over which you currently have the
                        cursor. Use this to 'arm' shovels, etc, too.
                        An item like a shovel will go in the weapon
                        hand so select 'W'.

      T-Take off armor -Like above, but will remove the item as long
                        as there's a blank spot in your backpack. Since
                        items switch automatically if something is
                        already worn, you don't really need this,
                        but it's nice to have anyway. After all, you
                        won't want to be wearing 100 pounds of armor
                        while visiting the girls of level six!
      ESC -Key         -Close your backpack.


                        *** OTHER COMMANDS ***
                               
   S-Savegame: Saves your current game progress.

   R-Restore : Restores a previous savegame.

  This will happen automatically whenever you begin play with
(Q)uest Onward from the menu screen when you begin a play session,
if you have a saved game. Please don't (Q)uest onward without first
saving a game, or it'll crash and you'll need to start your quest over!

   Y-Yell: You will learn many magic spells from Leomund. They are words
shouted in a deep, resonant voice, and will burn some mana. After you
tap Y for yell, just enter one of these words.

  For a list of spells and their words, USE the spellbook that Leomund
will give you. Talking with Leomund you'll learn everything you need
to know about spells. You will also find that the herbalist on Level One
knows much about herbal magic. Magic is endless in this game! Remember,
LOOK and USE are two different things. If you L for LOOK at a book, it
will simply say you see a book. To READ the book means to USE it.

   Q-Quit: When you are done adventuring, use this command to exit the game.
Be sure to save your current game first, as it does not automatically save.

                           *** ADDENDUM ***
NEW features in version 5.5:
-FULL save/load game feature. Fast!
-Detailed overhad map now available all the time!
-B for Blink command allows you to teleport to any level!
-Online help (this), when you type H.
-Improved prompts and highlighted commands for easier play.

  With all these NEW features, and a FULLY functional game, one may ask,
why register $15? Well, since version 5.0 was released, the sequel,
ULTIZURK III: The Guildmaster's Quest has been completed! It is a game
of EPIC proportions that rivals in quality commercial greats like
Ultima 6, Savage Empire, and Martian Dreams. You will see two snapshots
from ULTIZURK III whenever you quit Underland.

  The entire game "engine" has been completely re-designed in full 256
color VGA and mouse point and click interface. Party of characters,
beautiful protraits, and a scrolling system nearly SEVEN times faster
than Underland! The game uses a map of one MILLION tiles each rendered
in immpeccable detail. The realism is incredible. Literally dozens of
new features have been added, with an interface so easy and intuitive,
the readme.doc recommends to not bother to read it!

  It is Dr. Dungeon's Masterpiece. It is YOURS for a registration of
only $15 dollars! But that's not all! The Mach Gryphon game creation
pack, designed for UZ3, is so advanced that part 2 was erected very
quickly. Part 2 continues the quest. For a registration of only $30
dollars you will have BOTH:

ULTIZURK III Part 1: The Guildmaster's Quest and
ULTIZURK III Part 2: The Mobius Mind

  Each is a complete game unto itself.
That's $15 for Part 1 and $30 for both Part 1 and Part 2.

That's over FIFTEEN megabytes of spectacular adventure! You don't want
to miss it! Send your registrations to:

Robert J. Deutsch
1152 N. 14th St.
Allentown, PA
18102              If you have any questions regarding any of these games,
feel free to contact me at (610) 434-0723   Thank you, and enjoy your stay
in The Great Ultizurkian Underland.          -Dr. Dungeon

**************************************************************************
**************************************************************************
*************************REGISTER TODAY!**********************************
**************************************************************************
**************************************************************************
**************************************************************************

