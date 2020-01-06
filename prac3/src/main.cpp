#include "CamWithV4L2.h"
#include <linux/videodev2.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <linux/unistd.h>
#include <linux/input.h>
#include <iostream>
//#include <thread>
#include <pthread.h>
using namespace std;

/*
 * void * thread_EO(void *ptr)
 * OV2710 스레드
 * 1) dev = 0
 * 2) 640 x 480
 * 3) YUV422P 타입
 */
static int fd_eo;
void *thread_EO(void *ptr)
{
    struct v4l2_pix_format type;
    type.pixelformat = V4L2_PIX_FMT_YUV422P;
//    CamWithV4L2 * EO = new CamWithV4L2(0, 640, 480, type);
    CamWithV4L2 * EO = new CamWithV4L2(fd_eo, 640, 480, type);

    EO->Open_Device();
    EO->Init_Device();
    EO->Start_Capturing();
    EO->MainLoop();
    EO->Stop_Capturing();
    EO->Close_Device();
}

/*
 * void * thread_IR(void *ptr)
 * Lepton 스레드
 * 1) dev = 1
 * 2) 80 x 60
 * 3) UYVY 타입
 */
void *thread_IR(void *ptr)
{
    struct v4l2_pix_format type;
    type.pixelformat = V4L2_PIX_FMT_UYVY;
    CamWithV4L2 * IR = new CamWithV4L2(1, 80, 60, type);

    IR->Open_Device();
    IR->Init_Device();
    IR->Start_Capturing();
    IR->MainLoop();
    IR->Stop_Capturing();
    IR->Close_Device();
}

int main (int argc, char * argv[])
{
    pthread_t ir=0;
//    pthread_t eo=0;
 	fd_eo = 0;
    if(argc < 2)
	{
	    printf("Default EO /dev/video number = 0\n");
	}
    else 
	{
		printf("EO /dev/video number is = %d\n", atoi(argv[1]));
		fd_eo = atoi(argv[1]);
	}

    // if(pthread_create(&eo, NULL, thread_EO, NULL ) !=0)
    // {
        // perror("pthread create");
    // }
    if(pthread_create(&ir, NULL, thread_IR, NULL) !=0)
   {
        perror("pthread create");
    }

//    pthread_join(eo,NULL);
    pthread_join(ir,NULL);

    //glibcxx 버전으로 인해 std::thread 동작 불가
    //thread t1(thread_EO);
    //thread t2(thread_IR);

    //t1.join();
    //t2.join();

    return 0;
}


