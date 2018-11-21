#!/bin/sh
@echo "generating issueUpdate list for incorporation into MS3000Tool.py"
find ../../MS3000-WebInterface/ms3000-app/build/ -type file -exec echo issueUploadMS3000\(ser, \"{} {} \;
