# appel-gmod-x64
An x64 C++ Garry's Mod hack base with a unique UI.

<img src="https://github.com/AppleVegas/appel-gmod-x64/blob/ef17f7dcc9d88ca9767db8b5ba7f88e77a295c21/img/1.png" width="30%">

## Why?
- I hate most ImGui UIs
- I personally love how mod menus for **Grand Theft Auto V** generally look like, I find them *very* usable in terms of user experience. This is my desired UI for a game hack.

#### That's why I decided to port it to Garry's Mod, bringing a familiar menu UI from one my favourite game to another.

I wrote this code back in the middle of 2021, when I was still coding for gmod every now and then, but I stopped after I started attending university.

## Features
<details>
<summary>Custom fonts in UI embedded within the binary</summary>

 ![image](https://github.com/AppleVegas/appel-gmod-x64/blob/ef17f7dcc9d88ca9767db8b5ba7f88e77a295c21/img/2.png)
</details>
<details>
<summary>Text color tags</summary>
 
You can color labels in menu by using tags:
```
~r - Red
~g - Green
~b - Blue
~q - Pink
~o - Orange
~y - Yellow
~c - Grey
~p - Purple
~w - White
```
</details>
<details>
<summary>ImGui Lua Editor</summary>
  
![image](https://github.com/AppleVegas/appel-gmod-x64/blob/ef17f7dcc9d88ca9767db8b5ba7f88e77a295c21/img/7.png)
 
*Doesn't execute lua because of a missing interface. Leftover from my previous gmod hack for 32-bit version of the game, can still work though after the interface is implemented.*
</details>
<details>
<summary>Themes support</summary>
  
![image](https://github.com/AppleVegas/appel-gmod-x64/blob/ef17f7dcc9d88ca9767db8b5ba7f88e77a295c21/img/4.png)

![image](https://github.com/AppleVegas/appel-gmod-x64/blob/ef17f7dcc9d88ca9767db8b5ba7f88e77a295c21/img/5.png)

![image](https://github.com/AppleVegas/appel-gmod-x64/blob/ef17f7dcc9d88ca9767db8b5ba7f88e77a295c21/img/6.png)
 
</details>
  
## Compatibility
This base was updated to work with current build of **Garry's Mod** (as of November 2024). However, since I wrote it in 2021, some of the interfaces have to be updated. Necessary ones for the base are working fine though.

And, of course, this base was originally meant for **Garry's Mod**, but nothing stops you from adapting it to any other game. 

You will have to remove any references to engine functions, and rewrite them in some other way, since they are required in order to determine screen resolution and game activity.

## Compiling
All the necessary dependencies already are included in the project. 

This project had been updated in November of 2024 by me to compile under MS Visual Studio 2019.

## Projects used
*My old 32-bit gmod hack which is based on [FreeTheSkids](https://github.com/11Lee1/Free-The-Skids)*

*Lua editor powered by [ImGuiColorTextEdit](https://github.com/BalazsJako/ImGuiColorTextEdit)*

*UIManager stripped from [BigBase](https://gitlab.com/pocakking/bigbase)*

  
