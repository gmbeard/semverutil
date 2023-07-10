## 0.2.2
### PATCH Changes:
- Fixes an issue where the revision value was interpreted as a core version value if encountered early in the value string
- Adds an overflow check when parsing the numberical parts of a version string

## 0.2.1
### PATCH Changes:
- Fixes incorrect path in test wrapper shell scripts

## 0.2.0
### MINOR Changes:
- Improves the standard compliance of core version parser
- Adds a *revision* extension to the core version. This is indicated by an underscore (`_`) separated number, E.g `1.2.3_4`

## 0.1.0
### MINOR Changes:
- Initial release

