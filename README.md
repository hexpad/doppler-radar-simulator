# C++ Doppler Radar Sim

Hi there! This is a small project I built while I was teaching myself how Doppler radar works. 
I wanted to see if I could actually translate the physics of frequency shifts into a working C++ simulation.


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
