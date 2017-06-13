#include <opencv2\opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include <string>
#include <vector>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <sstream>

using namespace std;
using namespace cv;

// Grafica el rectangulo 
// Variables GLOBALES
const Scalar Blanco		=	Scalar(255.0, 255.0, 255.0);//
const Scalar Negro		=	Scalar(0.0, 0.0, 0.0);//
const Scalar Azul		=	Scalar(255.0, 0.0, 0.0);//
const Scalar Verde		=	Scalar(0.0, 255.0, 0.0);//
const Scalar Rojo		=	Scalar(0.0, 0.0, 255.0);//
													//
Mat frame;



void EntradaSalida()
{
	/////http://www.linuxhispano.net/2012/12/03/capturar-imagenes-desde-camara-opencv/
	VideoCapture TheVideoCapturer;
	Mat bgrMap;

		char key = 0;

		int numSnapshot = 0;
		std::string snapshotFilename = "0";

		std::cout << "Press 's' to take snapshots" << std::endl;
		std::cout << "Press 'Esc' to exit" << std::endl;

		TheVideoCapturer.open(0);

		if (!TheVideoCapturer.isOpened()) {
			std::cerr << "Could not open video" << std::endl;
		}

		while (key != 27 && TheVideoCapturer.grab()) {
			TheVideoCapturer.retrieve(bgrMap);

			cv::imshow("BGR image", bgrMap);

			if (key == 115) {
				cv::imwrite(snapshotFilename + ".jpg", bgrMap);
				numSnapshot++;
				snapshotFilename = static_cast<std::ostringstream*>(&(std::ostringstream() << numSnapshot))->str();
			}

			key = cv::waitKey(20);
		}

}





void main()
{
	//Declaracion de Variable
	// Rectangulo Verde 

	Mat hsv_Azul, binary_Azul;			// Matrices de HiperSaturacion y Binario
	int c = 0;							// Aux para deteccion de puntos
	EntradaSalida();
	destroyAllWindows();
	vector<Point> deteccion;
	
	Mat img=imread("0.jpg");
	imshow("adsadsadas", img);
	waitKey(0);
	img.copyTo(frame);

	// Inicio de Deteccion de la entrada y Salida

		// Transfomracion a HiperSaturacion 
		cvtColor(frame, hsv_Azul, CV_BGR2HSV);
		// Establecer la escala de color deseado 
		inRange(hsv_Azul, Scalar(110, 50, 50), Scalar(130, 255, 255), binary_Azul);  //Detecta Color Azul
		// Matriz para los elementos detectados 
		Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
		erode(binary_Azul, binary_Azul, element);
		dilate(binary_Azul, binary_Azul, element);
		// Buscar los contornos 
		vector<vector<Point>> contours;
		findContours(binary_Azul, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		drawContours(binary_Azul, contours, -1, Scalar(255), CV_FILLED);
		// Detectar los contornos 
		for (vector<Point> contour : contours) {
			Rect r = boundingRect(contour);
			rectangle(frame, r, Azul, 2, CV_AA, 0);
			Point center(r.x + (r.width / 2), r.y + (r.height / 2));

			// Escribir las coordenadas sobre la imagen
			ostringstream str;
			str << "E/S - " << center.x << "," << center.y;
			putText(frame, str.str(), center, FONT_HERSHEY_COMPLEX_SMALL, 0.60, Verde, 1, CV_AA);

			// Guarda los rectangulos detectados 
			if (c < 2) deteccion.push_back(center);   // Coordenadas de entrada y Salida
			
		}
		imshow("Original", frame);
		imshow("BInariio", binary_Azul);

		// Para ver los las detecciones

	for (int i = 0; i < deteccion.size(); i++)
	{
		cout << "\ni " << i << " " << deteccion.at(i)<< endl;
	}

	waitKey(0);


}

