# Build folder
Here are the instructions for building the application.

## Dependencies
The application depends on these libraries being installed:
- When building from source code the following tools are needed:
  - ```autoconf```
  - ```automke```
- These libraries are always needed:
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
