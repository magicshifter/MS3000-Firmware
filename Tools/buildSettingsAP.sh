#!/bin/bash
PREFAPBIN=./settings_ap.bin
PREFAP=MS3000-j
PREFAPPWD=makemuniccc

#!J! note - this does not pad the fields in the APConfig struct!
echo -n $PREFAP > $PREFAPBIN &&  echo -en "\000" >> $PREFAPBIN &&  echo -e $PREFAPPWD >> $PREFAPBIN && echo -en "\000" >> $PREFAPBIN
