#!/bin/bash

url=http://test.drawshield.home
target=/include/drawshield.php?
resultfile=results.txt

cat /dev/null > $resultfile

if [ $# -eq 0 ]; then
  myargs='testcases/*.txt'
else
  myargs="$*"
fi
for i in $myargs; do
  name=${i##*/}
  name=${name%.txt}
  echo -n "Testing: " $name
  args=$(sed -e '/^[[:space:]]*#/d' -e '/^[[:space:]]*$/d' -e 's/ *= */=/' -e 's/.*/--data-urlencode "&"/' $i | paste '-sd ')
  eval curl --silent $args $url$target > "responses/$name.svg"
  if [ ! -f "expected/$name.svg" ]; then
    echo " comparison file missing"
  else
    result=$(diff -I "timestamp" -I "release-id" "expected/$name.svg" "responses/$name.svg")
    if [ -z "$result" ]; then
      echo " PASSED"
    else
      echo =========== $name ============= >> $resultfile
      echo $result >> $resultfile
      echo " FAILED"
    fi
  fi
  sleep 1
done
echo

