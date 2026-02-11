# C++ Doppler Radar Sim

Hi there! This is a small project I built while I was teaching myself how Doppler radar works. 
I wanted to see if I could actually translate the physics of frequency shifts into a working C++ simulation.

<img width="1394" height="489" alt="doppler" src="https://github.com/user-attachments/assets/cc6c301b-0c4e-4f6b-8a54-bf6b1d890122" />


# Why I built this

I’ve always been curious about how police radars or weather stations measure speed. 
Instead of just reading the theory, I decided to code it. 
My goal was to see how velocity can be estimated from a frequency shift and, more importantly, how "noise" in the real world makes that job a lot harder.


# How It Works

1. Wavelength: It calculates the wavelength based on the frequency the user provides
   ```cpp
   wavelength = light speed/frequency
   ```
2. The Math: It uses the Doppler formula
    ```cpp
   doppler_shift = (2 * velocity) / wavelength
    ```
3. Adding Realism: I used a random Gaussian noise generator. 
   In a perfect world, the math is easy, but I wanted to see how a little bit of measurement error messes with the "Estimated Velocity."

4. Logging: It prints the results to the console and saves everything into a .csv file.


# Compile & Run

```cpp

#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>

constexpr double SPEED_OF_LIGHT = 3e8;

struct MovingTarget
{
    double position;
    double velocity;

    void move(double time)
    {
        position += velocity * time;
    }
};

class RadarSystem
{
private:
    double frequency;
    double wavelength;

public:
    RadarSystem(double freq)
        : frequency(freq)
    {

        wavelength = SPEED_OF_LIGHT / frequency;
    }

    double doppler(double targetV)
    {
        return (2.0 * targetV) / wavelength;
    }

    double estimate(double m_shift)
    {
        return (m_shift * wavelength) / 2.0;
    }
};

int main()
{

    double radar_frequency, distance, speed, noise, time_step;
    int total_steps;

    std::cout << "--- Radar Simulation Setup ---\n";

    std::cout << "Radar Frequency (GHz): ";
    std::cin >> radar_frequency;

    if (radar_frequency <= 0)
    {
        std::cerr << "Error: Frequency must be positive.\n";
        return 1;

    }

    std::cout << "Target Start Distance (m): ";
    std::cin >> distance;

    // Author : hexpad

    std::cout << "Target Speed (m/s): ";
    std::cin >> speed;

    std::cout << "Doppler Noise (Std Dev in Hz): ";
    std::cin >> noise;

    std::cout << "Time Step (dt in seconds): ";
    std::cin >> time_step;

    std::cout << "Simulation Steps: ";
    std::cin >> total_steps;


    RadarSystem radar(radar_frequency * 1.0e9);

    MovingTarget myTarget;
    myTarget.position = distance;
    myTarget.velocity = speed;


    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> noisedist(0.0, noise);

    std::string filename = "radar_output.csv";
    std::ofstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not create output file.\n";
        return 1;
    }


    file << "step,true_velocity,estimated_velocity,error_margin\n";

    std::cout << "\n--- Simulation Started ---\n";
    std::cout << std::fixed << std::setprecision(3);

    for (int step = 0; step < total_steps; ++step)
    {

        myTarget.move(time_step);

        double true_doppler = radar.doppler(myTarget.velocity);
        double measured_doppler = true_doppler + noisedist(gen);

        double est_speed = radar.estimate(measured_doppler);
        double error = est_speed - myTarget.velocity;

        file << step << ","
             << myTarget.velocity << ","
             << est_speed << ","
             << error << "\n";

        std::cout << "[Step " << step << "] "
                  << "True V: " << myTarget.velocity << " m/s | "
                  << "Est V: " << est_speed << " m/s | "
                  << "Err: " << error << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    file.close();
    std::cout << "\nSimulation complete. Data saved to " << filename << "\n";

    return 0;
}


```

# Example Input

When you run the code, it’ll ask you for some inputs in the terminal:

Radar frequency (GHz)

Target start distance (m)

Target speed (m/s)

Doppler noise (standard deviation in Hz)

Time step (seconds)

Number of simulation steps


After that, it runs the simulation and prints something like:

```cpp
[Step 0] True V: 30.000 m/s | Est V: 29.876 m/s | Err: -0.124
[Step 1] True V: 30.000 m/s | Est V: 30.052 m/s | Err: 0.052
```

# Output File

The program creates:
```cpp
radar_output.csv
```

You can open this file in Excel and analyze it.
