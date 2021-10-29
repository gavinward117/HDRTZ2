ps -aux | grep xwax
killall xwax inotifywait
cd api
npm start &
cd ../client
npm start &
cd ../Backend
./HDRTZ.out & 
./audio/xwax -t serato_2a -a default -q 0 -p audio/RomanceAnonimo.m4a &