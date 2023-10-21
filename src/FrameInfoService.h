//
// Created by Nikita on 10/15/23.
//

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>

//#include <boost/log/trivial.hpp>
#include <chrono>
#include <thread>

#include "ActivityMonitor.h"
#include "src/protobuf/proto/FrameInfo.pb.h"
#include "src/protobuf/proto/FrameInfoService.pb.h"
#include "src/protobuf/proto/FrameInfoService.grpc.pb.h"
#include "utilities/Utilities.h"

#ifndef TIMETRACKER_FRAMEINFOSERVICE_H
#define TIMETRACKER_FRAMEINFOSERVICE_H

using namespace telemetry;
using namespace grpc;

class FrameInfoServiceImpl final : public FrameInfoService::Service {
private:
    std::map<std::string, bool> stopRequestedMap;
    std::map<std::string, ServerWriter<::telemetry::TimeFrameInfo> *> writersMap;
    std::mutex map_guard_mutex;
    unsigned long afk_timeout = 15*60*1000;
    int reporting_interval = 5000;

public:
    Status Unsubscribe(ServerContext *context, const StreamUnsubscribeRequest *request,
                       telemetry::StreamUnsubscribeResponse *response) override;

    Status Subscribe(ServerContext *context, const StreamSubscribeRequest *request,
                     ServerWriter<::telemetry::TimeFrameInfo> *writer) override;

    bool IsAfk();

    void SendFrameInfo();

    void RunServer(const std::string &server_address = "127.0.0.1:50051");

};


#endif//TIMETRACKER_FRAMEINFOSERVICE_H
