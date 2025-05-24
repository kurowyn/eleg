# eleg
Elementary implementation of grep in the C programming language (thus the name, *eleg*).

# Usage Examples
```
$ make -B eleg
$ ./eleg
$ usage: ./eleg (word) (FILE(s))
$ ./eleg main main.c
$ 1 main.c: main found.
$ ./eleg main main.c doesntexist
$ 1 main.c: main found.
$ 2 doesntexist: no such file.
```
Note that in the the example above, *doesntexist* is assumed to be a file that doesn't actually exist.

# Recent Changes
- Fixed a bug where words are reported to not be in a file, even when they are.
- Fixed a bug where files are reported to not exist, even when they do.
- Fixed a bug where the availability of a word for each given file is shown in an arbitrary order (the order of file arguments is now followed).

# Future Plans
- Refactor code even more.
- Make a more user friendly interface.
- Show the line in which the word is found, if any.
- Write documentation.
