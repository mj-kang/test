#ifndef CAMWITHV4L2_H
#define CAMWITHV4L2_H

#include <linux/videodev2.h>
#include <iostream>
struct buffer
{
        void *start;
        size_t length;
};

class CamWithV4L2
{
public:
    CamWithV4L2();
    CamWithV4L2(int _num, int _width, int _height, struct v4l2_pix_format _pixel_fmt);
    ~CamWithV4L2();

    int Open_Device(void);
    void Init_Device(void);
    void Init_mmap(void);
    void Start_Capturing(void);
    void MainLoop(void);
    void Stop_Capturing(void);
    void Uninit_Device(void);
    void Close_Device(void);
    int read_frame(void);
    void Processing(void * start, uint32_t bytesused);

    struct buffer * buffers;    
    int dev_num;
    int width;
    int height;
    struct v4l2_pix_format  pixel_fmt;
private:
    int fd;
    int nbuffers;
    int thread_exit;
};
#endif //CAMWITHV4L2_H
