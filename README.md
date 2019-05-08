# word-rank

Counts the numbers of appearances of words in a text.

Currently the text is read from a file contained in the
INPUT_FILE constant.

To turn on debugging use the *-D DEBUG_ON* option when compiling:
```gcc -D DEBUG_ON rank.c```

TODO:
* Move the list types and functions to another file
* Handle parameters
  * Make usage function
* Investigate and maybe user wchar_t
