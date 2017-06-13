#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <queue>
#include <vector>

#define RMAX 9
#define CMAX 13
#define INF 1000000000

using namespace std;
using namespace cv;

class Nodo {
public:
	int fila, columna;

	Nodo() {

	}

	Nodo(int fila, int columna) {
		this->fila = fila;
		this->columna = columna;
	}
};

const int di[] = {1, -1, 0, 0};
const int dj[] = {0, 0, 1, -1};
int mapa[RMAX][CMAX] = {
	{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0},
	{0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
	{1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1},
	{1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1},
	{1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}
};
int dist[RMAX][CMAX];
int R = 9, C = 13;
Nodo padre[RMAX][CMAX];

vector<Nodo> obtenerCamino(Nodo entrada, Nodo salida) {

	for (int i = 0; i < R; ++i)
		for (int j = 0; j < C; ++j)
			dist[i][j] = INF;

	dist[entrada.fila][entrada.columna] = 0;

	queue<Nodo> q;
	q.push(entrada);

	while (!q.empty()) {
		Nodo u = q.front();
		q.pop();
		int i = u.fila, j = u.columna;

		for (int k = 0; k < 4; ++k) {
			int ni = i + di[k], nj = j + dj[k];

			if (ni >= 0 && ni < R && nj >= 0 && nj < C && mapa[ni][nj] && dist[ni][nj] == INF) {
				dist[ni][nj] = dist[i][j] + 1;
				padre[ni][nj] = u;

				if (ni == salida.fila && nj == salida.columna)
					break;

				q.push(Nodo(ni, nj));
			}
		}
	}

	vector<Nodo> camino;

	int d = dist[salida.fila][salida.columna];

	if (d < INF) {
		camino.resize(d + 1);
		int pos = d;
		Nodo paso = salida;

		while (pos >= 0) {
			camino[pos] = paso;
			paso = padre[paso.fila][paso.columna];
			--pos;
		}
	}

	return camino;
}

int distancia(Point x1, Point x2)/// entre adyacentes
{
	return (int)sqrt(pow(x1.x - x2.x, 2) + pow(x1.y - x2.y, 2));
}

Point centro(Point Izq, Point Op) {		/// Ecuentra el centro del cuadrado

	int Px, Py;
	Px = ((Op.x - Izq.x) / 2) + Izq.x;
	Py = ((Op.y - Izq.y) / 2) + Izq.y;
	Point centro(Px, Py);
	return centro;
}

int DeteccionColor(Mat cortada)				// Analisis de color
{
	int cn = 0, cb = 0, cRGB = 0;
	for (int i = 0; i < cortada.rows; i++)
	{
		for (int j = 0; j < cortada.cols; j++)
		{
			if (cortada.at<uchar>(i, j) > 200)
			{
				cb++;
			}
			if (cortada.at<uchar>(i, j) < 50)
			{
				cn++;
			}
			/*		if(cortada.at<Vec3b>(i,j)[0]==225 && cortada.at<Vec3b>(i, j)[1]==225 && cortada.at<Vec3b>(i, j)[2]==0 )
			{
			return int(3); cRGB++;
			}*/
		}
	}

	if ((cn) > cb) { return 1; }		// Negro
	if (cn < cb) { return 0; }		// Blanco
	else return 1;
}

vector<Point> Eliminar (vector<Point> puntos, int lugares)
{
	for (int i =0 ; i < lugares; i++) 
	{
		puntos.pop_back();
	}
	return puntos;
}

bool Pared(vector<Point> negros, Point lugar, int lado)
{
	int aux = 0;
	for (int i = 0; i < negros.size(); i++)
	{
		cout << "EStoy comparando " << negros.at(i) << " Y " << lugar <<" - "<< distancia(negros.at(i), lugar) << endl;
		if (distancia(negros.at(i),lugar) == lado) aux++;
	}
	if (aux >= 2) return 0;  /// no tiene salida 
	if (aux <= 2) return 1; //  salida 
}

bool Retroceso(vector<Point> camino, Point lugar,int lado)
{
	int aux=0;
	for (int i = 0; i < camino.size(); i++)
	{
		if(distancia(lugar, camino.at(i)) == lado) aux++;
	}
	if (aux == 1) return 0;	// retroceso
	if (aux == 2) return 1; //avanza
}

int main()
{
/*
	vector<Nodo> camino = obtenerCamino(Nodo(0, 3), Nodo(8, 9));

	for (int i = 0; i < camino.size(); ++i)
		printf("%d %d\n", camino[i].fila, camino[i].columna);

	system("pause");

	exit(0);
	*/

	// Declaracion de Variables 
	vector<Point> coordenada;			// Vector donde se almacenan los datos de las coordenadas en pixeles
	vector<Point> negros;				// Vector donde se almacenan los datos de las coordenadas en pixeles negros
	vector<Mat> corte;					// Vector de la matrices de las imagenes cortadas
	vector<Point>caminoX;				// Vecto con las coordenadas del camino
	vector<Point> auxCamino;
	int xMax, yMax;						// Valores maximos del cuadro 





	Mat img0 = imread("p5.jpg", 1);		// imagen Original
	Mat img;

	resize(img0, img, Size(600, 500), INTER_LINEAR);	// Redimensiona a => 800x600

	xMax = img.size().width;
	yMax = img.size().height;

	cout << xMax << "," << yMax;
	int lado = 100;					// lado del cuadrado minimo 
									//////////////////////////////////////////////////////////////////////////

									////////////////////////// BUSCA SALIDA//////////////////////////

	Mat Copy, imagen;
	img.copyTo(imagen);
	imagen.copyTo(Copy);

	// Transformacion A Binario
	cvtColor(Copy, Copy, CV_BGR2GRAY);
	Mat Copyaux;
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

	cvtColor(imagen, img, CV_BGR2GRAY);

	//Copyaux.copyTo(img);
	imshow(" IAMIDMASDIASMDSAIDMASI", img);

	//////////////////////////////////////////////////////////////////////////


	/////////////////Elementros para la cuadriculacion////////////////////////
	int
		vix = 37,					// Coordenadas del vertice Izquierdo eje x
		viy = 36,					// Coordenadas del vertice Izquierdo eje y 
		vox = lado + vix,			// Coordenadas del vertice Opuesto eje x 
		voy = lado + viy;			// Coordenadas del vertice Opuesto eje y
									//////////////////////////////////////////////////////////////////////////
	int l1 = 0, l2 = 0, l3 = 0, l4 = 0;

	for (int i = 0; i < (xMax / lado) - 1; i++)
	{
		for (int j = 0; j < (yMax / lado) - 1; j++)
		{
			l1 = vix + lado*i; l2 = viy + lado*j;
			l3 = vox + lado*i; l4 = voy + lado*j;

			cout << endl << "[ " << l1 << "," << l2 << "] " << " " << "[ " << l3 << "," << l4 << "] " << endl;
			Point central = centro(Point(l1, l2), Point(l3, l4));	// Determina del Centro del Cuadrado de Lado x Lado
			cout << "Coord" << (int)central.x << "," << central.y << endl;

			if (i < ((xMax / lado) - 1) && j < ((yMax / lado) - 1)) //66
			{
				Rect r(l1, l2, l3, l4);
				Mat smallImg = img(Rect(Point(l1, l2), Point(l3, l4)));

				if (DeteccionColor(smallImg) == 1)
				{
					cout << "Negro =>" << DeteccionColor(smallImg) << endl;
				}

				if (DeteccionColor(smallImg) == 0)
				{
					cout << "Blanco =>" << DeteccionColor(smallImg) << endl;
				}

				imshow("Corte", smallImg);
				waitKey(0);
				cout << " qwerty: " << DeteccionColor(smallImg) << endl;

				if (DeteccionColor(smallImg) == 0)// || DeteccionColor(smallImg) == 1)
				{	// Solo los cuadros que se determinen como blanco seran escritos 
					rectangle(img, Point(l1, l2), Point(l3, l4), Scalar(255, 255, 0), 1, 8, 0);
					circle(img, centro(Point(l1, l2), Point(l3, l4)), 50, Scalar(255, 0, 0), 2);
					coordenada.push_back(centro(Point(l1, l2), Point(l3, l4)));
					auxCamino.push_back(centro(Point(l1, l2), Point(l3, l4)));
				}

				if (DeteccionColor(smallImg) == 1)
				{	// cuarda los negros 
					negros.push_back(centro(Point(l1, l2), Point(l3, l4)));
				}
			}


			string msg = "*" + to_string(central.x) + "," + to_string(central.y);
			putText(img, msg, central, FONT_HERSHEY_SIMPLEX, 0.3, CV_RGB(0, 0, 255), 1, CV_AA);
			cout << "Centrol" << central << endl;
			cout << "i : " << i << " MAX " << (xMax / lado) << endl;
		}
	}

		cout << "DIME DEL VECTOR PPEPR : " << coordenada.size() << endl;
		cout << "Distancia : " << distancia(Point(87, 96), Point(187, 96)) << endl;
		for (int i = 0; i < coordenada.size(); i++)
		{
			cout << "i: " << i << " -" << coordenada.at(i) << endl;
		}
		cout << "Negro" << endl;
		for (int i = 0; i < negros.size(); i++)
		{
			cout << "i: " << i << " -" << negros.at(i) << endl;
		}
		//		cout << "DIM ADA ADN : " <<int(img.at<uchar>(87,200)) <<" - " <<endl ;

		// Buscar la Salida 
		Point Entrada = Point(100, 10);
		Point Salida = Point(520, 520);
		int menor = 10000;
		int aux = 0;
		int cercaSalida[2];
		int cercaEntrada[2];
		vector<Point> E_S; // [0] mas cerca de la salida  [1] mas cerca de la entrada 

		// Mas Cerca de la Salida
		for (int i = 0; i < coordenada.size(); i++)
		{
			//cout << "Caso :" <<i<<"_ "<< distancia(Salida, coordenada.at(i)) << " Y " << distancia(Salida, coordenada.at(i+1)) << endl;
			if (distancia(Salida, coordenada.at(i)) < menor)
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
			if (distancia(Entrada, coordenada.at(i)) < menor)
			{
				menor = distancia(Entrada, coordenada.at(i));
				//			cout << "Mas Cerca de la Entrada" << coordenada.at(i) << " - " << distancia(Salida, coordenada.at(i)) << endl;
				cercaEntrada[0] = coordenada.at(i).x; cercaEntrada[1] = coordenada.at(i).y;
			}

		}

		cout << "Mas Cerca de la Entrada [" << cercaEntrada[0] << "," << cercaEntrada[1] << "] " << endl;
		cout << "Mas Cerca de la Salida [" << cercaSalida[0] << "," << cercaSalida[1] << "] " << endl;

		menor = 10000;
		// Buscador de Caminos
		for (int i = 0; i < coordenada.size(); i++)
		{
			//cout << "Caso :" <<i<<"_ "<< distancia(Salida, coordenada.at(i)) << " Y " << distancia(Salida, coordenada.at(i+1)) << endl;
			if (distancia(Point(cercaEntrada[0], cercaEntrada[1]), coordenada.at(i)) == lado)
			{
				aux++;
				cout << "IR POR " << coordenada.at(i) << "- Distancia " << distancia(Point(cercaEntrada[0], cercaEntrada[1]), coordenada.at(i)) << endl;

			}
		}cout << "Caminos Detectados " << aux << endl;


// Busca Adyacentes
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

		//Copia
		for (int i = 0; i < coordenada.size(); i++)
		{
			//auxCamino.at(i)=coordenada.at(i);
		}

		//auxCamino.at(1) = coordenada.at(1);
		/// Ralacion entre adyacentes  para encontrar la salida

		Point IN = Point(cercaEntrada[0], cercaEntrada[1]);
		Point OUT = Point(cercaSalida[0], cercaSalida[1]);

		Point Nodo;
		int contA = 0, contB = 0;
		int i = 0;

		for (int j = 0; j < auxCamino.size(); j++)
		{
			if (j == 0) { Point OUT = Point(cercaSalida[0], cercaSalida[1]); }

			cout << "Estoy comparando " << OUT << " y " << auxCamino.at(j) << endl;
			
			if (distancia(OUT, auxCamino.at(j)) == lado)
			{
				cout << "I : " << j - 1 << "camino" << auxCamino.at(j) << endl;
				contA++;
				caminoX.push_back(auxCamino.at(j));
				//	else auxCamino.pop_back();
			}
			else
			{
				Nodo = auxCamino.at(j - 1);
				if (Pared(negros, Nodo) == 1)
				{
					cout << "Callejon sin salida ";
					negros.push_back(auxCamino.at(j));
					break;
				}

			}

		}
		
	// Generador de Nuevas Paredes	
		for (int j = 1; j < auxCamino.size(); j++)
		{
			if (Pared(negros, auxCamino.at(j),lado) == 0 && auxCamino.at(j) != Point(cercaSalida[0], cercaSalida[1] ))
				{
					cout << "nuevo Negro "<<endl;
					negros.push_back(auxCamino.at(j));
					//break;
				}		
		}
		
		Point Interno= Point(0,0);
		while (Interno != Point(cercaEntrada[0], cercaEntrada[1]))
		{
			for (int i = 0; i < auxCamino.size(); i++)
			{
				auxCamino.at(i);
			}
			
			cout << "El interno es " << Interno  <<endl;	
			break;
		}
		
		for (int i = 0; i < negros.size(); i++)
		{
			cout << "i: " << i << " -" << negros.at(i) << endl;
		}

		imshow("Muestra", img);

		while (char(waitKey(30)) != 27) {}
		return 0;

	}
