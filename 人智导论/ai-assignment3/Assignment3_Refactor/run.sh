g++ -std=c++11 -c *.cpp
mv *.o output/
g++ -std=c++11 -o output/main output/*.o
./output/main
