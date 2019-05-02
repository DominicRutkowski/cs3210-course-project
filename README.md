# cs3210-course-project
Thanks for taking the time to view my final project for CS:3210!
I have included a few notes about the project that are important
to keep in mind when running/grading it.

## Running the Project
* Compile the project with the following command:
`clang++ -std=c++17 -o main.out main.cpp Animal.cpp Environment.cpp Obstacle.cpp Organism.cpp Plant.cpp ViableUnit.cpp`
* Run the project with the following command:
`./main.out [path/to/map/file.txt] [path/to/species/file.txt]`

## Notes about the Project
* The animal movement algorithm is imperfect. Animals often make
decisions that don't seem smart to an onlooker. However, this is
(partially) by design. If animals always made ideal choices, the
simulation would be incredibly boring.
* Because of the randomness included in the animal movement
algorithm, writing tests for the code was basically impossible.
In lieu of formal tests (such as those using `catch.hpp`), I
have included more test inputs. Watching the extra inputs makes
it clear that the project is functioning properly. Note that the
extra tests are located in the `/input` folder. The test numbered
`0` is that which was provided at the beginning of the project,
but all other tests are my own.