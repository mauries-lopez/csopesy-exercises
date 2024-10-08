#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <string>
#include <filesystem>
#include <atomic>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
std::atomic<bool> running(true);
std::atomic<int> finished_processes(0);
std::vector<std::string> global_logs;
namespace fs = std::filesystem;

std::string get_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S")
       << '.' << std::setfill('0') << std::setw(3) << now_ms.count();
    return ss.str();
}

class Process {
public:
    std::string name;
    int num_prints;
    std::vector<std::string> print_logs;

    Process(std::string n, int prints) : name(n), num_prints(prints) {}

    void execute(int core_id) {
        for (int i = 0; i < num_prints; ++i) {
            std::string timestamp = get_timestamp();
            std::stringstream log_entry;
            log_entry << timestamp << " | Core: " << core_id << " | " << name << ": Print " << (i + 1) << "\n";
            print_logs.push_back(log_entry.str());
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        std::lock_guard<std::mutex> lock(mtx);
        std::ofstream log_file("process_logs/" + name + ".txt");
        for (const auto& log : print_logs) {
            log_file << log;
        }

        global_logs.insert(global_logs.end(), print_logs.begin(), print_logs.end());
        finished_processes++;

        cv.notify_all();
    }
};

class FCFS_Scheduler {
public:
    std::queue<Process> queue;
    int num_cores;

    FCFS_Scheduler(int cores) : num_cores(cores) {}

    void add_process(const Process& process) {
        queue.push(process);
    }

    void run() {
        std::vector<std::thread> threads;
        int core_id = 0;

        while (!queue.empty()) {
            Process process = queue.front();
            queue.pop();
            threads.push_back(std::thread(&Process::execute, process, core_id));

            core_id = (core_id + 1) % num_cores;
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }
};

void screen_is_listener() {
    int ctr = 0;
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::lock_guard<std::mutex> lock(mtx);
        
        std::cout << "\nFinished Processes Logs:\n";
        for (const auto& log : global_logs) {
            std::cout << log;
            
            
        }

        if (finished_processes > 0) {
            std::cout << "Total Finished processes: " << finished_processes << "\n";
        }
        
        if (finished_processes == 10) {
            std::cout << "All processes finished.\n";
            break;
        }
        ctr++;
        if(ctr >= finished_processes){
                break;
            }
    }
}

void command_listener(FCFS_Scheduler& scheduler) {
    std::thread screen_is_thread;

    while (running) {
        std::string command;
        std::cin >> command;

        if (command == "print") {
            std::lock_guard<std::mutex> lock(mtx);
            std::ofstream consolidated_log("process_logs/consolidated_log.txt");
            for (const auto& log : global_logs) {
                consolidated_log << log;
            }
            std::cout << "Consolidated log has been created in 'process_logs/consolidated_log.txt'\n";
        } 
        else if (command == "screen-Is") {
            if (!screen_is_thread.joinable()) {
                screen_is_thread = std::thread(screen_is_listener);
            }
        } 
        else if (command == "exit") {
            running = false;
            std::cout << "Exiting the emulator...\n";
            cv.notify_all();
            break;
        }
    }

    if (screen_is_thread.joinable()) {
        screen_is_thread.join();
    }
}

void test_case() {
    int num_processes;
    std::cout << "Enter the number of processes to simulate: ";
    std::cin >> num_processes;

    FCFS_Scheduler scheduler(4);

    if (!fs::exists("process_logs")) {
        fs::create_directory("process_logs");
    }

    for (int i = 1; i <= num_processes; ++i) {
        std::string process_name = "process_" + std::to_string(i);
        Process process(process_name, 100);
        scheduler.add_process(process);
    }

    std::thread scheduler_thread(&FCFS_Scheduler::run, &scheduler);

    command_listener(scheduler);

    scheduler_thread.join();
}

int main() {
    test_case();
    return 0;
}
