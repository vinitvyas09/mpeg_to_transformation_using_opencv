/********************************************//*
*                AFFINE_H
/*********************************************/

#ifndef AFFINE_H
#define AFFINE_H

/************** Header Files *****************/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstdlib>

#include <pthread.h>

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

/**************** Namespace ******************/

using namespace cv;
using namespace std;

/***************** Macros ********************/

#define ISOTROPIC_SCALE_FACTOR 0
#define ROTATION_ANGLE -180
#define SOBEL_DEPTH CV_16S
#define SOBEL_SCALE 1
#define SOBEL_DELTA 0
#define NUM_THREADS 20
#define NUM_OF_FRAMES_PER_THREAD 100
#define SLEEP_SEC 3
#define handle_error_en(en, msg) \
do{errno=en;perror(msg);exit(1);}while (0)

/***************** Structs *******************/
struct thread_limits 
{
    int start_no;
    int stop_no;
};

/*********** Global Variables ****************/

char frame_name  [19];
char frame_name2 [25];
char frame_name3 [26];
int option;

#endif // AFFINE_H //

/*********************************************/
