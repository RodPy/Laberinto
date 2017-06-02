/*
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

Mat img0=imread("laberinto.jpg");
Point corner1, corner2;
Rect box;

void CapturarCoordenadas(Point xx)
{
	vector <Point> Datos;
	
	Datos.push_back(xx);
	cout << "Size: " << Datos.size()<<"  "<<xx<<endl ;// " (" << Datos[i].x << "," << Datos[i].y << ")" << endl;

	circle(img0, xx, 250, Scalar(255, 0, 0), 3);
}


static void mouse_callback(int event, int x, int y, int, void *)
{
	int xx=0 ;
	ostringstream str;
	vector <Point> Datos;

	// Al hacer clic sobre la imagen guardamos las coordenadas
/*	if (event == EVENT_LBUTTONDBLCLK)
	{
		corner1.x = x;
		corner1.y = y;
		Datos.push_back(corner1); xx++;
		cout << "x: " << xx << " Coordenadas: " << Datos.size() << endl;
		str << " " << corner1.x << " , " << corner1.y;
		putText(img, str.str(), corner1, FONT_HERSHEY_SIMPLEX, 0.50, CV_RGB(0, 255, 0), 1, CV_AA);
		if (Datos.size() > 4) {
			cout << "Coordenadas: " << endl;
			for (int i = 0; i < Datos.size(); i++)
				cout << "(" << Datos[i].x << "," << Datos[i].y << ")" << endl;
		}
	}

	string msg = "mouse - movido: X = " + to_string(x) + " Y = " + to_string(y);
	CapturarCoordenadas(Point(x,y));
	str << " " << x << " , " << y;
	


	Mat img(200, 650, CV_8UC3);
	switch (event)
	{
	case CV_EVENT_MOUSEMOVE:

		putText(img, msg, Point(10, 100), 4, 1, CV_RGB(0, 255, 0), 0, CV_AA);
		putText(img0, str.str(), Point(x, y), FONT_HERSHEY_SIMPLEX, 0.50, CV_RGB(0, 255, 255), 1, CV_AA);
		imshow("Uso del raton", img);
		imshow("Recortar Imagen", img0);
		xx++;
	//	cout << "x: " << xx << " Coordenadas: " << Datos.size() << endl;
		break;
	case CV_EVENT_LBUTTONDOWN:  break;
	case CV_EVENT_RBUTTONDOWN:  break;
	case CV_EVENT_MBUTTONDOWN:  break;
	case CV_EVENT_LBUTTONUP:  break;
	case CV_EVENT_RBUTTONUP:  break;
	case CV_EVENT_MBUTTONUP:  break;
	case CV_EVENT_LBUTTONDBLCLK:  break;
	case CV_EVENT_RBUTTONDBLCLK:  break;
	case CV_EVENT_MBUTTONDBLCLK:  break;
	}
}

int main()
{
	

	//imshow("Recortar Imagen", img0);

	namedWindow("Uso del raton", 0);
	setMouseCallback("Recortar Imagen", mouse_callback);

	while (char(waitKey(30)) != 27) {}

	return 0;
}
*/

#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

static void onMouse(int event, int x, int y, int, void*)
{
	ostringstream str;
	string msg = "mouse - movido: X = " + to_string(x) + " Y = " + to_string(y);
	Mat img(200, 650, CV_8UC3);

	switch (event)
	{
	case CV_EVENT_MOUSEMOVE:
		putText(img, msg, Point(10, 100), 4, 1, CV_RGB(0, 255, 0), 0, CV_AA);
		putText(img, str.str(), Point(x, y), FONT_HERSHEY_SIMPLEX, 0.50, CV_RGB(0, 255, 255), 1, CV_AA);
		imshow("Uso del raton", img);
		break;
	case CV_EVENT_LBUTTONDOWN:  break;
	case CV_EVENT_RBUTTONDOWN:  break;
	case CV_EVENT_MBUTTONDOWN:  break;
	case CV_EVENT_LBUTTONUP:  break;
	case CV_EVENT_RBUTTONUP:  break;
	case CV_EVENT_MBUTTONUP:  break;
	case CV_EVENT_LBUTTONDBLCLK:  break;
	case CV_EVENT_RBUTTONDBLCLK:  break;
	case CV_EVENT_MBUTTONDBLCLK:  break;
	}

}

int main(int argc, char** argv)
{
	namedWindow("Uso del raton", 0);
	setMouseCallback("Uso del raton", onMouse);
	waitKey();

	return 0;
}