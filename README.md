# cvMatToHeif
cv::Mat Convert to HEIF ; HEIF Convet to cv::Mat

## macOS
1.Install dependencies with Homebrew

    brew install cmake make pkg-config x265 libde265 libjpeg libtool

2.Configure and build project (--preset argument needs CMake >= 3.21):

    mkdir build
    cd build
    cmake --preset=release ..
    ./configure
    make

### Windows
You can build and install libheif using the vcpkg dependency manager:

    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.bat
    ./vcpkg integrate install
    ./vcpkg install libheif

The libheif port in vcpkg is kept up to date by Microsoft team members and community contributors. If the version is out of date, please create an issue or pull request on the vcpkg repository.

