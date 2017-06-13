#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int distancia(Point x1, Point x2)/// entre adyacentes
{
	if (abs(x2.x - x1.x) == 0)
	{// Desplazamiento Vertical
//		return abs(x2.y - x1.y);
	}
	if (abs(x2.y - x1.y) == 0)
	{// Desplazamiento Horizontal
//		return abs(x2.x - x1.x);
	}

	//return (87-87)^ 2 + (196-96)^ 2;
//	return abs(int(      ((x1.x - x2.x)^2+(x1.y - x2.y)^2) ^ (1/2)        ));
	return (int)sqrt(pow(x1.x - x2.x, 2) + pow(x1.y - x2.y, 2));
}

Point centro(Point Izq,Point Op) {		/// Ecuentra el centro del cuadrado
	
	int Px, Py;	
	Px = ((Op.x - Izq.x) / 2) + Izq.x;
	Py = ((Op.y - Izq.y) / 2) + Izq.y;
	Point centro(Px, Py);
	return centro;
}

int DeteccionColor(Mat cortada)				// Analisis de color
{
	int cn = 0, cb = 0, cRGB=0;
	for (int i = 0; i < cortada.rows; i++)
	{
		for (int j = 0; j < cortada.cols; j++)
		{
			if (cortada.at<uchar>(i, j) == 255)
			{
				cb++;
			}
			if (cortada.at<uchar>(i, j) == 0)
			{
				cn++;
			}
	/*		if(cortada.at<Vec3b>(i,j)[0]==225 && cortada.at<Vec3b>(i, j)[1]==225 && cortada.at<Vec3b>(i, j)[2]==0 )
			{
				return int(3); cRGB++;
			}*/
		}
	}

	if (cn > cb ) { return 1; }		// Negro
	if (cn < cb ) { return 0; }		// Blanco
	else return 1;
}

	int main()
	{
		// Declaracion de Variables 
		vector<Point> coordenada;			// Vector donde se almacenan los datos de las coordenadas en pixeles
		vector<Mat> corte;					// Vector de la matrices de las imagenes cortadas
		vector<Point>camino;				// Vecto con las coordenadas del camino
		int xMax,yMax;						// Valores maximos del cuadro 
		
		Mat img0= imread("laberinto.jpg",1);		// imagen Original
		Mat img;

		resize(img0,img,Size(800, 600), INTER_LINEAR);	// Redimensiona a => 800x600

		xMax = img.size().width;																
		yMax = img.size().height;

		cout << xMax<<","<<yMax;
		int lado = 100;					// lado del cuadrado minimo 
		//////////////////////////////////////////////////////////////////////////

		////////////////////////// BUSCA SALIDA//////////////////////////
		
		Mat Copy, imagen;
		img.copyTo(imagen);
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
	//	waitKey(0);
	//	destroyAllWindows;

		imagen.copyTo(img);
	//imshow(" IAMIDMASDIASMDSAIDMASI", img);

		//////////////////////////////////////////////////////////////////////////


		/////////////////Elementros para la cuadriculacion////////////////////////
		int
			vix = 37,					// Coordenadas del vertice Izquierdo eje x
			viy = 36,					// Coordenadas del vertice Izquierdo eje y 
			vox = lado + vix,			// Coordenadas del vertice Opuesto eje x 
			voy = lado + viy;			// Coordenadas del vertice Opuesto eje y
		//////////////////////////////////////////////////////////////////////////
		int l1=0, l2=0,l3=0, l4=0;
		
		for (int i = 0; i < (xMax / lado)-1; i++)
		{
			for (int j = 0; j < (yMax / lado)-1; j++)
			{
				l1 = vix + lado*i; l2 = viy +lado*j;
				l3 = vox + lado*i; l4 = voy +lado*j;

				cout <<endl<< "[ " << l1 << "," << l2 << "] " << " " << "[ " << l3 << "," << l4 << "] " << endl;
				Point central = centro(Point(l1, l2), Point(l3, l4));	// Determina del Centro del Cuadrado de Lado x Lado
				cout <<"Coord"<< (int)central.x <<","<< central.y << endl;
			
				if(i<((xMax / lado) - 1) && j<((yMax / lado) - 1)) //66
				{
					Rect r(l1, l2, l3, l4);
					Mat smallImg = img(Rect(Point(l1, l2), Point(l3, l4)));
						
					if (DeteccionColor(smallImg) == 1) {
						cout << "Negro =>" << DeteccionColor(smallImg) << endl;
					}

					if (DeteccionColor(smallImg) == 0) {
						cout << "Blanco =>" << DeteccionColor(smallImg) << endl;
					}

					imshow("Corte", smallImg);
		//			waitKey(0);
					cout <<" qwerty: "<< DeteccionColor(smallImg)<<endl;
					if (DeteccionColor(smallImg) == 0) {	// Solo los cuadros que se determinen como blanco seran escritos 
						rectangle(img, Point(l1, l2), Point(l3, l4), Scalar(255, 255, 0), 1, 8, 0);
						circle(img, centro(Point(l1, l2), Point(l3, l4)), 50, Scalar(255, 0, 0), 2);
						coordenada.push_back(centro(Point(l1, l2), Point(l3, l4)));
					}				
			}
			
			string msg = "*" + to_string(central.x) + "," + to_string(central.y);
			putText(img, msg, central, FONT_HERSHEY_SIMPLEX, 0.3, CV_RGB(0, 0, 255), 1, CV_AA);
			cout << "Centrol" << central << endl;
			cout << "i : " << i << " MAX " << (xMax / lado) << endl;
			}
		}
		
		cout << "DIME DEL VECTOR PPEPR : " << coordenada.size()<<endl;
		cout << "Distancia : " << distancia(Point(87, 96), Point(187, 96)) << endl;
		for (int i =0; i < coordenada.size(); i++) 
		{
			cout <<"i: "<<i<<" -"<< coordenada.at(i)<<endl;
		}
//		cout << "DIM ADA ADN : " <<int(img.at<uchar>(87,200)) <<" - " <<endl ;
		
// Buscar la Salida 
		Point Entrada= Point (100,10);
		Point Salida = Point(520, 520);
		int menor=10000;
		int aux = 0;
		int cercaSalida[2];
		int cercaEntrada[2];
		vector<Point> E_S; // [0] mas cerca de la salida  [1] mas cerca de la entrada 
		
// mas Cerca de la Salida
		for (int i = 0; i < coordenada.size(); i++)
		{
			//cout << "Caso :" <<i<<"_ "<< distancia(Salida, coordenada.at(i)) << " Y " << distancia(Salida, coordenada.at(i+1)) << endl;
			if (distancia(Salida, coordenada.at(i))<menor )
				{
				menor = distancia(Salida, coordenada.at(i));
	//			cout << "Mas Cerca de la Salida" << coordenada.at(i)<< " - " << distancia(Salida, coordenada.at(i)) << endl;
				cercaSalida[0] = coordenada.at(i).x; cercaSalida[1] = coordenada.at(i).y;
				}
		}
		menor = 1000;
		//MAS cerca entrada
		for (int i = 0; i < coordenada.size(); i++)
		{
			if (distancia(Entrada, coordenada.at(i))<menor)
			{
				menor = distancia(Entrada, coordenada.at(i));
	//			cout << "Mas Cerca de la Entrada" << coordenada.at(i) << " - " << distancia(Salida, coordenada.at(i)) << endl;
				cercaEntrada[0] = coordenada.at(i).x; cercaEntrada[1] = coordenada.at(i).y;
			}

		}
		cout << "Mas Cerca de la Entrada [" << cercaEntrada[0] << "," << cercaEntrada[1]<< "] "<< endl;
		cout << "Mas Cerca de la Salida [" << cercaSalida[0] << "," << cercaSalida[1] << "] " << endl;

		menor = 10000;
		 for (int i = 0; i < coordenada.size(); i++)
		 {
			 //cout << "Caso :" <<i<<"_ "<< distancia(Salida, coordenada.at(i)) << " Y " << distancia(Salida, coordenada.at(i+1)) << endl;
			 if (distancia(Entrada, coordenada.at(i))<lado*2)
			 {
				 aux++;
				 cout << "IR POR " << coordenada.at(i)  << "- Distancia "<< distancia(Salida, coordenada.at(i)) << endl;
				
			 }
		 }cout << "Caminos DEtectados " << aux-1 << endl;

////
		 vector<Point> auxCamino;

		for (int i = 0; i < coordenada.size(); i++)
		{
			for (int j = 0; j < coordenada.size(); j++) 
			{

				if (distancia(coordenada.at(i), coordenada.at(j)) == lado)
				{
					cout << "adyacentes" << coordenada.at(i) << " Y " << coordenada.at(j) << endl;
				}

			}
			
		}
/*
		//Copia
		for (int i = 0; i < coordenada.size(); i++)
		{
			auxCamino.at(i)=coordenada.at(i);

		}
*/
		/// Ralacion entre adyacentes  para encontrar la salida

		Point IN = Point(cercaEntrada[0], cercaEntrada[1]); 
		
		for (int i = 0; i < coordenada.size(); i++)
		{
			for (int j = 0; j < coordenada.size(); j++)
			{
				if (i == 0) { Point IN = Point(cercaEntrada[0], cercaEntrada[1]); }

				if (distancia(IN, coordenada.at(j)) == lado)
				{
					if (IN != coordenada.at(j))
					{
					cout << "i : " << i << "camino" << coordenada.at(i) << endl;
					IN = coordenada.at(i);
					}
				}
			}
			//	else auxCamino.pop_back();
		}
		

		imshow("Muestra", img);

	while (char(waitKey(30)) != 27) {}
	return 0;
	}

	