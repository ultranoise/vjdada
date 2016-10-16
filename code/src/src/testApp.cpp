
#include "testApp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float pos[32]={0.0,0.003263,0.030294,0.037731,0.060047,0.069662,
0.104315,0.124630,0.145130,0.161456,0.186129,0.228766,0.278655,
0.321470,0.467874,0.517036, 0.545883,0.575272,0.675776,0.764486,
0.797143,0.824354,0.842132,0.854106,0.863180,0.876243,0.900732,
0.917968,0.921,0.975657,0.997425,0.9999999999999}; //28: 0.939373


int seq[335]={  //antes [370]
0,0,23,0,0,0,0,0,23,0,
0,0,0,0,23,26,0,0,0,0,
0,23,0,0,0,0,0,23,26,0,
0,0,23,0,0,0,0,0,23,21,
0,0,0,0,23,26,21,20,0,0,
23,20,0,0,0,21,22,23,26,21,
20,22,0,0,21,22,0,0,21,23,
23,26,21,23,23,23,23,0,0,0,
0,0,0,29,0,0,0,0,0,0,
29,0,0,0,0,0,16,0,0,0,
0,0,0,0,24,7,0,0,0,0,
0,24,24,24,24,24,24,24,24,0,
0,24,24,24,24,24,24,24,24,0,
0,0,0,0,0,16,0,0,0,0,
0,0,27,0,8,0,0,0,8,0,
10,0,0,0,13,14,0,0,0,27,
0,0,0,0,3,3,3,3,9,9,
9,9,3,3,3,3,9,9,9,9,
0,3,3,3,3,4,4,4,4,4,
4,4,4,4,4,4,4,4,4,5,
4,4,4,4,4,4,4,4,4,4,
5,0,0,0,0,0,0,0,0,0,
18,28,28,28,28,26,26,26,26,29,
29,29,29,0,0,0,0,0,0,0,
0,15,0,0,0,0,17,0,0,0,
0,0,0,0,3,3,3,3,9,9,
9,9,3,3,3,3,9,9,9,9,
0,3,3,3,3,4,4,4,4,4,
4,4,4,4,4,4,4,4,4,5,
4,4,4,4,4,4,4,4,4,4,
5,0,0,0,0,0,0,0,0,0,
18,28,28,28,28,26,26,26,26,29,
29,29,29,0,0,0,0,0,0,15,
0,0,0,0,0};
//15,4,4,4,4,4,4,4,4,5,
//4,4,4,4,4,4,4,4,4,4,
//5,28,28,28,28,26,26,26,26,29,
//29,29,29,30,30,30,30,30,30,30};

//el final original
//15,4,4,4,4,4,4,4,4,5,
//4,4,4,4,4,4,4,4,4,4,
//5,28,28,28,28,26,26,26,26,29,
//29,29,29,30,30,30,30,30,30,30};

//--------------------------------------------------------------
void testApp::setup(){

    //load movies
    vidPlayer.loadMovie("richter_cut.mov");
    vidTitle.loadMovie("intro_768_576.mov");//("titles.mp4");

    //nuevo
    vidRuido.loadMovie("ruido320.mov");
    vidAlea.loadMovie("richter_cut.mov");

        //FUNCIONA PARA DOS VIDEOS EN PARALELO
        /*
		vidPlayer2.loadMovie("hansmov.mov");
        //vidPlayer.loadMovie("richter.avi");
        vidPlayer2.play();
		vidPlayer2.setVolume(0);
		//vidPlayer.getPosition();
		//vidPlayer.
		vidPlayer2.setLoopState(0x01);
		*/

    colorImg.allocate(320,240);
    colorImg2.allocate(320,240);
    colorImg3.allocate(320,240);

	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);

	titleImg.allocate(768,576);

    //nuevo
    ruidoImg.allocate(320,240);
    aleaImg.allocate(320,240);

    //saver
    //img_saver.allocate(ofGetScreenWidth(), ofGetScreenHeight());


    //for storing pixels
    outputPixels = new unsigned char[ 320 * 240 * 3 ];

	bLearnBakground = true;
	threshold = 60;
	speed=1.0;
	frame=0;
	cerrado=0;
	bNewLoop=false;
	bNewFrame = false;
	section=-1;
	frameTitle=0;
	paso=0.0f;
	saved_image_num=0;

    part=0;
    viewAnalysis=false;
    contador_loop=0;
    incremento=0.03f;

	// open an outgoing connection to HOST:PORT (OSC)
	sender.setup( HOST, PORT );

    ofxOscMessage m11;

    //for setting the framerate
    //ofSetFrameRate(1);

}

//--------------------------------------------------------------
void testApp::update(){
    ofSetColor(0xFFFFFF);
	ofBackground(100,100,100);

	bool bNewFrame = false;
    bool bNewFrameTitle = false;

    //vidPlayer.idleMovie();

       //Update speed
            //vidPlayer.idleMovie();
            if(seq[part]==27) {   //detras del || es nuevo
                speed=0.015;
                threshold = 44;
                //printf("parte %i\n", seq[part]);
            }
            else {
                //para poder cambiarlo con el teclado comentar
                //threshold = 60;
            }

            vidPlayer.setSpeed(speed);
            vidPlayer.idleMovie();

    if (section==0) {  //play titles

        vidTitle.idleMovie();
        bNewFrameTitle= vidTitle.isFrameNew();

        //play the title when newFrame
        if(bNewFrameTitle) {

            frameTitle=frameTitle+1;
            titleImg.setFromPixels(vidTitle.getPixels(),768,576);
                //printf("pos %f",vidTitle.getPosition());
            if(vidTitle.getPosition()>=0.95f) {
                    section=section+1;
            }

        }
    }
    else {

        if(section==2) {

            //sequencer tasks:
            //1. check if we are still in a period of film
            if(vidPlayer.getPosition()>=pos[head+1]) {
                if(part>=sizeof(seq)/4) {
                    //vidPlayer.setPosition(pos[31]);

                    //descomentar lo siguiente
                    vidPlayer.stop();
                    part=1;
                }
                else {
                    part++;
                    if (part>160 ) {
                        if (part>300 && seq[part]==15) {
                            speed=0.1;
                        }
                        else {
                            if(seq[part]==15 || seq[part]==16) {
                                speed=1.0;
                            }else {
                                speed=4.0;
                            }
                        }
                    }else {
                            speed=1.0;

                    }
                    head=seq[part];
                    vidPlayer.setPosition(pos[head]);
                }
            }
        }

        //nuevo
        if(section==3) {
             vidRuido.idleMovie();
             //speed=0.3;
             vidRuido.setSpeed(speed);
             //threshold = 11;
        }

                //nuevo
        if(section==4) {
             vidRuido.stop();
             vidAlea.idleMovie();
             speed=4.5;
             vidAlea.setSpeed(speed);
             threshold = 60;
             if(contador_loop==10){
                vidAlea.setPosition(ofRandom(0.36f+paso,0.36f+incremento+paso));
                contador_loop=0;
             }
             contador_loop++;
        }


        bNewFrame = vidPlayer.isFrameNew() || vidRuido.isFrameNew() || vidAlea.isFrameNew();
    }


	if (bNewFrame){

	    if(section==4){
	        //ofSeedRandom(31);
	        //frame=ofRandom(0,300);
	        //ofRandom(
	        //printf("frame%f",frame);
	        //frame=frame+50;
	        frame=frame+1;
	    }
        else{

            frame=frame+1;
            //printf("paso");
        }

            //gets the pixels and the analysis starts
            if(section==3){
                colorImg.setFromPixels(vidRuido.getPixels(), 320,240);
            }else if(section==4){
                colorImg.setFromPixels(vidAlea.getPixels(), 320,240);
                }
            else{
                colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);
            }
            grayImage = colorImg; //to gray
            if (bLearnBakground == true){
                grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
                bLearnBakground = false;
            }

            // take the abs value of the difference between background and incoming and then threshold:
            grayDiff.absDiff(grayBg, grayImage);
            grayDiff.threshold(threshold);

            // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
            // also, find holes is set to true so we will get interior contours as well....
            if(section==3){
                contourFinder.findContours(grayDiff, 1, (340*240), 7, false);	// find holes
                }
            else {
                contourFinder.findContours(grayDiff, 20, (340*240), 6, true);	// find holes
            }



	}
}

//--------------------------------------------------------------
void testApp::draw(){


	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetColor(0xffffff);

	if(section==0) {  //titles
	    //printf("pinto");
        titleImg.draw(1020,0,1024,768);
	}
	else { //film-performance

        colorImg.draw(1020,0,1024,768);

        //fps calculation
        //timeNow=ofGetElapsedTimef();
        //double diff=timeNow-timeThen;
        //if(diff>0.0f){
          //  fps=1.0/diff;
           // frameRate*=0.9;
           // frameRate+=0.1*fps;
        //}
        //timeThen=timeNow;
        //printf("fps=%f\n",frameRate);

        //fps=ofGetFrameRate();
        //printf("fps=%f\n",fps);

        //GRAB SCREEN
        //static char file_name[255];
        //sprintf(file_name,"output3/output_%0.4i.bmp",saved_image_num);

        //img_saver.grabScreen(1024,0,1024,768);
        //img_saver.saveImage(file_name);

        //saved_image_num++;
        //END OF GRAB SCREEN

        // or instead we can draw each blob individually,
        // this is how to get access to them:
        char cString[12];
        char cString1[12];
        char nBlobs[2];
        char centroidX[12];
        char centroidY[12];
        char area[10];
        char espacio[2]=" ";
        char final[12]="\n";

        //start OSC messages
		ofxOscMessage m;
		m.setAddress( "/position" );
		m.addFloatArg(vidPlayer.getPosition());
		sender.sendMessage( m );
		ofxOscMessage m1;
		m1.setAddress( "/nrblobs" );
		m1.addIntArg(contourFinder.nBlobs);
		sender.sendMessage( m1 );

        ofxOscMessage m5;
        m5.setAddress( "/parte" );
		m5.addIntArg(part);
		sender.sendMessage( m5 );

		ofxOscMessage m6;
        m6.setAddress( "/seccion" );
		m6.addIntArg(section);
		sender.sendMessage( m6 );

        // then draw the contours:
        //ofFill();
        ofSetColor(0x333333);

        //uncomment to see screen rectangle
        ofRect(360,400,320,240);

        //draw blobs
        ofSetColor(0xffffff);
        if(viewAnalysis)  {
            for (int i = 0; i < contourFinder.nBlobs; i++){
                //uncomment for drawing
                contourFinder.blobs[i].draw(360,400);
            }
        }


        ofxOscMessage m2;
        m2.setAddress( "/areas" );
        for (int i = 0; i < contourFinder.nBlobs; i++){
            m2.addFloatArg(contourFinder.blobs[i].area);
        }
        sender.sendMessage( m2 );

        ofxOscMessage m3;
        m3.setAddress( "/centroideX" );
        for (int i = 0; i < contourFinder.nBlobs; i++){
            m3.addIntArg(contourFinder.blobs[i].centroid.x);
        }
        sender.sendMessage( m3 );

        ofxOscMessage m4;
        m4.setAddress( "/centroideY" );
        for (int i = 0; i < contourFinder.nBlobs; i++){
            m4.addIntArg(contourFinder.blobs[i].centroid.y);
        }
        sender.sendMessage( m4 );


        //finish message

        if(viewAnalysis) {
            // finally, a report:
            ofSetColor(0xf00000);
            char reportStr[1024];
            char reStr[1024];
            sprintf(reStr,"VJ-dada");
            ofDrawBitmapString(reStr, 20, 370);
            sprintf(reportStr, "nthreshold %i \nnum blobs found %i\n Position:  %f \n Speed:  %f \n Parte %i \n", threshold, contourFinder.nBlobs, vidPlayer.getPosition(), vidPlayer.getSpeed(),seq[part]);
            ofDrawBitmapString(reportStr, 20, 500);
        }
    } //fin del else de section

    if(part==335) {//if(seq[part]==30) {
        vidPlayer.stop();
    }
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
        case 'v':
			viewAnalysis=!viewAnalysis;
			break;
        case 'g':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case 'f':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
		case 'r':
			speed=speed+0.01;
			break;
        case 'a':
			paso=paso+0.01;
			break;
        case 'z':
			paso=paso-0.01;
			break;
		case 'l':
			speed=speed-0.01;
			if (speed < 0) speed = 0;
			break;
        case 'i':
			incremento=incremento-0.005;
			if (incremento <= 0) incremento = 0;
			break;
        case 'o':
			incremento=incremento+0.005;
			if (incremento >= 0.1) incremento = 0.1;
			break;
        case '7': //close
            {ofxOscMessage m12;
            m12.setAddress( "/nrblobs" );
            m12.addIntArg(0);
            sender.sendMessage( m12 );
            OF_EXIT_APP(0);}
        break;
        case '9': //close
            vidRuido.stop();
            vidRuido.idleMovie();
            {ofxOscMessage m13;
            m13.setAddress( "/nrblobs" );
            m13.addIntArg(0);
            sender.sendMessage( m13 );}
        break;

        //case 'o':
            //frameByframe=!frameByframe;
            //vidPlayer.setPaused(true);
        //break;
        case OF_KEY_LEFT:
            vidPlayer.previousFrame();
        break;
        case OF_KEY_RIGHT:
            vidPlayer.nextFrame();
        break;
		//case 'p':
			//vidPlayer.setPaused(true);
			//vidPlayer.setSpeed(0);
        //break;
        case '0':
            vidTitle.play();
            vidTitle.idleMovie();
            vidTitle.setVolume(0);
            vidTitle.getPosition();
            vidTitle.setLoopState(0x01);
			section=0;
        break;
        case '1':
			vidPlayer.play();
			vidTitle.idleMovie();
            vidPlayer.setVolume(0);
            //vidPlayer.getPosition();
            vidPlayer.setLoopState(0x01);
            vidPlayer.setPosition(0.0f);
            section=1;
			break;
        case '2':
            vidPlayer.play();
            vidTitle.idleMovie();
            //vidPlayer.stop();
            vidPlayer.setVolume(0);
            //vidPlayer.getPosition();
            vidPlayer.setLoopState(OF_LOOP_NORMAL);
            //printf("in %i",seq.index);
            //int in=seq.index;

            //comentar para ir al principio
            //part=329;

            head=seq[part];
			section=2;
        break;
        case '3':
            vidRuido.play();
            vidPlayer.idleMovie();
            //vidPlayer.stop();
            vidRuido.setVolume(0);
            //vidPlayer.getPosition();
            vidRuido.setLoopState(OF_LOOP_NORMAL);
            speed=0.3f;
            threshold=11;
            vidRuido.setSpeed(0.3f);
            //printf("in %i",seq.index);
            //int in=seq.index;

            //comentar para ir al principio
            //part=329;

            //head=seq[part];
			vidRuido.setPosition(0.0f);
			section=3;
        break;
        case '4':
            //vidPlayer.idleMovie();
            vidAlea.play();
            vidRuido.idleMovie();
            //vidPlayer.stop();
            vidAlea.setVolume(0);
            //vidPlayer.getPosition();
            vidAlea.setLoopState(OF_LOOP_NORMAL);
            speed=2.3f;
            vidAlea.setSpeed(2.3f);
            threshold=60;
            //printf("in %i",seq.index);
            //int in=seq.index;

            //comentar para ir al principio
            //part=329;

            //head=seq[part];
			//vidAlea.setPosition(0.0f);
			section=4;
        break;

		case 'c':
			vidPlayer.setPaused(false);
			vidPlayer.setSpeed(speed);
			break;
		case 'q':

			break;






	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(){

}



