#!/bin/bash

 # making files unexecutable
 echo "Making files no executable"
 chmod -x *.cpp *.h

 FILES="*.cpp *.h"

 # dos2unix
 for i in $FILES
 do
  dos2unix $i
 done

 # converting files
 FILES="*.cpp *.h"
 for i in $FILES
 do
  echo "Converting $i from cp1251 to utf8"
  mv $i $i.icv
  iconv -f cp1251 -t utf8 $i.icv > $i
  rm -f $i.icv
 done
