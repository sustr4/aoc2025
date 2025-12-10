make
rm *.sm2
./day10
for f in *.sm2; do z3 $f; done | grep '((' | grep -o '[0-9][0-9]*' | awk '{print "Minimal value = " $1; sum+=$1} END {print sum}'
