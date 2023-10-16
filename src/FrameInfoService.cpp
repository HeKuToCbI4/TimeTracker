//
// Created by Nikita on 10/15/23.
//

#include "FrameInfoService.h"

Status FrameInfoServiceImpl::Unsubscribe(ServerContext *context, const StreamUnsubscribeRequest *request,
                                         StreamUnsubscribeResponse *response) {
    auto consumer_id = request->consumer_id();
    auto reply = std::format("Client %s unsubscribed.", consumer_id);
    this->stopRequestedMap[consumer_id] = true;
    response->set_message(reply);
    response->set_status_code(200);
    return Status::OK;
}

Status FrameInfoServiceImpl::Subscribe(ServerContext *context, const StreamSubscribeRequest *request,
                                       ServerWriter<::telemetry::TimeFrameInfo> *writer) {
    auto consumer_id = request->consumer_id();
    this->writersMap.insert(std::pair<std::string, ServerWriter<::telemetry::TimeFrameInfo> *>(consumer_id, writer));
    this->stopRequestedMap.insert(std::pair<std::string, bool>(consumer_id, false));
    while (!context->IsCancelled() && !this->stopRequestedMap.at(consumer_id)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (context->IsCancelled()) {
        return Status::CANCELLED;
    }
    if (this->stopRequestedMap.at(consumer_id)) {
        this->stopRequestedMap.erase(consumer_id);
        this->writersMap.erase(consumer_id);
        return Status::OK;
    }
    return {StatusCode::UNKNOWN, "Error happened during Subscribe, graceful termination is not completed!"};
}

void FrameInfoServiceImpl::GetStatistics() {
    auto snapshot = activity_monitor::ActivityMonitor::getSnapshot();
    auto frameInfo = telemetry::TimeFrameInfo();
    if (snapshot->window_title != nullptr) {
        frameInfo.set_window_title(utilities::Utilities::utf8_encode(*(snapshot->window_title)));
    } else {
        frameInfo.set_window_title("");
    }
    if (snapshot->process_executable_path != nullptr) {
        frameInfo.set_process_executable_path(utilities::Utilities::utf8_encode(*(snapshot->process_executable_path)));
    } else {
        frameInfo.set_process_executable_path("");
    }
    frameInfo.set_id(snapshot->id);
    frameInfo.set_utc_timestamp(snapshot->utc_timestamp);
    for (auto it = this->writersMap.begin(); it != this->writersMap.end(); it++) {
        auto consumer_id = it->first;
        auto writer = it->second;
        // TODO: Handle exceptions :)
        // TODO: Handle concurrency.
        writer->Write(frameInfo);
    }
}
