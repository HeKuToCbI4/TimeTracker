import grpc
import FrameInfo_pb2 as fi_proto
import FrameInfoService_pb2 as fis_proto
import FrameInfoService_pb2_grpc as fis_grpc_proto
import ctypes

if __name__ == "__main__":
    channel = grpc.insecure_channel("localhost:50051")
    stub = fis_grpc_proto.FrameInfoServiceStub(channel)
    subscribe = fis_proto.StreamSubscribeRequest()
    subscribe.consumer_id = "test_consumer"
    incoming_frame_info: fi_proto.TimeFrameInfo
    for incoming_frame_info in stub.Subscribe(subscribe):
        print(incoming_frame_info.id)
        print(incoming_frame_info.process_executable_path)
        print(incoming_frame_info.window_title)
        print(incoming_frame_info.utc_timestamp)
