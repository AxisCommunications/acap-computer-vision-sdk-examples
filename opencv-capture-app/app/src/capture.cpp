// UniMatrix zero-copy example
#include <time.h>

#include <iomanip>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;

static inline uint64_t
monotonic_now()
{
    struct timespec ts = {0};

    if (clock_gettime(CLOCK_MONOTONIC, &ts) < 0)
        throw std::runtime_error("Clock Monotonic failed");

    return uint64_t(ts.tv_sec) * 1000000u + (ts.tv_nsec / 1000u);
}

int main(void) try
{
    // Number of outstanding zero-copy buffers (This is a very precious resource)
    constexpr size_t BUFFERS = 2;
    constexpr size_t NFRAMES = 30;
    Mat frame[BUFFERS];

    // Start a stream from 'CAP_PROP_CHANNEL: 1'
    VideoCapture cap(1);
    cap.set(CAP_PROP_FPS, 30);
    cap.set(CAP_PROP_FRAME_WIDTH,  640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 360);

    // The most efficient color format is always default.
    // Only the default format is guaranteed to have zero-copy.
    // It's mandatory to support at least one of the formats below.
    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('N','V','1','2'));
    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('N','V','2','1'));
    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('R','G','B','3'));

    // Monochrome is mandatory
    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('Y','8','0','0'));

    // Request one extra in-flight buffer to reach target FPS
    cap.set(CAP_PROP_UNIMATRIX_MAX_BUFFERS, BUFFERS + 1);

    // Validate all properties by trying to grab one frame
    try { cap.grab(); }
    catch(const std::exception& e)
    {
        cerr << "Failed to open stream: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    if (BUFFERS > cap.get(CAP_PROP_UNIMATRIX_MAX_BUFFERS)) {
        cerr << "Insufficient UniMatrix Buffers!" << endl;
        exit(EXIT_FAILURE);
    }

    uint64_t start = monotonic_now();

    // Fetch frames
    for (size_t i = 0; i < NFRAMES; ++i)
    {
        Mat& mat = frame[i%BUFFERS];

        // Blocking read
        cap.read(mat);

        // Make sure capture succeeded
        if (mat.empty())
            throw std::runtime_error("Failed to fetch frame");

        // Fetch UniMatrix zero-copy properties
        int    fd   = cap.get(CAP_PROP_UNIMATRIX_FD);
        off_t  offs = cap.get(CAP_PROP_UNIMATRIX_FD_OFFSET);
        size_t size = cap.get(CAP_PROP_UNIMATRIX_FD_CAPACITY);

        uint32_t seqnum = cap.get(CAP_PROP_POS_FRAMES);

        // Success!
        cout << "FRAME["   << std::setw(2) << seqnum << "] "
             << mat.cols << "x" << mat.rows << "\t"
             << "fd["      << fd   << "]\t"
             << std::hex
             << "offs[0x"  << offs << "]\t"
             << "size[0x"  << size << "]\t"
             << std::dec
             << "\n";
    }

    uint64_t delta = monotonic_now() - start;
    cout << std::fixed;
    cout << "Captured " << NFRAMES << " frames in " << delta << "µs\n";

    start = monotonic_now();

    // Read misc camera properties
    constexpr size_t ITER = 3u;
    for (size_t i = 0; i < ITER; ++i)
    {
        double gain    = cap.get(CAP_PROP_GAIN);
        double expo    = cap.get(CAP_PROP_EXPOSURE);
        double zoom    = cap.get(CAP_PROP_ZOOM);
        double focus   = cap.get(CAP_PROP_FOCUS);
        double fnumber = cap.get(CAP_PROP_UNIMATRIX_FNUMBER);

        // Success!
        cout << "iteration["  << std::setw(1) << i << "] "
             << "gain: "      << gain         <<      "   "
             << "expo: "      << expo         <<      "   "
             << "zoom: "      << zoom         <<      "   "
             << "focus: "     << focus        <<      "   "
             << "f-number: "  << fnumber      <<      "\n";
    }

    delta = monotonic_now() - start;
    cout << "Queried settings " << ITER << " times in " << delta << "µs\n";

    return 0;
}
catch(const std::exception& e)
{
    cerr << e.what() << endl;
}
