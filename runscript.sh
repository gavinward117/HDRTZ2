ps -aux | grep xwax
killall xwax inotifywait
ps -aux | grep getbluetooth
killall getbluetooth inotifywait
cd api
npm start &
cd ../client
npm start &
cd ../Backend
./getbluetooth &
./HDRTZ.out & 
./audio/xwax -t serato_2a -a default -q 0 -p audio/RomanceAnonimo.m4a &