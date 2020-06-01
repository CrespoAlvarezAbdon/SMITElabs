# SMITElabs

**What is SMITElabs?**

SMITElabs is a near-1:1 recreation of SMITE's mechanics in Unreal Engine 4. I am planning to use the "game" as a way to showcase "God"-kit concepts to the game's community in a viewable format rather than reading a wall of text.

**So it's a game?**

Not quite, it's *heavily based* on a game, but it's not really a game in itself. Think of it as an extension of UE4 that can be used in a way similar to how Titan Forge Games (Hi-Rez Studios) creates new Gods for its flagship game SMITE.

**SMITE? Gods? What even is SMITE anyway?**

SMITE (or Smite) is a Third-Person MOBA (**M**ultiplayer **O**nline **B**attle **A**rena) created by Hi-Rez Studios which is playable on PC, Xbox One, PS4 and Nintendo Switch. It used to have Mac support but this was recently removed due to unforseen circumstances relating to the removal of all 32-bit code from the OS since the Catalina update. SMITE took heavy inspiration from Defense of the Ancients (DotA) and League of Legends, but unlike those Top-Down Mouse-Movement games it uses a Third Person perspective similar to games such as Gears of War and Fortnite. Graphically, the game is completely 3D, however the playspace is mostly 2D and there is very little movement on the Z-Axis (UE4's Up and Down movement uses the Z axis, most other software I'm aware of refers to this as the Y-Axis). The game is Free-to-Play and exclusively Multiplayer, with matches ranging from 1v1 to 5v5 depending on the gamemode. It is themed around characters from mythology and religion fighting each other; though the Player Characters are referred to as Gods (rather than Champions or Heroes in other Hero-based games), it doesn't necessarily they were considered a God in the real world. The game officially released in 2014, but has been around for a few years before then as a Public Pre-Release. It still receives updates every two weeks.

**What do you mean by kits?**

Each God (Player Character) in SMITE has 1 Passive Ability, 3 Standard Abilities and 1 Ultimate Ability. Alongside this there are many declared statistics such as Health, Mana and Movement Speed, as well as hidden statistics such as Prefire Progression, Hitbox Size and Basic Attack Size. Each God also has a Class assigned to them which assigns them a Damage Type (Physical or Magical) and a general playstyle. All of these things and more make up a God's kit.

**How can you make all of that? A big company works on SMITE and you're one guy, why bother trying?**

Most of the complex parts of the game come from the implementation of Multiplayer and the Replication that comes with it. The game itself isn't particularly complex, especially since there aren't many physics involved. 99% of the important stuff to copy is well-documented within and outside of the game so it's only a matter of getting that stuff into UE4 with C++. I don't have to worry about Graphics, Pretty UI, Replication, Controller Support (yet), Items (which will be implemented slightly differently here, but work roughly the same), Gamemodes, Marketing, Monetization, Balance or the complexities (and bugs) that come with having many unique Characters fight each other. It will play like SMITE, but it won't be SMITE, I'll leave that to the actual devs.

**Why are you making this?**

Because I like SMITE and want to do something cool and original with it. I know it might sound weird that creating a near-exact copy of something is cool and original, but it's not so much the "game" that's the interesting part but more what can be done with it and how it can be used to visualize a God's kit in a way that hasn't previously been possible.

If you go to  https://www.reddit.com/r/smite, you will see that the most popular posts there are "Skin Concept" posts. Skins are basically alternative costumes for Gods you can buy in the game, usually with real money. People enjoy Skin Concepts because a lot of effort goes into making them and very little effort goes into appreciating them, many Community Skin Concepts even make it into the game.

"God Concepts" are basically simplified overviews of kits attached to a Mythological or Religious Character which will usually include some lore about the God, the Class, the 5 main abilities and a few other tidbits of information and flavour. While often fun reads, they're not particularly popular because they don't take as much effort as Art does, they're often quite lengthy reads and they leave a lot to imagination since you can't actually see how a God works, just a description of what it does. I aim to take God Concepts and make them a bit less boring by giving people something to look at rather than just read about. I don't expect anything I make to actually get into the game since a whole lot of planning and effort goes into each God release that happens every other month, this is largely a creative exercise and a bit of fun.

**Where do you get your info for the game from?**

Most of the relevant info is explained in the game itself, but for the things that aren't such as algorithms and lesser details, I often refer to the Word of Thoth by Community Figure "Flareb00t". This is a compendium of information that often isn't mentioned within the game itself. Anything I can't find there I can find through my own testing within the game or from posts from Hi-Rez Officials such as TitanAjax. I also occasionally ask certain people for information if I cannot obtain it myself.

Flareb00t's Word of Thoth: https://www.scribd.com/doc/166529015/The-Word-of-Thoth-A-Smite-Mechanics-Guide

TitanAjax's Tumblr Post Regarding Basic Attacks and Hitboxes: http://forums.smitegame.com/showthread.php?172266-HiRezAjax-Do-gods-have-different-sizes-speeds-or-ranges-in-their-basic-attacks

**I heard that UE4 has Visual Scripting in the form of Blueprint, did you use any when making this?**

Currently no. I have no plans to in the future either. Everything is done with C++ except:

* Any main()-style stuff, such as loading a starting Map or choosing a starting Player Pawn, which is handled by the editor. This isn't something you really have the option to do in code with in UE4 as far as I'm aware.
* Attaching references of Meshes, Materials, Actors etc. to pointers in Objects will be done in the editor, so though I will often make Actor Blueprints that derive from a C++ Class, the only thing they're really doing is allowing for easy application of references and there is no actual Visual Scripting (pins and wires) involved. Some non-pointer variables are currently exposed to Blueprint to change the defaults in the editor, but this is only for the purpose of making "Dev Blog" videos easier and will be disabled later.
* UI stuff will be done using UMG rather than Slate, since that's generally the preffered way of dealing with UI. Slate is powerful but heavy and slow to create with, so I don't want to dwell on it too much. Meanwhile I prefer C++ over Blueprint since the Visual Scripting can get a bit messy when creating lots of functionality, while code is much more readable.

# Dev Blog Playlist Here: https://www.youtube.com/playlist?list=PL2mLxWY6M97V4N0Ytk5tC4h02C3jYHdMD
