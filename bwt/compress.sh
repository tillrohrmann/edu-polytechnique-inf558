#!/bin/bash
#
# first parameter is the file to compress.
# second parameter is the block size in bytes for the burrows wheeler transformation

./bwt $1 $2 > bwtoutput
./mtf bwtoutput > mtfoutput
./ahm mtfoutput

rm bwtoutput
rm mtfoutput
