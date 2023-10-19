#include "FrameInfoService.h"
//#include <boost/log/trivial.hpp>


int main() {
    //    BOOST_LOG_TRIVIAL(trace) << "Succccccccccccccessss";

    auto server = FrameInfoServiceImpl();
    server.RunServer();
    return 0;
}
