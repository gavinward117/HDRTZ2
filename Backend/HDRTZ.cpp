//#include <SDL.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <jsoncpp/json/json.h>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <ctime>
#include <thread>
#include <unistd.h>		//UNIX standard function definitions
#include <fcntl.h>		//File control definitions
#include <errno.h>		//Error number definitions
#include <termios.h>
#include <string.h>
#include <SDL2/SDL_image.h>
#include <fstream>
#include <stdlib.h>
using namespace cv;
using namespace std;

const int SCREEN_WIDTH = 3840;
const int SCREEN_HEIGHT = 2160;
const int fps = 24;

//function to convert an openCV Mat to a SDL_Texture
SDL_Texture * TexFromCV(const Mat& mat, SDL_Renderer* renderer);

int main(int argc, char* args[]){

  //usb device to read crank input from
  int USB = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY );
  float min = 100;
  float max = 0;

  
  
  struct termios tty;
  struct termios tty_old;
  memset (&tty, 0, sizeof tty);
  
  /* Error Handling */
  if ( tcgetattr ( USB, &tty ) != 0 ) {
    std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
  }
  
  /* Save old tty parameters */
  tty_old = tty;
  
  /* Set Baud Rate */
  cfsetospeed (&tty, (speed_t)B9600);
  cfsetispeed (&tty, (speed_t)B9600);
  
  /* Setting other Port Stuff */
  tty.c_cflag     &=  ~PARENB;            // Make 8n1
  tty.c_cflag     &=  ~CSTOPB;
  tty.c_cflag     &=  ~CSIZE;
  tty.c_cflag     |=  CS8;
  
  tty.c_cflag     &=  ~CRTSCTS;           // no flow control
  tty.c_cc[VMIN]   =  1;                  // read doesn't block
  tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
  tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
  
  /* Make raw */
  cfmakeraw(&tty);
  
  /* Flush Port, then applies attributes */
  tcflush( USB, TCIFLUSH );
  if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
    std::cout << "Error " << errno << " from tcsetattr" << std::endl;
  }
  
  int n = 0,
      spot = 0;
  char buf = '\0';

/* Whole response*/
 char response[1024];
 memset(response, '\0', sizeof response);

  int rollingAverage[5];
  rollingAverage[0] = 82;
  rollingAverage[1] = 82;
  rollingAverage[2] = 82;
  rollingAverage[3] = 82;
  rollingAverage[4] = 82;
  int averageCount = 0;
  int lf_count = 0;
  float MAX_INTERVAL = 15;
  float audio = 0;
  float audioSpeed = 0;
  char charAudio[10];
  char* audioCommand = (char*)malloc(100*sizeof(char));
  float interval = 0;
  SDL_Window* window = NULL;
  SDL_Surface* screenSurface = NULL;
  SDL_Renderer* renderer = NULL;

  //initialize SDL Things
  if(SDL_Init(SDL_INIT_VIDEO) < 0) printf("SDL_ERROR: %s\n", SDL_GetError());
  window = SDL_CreateWindow("HDRTZ", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			    SCREEN_WIDTH, SCREEN_HEIGHT,  SDL_WINDOW_FULLSCREEN_DESKTOP);

  if(window == NULL) printf("Window not created! SDL_ERROR: %s\n", SDL_GetError());
  else{
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) printf("Surface not created! SDL_ERROR: %s\n", SDL_GetError());
    else{
      SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    }
  }
  int imgFlags = IMG_INIT_PNG;
  if(!(IMG_Init(imgFlags) & imgFlags)) printf("SDL Image not initialized");
  SDL_Surface* maskSurf = IMG_Load("test.png");
  SDL_Texture* maskTex  = SDL_CreateTextureFromSurface(renderer, maskSurf);
  SDL_Surface* xSurf    = IMG_Load("xhair.png");
  SDL_Texture* xTex     = SDL_CreateTextureFromSurface(renderer, xSurf);

  //Initialize openCV camera and set up image capture
  VideoCapture cap;
  cap.open(1);
  cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
  cap.set(CV_CAP_PROP_FPS, fps);
  cout << "setting fps supported" << endl;
  if(!cap.isOpened()){
    cout << "error opening stream" << endl;
    return -1;
  }


  SDL_Point p;
  double angle = 0;
  SDL_Event e;
  bool quit = false;

 
  clock_t startTime, currentTime;
  Uint32 start_tick;

  

  while(!quit){
    
     std::ifstream coords("/home/nvidia/Downloads/sdl2/HDRTZ2/api/output.json", std::ifstream::binary);
     //Handle the input from the website
    ifstream ifs("/home/nvidia/Downloads/sdl2/HDRTZ2/api/output.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);
    int crosshair;
    int mask;


    //Resize the capture to be the size of the screen
    SDL_Rect videostream;
    //videostream.w = 1920;
    //videostream.h = 1080;
    videostream.w=1920;
    videostream.h = 1080;
    
    videostream.x =-1 * (obj["x"].asInt());
    p.x = 960+obj["x"].asInt();
    videostream.y =-1 * (obj["y"].asInt());
    p.y = 540+obj["y"].asInt();
    crosshair = obj["crosshair"].asInt();
    mask = obj["mask"].asInt();

    
    
    while( SDL_PollEvent( &e ) != 0 )
        {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {	
                        quit = true;
                    }


		    //Debug features for testing without crank
		    else if( e.type == SDL_KEYDOWN){
		      switch(e.key.keysym.sym)
			{
			case SDLK_RIGHT:
			  cout << "Hello" << endl;
			  interval -= .5;

			
			  
			  break;
	  
			case SDLK_LEFT:
			  interval += .5;
			  break;
			case SDLK_UP:
			  std::cout << interval << std::endl;
			  break;
			case SDLK_DOWN:
			  quit = true;
			  return 0;
		        default:
			  break;
			}
		    }
		    }   
		    

		
     do{
       n = read( USB, &buf, 1 );
       sprintf( &response[spot], "%c", buf );
       spot += n;
      } while( buf != '\r' && n > 0);
    
    tcflush( USB, TCIFLUSH );
    
    if (n < 0) {
      std::cout << "Error reading: " << strerror(errno) << std::endl;
      //interval = 0;
    }
    else if (n == 0) {
      std::cout << "Read nothing!" << std::endl;
    }

    
    // convert response into interval
    
    /*
     ifstream myfile;
     myfile.open("serial_data");
     int raw_crank_data;
     if (myfile.is_open()){
       while(!myfile.eof()){
	 myfile >> response;
       }
     }
     myfile.close();

    */

    //handle crank data
    double raw_crank_data;
    raw_crank_data = (double)atoi(response);

    //these values might need to change based on environment and setup
    float idleLow  = 80.4,//79, //91
      idleHigh = 84.0,//85,   //94
      minVal   = 77.4,//80,//74,
      maxVal   = 86.8;//103;//90;

    /*int idleLow  = 368,
      idleHigh = 419,
      minVal   = 450,
      maxVal   = 368;
    */
    rollingAverage[averageCount] = raw_crank_data;
    double sum = 0;
    for(int i = 0; i < 5; i++){
      sum = sum + rollingAverage[i];
    }
    raw_crank_data = sum/5.0;
    averageCount = (averageCount + 1)%5;

    if (n > 0){
      if(raw_crank_data >= idleLow && raw_crank_data <= idleHigh){
	      interval += 0;
      }      
      /*else if (raw_crank_data > maxVal){
	interval = MAX_INTERVAL;
      }
      else if(raw_crank_data < minVal){
	interval = MAX_INTERVAL * -1;
      }*/
      else if( raw_crank_data < idleLow){
	//interval = (raw_crank_data - idleLow) * (MAX_INTERVAL / (idleLow - minVal));
	interval += .04*(raw_crank_data - idleLow) * (MAX_INTERVAL / (idleLow - minVal));
   
      }      
      else if(raw_crank_data > idleHigh){
	//interval = (raw_crank_data - idleHigh) * (MAX_INTERVAL / (maxVal - idleHigh));
	interval += .04*(raw_crank_data - idleHigh) * (MAX_INTERVAL / (maxVal - idleHigh));
      }
    }

    if(interval > 0){
      interval *= 0.92;
      if(interval < .01){
	interval = 0;
      }
    }
    else if(interval < 0){
      interval *= 0.92;
      if(interval > -0.01){
	interval = 0;
      }
    }
    if(interval > MAX_INTERVAL){
      interval = MAX_INTERVAL;
    }
    else if(interval < MAX_INTERVAL*-1){
      interval = MAX_INTERVAL*-1;
    }
    if(raw_crank_data < min){
      min = raw_crank_data;
    }
    if(raw_crank_data > max){
      max = raw_crank_data;
    }
    std::cout << "rawCrankVal = " << raw_crank_data << " interval = " <<interval <<"\n";
    std::cout << "current max = " << max << " min = " << min <<"\n";
    
    
    
    
    spot = 0;
    Mat frame;
    SDL_Texture* tex = NULL;
    
    //get the latest frame from openCV
    cap >> frame;
    waitKey(1);

    //convert that frame to a texture
    tex = TexFromCV(frame, renderer);
    
    if(tex != NULL){
      //Render the frame if it exists
      SDL_RenderClear(renderer);
      SDL_RenderCopyEx(renderer, tex, NULL, &videostream, angle, &p, SDL_FLIP_NONE);
    cout << "mask: " << mask << " crosshair: " << crosshair << endl;

    //render the image mask
    if(mask == 1){
      SDL_RenderCopy(renderer, maskTex, NULL, NULL);
    }

    //render the crosshair
    if(crosshair == 1){
      SDL_RenderCopy(renderer, xTex, NULL, NULL);
    }

    //present the texutre and prepare for the next frae
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(tex);
    angle += interval;

   
   //HERE IS WHERE AUDIO WILL BE PLAYED ACCORDING TO INTERVAL
   // shell command is echo "1.0" > /tmp/pitch where the number is the float value
    //allocate 64 bytes jus cause
    /*
    audioSpeed = interval/(MAX_INTERVAL);
    //audioSpeed = floorf(audioSpeed*100)/100;
    sprintf(charAudio,"%.1f",audioSpeed);
    strcpy(audioCommand,"echo \"");
    strcat(audioCommand, charAudio);
    strcat(audioCommand,"\" > /tmp/pitch");

    printf(audioCommand,"\n");
    system(audioCommand);*/
    //free(audioCommand);

    audio = interval / (MAX_INTERVAL);
    ofstream outputFile("/tmp/pitch");
    //...
    sprintf(charAudio,"%f",audio);
    printf(charAudio);
    outputFile << to_string(audio);
    outputFile.close();
    }
    
    //lock the framerate to the value provided by fps
    //if this isn't here, the image can become jittery
    /*    if((1000/fps) > SDL_GetTicks() - start_tick){
	SDL_Delay((1000/fps)-(SDL_GetTicks()-start_tick));
      }
    else if((1000/fps) < SDL_GetTicks() - start_tick){
		  lf_count++;
		  }
    */
  }
  free(audioCommand);
}

//Function to convert openCV mat to SDL_Texture
SDL_Texture* TexFromCV(const Mat &mat, SDL_Renderer* renderer){

  IplImage cvFrame = (IplImage)mat;
  IplImage* cvFramePtr = &cvFrame;
  
  //create a surface first, luckily IplImage and surface seem to play nice
  SDL_Surface* frameData = SDL_CreateRGBSurfaceFrom(
						    (void*)cvFramePtr -> imageData,
						    cvFramePtr->width, cvFramePtr->height,
						    cvFramePtr->depth * cvFramePtr->nChannels,
						    cvFramePtr->widthStep,
						    0xff0000, 0x00ff00, 0x0000ff, 0);



  if(frameData == NULL){
    printf("Failed to convert Mat to Surface");
    return NULL;
  }

  
  //if the mat converts to surface nicely, then it will convert the surface to a texture
  SDL_Texture* frameTex = NULL;
  frameTex = SDL_CreateTextureFromSurface(renderer, frameData);


  
  //  SDL_FreeSurface(frameData);
  if(frameTex == NULL) return NULL;
  else {
    SDL_FreeSurface(frameData);

    //    cvReleaseImage(&cvFramePtr);
    return frameTex;
  }
}
