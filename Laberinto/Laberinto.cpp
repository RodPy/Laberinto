#include <opencv2\opencv.hpp>
#include<iostream>
#include <string>
#include <vector>
using namespace std;

using namespace cv;

int main() {

	Mat imagen = imread("maze3.jpg");
	Mat Copy;
	imagen.copyTo(Copy);

	// Transformacion A Binario
	cvtColor(Copy, Copy, CV_BGR2GRAY);
	threshold(Copy, Copy, 50, 255, CV_THRESH_BINARY_INV);

	namedWindow("Imagen", CV_WINDOW_FREERATIO);
	imshow("Imagen", imagen);
	waitKey(0);

	// Buscar Los contornos
	vector<vector<Point>> Contorno;
	findContours(Copy, Contorno, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	if (Contorno.size() != 2)
	{
		cout << "Hay mas de dos Paredes";
	}

	// Graficar Contorno
	Mat draw;
	draw = Mat::zeros(imagen.size(), CV_32FC1);
	cout << Contorno.size();
	drawContours(draw, Contorno, 0, Scalar(255), -1);
	namedWindow("Imagen2", CV_WINDOW_FREERATIO);
	imshow("Imagen2", draw);

	Mat dilated, eroded;
	Mat kernel = Mat::ones(21, 21, CV_8UC1);

	//Dilating
	dilate(draw, dilated, kernel, Point(-1, -1), 2, BORDER_CONSTANT);
	namedWindow("Dilate", CV_WINDOW_FREERATIO);
	imshow("Dilate", dilated);

	//Eroding
	erode(dilated, eroded, kernel, Point(-1, -1), 2, BORDER_CONSTANT);
	namedWindow("Erode", CV_WINDOW_FREERATIO);
	imshow("Erode", eroded);

	// Diferencia Absoluta entre la erocion y dilatacion
	Mat diff;
	absdiff(dilated, eroded, diff);
	diff.convertTo(diff, CV_8UC1);
	namedWindow("DIFF", CV_WINDOW_FREERATIO);
	imshow("DIFF", diff);

	// Graficar resultado de la diferencia 
	vector <vector <Point>> Solucion;
	findContours(diff, Solucion, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	Mat draw2;
	draw2 = Mat::zeros(imagen.size(), CV_32FC1);
	cout << Solucion.size();
	drawContours(imagen, Solucion, 0, Scalar(255, 255, 0), -1);
	namedWindow("RESUELTO", CV_WINDOW_FREERATIO);
	imshow("RESUELTO", imagen);
	waitKey(0);
	destroyAllWindows;
}