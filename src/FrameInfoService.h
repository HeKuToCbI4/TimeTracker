//
// Created by Nikita on 10/15/23.
//
#include "proto/FrameInfoService.grpc.pb.h"
#include <chrono>
#include <thread>
#include "proto/FrameInfo.pb.h"
#include "ActivityMonitor.h"
#include "utilities/Utilities.h"

#ifndef TIMETRACKER_FRAMEINFOSERVICE_H
#define TIMETRACKER_FRAMEINFOSERVICE_H

using namespace telemetry;
using namespace grpc;

class FrameInfoServiceImpl final : public FrameInfoService::Service {
private:
    std::map<std::string, bool> stopRequestedMap;
    std::map<std::string, ServerWriter<::telemetry::TimeFrameInfo> *> writersMap;
public:
    Status Unsubscribe(ServerContext *context, const StreamUnsubscribeRequest *request,
                       telemetry::StreamUnsubscribeResponse *response) override;

    Status Subscribe(ServerContext *context, const StreamSubscribeRequest *request,
                     ServerWriter<::telemetry::TimeFrameInfo> *writer) override;

    void GetStatistics();
};


#endif //TIMETRACKER_FRAMEINFOSERVICE_H
