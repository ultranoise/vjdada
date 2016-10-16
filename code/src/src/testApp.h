#ifndef _TEST_APP
#define _TEST_APP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ofMain.h"

#define OF_ADDON_USING_OFXOPENCV
#define OF_ADDON_USING_OFXOSC

#include "ofAddons.h"
#include "ofxOsc.h"

#include "ofxOpenCv.h"
#include "ofxOsc.h"

//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file
#define HOST "localhost"
#define PORT 9999

#define OF_LOOP_NORMAL					0x03

class testApp : public ofSimpleApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();

        #ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		vidGrabber;
		#else

		#endif

        ofVideoPlayer 		vidPlayer;
        ofVideoPlayer 		vidPlayer2;
        ofVideoPlayer 		vidTitle;
        ofVideoPlayer 		vidRuido;
        ofVideoPlayer 		vidAlea;

        ofxCvColorImage		titleImg;
        ofxCvColorImage		    colorImg;
        ofxCvColorImage		    colorImg2;
        ofxCvColorImage         colorImg3;
        ofxCvColorImage     ruidoImg;
        ofxCvColorImage     aleaImg;
        ofImage             img_saver;

        ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;

        ofxCvContourFinder 	contourFinder;

		//ofImage				bikers;
		unsigned char *     pixels2;
		unsigned char *     outputPixels;
		unsigned char *     frameBuffer;

		int 				threshold;
		bool				bLearnBakground;
		bool                bNewFrame;
		bool                bNewLoop;
		float				speed;
		int					frame;
		FILE				*fichero;
		char				nombre[11];
		int					cerrado;
		int                 section;
		int                 frameTitle;
		int                 part;
		int                 head;
		bool                viewAnalysis;
		float               paso;
		int                 contador_loop;
		float               incremento;
		int                 saved_image_num;
		double              timeNow, timeThen, fps, frameRate;

        //float pos[33];


        //struct sequencer seq;

		ofxOscSender sender;


};

#endif
