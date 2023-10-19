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
    // SAFE INSERT
    this->map_guard_mutex.lock();
    this->writersMap.insert(std::pair<std::string, ServerWriter<::telemetry::TimeFrameInfo> *>(consumer_id, writer));
    this->stopRequestedMap.insert(std::pair<std::string, bool>(consumer_id, false));
    this->map_guard_mutex.unlock();
    // RELEASE MUTEX
    while (!context->IsCancelled() && !this->stopRequestedMap.at(consumer_id)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    std::cout << "stop requested for " << consumer_id << " channel." << std::endl;
    auto status = Status(StatusCode::UNKNOWN, "Error happened during Subscribe, graceful termination is not completed!");
    if (context->IsCancelled()) {
        status = Status::CANCELLED;
    }
    this->map_guard_mutex.lock();
    if (this->stopRequestedMap.at(consumer_id)) {
        status = Status::OK;
    }
    this->stopRequestedMap.erase(consumer_id);
    this->writersMap.erase(consumer_id);
    this->map_guard_mutex.unlock();
    return status;
}

void FrameInfoServiceImpl::SendFrameInfo() {
    auto snapshot = activity_monitor::ActivityMonitor::getSnapshot();
    auto frameInfo = telemetry::TimeFrameInfo();
    frameInfo.set_id(snapshot->id);
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
    std::lock_guard<std::mutex> guard(this->map_guard_mutex);
    for (auto it = this->writersMap.begin(); it != this->writersMap.end(); it++) {
        auto consumer_id = it->first;
        auto writer = it->second;
        if (writer == nullptr) {
            this->stopRequestedMap[consumer_id] = true;
        }
        std::cout << "Sending frame: " << frameInfo.SerializeAsString() << std::endl;
        try {
            if (!writer->Write(frameInfo)) {
                std::cout << "The writer for client " << consumer_id << " has been disconnected." << std::endl;
                this->stopRequestedMap[consumer_id] = true;
            }
        } catch (...) {
            std::cout << "Issue with client " << consumer_id << ". Unsubscribing." << std::endl;
            this->stopRequestedMap[consumer_id] = true;
        }
    }
}

void FrameInfoServiceImpl::RunServer(const std::string &server_address) {
    std::cout << "Starting server with address " << server_address << std::endl;
    grpc::ServerBuilder builder;

    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(this);

    auto server = builder.BuildAndStart();
    std::cout << "Server started and waiting for connections on " << server_address << std::endl;
    while (true) {
        if (!this->IsAfk()) { this->SendFrameInfo(); }
        std::this_thread::sleep_for(std::chrono::milliseconds(this->reporting_interval));
    }
}
bool FrameInfoServiceImpl::IsAfk() {
    LASTINPUTINFO last_input;
    last_input.cbSize = sizeof(last_input);
    GetLastInputInfo(&last_input);
    auto tick_count = GetTickCount();
    auto elapsed = tick_count - last_input.dwTime;
    std::cout << "Time elapsed since last input [ms]: " << elapsed << std::endl;
    if (elapsed > this->afk_timeout) {
        return true;
    }
    return false;
}
