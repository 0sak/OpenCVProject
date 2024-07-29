#include "ProcessImages.h"
#include "Metric.h"

int main(int argc, char** argv)
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);
    ProcessImages pc;

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();


    pc.initLabels();
    pc.execute();

    auto t2 = high_resolution_clock::now();

    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "Time of execution in int " << ms_int.count() << " ms\n";
    std::cout << "Time of execution in double " << ms_double.count() << " ms\n";
    return 0;
}