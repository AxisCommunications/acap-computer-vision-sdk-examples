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

    // Choose Rotation before grabbing the first frame.
    // This is a potentially slow operation any active streams will be closed!
    try
    {
        cout << "Setting Rotation 270: " << flush;
        cap.set(CAP_PROP_UNIMATRIX_ROTATION, 270);
        cout << "Done" << endl;

        cout << "Setting Rotation 180: " << flush;
        cap.set(CAP_PROP_UNIMATRIX_ROTATION, 180);
        cout << "Done" << endl;

        cout << "Setting Rotation  90: " << flush;
        cap.set(CAP_PROP_UNIMATRIX_ROTATION, 90);
        cout << "Done" << endl;

        cout << "Setting Rotation   0: " << flush;
        cap.set(CAP_PROP_UNIMATRIX_ROTATION, 0);
        cout << "Done" << endl;
    }
    catch(const std::exception& e)
    {
        cerr << "Failed setting Rotation: " << e.what() << endl;
    }

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
    uint64_t delta = 0;

    cap.set(CAP_PROP_UNIMATRIX_TONEMAPPING, 50);
    cap.set(CAP_PROP_UNIMATRIX_TEMPORAL_FILTER, 50);
    cap.set(CAP_PROP_UNIMATRIX_EXPOSURE_MODE, CAP_UNIMATRIX_EXPOSURE_MODE_HOLD);
    cap.set(CAP_PROP_UNIMATRIX_EXPOSURE_MODE, CAP_UNIMATRIX_EXPOSURE_MODE_AUTO);
    cap.set(CAP_PROP_UNIMATRIX_MAX_EXPOSURE_us, 10000); // 10000µs = 1/100s

    delta = monotonic_now() - start;
    cout << "Configured image parameters in " << delta << "µs" << endl;

    // Fetch frames
    start = monotonic_now();
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
             << endl;
    }

    delta = monotonic_now() - start;
    cout << std::fixed;
    cout << "Captured " << NFRAMES << " frames in " << delta << "µs" << endl;

    start = monotonic_now();

    switch (lround(cap.get(CAP_PROP_UNIMATRIX_OPTICS_TYPE)))
    {
    case CAP_UNIMATRIX_OPTICS_TYPE_MANUAL:
        cout << "Optics: Manual zoom/focus/iris" << endl;
        break;
    case CAP_UNIMATRIX_OPTICS_TYPE_DC:
        cout << "Optics: DirectControl-iris" << endl;
        break;
    case CAP_UNIMATRIX_OPTICS_TYPE_P:
        cout << "Optics: Precise-iris" << endl;
        break;
    case CAP_UNIMATRIX_OPTICS_TYPE_iCS:
        cout << "Optics: iCS" << endl;
        break;
    case CAP_UNIMATRIX_OPTICS_TYPE_CAMBLOCK:
        cout << "Optics: Camblock" << endl;
        break;
    default:
        cout << "Optics: Unknown" << endl;
        break;
    }

    // Read misc camera properties
    constexpr size_t ITER = 3u;
    for (size_t i = 0; i < ITER; ++i)
    {
        double gain    = cap.get(CAP_PROP_GAIN);
        double expo    = cap.get(CAP_PROP_EXPOSURE);
        double zoom    = cap.get(CAP_PROP_ZOOM);
        double focus   = cap.get(CAP_PROP_FOCUS);
        double fnumber = cap.get(CAP_PROP_UNIMATRIX_FNUMBER);
        double tonemapping     = cap.get(CAP_PROP_UNIMATRIX_TONEMAPPING);
        double temporal_filter = cap.get(CAP_PROP_UNIMATRIX_TEMPORAL_FILTER);

        // Success!
        cout << "iteration["        << std::setw(1)    << i << "] "
             << "gain: "            << gain            <<      "  "
             << "expo: "            << expo            <<      "  "
             << "zoom: "            << zoom            <<      "  "
             << "focus: "           << focus           <<      "  "
             << "f-number: "        << fnumber         <<      "  "
             << "tonemapping: "     << tonemapping     <<      "  "
             << "temporal-filter: " << temporal_filter <<  endl;
    }

    delta = monotonic_now() - start;
    cout << "Queried settings " << ITER << " times in " << delta << "µs" << endl;

    return 0;
}
catch(const std::exception& e)
{
    cerr << e.what() << endl;
}
