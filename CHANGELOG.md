## 0.5.0
### MINOR Changes:
- Adds a `+FORMAT` cmdline parameter that interprets `%`-prefixed sequences for more control of the output

### PATCH Changes:
- Fixes an issue where the metadata value was parsed as the pre-release value if the pre-release value was missing from the input [#4](https://github.com/gmbeard/semverutil/issues/4)

## 0.4.0
### MINOR Changes:
- Restructures the project to add a libsemverutil install target for devel output

### PATCH Changes:
- Fixes compilation issues on compilers with incomplete C++20 support ([#1](https://github.com/gmbeard/semverutil/issues/1))

## 0.3.0
### MINOR Changes:
- Fixes a parsing issue where additional revision values on the input were treated as valid
- Fixes and issue where incrementing the major version didn't reset the revision value

### PATCH Changes:
- Slight improvements to the `add-version-change` utility

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

