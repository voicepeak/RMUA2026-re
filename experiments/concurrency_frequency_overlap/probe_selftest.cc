#include <string>
#include <thread>
#include <chrono>
namespace airsim_ros { __attribute__((noinline)) void record_frequency_data(std::string const& s) { std::this_thread::sleep_for(std::chrono::milliseconds(200)); asm volatile("" : : "r"(&s) : "memory"); } }
int main(){ std::thread a([]{airsim_ros::record_frequency_data("same");});std::thread b([]{airsim_ros::record_frequency_data("same");});a.join();b.join(); }
