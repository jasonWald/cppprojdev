#include <iostream>
#include <string>
#include <thread>

#include <condition_variable>
#include <fstream>
#include <vector>

#include <filesystem>




SearchFile sf;
std::condition_variable cv;
std::vector<fs::path> arr;



void do_division(const fs::path path, const u_int times) {
    u_int files_num = sf.get_num_of_files_recursively(path),
        part = files_num / times;
    u_int i = 0;
    arr.push_back(path);
    for (fs::recursive_directory_iterator it(path, fs::directory_options::skip_permission_denied);
        it != fs::recursive_directory_iterator(); ++it) {
        if (i == part) {
            arr.push_back(it->path());
            part += part;
        }
        i++;
    }
}

void do_job(const fs::path path, const std::string name) noexcept {
    for (auto& p : fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied)) {
        std::cout << std::this_thread::get_id() << " - " << p.path() << "\n";

        if (sf.find_file_in_directory(name, p.path())) {
            cv.notify_all();
            std::fstream f;
            std::string path = p.path().string();
            f.open(path);
            f << path;
            f.close();
            return;
        }
    }
}

int main() {
    std::string filename = "MyFile.txt";
    std::string path = "/";

    do_division("/", 8);

    while (1) {
        std::thread t1(do_job, arr[0], filename);
        t1.detach();

        std::thread t2(do_job, arr[1], filename);
        t2.detach();

        std::thread t3(do_job, arr[2], filename);
        t3.detach();

        std::thread t4(do_job, arr[3], filename);
        t4.detach();

        std::thread t5(do_job, arr[4], filename);
        t5.detach();

        std::thread t6(do_job, arr[5], filename);
        t6.detach();

        std::thread t7(do_job, arr[6], filename);
        t7.detach();

        std::thread t8(do_job, arr[7], filename);
        t8.detach();
    }
    return 0;
}