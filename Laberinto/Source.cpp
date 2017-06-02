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

void VerificaColor() {


}

void Cuadriculacion(Mat img) {
	vector<Mat> Cuadriculacion;


	Rect croppedRectangle = Rect(37, 146, 137, 246);
	Mat CroppedImage = img(croppedRectangle);
	
	Cuadriculacion.push_back(CroppedImage);
//	imshow("CORTE",CroppedImage);
	cout << "Tamaño del vector  : " << Cuadriculacion.size() << endl;

	
}

void dibujador(Mat img, int xMax, int yMax, int xMin,int yMin) {
	vector<Mat> Cuadriculacion;
	int lado = 100; // lado del Cuadrado de referencia

	cout << "\nxMax: " << (xMax/lado) << " yMax:  " << (yMax/lado) << " xmin: " << xMin << "  Ymin  " << yMin<<"...."<<endl;

	

// Elementos Necesarios Para el Rectangulo
	int VSIx = xMin;		// Coordenadas superior IZQ
	int VSIy = yMin;

	int VIDx = xMin + 100;	//	Coordenadas Vertice Opuesto al anterior
	int VIDy = yMin + 100;	
	int xx = 0;
//	int Sx,Sy,Ix,Iy;
	int Sx = VSIx, Sy = VSIy, Ix = VIDx, Iy = VIDy;
	//rectangle(img, Point(0, 0), Point(100, 100), Verde);


	for (int i= 0; i<(xMax/lado)+1; i++)
	{ 
		for (int j = 0; j < ((yMax/lado)-2); j++)
		{
			
	//		cout << "i: " << i << " , " << " j: " << j << endl;
			cout << "VSI (" << Sx << "," << Sy << " ) " << "VID (" << Ix << "," << Iy << " ) " << endl;
			
			rectangle(img, Point(Sx,Sy), Point(Ix, Iy), Verde);
	//		Rect Rectangulos = Rect(37,46,137,146);
	//		Centro(img, Rectangulos);
	//		Mat CroppedImage = img(Rect(Sx, Sy, Ix, Iy));
	//		Cuadriculacion.push_back(CroppedImage);
	//		imshow("CORTE", CroppedImage);

			Sx = VSIx + 100 * i; Sy = VSIy + 100 * j;
			Ix = VIDx + 100 * i; Iy = VIDy + 100 * j; xx++;
		}

	}
	
	cout<<"Tamaño del vector  : " << xx <<endl;

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
	Mat img_0 = imread("laberinto.jpg");
	Mat img;
	resize(img_0, img, Size(800, 600), 0, 0, INTER_LINEAR);  // ya es de 800x600 por defecto
	imwrite("LABv2.jpg", img);



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

	dibujador(img, xMax, yMax, xIN, yIN);

	imshow("Original", img);
	//imshow("BIniraio", binary);

	waitKey(0);
	posx = 0;
}

