
#include <linux/videodev2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <signal.h>
#include <time.h>
#include <linux/unistd.h>
#include <linux/input.h>
#include <iostream>

#include "CamWithV4L2.h"


#define CLEAR(x) memset(&(x), 0, sizeof(x))                             //메모리 클리어
using namespace std;

static int xioctl(int fh, int request, void *arg)
{
        int r;

        do {
                r = ioctl(fh, request, arg);
        } while (-1 == r && EINTR == errno);

        return r;
}

/* CamWithV4L2()
 * 기본 생성자
 * Default: OV2710
 */
CamWithV4L2::CamWithV4L2()
{
    fd = -1;
    dev_num = 0;
    width   = 640;
    height  = 480;
    pixel_fmt.pixelformat   = V4L2_PIX_FMT_YUV422P;
    thread_exit = 1;
}

/* CamWithV4L2(int _num, int _width, int _height, struct v4l2_pix_format _pixel_fmt)
 * Input:
 *  비디오 디바이스 번호
 *  영상 사이즈: width / height
 *  픽셀 포맷 - ex)  UYVY / YUV422P
 */

CamWithV4L2::CamWithV4L2(int _num, int _width, int _height, struct v4l2_pix_format _pixel_fmt)
{
    fd = -1;
    dev_num = _num;
    width = _width;
    height = _height;
    pixel_fmt = _pixel_fmt;
    thread_exit = 1;
}

/* 
 * int Open_Device(void)
 * 디바이스 오픈
 * 1) 디바이스 정보 확인
 * 2) 디바이스 오픈
 * 3) return fd
 *  -> OPEN 여부를 확인하기 위함, 현재는 에러발생 시 종료하게 되므로 정확히 사용되지는 않음
 *  -> fd는 class 멤버 변수
 */
int CamWithV4L2::Open_Device(void)
{
        struct stat st;
        char dev_name[128];
        sprintf(dev_name, "/dev/video%d",dev_num);
        if (-1 == stat(dev_name, &st)) 
        {
            perror("Device open(): ");
            exit(EXIT_FAILURE);
        }
        if (!S_ISCHR(st.st_mode)) {
                fprintf(stderr, "%s is no device\n", dev_name);
            exit(EXIT_FAILURE);
        }
        fd = open(dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);

        if (-1 == fd)
        {
            fprintf(stderr, "Cannot open '%s': %d, %s\n",
                         dev_name, errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        return fd;
}

/*
 * void Init_Device(void)
 * 카메라 디바이스 설정
 * 1) Capabilities가 V4L2_CAP_VIDEO_CAMERA 인지 확인
 * 2) V4L2 INPUT 설정 => index = 0
 * 3) Crop Capability 설정
 *  -> OV2710은 Cropcap을 잘 받아오지 못했음
 * 4) V4L2 포맷 설정
 * 5) fps 설정
 * 6) 버퍼 설정 -> Init mmap()
 */
void CamWithV4L2::Init_Device(void)
{
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;
    struct v4l2_input input;
    if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) 
    {
        perror("VIDIOC_QUERYCAP");
        exit(EXIT_FAILURE);   
    }

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) 
    {
        perror("V4L2_CAP_VIDEO_CAPTURE");
        exit(EXIT_FAILURE);
    }
//----------------------------------------------------------------
    CLEAR(input);

    if(xioctl(fd, VIDIOC_ENUMINPUT, &input) < 0)
	{
        perror("VIDIOC_ENUMINPUT ");
    }	

#if __DEBUG
    printf("Name : %s\n", input.name);
	printf("Input type : 0x%x\n", input.type);
	printf("Audio set : 0x%x\n", input.audioset);
	printf("tuner : 0x%x\n", input.tuner);
	printf("std : 0x%llx\n", input.std);
	printf("status : 0x%x\n", input.status);
	printf("cap : 0x%x\n", input.capabilities);
#endif

    int index = 0;
	if(ioctl(fd, VIDIOC_S_INPUT, &index) == -1)
	{
        perror("VIDIOC_S_INPUT");
    }
//----------------------------------------------------------------      무엇?
    CLEAR(cropcap);
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) 
    {
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    }

    CLEAR(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = width;
    fmt.fmt.pix.height      = height;
    fmt.fmt.pix.pixelformat = pixel_fmt.pixelformat;
    fmt.fmt.pix.field       = V4L2_FIELD_ANY;
    cout << "width: " << width << " height: " <<height <<endl;
    char fourcc[6];
    strncpy(fourcc, (char *)&fmt.fmt.pix.pixelformat, 4);
    cout << "pixel format: " << fourcc <<endl;
    if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
    {
        perror("VIDIOC_S_FMT");
        exit(EXIT_FAILURE);
    }
        
//------------------------------------------------------------
    struct v4l2_streamparm setfps;
    CLEAR(setfps);
	setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	setfps.parm.capture.timeperframe.numerator = 1;
	setfps.parm.capture.timeperframe.denominator = 30;
//------------------------------------------------------------      무엇
    
    /* Buggy driver paranoia. */
    /*
    unsigned int min;

    min = fmt.fmt.pix.width * 2;
    if (fmt.fmt.pix.bytesperline < min)
            fmt.fmt.pix.bytesperline = min;
    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
    if (fmt.fmt.pix.sizeimage < min)
        fmt.fmt.pix.sizeimage = min;
    */
    Init_mmap();
}

/*
 * void Init_mmap(void)
 * 버퍼 셋팅 및 mmap
 * 1) Request buffer 설정
 * 2) buffer 메모리 할당
 * 3) req buffer 개수만큼 mmap 할당
 */
void CamWithV4L2::Init_mmap(void)
{
    struct v4l2_requestbuffers req;
    struct v4l2_buffer buf;

    CLEAR(req);
    req.count   = 4;
    req.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory  = V4L2_MEMORY_MMAP;

    if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) 
    {
        if (EINVAL == errno) 
        {
            perror("NOT SUPPORT MMAP");
            exit(EXIT_FAILURE);
        }
        else 
        {
            perror("VIDIOC_REQBUFS");
            exit(EXIT_FAILURE);
        }
    }
    
    if (req.count < 2) 
    {   
        perror("Insufficient buffer memory");
        exit(EXIT_FAILURE);
    }

    buffers = (struct buffer *)calloc(req.count, sizeof(*buffers));
    if (!buffers) 
    {
        perror("Out of memory");
        exit(EXIT_FAILURE);
    }

    
    for(nbuffers = 0; nbuffers < req.count; ++nbuffers)
    {
        CLEAR(buf);
        buf.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory  = V4L2_MEMORY_MMAP;
        buf.index   = nbuffers;

        if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
        {
            perror("VIDIOC_QUERYBUF");
            exit(EXIT_FAILURE);
        }

        buffers[nbuffers].length    = buf.length;
        buffers[nbuffers].start     
            = mmap(NULL, buf.length, PROT_READ | PROT_WRITE , MAP_SHARED, fd, buf.m.offset);
#if __DEBUG
        printf("start : %x\n", buffers[nbuffers].start);
#endif 
        if (MAP_FAILED == buffers[nbuffers].start)
        {
            perror("MMAP");
            exit(EXIT_FAILURE);
        }
    }
}

/*
 * void Start_Capturing(void)
 * 캡처 시작
 * 1) 각 버퍼당 QBUF 요청
 * 2) STREAM ON
 */
void CamWithV4L2::Start_Capturing(void)
{
    enum v4l2_buf_type type;
    struct v4l2_buffer buf;
    for(int i=0; i<nbuffers; ++i)
    {
        CLEAR(buf);

        buf.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory  = V4L2_MEMORY_MMAP;
        buf.index   = i;

        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
        {
            perror("VIDIOC_QBUF");
            exit(EXIT_FAILURE);
        }
    }
    
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
    {
        perror("VIDIOC_STREAMON");
        exit(EXIT_FAILURE);
    }
}

/*
 * void MainLoop(void)
 * 무한 루프 돌면서 버퍼를 받아온다
 * 1) fd_set을 이용하여 디스크립터 변화 감지
 * 2) timeout 2초 설정
 * 3) 파일 디스크립터 변화 있을 경우 read_frame() 이동
 * 4) 111*1000 usleep -> 약 9fps, Lepton2를 위한 설정이므로 삭제예정
 */
void CamWithV4L2::MainLoop(void)
{
    fd_set fds;
    struct timeval tv;
    int r;
    while(thread_exit)
    {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        tv.tv_sec   = 2;
        tv.tv_usec  = 0;

        r =  select(fd+1, &fds, NULL,NULL,&tv);

		if (r == -1)
		{
			if(errno == EINTR) continue;
			perror("Select: ");
		}
		if (r == 0)
		{
			perror("Select Timeout:");
			continue;
		}

        read_frame();
        usleep(111*1000);
    }
}

/*
 * void Stop_Capturing(void)
 * 1) STREAM OFF
 */
void CamWithV4L2::Stop_Capturing(void)
{
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
    {
        perror("VIDIOC_STREAMOFF");
        exit(EXIT_FAILURE);
    }
}

/*
 * void Uninit_Device(void)
 * 메모리 할당 해제
 * 1) munmap
 * 2) free
 */

void CamWithV4L2::Uninit_Device(void)
{
    for (int i = 0; i < nbuffers; ++i)
    {
        if (-1 == munmap(buffers[i].start, buffers[i].length))
        {
            perror("munmap");
            exit(EXIT_FAILURE);
        }
    }
    free(buffers);
}

/*
 *  int read_frame(void)
 *  파일 디스크립터에 버퍼가 써졌을 때 호출
 *  1) DQBUF: 데이터를 kernel 영역에서 user영역으로 복사
 *  2) 인덱스가 맞는지 확인
 *  3) 데이터 처리 - Processing 함수 내부에서 영상처리
 *  4) QBUF: 데이터 요청
 */ 
int CamWithV4L2::read_frame(void)
{
    struct v4l2_buffer buf;

    CLEAR(buf);

    buf.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory  = V4L2_MEMORY_MMAP;
    if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) 
    {
        perror("VIDIOC_DQBUF");
        exit(EXIT_FAILURE);
    }

    assert(buf.index < nbuffers);
#if __DEBUG
    printf("index -> %d addr -> %x\n | byteused %u\n",buf.index, buffers[buf.index].start, buf.bytesused);
#endif
    Processing(buffers[buf.index].start, buf.bytesused);
    if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
    {
        perror("VIDIOC_QBUF");
        exit(EXIT_FAILURE);
    }
    return 1;
}

/*
 * void Close_Device(void)
 * close
 */
void CamWithV4L2::Close_Device(void)
{
   if (-1 == close(fd))
   {
        perror("close");
        exit(EXIT_FAILURE);
   }
   fd = -1;
}

/*
 * void Processing(void * start, uint32_t bytesused)
 * 데이터 받아서 처리하는 부분
 * -> 현재는 파일 저장
 * 1) 몇 바이트 들어왔는지 표시 -> debug 메세지
 * 2) 파일 쓰기
 *  => 후에 영상 처리 및 HMD 영상 출력으로 변경 필요
 */
int cnt=0;
int fp;
char outfile[10];
void CamWithV4L2::Processing(void * start, uint32_t bytesused)
{
    cout << "bytesused: " <<bytesused<<endl;
    sprintf(outfile, "test%d-%d.yuv",dev_num, cnt++);
    fp = open(outfile, O_RDWR | O_CREAT);
    write(fp, start, bytesused);
    close(fp);
}