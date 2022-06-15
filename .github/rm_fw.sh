#!/bin/bash

rm -f images/*.*
git add .
git commit -m "Remove all the current firmwares"
git push origin master
