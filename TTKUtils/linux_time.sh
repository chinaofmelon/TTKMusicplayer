#!/bin/sh  
find . -exec touch -c -m -d "2024-06-18 12:00" {} \;
