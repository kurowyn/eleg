# eleg
Elementary implementation of grep in the C programming language (thus the name, *eleg*).

# Build
```
$ git clone https://github.com/kurowyn/eleg -b eleg-version-0.2
$ cd eleg
$ make -B eleg
```

# Usage Examples
```
$ ./eleg
$ usage: ./eleg (word) (FILE(s))
$ ./eleg main main.c
$ 1 main.c: main found.
$ ./eleg main main.c doesntexist
$ 1 main.c: main found.
$ 2 doesntexist: no such file.
$ ./eleg MAIN main.c doesntexist utils.c
$ 1 main.c: MAIN not found.
$ 2 doesntexist: no such file.
$ 3 utils.c: MAIN not found.
```
Note that in the examples above, *doesntexist* is assumed to be a file that doesn't actually exist.

# Recent Changes
- Fixed a bug where words are reported to not be in a file, even when they are.
- Fixed a bug where files are reported to not exist, even when they do.
- Fixed a bug where the availability of a word for each given file is shown in an arbitrary order (the order of the given file arguments is now followed).

# Future Plans
- Refactor code even more.
- Make a more user friendly interface.
- Show the line in which the word is found, if any.
- Write documentation.
