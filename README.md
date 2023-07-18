```
NAME
       semverutil - Semantic version command line utility

SYNOPSIS
       semverutil [ -MNPR ] [+FORMAT]

DESCRIPTION
       semverutil  parses  semantic  version strings from STDIN and optionally
       increments the major, minor, patch or revision. If no options are spec-
       ified  then  semverutil will output the highest of the semantic version
       values given in STDIN.

       semverutil includes an extension to the semver standard that allows re-
       visions	-  an additional value to the immediate right-hand side of the
       core version, separated by an underscore.

   Options
       -M     Increment the major version

       -N     Increment the minor version

       -P     Increment the patch version

       -R     Increment the revision

   FORMAT
       This controls the output. Interpreted sequences are:

       %M     The MAJOR component

       %N     The MINOR component

       %P     The PATCH component

       %R     The REVISION component

       %L     The PRE-RELEASE component

       %D     the METADATA component
```
