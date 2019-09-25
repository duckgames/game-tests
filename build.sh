gcc main.cpp background.cpp following-background.cpp system.cpp world.cpp -o libs -I"./lib/lua" -L"./lib/lua" -llua -lm -ljsoncpp -lsfml-graphics -lsfml-window -lsfml-system -lstdc++
./libs