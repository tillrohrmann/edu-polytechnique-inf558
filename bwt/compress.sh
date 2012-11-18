#!/bin/bash

./bwt $1 $2 > bwtoutput
./mtf bwtoutput > mtfoutput
./ahm mtfoutput
