<p align="center">
  <img alt="Scrabble 1D - ncurses" src="https://user-images.githubusercontent.com/48584741/132108460-fd350d2d-3701-4cd6-adbe-f6ed216168a5.png">
</p>

## About the game
This is a one-dimentional scrabble-like console game written in **C**. The idea is:

> You get a couple of randomly generated letters and you have to make a word with them.

The word is checked in a **dictionary** (a file with all the valid words).

You can add your own **words** and customise the number of **random letters** and **rounds** in a game!

## How to install
**Note**: The game is **only** supported on **Linux**.

### 1. Install from AUR
If you're using an Arch based Linux distribution you can get the game from the **Arch User Repository** [here](https://aur.archlinux.org/packages/scrabble-1d/)!

Manually:
```
git clone https://aur.archlinux.org/scrabble-1d.git
cd scrabble-1d
makepkg -si
```

Using a helper:
```
yay -S scrabble-1d
```

### 2. Normal install

Make sure you have these libraries installed:
- ```glib-2.0```
- ```json-glib```
- ```ncurses``` + ```menu``` & ```form``` libraries

After that there are two ways to install the game:

### Build from tarball (Recommended)
Download and extract the [latest release](https://github.com/YassenEfremov/scrabble-1d/releases) archive. Then in the main directory do:
```
./configure
make
sudo make install
```

#### Build from source code
For this you will need to have ```pkg-config``` and [autotools](https://wiki.debian.org/AutoTools) (```autoconf``` and ```automake```) installed.
 
Download the source code from Github and in the main directory do:
```
./autogen.sh
./configure
make
sudo make install
```

## History
This game was created as an end of the year school project for a C programming class.

Me and my classmates developed this game as a team (You can find their profiles in the contributors tab). We first created the base game without any fancy interface and that's what we presented to our teacher. After that I continued to develop the game alone. I added autotools, ncurses UI and lots of other things in order to make this project as professional as possible.

Here's how we split up the work initially (kind of):
- Game logic - _Danail Bozhkov_ (good job man)
- Generating and fetching a _trie_ structure - Yassen Efremov (i also kinda managed the whole project)
- Other _trie_ structure operations - _Lachezar Velinov_ (helped me and i helped him, my guy)
- Validating a word in the _trie_ structure - _Lachezar Lazarov_ (you know what you did...)

Nevertheless I would like to thank them all for participating in this project with me. The whole development process was a major learning experience and I'm very glad it turned out the way it did.
