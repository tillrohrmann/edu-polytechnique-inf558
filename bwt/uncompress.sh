#!/bin/bash
#
# first argument is the file which shall be uncompressed

./unahm $1 > unahmoutput
./unmtf unahmoutput > unmtfoutput
./unbwt unmtfoutput
