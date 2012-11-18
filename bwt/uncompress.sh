#!/bin/bash

./unahm $1 > unahmoutput
./unmtf unahmoutput > unmtfoutput
./unbwt unmtfoutput