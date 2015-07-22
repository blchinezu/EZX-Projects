#!/bin/sh

echo "Creating '-off' bitmaps..."

for i in \
  title/one-player \
  title/two-players \
  title/two-player-vs \
  title/options \
  title/highscore \
  title/exit \
  options/normal \
  options/zen \
  options/hard \
  options/0percent \
  options/25percent \
  options/50percent \
  options/75percent \
  options/100percent \
  options/ok
do
  echo $i
  pngtopnm $i.png | ppmbrighten -value -50 | ppmquant 256 | ppmtopng > \
    $i-off.png
done

echo "All done - I'd pngcrush them if I were you..."
echo
