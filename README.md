# glibc_trig_timing

**Purpose**: Test run time of trigonometric functions.

**Authors**:

*   Petr Cervenka - Filing Bug Report, initial code
*   Eric Cousineau - Modified code

# References

*   [Launchpad Bug #192134](https://bugs.launchpad.net/ubuntu/+source/glibc/+bug/192134) - Referenced by Johnathan van Why

# Instructions

Ensure that you have the appropriate build dependencies:

    sudo apt-get install build-essential

Next ensure that your user account has sudo privileges. Then run the following script:

    ./run.sh

This will build the script, run the tests, and output the results in a YAML-compatible format.
