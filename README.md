# qbdiff

*Note: I made this almost a decade ago, but apparently doing a `git commit --amend` and force-pushing over it changes the commit timestamp in the GitHub UI (and only the GitHub UI, it still renders as-was in a `git log`). Computers, amirite?*

**qbdiff** (**q**uick **b**inary **diff**) provides a quick overview of where the contents of two binary files diverge.

Usage:  ``qbdiff FILES``

Example output:

    $ qbdiff qbdiff.bin brailify.bin 
    'qbdiff.bin' is shorter than 'brailify.bin' by 161892 bytes.
    Files diverge at 0x00000014:
    0x00000010: 10 00 00 00 80 05 00 00 85 00 20 00 00 00 00 00 (qbdiff.bin)
    0x00000010: 10 00 00 00 70 06 00 00 85 00 21 00 00 00 00 00 (brailify.bin)
                            ^
