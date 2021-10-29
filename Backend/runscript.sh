ps -aux | grep xwax
killall xwax inotifywait
./HDRTZ.out & 
./audio/xwax -t serato_2a -a default -q 0 -p audio/RomanceAnonimo.m4a &