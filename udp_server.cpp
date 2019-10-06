// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <iostream> 


#define PORT	 8080 
#define MAXLINE 1024 

//opencv 
#include "opencv2/opencv.hpp"

using namespace cv;

// Driver code 
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "Hello from server"; 
    struct sockaddr_in servaddr, cliaddr; 

    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 

    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 

    // Filling server information 
    servaddr.sin_family = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 

    std::cout<<"servaddr.sin_family = "<<servaddr.sin_family<<std::endl;
    std::cout<<"servaddr.sin_addr.s_addr = "<<servaddr.sin_addr.s_addr<<std::endl;
    std::cout<<"servaddr.sin_port = "<<servaddr.sin_port<<std::endl;

    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
                sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

    socklen_t len; 
    int n; 


    //create a mat
    Mat mat(480,640, CV_8UC3, Scalar(0,0,255));
    int x = 0;
    int y = 0;
    float blue = 0;
    float green = 0;
    float red = 0;
    int count = 0;
    while(true)
    {
        count++;
        count %= 100000;

        n = recvfrom(sockfd, (void *)buffer, MAXLINE, 
                MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                &len); 
        //buffer[n] = '\0';
        void * bufferBackup = buffer; 
        //printf("Client : %s\n", buffer); 
        //std::cout<<"Client sent data:"<<std::endl;
        x = *((int*)bufferBackup);
        //std::cout<<"x = "<< x <<std::endl;
        bufferBackup += sizeof(int); 

        y = *((int*)bufferBackup);
        //std::cout<<"y = "<< y <<std::endl; 
        bufferBackup += sizeof(int); 

        blue = *((float*)bufferBackup);
        //std::cout<<"b = "<<  blue <<std::endl; 
        bufferBackup += sizeof(int);

        green = *((float*)bufferBackup); 
        //std::cout<<"g = "<<  green <<std::endl; 
        bufferBackup += sizeof(int);

        red = *((float*)bufferBackup); 
        //std::cout<<"r = "<< red<<std::endl; 
        bufferBackup += sizeof(int); 
        //sendto(sockfd, (const char *)hello, strlen(hello), 
        //MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
        //    len); 
        //printf("Hello message sent.\n");
        mat.at<cv::Vec3b>(y,x)[0] = blue; 
        mat.at<cv::Vec3b>(y,x)[1] = green; 
        mat.at<cv::Vec3b>(y,x)[2] = red; 
        //std::cout<<x<<", "<<y<<","<<y<<","<<blue<<","<<green<<","<<red<<std::endl;
        if(count%2000==0)
        {
            imshow("mat", mat);
            waitKey(1);
        }
    }
    return 0; 
} 

