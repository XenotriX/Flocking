# Flocking simulation

This is a simulation of the flocking behaviour of birds.
This simulation follows the 3 simple rules Craig Reynolds defined in his [paper](http://www.cs.toronto.edu/~dt/siggraph97-course/cwr87/) published in 1986.

**Separation** The birds, also called boids, steer to avoid local flockmates

**Alignment** The boids steer towards the local average heading of local flockmates

**Cohesion** The boids steer to move towards the average position of local flockmates

## Getting Started

1. Clone this repository
2. Install SFML
3. Create a `build` directory
4. Inside your `build` directory run `cmake ..`
5. Once the CMake configuration is done, compile with `make`

> The CMake file does neither check for nor include the SFML library so you might have to include it yourself

## License

This Project is licensed under the MIT License. For more information see the [License](https://github.com/xenotrix1337/blob/master/LICENSE) file.
