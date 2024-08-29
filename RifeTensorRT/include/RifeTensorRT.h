#ifndef RIFETENSORRT_H
#define RIFETENSORRT_H
#pragma once
#include <torch/torch.h>
#include <torch/cuda.h>
#include <NvInfer.h>
#include <fstream>
#include <c10/cuda/CUDAStream.h> // Ensure correct include for CUDAStream
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
}

class RifeTensorRT {
public:
    RifeTensorRT(
        std::string interpolateMethod = "rife4.20-tensorrt",
        int interpolateFactor = 2,
        int width = 0,
        int height = 0,
        bool half = true,
        bool ensemble = false
    );

    void run(const at::Tensor& frame, bool benchmark, AVCodecContext* enc_ctx, AVFrame* outputFrame, AVFormatContext* fmt_ctx, AVStream* video_stream, int64_t pts);

private:
    void handleModel();
    at::Tensor processFrame(const at::Tensor& frame) const;
    void cacheFrame();
    void cacheFrameReset(const at::Tensor& frame);

    std::string interpolateMethod;
    int interpolateFactor;
    int width;
    int height;
    bool half;
    bool ensemble;
    bool firstRun;
    bool useI0AsSource;

    torch::Device device;
    torch::Tensor I0, I1, dummyInput, dummyOutput;
    std::vector<void*> bindings;
    torch::ScalarType dType;
    c10::cuda::CUDAStream stream;

    nvinfer1::ICudaEngine* engine;
    nvinfer1::IExecutionContext* context;
    std::string enginePath;
};

#endif
