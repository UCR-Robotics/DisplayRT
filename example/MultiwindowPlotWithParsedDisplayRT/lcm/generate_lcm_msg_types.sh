#!/bin/bash

# generate all message types in the current folder,
# and store all headers to /msg
lcm-gen -x ./types/*.lcm --cpp-hpath .