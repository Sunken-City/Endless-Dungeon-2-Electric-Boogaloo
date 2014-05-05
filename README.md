<pre>
 _____          _ _                ______                                      _____ _____ 
|  ___|        | | |               |  _  \                                    |_   _|_   _|
| |__ _ __   __| | | ___  ___ ___  | | | |_   _ _ __   __ _  ___  ___  _ __     | |   | |  
|  __| '_ \ / _` | |/ _ \/ __/ __| | | | | | | | '_ \ / _` |/ _ \/ _ \| '_ \    | |   | |  
| |__| | | | (_| | |  __/\__ \__ \ | |/ /| |_| | | | | (_| |  __/ (_) | | | |  _| |_ _| |_ 
\____/_| |_|\__,_|_|\___||___/___/ |___/  \__,_|_| |_|\__, |\___|\___/|_| |_|  \___/ \___/ 
                                                       __/ |                               
                                                      |___/                                
      __ _           _        _          ___                         _             
     /__\ | ___  ___| |_ _ __(_) ___    / __\ ___   ___   __ _  __ _| | ___   ___  
    /_\ | |/ _ \/ __| __| '__| |/ __|  /__\/// _ \ / _ \ / _` |/ _` | |/ _ \ / _ \ 
   //__ | |  __/ (__| |_| |  | | (__  / \/  \ (_) | (_) | (_| | (_| | | (_) | (_) |
   \__/ |_|\___|\___|\__|_|  |_|\___| \_____/\___/ \___/ \__, |\__,_|_|\___/ \___/ 
                                                         |___/                     


  .--.      .-'.      .--.      .--.      .--. 
:::::.\::::::::.\::::::::.\::::::::.\::::::::.\
'      `--'      `.-'      `--'      `--'      
   ______            __             __    
  / ____/___  ____  / /__________  / /____
 / /   / __ \/ __ \/ __/ ___/ __ \/ / ___/
/ /___/ /_/ / / / / /_/ /  / /_/ / (__  ) 
\____/\____/_/ /_/\__/_/   \____/_/____/  

  .--.      .-'.      .--.      .--.      .--. 
:::::.\::::::::.\::::::::.\::::::::.\::::::::.\
'      `--'      `.-'      `--'      `--'      
        </pre>                                

Movement-> WASD or Arrow Keys
Hover over anything to receive more information about it
Click on any items in your inventory to use them.

DEVELOPER CHEAT:
Press Q to go down a floor immediately. 
<pre>

  .--.      .-'.      .--.      .--.      .--. 
:::::.\::::::::.\::::::::.\::::::::.\::::::::.\
'      `--'      `.-'      `--'      `--'      
    ______           __                      
   / ____/__  ____ _/ /___  __________  _____
  / /_  / _ \/ __ `/ __/ / / / ___/ _ \/ ___/
 / __/ /  __/ /_/ / /_/ /_/ / /  /  __(__  ) 
/_/    \___/\__,_/\__/\__,_/_/   \___/____/  
                                             
  .--.      .--.      .--.      .--.      .--. 
:::::.\::::::::.\::::::::.\::::::::.\::::::::.\
'      `--'      `.-'      `--'      `--'      
</pre>
<+><+><+><+><+><+><+><+>

  Inventory Sorting

<+><+><+><+><+><+><+><+>

Whenever you pick up a new item, the inventory is automatically sorted by type.
The order is as follows:

Health Potions -> Weapons -> Shields -> Armor -> Scrolls

When you buy something from a shop, the inventory doesn't sort until you pick something else up.
This is to discourage you from selling the new item back on accident, and makes it easy to find and equip.
The shop has a simmilar behavior, and won't sort items that you sell into the mix either. (That would be too evil, even for the shop!)



<+><+><+><+><+><+><+><+><+>

 Random Weapon Generation

<+><+><+><+><+><+><+><+><+>

Instead of implementing difficulty in the standard way, I decided to go about creating weapons by having the program do it for me.
I start off by taking in a weapon type and the current difficulty for the floor the player is on. These two attributes are all I need
to create all of the items for a floor. 

The first thing I do is to calculate an offset of up to +/- 25 points of difficulty. This way, the
transition from one tier of items to the next is a little more gradual, and you'll end up with a floor or two with items of two different tiers.
I then use division to get a good number for the effect value of the item from the difficulty that is tweaked with for balance.
To pick the correct sprite/tile number for the image, I divide by 100 to get a small, 1-2 digit number from the difficulty. I then add this number
to the first sprite image on that row so that it will progress through the sprites over time. Before adding it, I min it with a seperate hex number
to make sure that once we hit the last item on the row, we won't go any further. Based on the type of the item, I grab a function for it from a
map of all the possible item behaviors/types and set the use function equal to that.

Once all of this has been done, I do a few more things depending on what type of item it is, and then I have a working pickup definition for the level!




<+><+><+><+><+><+><+><+><+>

 Random Monster Generation

<+><+><+><+><+><+><+><+><+>

Random Monster generation works almost in the same way as the weapon generation, but with a few exceptions. Because I couldn't simply go
straight across the sprite sheet, I had to create a set of Monster Tiers that described how each monster was going to "evolve" over the 
course of the game. There are currently 4 tiers, with room for plenty more. I had to hard code these in order to ensure that the monsters wouldn't look
random or out of place. For instance, you wouldn't expect a large dragon and a puppy to be fighting along side at the same difficulty, so I made the puppy
grow over time until it became a much more menacing wolf at the final tier. This was accomplished by creating 4 maps to hold each of the sprite locations for that
particular family number. In order to give each of these monsters a unique name, I made another map with names that held the name of each sprite using that sprite's
tile number as the key for the lookup. After about 3-5 levels, the monsters will begin to upgrade into their new forms. A few will disappear entirely (there were
a couple of "one-off" monsters that were unique and had no family), but the majority slowly grow more menacing over time. With this change, you can feel the monsters
slowly growing more powerful as the difficulty slowly ramps up.




<+><+><+><+><+><+><+><+>

        Scrolls

<+><+><+><+><+><+><+><+>

Instead of using a mana system, which didn't work as well with the procedural generation, I decided to instead implement a Scroll system.
Each scroll is a single-use item that would cast a random spell on you. Some of the effects were positive, such as healing the player for varying amounts,
whereas others were negative, such as breaking some of the player's currently equipped weapons. Others did more esoteric things, such as going down a floor instantly.
Although there were plenty of scrolls that I would have liked to have implement, I only got around to adding a few.
However, I wrote the code so that it would be easy to add lots more whenever I had more time.

These are all lambda functions implemented inside of the behavior map, after the other behaviors have been instantiated. In order to know how many functions I had
to pick from, I added an item to the enum called END_OF_LIST, which had an auto-assigned value to it. By doing this, I could use it as the "last index" of the enum,
and effectively the map, which allowed me to pick a random spell when I created a scroll. In order to facilitate this new use function, I had to rework saving a little bit.
I added a second behavior to each item, which became the identifier for what the item was going to do, while the first specified the actual type of the item.



<+><+><+><+><+><+><+><+>

     Terrain Themes

<+><+><+><+><+><+><+><+>

I made a simple way to change the aesthetics of a level early on using Terrain Themes. I created a little struct called a TileSet, and gave it a floor, wall, and a 
"cracked tile" for each level (although the cracked tile was sometimes the same tile as the floor for a smooth look). At the beginning of the game, I created a vector
and hardcoded the individual themes I wanted to show up on various floors. I put the themes together by hand instead of procedurally generating them in order to 
make sure each pairing of wall and floor would look good together. When I want to go to a new floor, I set the TileSet for that level by grabbing a random TileSet from
the vector. 



<+><+><+><+><+><+><+><+>

    Other Aesthetics

<+><+><+><+><+><+><+><+>

There were a lot of minor things that I ended up implementing for look and feel. Here's a list of a few:

-Health color:
	To make the health more prominent on the menu, I decided to change its color. To do so, I used some math to make the ratio of
	green and red for the text color proportional to the current health divided by the maximum health. When the player loses health,
	the text slowly changes from green to red, with the most prominent colors at the near death and full hp stages.

-Attack effects:
	When anything takes damage, I change its color to red for a number of frames equal to the damage just taken. The same goes for 
	whenever a player heals himself, except it utilizes a green color instead. This mechanic uses the same counter for both settings.
	If the player has taken damage, then the counter is positive and counts down. The counter is set to a negative number and counts up when
	healing. If the number is negative, the player is rendered with a green filter, red for positive, and no filter for a value of 0.
