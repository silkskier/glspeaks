#ifndef FILTER_HPP
#define FILTER_HPP

#include <iostream>
#include <atomic>
#include <chrono>
#include <chrono>
#include <thread>
#include <iomanip>

std::atomic<float> progressValue(0);
std::atomic<int> timeLeft(0), filesComputed(0);

inline bool filter(float frequency, float power, float min_value, float filter_range, float amplitude, float min_amplitude, float max_amplitude){

    if (power < min_value) return false; //powers filter

    if (amplitude < min_amplitude || amplitude > max_amplitude) return false; //amplitudes filter

    float filter_frequencies[] = {1, 2, 3, 4, 5, 8};
    unsigned int num_frequencies = sizeof(filter_frequencies) / sizeof(filter_frequencies[0]);
    if (frequency == 0) return 0;
    for (unsigned int i = 0; i < num_frequencies; ++i)
    {if (frequency > filter_frequencies[i] - (filter_range / 2) && frequency < filter_frequencies[i] + (filter_range / 2)) return false;}//frequencies filter

    return true;
}

inline void printProgress(int max_progress, const std::chrono::steady_clock::time_point startTime) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (filesComputed.load() == max_progress) {
            return;
        }
        int currentFilesComputed = filesComputed.load();
        float progress = 100. * static_cast<float>(currentFilesComputed) / static_cast<float>(max_progress);
        std::chrono::duration<double> elapsedTime = std::chrono::steady_clock::now() - startTime;
        float localTimeLeft = static_cast<float>(elapsedTime.count()) * (float(max_progress) - float(currentFilesComputed)) / float(currentFilesComputed);
        progressValue.store(10 * progress);
        timeLeft.store(int(localTimeLeft));
        std::cout << std::fixed << std::setprecision(2) << "\r" << progress << "% complete, " << localTimeLeft << " seconds left." << std::flush;
    }
}

#endif

