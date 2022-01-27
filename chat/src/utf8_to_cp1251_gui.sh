#!/bin/bash

#
# Converting files from koi8-r to cp1251
#

 FILES="*.cpp *.h"

 for i in $FILES
 do
  echo "Converting $i from utf8 to cp1251"
  mv $i $i.icv
  iconv -f utf8 -t cp1251 $i.icv > $i
  rm -f $i.icv
 done
