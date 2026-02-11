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
