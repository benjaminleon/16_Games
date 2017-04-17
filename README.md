# 16 Games from FamTrinli on YouTube on Linux

Videos found at his YouTube channel, here is an example:

https://www.youtube.com/watch?v=_4EuZI8Q8cs

This is a fork from https://yadi.sk/d/uqz1HerKrKWjV

## Link and compile
`sudo apt-get install libsfml-dev`

g++ -c filename.cpp -std=c++11

g++ object_file.o -o output_name -lsfml-graphics -lsfml-window -lsfml-system -std=c++11

Don't forget to run `chmod +x compile` in the terminal if you'd like to use the compile script