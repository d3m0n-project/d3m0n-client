rm -rf client
clear

g++ *.cpp -o client -std=c++17 -lstdc++ -lstdc++fs


chmod 777 ./client
./client
