# Source
https://github.com/cktan/tomlc99

# License
MIT

# Description
Used for generating and reading config files in an easy to use format.
It currently targets version 1 of the spec and the lib might need updating from time to time.

# Notes
It might be preferable to change to a submodule.
There is currently an error related to -Werror=char-subscripts, possible solutions are in https://stackoverflow.com/questions/9972359/warning-array-subscript-has-type-char but for the moment all instances of `isdigit` have been adjusted to have `(unsigned char)` in the first parameter.