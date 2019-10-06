// Client side implementation of UDP client-server model 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <bitset>

#define PORT     8080 
#define MAXLINE 1024 


//opencv 
#include "opencv2/opencv.hpp"
using namespace cv;


void copyDataToPtr(Mat mat,uint* dataPtr)
{

    for (int i=0; i < mat.rows; ++i){
        for (int j=0; j < mat.cols; ++j){
            Vec3f intensity = mat.at<Vec3f>(i, j);
            float blue = intensity.val[0];
            float green = intensity.val[1];
            float red = intensity.val[2];
        }
    }


}

int main(int, char**)
{

    // setup udp
    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from client";
    struct sockaddr_in   servaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    socklen_t len;
    int n;



    VideoCapture cap(0); // open the default camera
    //VideoCapture cap(1); // open the second camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    Mat edges;
    //namedWindow("edges",1);
    //for(;;)
    //{
    Mat frame;
    Mat prev_frame;
    cap >> frame; // get a new frame from camera
    frame.copyTo(prev_frame);
    cvtColor(frame, edges, COLOR_BGR2GRAY);
    //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
    //Canny(edges, edges, 0, 30, 3);
    imshow("frame", frame);
    waitKey(30);

    //copyDataToPtr(edges,dataPtr);

    //test access pixel and send
    int dataLen = 5;
    int x;
    int y;
    float blue  = 0;
    float green = 0;
    float red   = 0;
    float pre_blue  = 0;
    float pre_green = 0;
    float pre_red   = 0;

    float diff = 0;
    float threshold = 50;

    int frameCount = 0;
    while(true)
    {
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, COLOR_BGR2GRAY);
        //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        //Canny(edges, edges, 0, 30, 3);
        imshow("frame", frame);
        waitKey(1);
        frameCount++;

        for(int x = 0 ; x < frame.cols ; ++x)
        {
            for(int y = 0 ; y < frame.rows ; ++y)
            {
                blue  = frame.at<cv::Vec3b>(y,x)[0];
                green = frame.at<cv::Vec3b>(y,x)[1];
                red   = frame.at<cv::Vec3b>(y,x)[2];

                pre_blue  = prev_frame.at<cv::Vec3b>(y,x)[0];
                pre_green = prev_frame.at<cv::Vec3b>(y,x)[1];
                pre_red   = prev_frame.at<cv::Vec3b>(y,x)[2];

                diff = 0;
                diff += fabs(pre_blue - blue);
                diff += fabs(pre_green - green);
                diff += fabs(pre_red - red);

                if(frameCount%5==0 || diff>threshold)
                {

                    //std::cout<<"blue = "<<blue<<std::endl;
                    //std::cout<<"green = "<<green<<std::endl;
                    //std::cout<<"red = "<<red<<std::endl;
                    //int 4 bytes and float 4 bytes...
                    // 2 ints and 3 floats = 20 bytes
                    // or same as the size of 5 floats
                    void * dataPtr = (void *) calloc(5,sizeof(float));
                    void * dataPtrBackup = dataPtr;

                    memcpy(dataPtr,&x,sizeof(float));
                    dataPtr += sizeof(float);

                    memcpy(dataPtr,&y,sizeof(float));
                    dataPtr += sizeof(float);

                    memcpy(dataPtr,&blue,sizeof(float));
                    dataPtr += sizeof(float);

                    memcpy(dataPtr,&green,sizeof(float));
                    dataPtr += sizeof(float);

                    memcpy(dataPtr,&red,sizeof(float));

                    dataPtr = dataPtrBackup;


                    sendto(sockfd, (const void *)dataPtr, dataLen*sizeof(float),
                            MSG_CONFIRM, (const struct sockaddr *) &servaddr,
                            sizeof(servaddr));
                    //std::cout<<"message for pixel("<<x<<","<<y<<")sent.\n";
                    delete(dataPtr);
                }
            }
        }
        frame.copyTo(prev_frame);
    }


    close(sockfd);


    return 0;
}
