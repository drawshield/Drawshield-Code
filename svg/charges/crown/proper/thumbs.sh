#!/bin/zsh

for file in *.svg; do
    magick -background white "$file" \
        -thumbnail 200x200 \
        -gravity center \
        -extent 200x200 \
        "${file:r}.jpg"
done
