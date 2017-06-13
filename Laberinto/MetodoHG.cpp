#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <queue>
#include <vector>

#define xMax 600
#define yMax 500
#define lado 100
#define RMAX (yMax/lado)-1
#define CMAX (xMax/lado)-1
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

const int di[] = { 1, -1, 0, 0 };
const int dj[] = { 0, 0, 1, -1 };

int mapa[RMAX][CMAX] = {
	{ 1,1,1,1,1 },
	{ 1,0,0,0,1 },
	{ 1,0,0,0,1 },
	{ 1,0,0,1,1 },
};
int dist[RMAX][CMAX];
int R = 4, C = 5;
Nodo padre[RMAX][CMAX];

vector<Nodo> obtenerCamino(Nodo entrada, Nodo salida, int map[RMAX][CMAX]) {

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

			if (ni >= 0 && ni < R && nj >= 0 && nj < C && map[ni][nj] && dist[ni][nj] == INF) {
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
			if (cortada.at<uchar>(i, j) < 100)
			{
				cn++;
			}
		}
	}

	if ((cn) > cb) { return 1; }		// Negro
	if (cn < cb) { return 0; }		// Blanco
	else return 1;
}

vector<Point> Eliminar(vector<Point> puntos, int lugares)
{
	for (int i = 0; i < lugares; i++)
	{
		puntos.pop_back();
	}
	return puntos;
}

bool Pared(vector<Point> negros, Point lugar)
{
	int aux = 0;
	for (int i = 0; i < negros.size(); i++)
	{
		cout << "EStoy comparando " << negros.at(i) << " Y " << lugar << " - " << distancia(negros.at(i), lugar) << endl;
		if (distancia(negros.at(i), lugar) == lado) aux++;
	}
	if (aux >= 2) return 0;  /// no tiene salida 
	if (aux <= 2) return 1; //  salida 
}

bool Retroceso(vector<Point> camino, Point lugar)
{
	int aux = 0;
	for (int i = 0; i < camino.size(); i++)
	{
		if (distancia(lugar, camino.at(i)) == lado) aux++;
	}
	if (aux == 1) return 0;	// retroceso
	if (aux == 2) return 1; //avanza
}

char comandoGuia(Point Aranita, Point Nodo)
{
	char w, s, a, d, q;
	int min = 10;
	int pixMin = 3;

	if ((Nodo.x - Aranita.x) < min && distancia(Aranita, Nodo) > pixMin)
	{
		return 'w'; /// ir adelante
		cout << "Ir adelante" << endl;
	}

	if ((Nodo.y - Aranita.y) < min && distancia(Aranita, Nodo) > pixMin)
	{
		return 's'; /// ir Izq
		cout << "Ir IZQ" << endl;
	}

	if ((Nodo.y - Aranita.y) < 0 && distancia(Aranita, Nodo) > pixMin)
	{
		return 'a'; /// ir Derecha
		cout << "Ir Derecha" << endl;
	}
}

int main()
{
	// Declaracion de Variables 
	vector<Point> coordenada;			// Vector donde se almacenan los datos de las coordenadas en pixeles
	vector<Point> negros;				// Vector donde se almacenan los datos de las coordenadas en pixeles negros
	vector<Mat> corte;					// Vector de la matrices de las imagenes cortadas
	vector<Point>caminoX;				// Vecto con las coordenadas del camino
	vector<Point> auxCamino;
	Point  MATIX[RMAX][CMAX];

	Mat img0 = imread("maze3.jpg", 1);		// imagen Original
	Mat img;

	resize(img0, img, Size(xMax, yMax), INTER_LINEAR);	// Redimensiona a => 800x600
	img.copyTo(img0);

	cout << xMax << "," << yMax;


	cvtColor(img, img, CV_BGR2GRAY);

	//Copyaux.copyTo(img);

	//////////////////////////////////////////////////////////////////////////


	/////////////////Elementros para la cuadriculacion////////////////////////
	int
		vix = 50,					// Coordenadas del vertice Izquierdo eje x
		viy = 50,					// Coordenadas del vertice Izquierdo eje y 
		vox = lado + vix,			// Coordenadas del vertice Opuesto eje x 
		voy = lado + viy;			// Coordenadas del vertice Opuesto eje y
									//////////////////////////////////////////////////////////////////////////
	int l1 = 0, l2 = 0, l3 = 0, l4 = 0;
	int fila = (xMax / lado)-1;
	int columna = (yMax / lado) - 1;

	cout<<endl << "Fila es : " << fila << "Columna es : " << columna << endl;

	int zerouno[RMAX][CMAX];

	for (int i = 0; i < (xMax / lado) ; i++)
	{
		for (int j = 0; j < (yMax / lado) ; j++)
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
	//			waitKey(0);
				cout << " qwerty: " << DeteccionColor(smallImg) << endl;

				if (DeteccionColor(smallImg) == 0)// || DeteccionColor(smallImg) == 1)
				{	// Solo los cuadros que se determinen como blanco seran escritos 
					rectangle(img0, Point(l1, l2), Point(l3, l4), Scalar(255, 255, 0), 1, 8, 0);
					circle(img0, centro(Point(l1, l2), Point(l3, l4)), 50, Scalar(255, 0, 0), 2);
					coordenada.push_back(centro(Point(l1, l2), Point(l3, l4)));
					zerouno[j][i] = 1;
					MATIX[j][i] = Point (centro(Point(l1, l2), Point(l3, l4)).x, centro(Point(l1, l2), Point(l3, l4)).y);
				}

				if (DeteccionColor(smallImg) == 1)
				{	// cuarda los negros 
					zerouno[j][i] = 0;
					negros.push_back(centro(Point(l1, l2), Point(l3, l4)));
					auxCamino.push_back(centro(Point(l1, l2), Point(l3, l4)));
					MATIX[j][i] = Point (centro(Point(l1, l2), Point(l3, l4)).x, centro(Point(l1, l2), Point(l3, l4)).y);
				}
			}


			string msg = "*" + to_string(central.x) + "," + to_string(central.y);
			putText(img0, msg, central, FONT_HERSHEY_SIMPLEX, 0.3, CV_RGB(0, 0, 255), 1, CV_AA);
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
	Point Entrada = Point(200, 10);
	Point Salida = Point(400, 520);
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
	
	// Calculo de los inicios con respecto a la representacion binaria de la martix laberinto

	Nodo Ent,Sal;
	for (int i = 0; i<RMAX; i++)
	{
		for (int j = 0; j< CMAX; j++)
		{
			if (MATIX[i][j] == Point(cercaEntrada[0], cercaEntrada[1])) Ent = Nodo(i, j);
			
			if (MATIX[i][j] == Point(cercaSalida[0], cercaSalida[1])) Sal = Nodo(i, j);

		}
	}
	cout << "Cerca entrada y Salida : " << Ent.fila << ", " << Ent.columna << " - " << Sal.fila << ", " << Sal.columna << endl;
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

	Point IN = Point(cercaEntrada[0], cercaEntrada[1]);
	Point OUT = Point(cercaSalida[0], cercaSalida[1]);

	for (int i = 0; i<RMAX; i++)
	{
		for (int j = 0; j< CMAX; j++)
		{
			cout <<i<<","<<j <<" "<< zerouno[i][j] <<"  ";
			if (j == CMAX) cout << endl;
		}
	}

	imshow("Muestra", img0);

	vector<Nodo> camino = obtenerCamino(Ent, Sal,zerouno);

	for (int i = 0; i < camino.size(); ++i)
	{
		caminoX.push_back(MATIX[camino[i].fila][camino[i].columna]);

		circle(img0, MATIX[camino[i].fila][camino[i].columna] , 25, Scalar(0, 0 , 255), 2);
	}

	
	for (int i = 0; i < caminoX.size(); ++i)
	{
		if (i < caminoX.size()-1)
			line(img0, caminoX.at(i), caminoX.at(i+1), Scalar(0, 0, 255), 2, 8, 0);
		
	}

	// La salida es 
	cout << "salida" << endl;
	for (int i = 0; i < caminoX.size(); ++i)
	{
		cout << " i " <<i<<": "<< caminoX.at(i);

	}

//	caminoX.pop_back();
	
	cout <<"camino "<<caminoX.size()<< endl << endl;
	Point Aranita;
	int i = 0;
	/*
	while (!camino.empty())
	{
		if (distancia(Aranita, caminoX.at(i)) > 0) comandoGuia(Aranita, caminoX.at(i));

		if (distancia(Aranita, caminoX.at(i)) == 0) i++;
	}
	*/
	
	for (int i = 0; i < caminoX.size(); ++i)
	{
		//printf("%d %d\n", camino[i].fila, camino[i].columna);
		cout << " i " << i << ": " << caminoX.at(i);
	}

	imshow("Muestra", img0);

	while (char(waitKey(30)) != 27) {}
	return 0;
	
}
