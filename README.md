# eleg
Elementary implementation of grep in the C programming language.

# Usage
```
make -B
./eleg
```

# Recent Changes
- Fixed a bug where words are reported to not be in a file, even when they are.
- Fixed a bug where files are reported to not exist, even when they do.
- Fixed a bug where the availability of a word for each given file is shown in an arbitrary order (the order of file arguments is now followed).

# Future Plans
- Make a more user friendly interface.
- Show the line in which the word is found, if any.
- Write documentation.
