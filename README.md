```
SEMVERUTIL(1) General Commands Manual SEMVERUTIL(1)

NAME 
 semverutil - Semantic version command line utility

SYNOPSIS 
 semverutil [ -MNPR ]

DESCRIPTION 
 semverutil parses semantic version strings from STDIN and optionally increments the major, minor, patch,
 or revision. If no options are specified then semverutil will output the highest of the semantic version
 values given in STDIN.

 semverutil includes an extension to the semver standard that allows revisions - an additional value to the
 immediate right-hand side of the core version, separated by an underscore.

 Options
   -M
     Increment the major version
   -N
     Increment the minor version
   -P
     Increment the patch version
   -R
     Increment the revision
```
