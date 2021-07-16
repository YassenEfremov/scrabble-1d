# Build folder
Here are the instructions for building the application.

You need to have installed:
- Tools for building:
  - ```autoconf```
  - ```automke```
- Libraries:
  - ```ncurses```
  - ```menu```
  - ```form```
  - ```libconfig```


## Build from tarball
After downloading and extracting the latest release archive do:
```
cd scrabble/build/
../configure
make
```
To run the game: **From the build folder** do:
```
./src/scrabble
```

## Build from source code
After downloading the source code from Github do:
```
cd scrabble/
./autogen.sh
cd build/
../configure
make
```
To run the game: **From the build folder** do:
```
./src/scrabble
```
