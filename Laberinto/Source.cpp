#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2\opencv.hpp>
#include<iostream>
#include <string>
#include <vector>
using namespace std;

using namespace cv;

// Grafica el rectangulo 

// Variables GLOBALES
const Scalar Blanco		=	Scalar(255.0, 255.0, 255.0);
const Scalar Negro		=	Scalar(0.0, 0.0, 0.0);
const Scalar Azul		=	Scalar(255.0, 0.0, 0.0);
const Scalar Verde		=	Scalar(0.0, 255.0, 0.0);
const Scalar Rojo		=	Scalar(0.0, 0.0, 255.0);

void Centro(Mat img,Rect r) {

		Point center(r.x + (r.width / 2), r.y + (r.height / 2));
		ostringstream str;
		str << " [ " << center.x << "," << center.y<<"] ";
		putText(img, str.str(), center, FONT_HERSHEY_COMPLEX_SMALL, 0.60, CV_RGB(0, 255, 0), 1, CV_AA);
		
	}



void main()
{
	int posy = 0, posx = 0;
	int c = 0;
	int IN[2];				// Coordenadas de la Entrada
	int OUT[2];				// coodenadas de la Salida
	int xMax, yMax;
	int xIN = 0, yIN = 0;		// desplazamientos de la entrada inicial al borde

								//crear una imagen de 800x600 pixeles
								//canal BGR de tipo CV_8U( valores de 0 a 255)
								//inicializamos a 0 (color negro)
	Mat binary, hsv, wtr;
	Mat img_0;
		//= imread("laberinto.jpg");
	Mat img;
	

	VideoCapture capWebcam(1);
	char charCheckForEscKey = 0;
	while (charCheckForEscKey != 27 && capWebcam.isOpened()) {		// until the Esc key is pressed or webcam connection is lost
		bool blnFrameReadSuccessfully = capWebcam.read(img_0);		// get next frame

		if (!blnFrameReadSuccessfully || img_0.empty()) {		// if frame not read successfully
			std::cout << "error: frame not read from webcam\n";		// print error message to std out
			break;													// and jump out of while loop
		}
		imshow("sdfsdfsd",img_0);
	}

	system("pause");
	//imwrite("Prueba.jpg", img);
	(img_0, img, Size(800, 600), 0, 0, INTER_LINEAR);  // ya es de 800x600 por defecto
	//imwrite("Prueba.jpg", img);
	//imwrite("LABv2.jpg", img);
//	img = imread("Prueba.jpg");

	xMax = img.size().height;
	yMax = img.size().width;
//	cout << "\nxMax: " << (xMax/100) << " yMax:  " << (yMax/100) << endl;
	//	cout << img;

	cvtColor(img, hsv, CV_BGR2HSV);
	int sensitivity = 15;
	inRange(hsv, Scalar(110, 50, 50), Scalar(130, 255, 255), binary);  //Detecta Color Azu
	//inRange(hsv, Scalar(255, 255, 255), binary); // detecta blanco

	Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
	erode(binary, binary, element);
	dilate(binary, binary, element);

	vector<vector<Point>> contours;
	findContours(binary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	drawContours(binary, contours, -1, Scalar(255), CV_FILLED);

	for (vector<Point> contour : contours) {
		// dibujar rectangulo

		Rect r = boundingRect(contour);
		rectangle(img, r, CV_RGB(255, 0, 0), 2, CV_AA, 0);

		if (c == 1) {
			yIN = r.y + r.height; xIN = r.x;
			cout << "\nRx: " << r.x << " Rwidth:  " << r.width << " Ry:  " << r.y << " r.height:  " << r.height << ".....";
		}
		Point center(r.x + (r.width / 2), r.y + (r.height / 2));


		ostringstream str;
		str << "E/S; " << center.x << "," << center.y;
		putText(img, str.str(), center, FONT_HERSHEY_COMPLEX_SMALL, 0.60, CV_RGB(0, 255, 0), 1, CV_AA);

		if (c < 1) {
			c++;
			OUT[0] = center.x; OUT[1] = center.y;
			//			cout << "\n" << OUT[0] << " - " << OUT[1];
		}

		if (c < 2) {

			IN[0] = center.x;  IN[1] = center.y;
			//			cout << "\n" << c << " - " << IN[0] << " - " << IN[1];

		}
	}

	c = 0;

	//Cuadriculacion(img);

	imshow("Original", img);
	//imshow("BIniraio", binary);

	waitKey(0);
	posx = 0;
}

