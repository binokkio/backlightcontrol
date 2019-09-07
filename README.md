# backlightcontrol

This is a minimal C codebase that compiles to an executable with which the brightness of laptop screens can be controlled on Linux.

## Installation guide

`make`

As root, adjust paths appropriately for the Linux distribution used:

`cp backlightcontrol /usr/bin`
`chmod u+s /usr/bin/backlightcontrol`

## Usage examples

`backlightcontrol set 50`
`backlightcontrol increment 10`
`backlightcontrol decrement 10`
