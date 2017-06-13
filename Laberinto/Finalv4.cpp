#include <opencv2\opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include <string>
#include <vector>
#include <sstream>
#include <queue>

using namespace std;
using namespace cv;

// Grafica el rectangulo 
// Variables GLOBALES
const Scalar Blanco = Scalar(255.0, 255.0, 255.0);//
const Scalar Negro = Scalar(0.0, 0.0, 0.0);//
const Scalar Azul = Scalar(255.0, 0.0, 0.0);//
const Scalar Verde = Scalar(0.0, 255.0, 0.0);//
const Scalar Rojo = Scalar(0.0, 0.0, 255.0);//
											//
/*//*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

int Camara = 0;

// 1 para webcam externa
// 0 para webcam de la note


/*//*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

#define xMax 600
#define yMax 500
#define lado 100
#define RMAX (yMax/lado)-1
#define CMAX (xMax/lado)-1
#define INF 1000000000

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

int dist[RMAX][CMAX];
int R = 4, C = 5;
Nodo padre[RMAX][CMAX]; 
Mat frame;
Mat guia(200, 650, CV_8UC3);

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

void EntradaSalida()
{
	/////http://www.linuxhispano.net/2012/12/03/capturar-imagenes-desde-camara-opencv/
	VideoCapture TheVideoCapturer;
	Mat bgrMap;

	char key = 0;

	int numSnapshot = 0;
	std::string snapshotFilename = "0";

	std::cout << "presionar 's' para tomar una captura " << std::endl;
	std::cout << "Presionar 'Esc' para Continuar" << std::endl;

	TheVideoCapturer.open(Camara);

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

char comandoGuia(Point Aranita, Point Nodo)
{
	char w, s, a, d, q;
	string msg;
	int min = 10;
	int pixMin = 3;

	if ((Nodo.x - Aranita.x) < min && distancia(Aranita, Nodo) > pixMin)
	{
		return 'w'; /// ir adelante
		cout << "Ir adelante" << endl;
		putText(guia, "Avanzar", Point(10, 100), 4, 1, CV_RGB(0, 255, 0), 0, CV_AA);
		imshow("GUIA", guia);
	}

	if ((Nodo.y - Aranita.y) < min && distancia(Aranita, Nodo) > pixMin)
	{
		return 's'; /// ir Izq
		cout << "Ir IZQ" << endl;
		putText(guia, "Giro", Point(10, 100), 4, 1, CV_RGB(0, 255, 0), 0, CV_AA);
		imshow("GUIA", guia);
	}

	if ((Nodo.y - Aranita.y) < 0 && distancia(Aranita, Nodo) > pixMin)
	{
		return 'a'; /// ir Derecha
		cout << "Ir Derecha" << endl;
		putText(guia, "Giro", Point(10, 100), 4, 1, CV_RGB(0, 255, 0), 0, CV_AA);
		imshow("GUIA", guia);
	}
}


void main()
{

	/* // Arduino
	// Setup serial port connection and needed variables.
	HANDLE hSerial = CreateFile("COM6", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	
	stringstream ss;
	string buff;
	CString data;
	
	//char outputChars[] = "c";
	DWORD btsIO;


	if (hSerial != INVALID_HANDLE_VALUE)
	{
		printf("Port opened! \n");

		DCB dcbSerialParams;
		GetCommState(hSerial, &dcbSerialParams);

		dcbSerialParams.BaudRate = CBR_9600;
		dcbSerialParams.ByteSize = 8;
		dcbSerialParams.Parity = NOPARITY;
		dcbSerialParams.StopBits = ONESTOPBIT;

		SetCommState(hSerial, &dcbSerialParams);
	}
	else
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			printf("Serial port doesn't exist! \n");
		}

		printf("Error while setting up serial port! \n");
	}
	
	
	
	
	
	
	*/




////////////////////////////////////Declaracion de Variables////////////////////////////////////////////////////////////////
	// Rectangulo Verde 

	Mat hsv_Azul, binary_Azul;			// Matrices de HiperSaturacion y Binario
	int c = 0;							// Aux para deteccion de puntos
	vector<Point> deteccion;		// vector de posiciones para la entrada y salida

	// Solucion del laberinto
	vector<Point> coordenada;			// Vector donde se almacenan los datos de las coordenadas en pixeles
	vector<Point> negros;				// Vector donde se almacenan los datos de las coordenadas en pixeles negros
	vector<Mat> corte;					// Vector de la matrices de las imagenes cortadas
	vector<Point>caminoX;				// Vecto con las coordenadas del camino
	Point  MATIX[RMAX][CMAX];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	EntradaSalida();				// Funcion para sacar una captura 
	destroyAllWindows();
	

	Mat img0 = imread("0.jpg");
//	Mat img0 = imread("laberinto.jpg");
	Mat img;
	resize(img0, img, Size(xMax, yMax), INTER_LINEAR);	// Redimensiona a => 800x600
	img.copyTo(img0);
	img.copyTo(frame);
	imshow("adsadsadas", img);
	waitKey(0);
	cout << endl << "Varificar si encontro la entrada y salida Entrer para continuar" << endl;

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
		c++;
	}
	imshow("Original", frame);
	imshow("BInariio", binary_Azul);

	// Para ver los las lugares 

	for (int i = 0; i < deteccion.size(); i++)
	{
		cout << "\ni " << i << " " << deteccion.at(i) << endl;
	}
	c = 0;
	waitKey(0);

	///// Cuadriculacion
	cvtColor(img, img, CV_BGR2GRAY);
	//////////////////////////////////////////////////////////////////////////


	/////////////////Elementros para la cuadriculacion////////////////////////
	int
		vix = 50,					// Coordenadas del vertice Izquierdo eje x
		viy = 50,					// Coordenadas del vertice Izquierdo eje y 
		vox = lado + vix,			// Coordenadas del vertice Opuesto eje x 
		voy = lado + viy;			// Coordenadas del vertice Opuesto eje y
	//////////////////////////////////////////////////////////////////////////
	int l1 = 0, l2 = 0, l3 = 0, l4 = 0;
	int fila = (xMax / lado) - 1;
	int columna = (yMax / lado) - 1;

	cout << endl << "Fila es : " << fila << "Columna es : " << columna << endl;

	int zerouno[RMAX][CMAX];

	// Cuadriculacion
	for (int i = 0; i < (xMax / lado); i++)
	{
		for (int j = 0; j < (yMax / lado); j++)
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
					MATIX[j][i] = Point(centro(Point(l1, l2), Point(l3, l4)).x, centro(Point(l1, l2), Point(l3, l4)).y);
				}

				if (DeteccionColor(smallImg) == 1)
				{	// cuarda los negros 
					zerouno[j][i] = 0;
					negros.push_back(centro(Point(l1, l2), Point(l3, l4)));
					MATIX[j][i] = Point(centro(Point(l1, l2), Point(l3, l4)).x, centro(Point(l1, l2), Point(l3, l4)).y);
				}
			}


			string msg = "*" + to_string(central.x) + "," + to_string(central.y);
			putText(img0, msg, central, FONT_HERSHEY_SIMPLEX, 0.3, CV_RGB(0, 0, 255), 1, CV_AA);
			cout << "Centrol" << central << endl;
			cout << "i : " << i << " MAX " << (xMax / lado) << endl;
		}
	}

	cout <<endl<< "Dimension Vector Coordenadas : " << coordenada.size() << endl;

	// ver el vector generado
	for (int i = 0; i < coordenada.size(); i++)
	{
		cout << "i: " << i << " -" << coordenada.at(i) << endl;
	}

	


	// Buscar la Salida 
	Point Entrada;// = deteccion[0];
	Point Salida;// = deteccion[1];
	int menor = 10000;
	int aux = 0;
	int cercaSalida[2];
	int cercaEntrada[2];
	vector<Point> E_S; // [0] mas cerca de la salida  [1] mas cerca de la entrada 

	// Logica para encontrar la entrada y la salida 

	// para las posiciones de los pixeles, es una matriz desde [0 0] a [Rmax Cmax]

	if (deteccion.at(0).y < deteccion.at(1).y)		/// para un caso espeficio la entrada simpre va a estar cerca del  Y-> 0 mas arriba
	{
		Entrada = deteccion[0];
		Salida = deteccion[1];
	}

	if (deteccion.at(0).y > deteccion.at(1).y)
	{
		Entrada = deteccion[1];
		Salida = deteccion[0];
	}


	 // Mas Cerca de la Salida
	for (int i = 0; i < coordenada.size(); i++)
	{
		if (distancia(Salida, coordenada.at(i)) < menor)
		{
			menor = distancia(Salida, coordenada.at(i));
			cercaSalida[0] = coordenada.at(i).x; 
			cercaSalida[1] = coordenada.at(i).y;
		}
	}

	menor = 1000;
	//MAS cerca entrada
	for (int i = 0; i < coordenada.size(); i++)
	{
		if (distancia(Entrada, coordenada.at(i)) < menor)
		{
			menor = distancia(Entrada, coordenada.at(i));
			cercaEntrada[0] = coordenada.at(i).x;
			cercaEntrada[1] = coordenada.at(i).y;
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

	Nodo Ent, Sal;
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
	//			cout << "adyacentes" << coordenada.at(i) << " Y " << coordenada.at(j) << endl;
			}

		}

	}

	Point IN = Point(cercaEntrada[0], cercaEntrada[1]);
	Point OUT = Point(cercaSalida[0], cercaSalida[1]);

	for (int i = 0; i<RMAX; i++)
	{
		for (int j = 0; j< CMAX; j++)
		{
			cout << i << "," << j << " " << zerouno[i][j] << "  ";
			if (j == CMAX) cout << endl;
		}
	}

	imshow("Muestra", img0);

	vector<Nodo> camino = obtenerCamino(Ent, Sal, zerouno);

	for (int i = 0; i < camino.size(); ++i)
	{
		caminoX.push_back(MATIX[camino[i].fila][camino[i].columna]);

		circle(img0, MATIX[camino[i].fila][camino[i].columna], 25, Scalar(0, 0, 255), 2);
	}


	for (int i = 0; i < caminoX.size(); ++i)
	{
		if (i < caminoX.size() - 1)
			line(img0, caminoX.at(i), caminoX.at(i + 1), Scalar(0, 0, 255), 2, 8, 0);

	}

	// La salida es 
	cout << "salida" << endl;
	for (int i = 0; i < caminoX.size(); ++i)
	{
		cout << " i " << i << ": " << caminoX.at(i);
	}


	cout << "camino " << caminoX.size() << endl << endl;
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

	// Reconocimiento del objeto verde en movimento

	VideoCapture capWebcam(Camara);		// declare a VideoCapture object and associate to webcam, 0 => use 1st webcam

		if (capWebcam.isOpened() == false) {										// check if VideoCapture object was associated to webcam successfully
			std::cout << "error: capWebcam not accessed successfully\n\n";			// if not, print error message to std out
																					// and exit program
		}

		Mat imgOriginal;		// input image
		Mat imgHSV;
		Mat imgThreshLow;
		Mat imgThreshHigh;
		Mat imgThresh;

		vector<Vec3f> v3fCircles;				// 3 element vector of floats, this will be the pass by reference output of HoughCircles()
												//	imgOriginal = imread("rojo.png");
		char charCheckForEscKey = 0;

		while (charCheckForEscKey != 27 && capWebcam.isOpened()) {		// until the Esc key is pressed or webcam connection is lost

			bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);		// get next frame
			resize(imgOriginal, imgOriginal, Size(xMax, yMax), INTER_LINEAR);	// Redimensiona a => 800x600

			if (!blnFrameReadSuccessfully || imgOriginal.empty()) {		// if frame not read successfully
				std::cout << "error: frame not read from webcam\n";		// print error message to std out
				break;													// and jump out of while loop
			}

			cvtColor(imgOriginal, imgHSV, CV_BGR2HSV);

		//	inRange(imgHSV, cv::Scalar(0, 155, 155), cv::Scalar(18, 255, 255), imgThreshLow);
		//	inRange(imgHSV, cv::Scalar(165, 155, 155), cv::Scalar(179, 255, 255), imgThreshHigh);
		//	add(imgThreshLow, imgThreshHigh, imgThresh);

			inRange(imgHSV, Scalar(49, 50, 50), Scalar(80, 255, 255), imgThresh);

			GaussianBlur(imgThresh, imgThresh, cv::Size(3, 3), 0);

			Mat structuringElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

			dilate(imgThresh, imgThresh, structuringElement);
			erode(imgThresh, imgThresh, structuringElement);

			// fill circles vector with all circles in processed image
			HoughCircles(imgThresh,			// input image
				v3fCircles,							// function output (must be a standard template library vector
				CV_HOUGH_GRADIENT,					// two-pass algorithm for detecting circles, this is the only choice available
				2,									// size of image / this value = "accumulator resolution", i.e. accum res = size of image / 2
				imgThresh.rows / 4,				    // min distance in pixels between the centers of the detected circles
				100,								// high threshold of Canny edge detector (called by cvHoughCircles)						
				50,									// low threshold of Canny edge detector (set at 1/2 previous value)
				10,									// min circle radius (any circles with smaller radius will not be returned)
				400);								// max circle radius (any circles with larger radius will not be returned)

			for (int i = 0; i < v3fCircles.size(); i++) {						// for each circle . . .
																				// show ball position x, y, and radius to command line
				std::cout << "ball position x = " << v3fCircles[i][0]			// x position of center point of circle
					<< ", y = " << v3fCircles[i][1]								// y position of center point of circle
					<< ", radius = " << v3fCircles[i][2] << "\n";				// radius of circle

				string msg = "*" + to_string(v3fCircles[i][0]) + "," + to_string(v3fCircles[i][1]);

				putText(imgOriginal, msg, Point(v3fCircles[i][0], v3fCircles[i][1]), FONT_HERSHEY_SIMPLEX, 0.3, CV_RGB(0, 0, 255), 1, CV_AA);

				// draw small green circle at center of detected object
				circle(imgOriginal,												// draw on original image
					cv::Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]),		// center point of circle
					3,																// radius of circle in pixels
					Azul,															// draw pure AZUL (remember, its BGR, not RGB)
					CV_FILLED);														// thickness, fill in the circle

																					// draw red circle around the detected object
				circle(imgOriginal,													// draw on original image
					cv::Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]),		// center point of circle
					(int)v3fCircles[i][2],											// radius of circle in pixels
					cv::Scalar(0, 0, 255),											// draw pure red (remember, its BGR, not RGB)
					3);																// thickness of circle in pixels
			
			/////////////////////////////////// conduccion de la arañita////////////////////////////////////////////////////////////////////////
				Aranita = (Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]));
		
				namedWindow("GUIA", 0);
				//imshow("GUIA", guia);

				if (c < caminoX.size()) 
				{	
					if (distancia(Aranita, caminoX.at(c)) > 0)							//busca la distancia la posicion de la arañita y compara con el primer nodo
					{
						comandoGuia(Aranita, caminoX.at(c));							//  ver funcion  esta al comienzo
						
					}
						

					if (distancia(Aranita, caminoX.at(c)) < 10)							// Si la distancia es baja, pasa al siguiente nodo
					{
						c++;									
						cout << endl << "continuar al SGte" << endl;
					}
				}

				else
				{
					break;
				}

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}	// end for

				// declare windows
			namedWindow("Imagen en Vivo", CV_WINDOW_AUTOSIZE);	// note: you can use CV_WINDOW_NORMAL which allows resizing the window
			namedWindow("Solo Verde", CV_WINDOW_AUTOSIZE);	// or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
																// CV_WINDOW_AUTOSIZE is the default
			namedWindow("Mezclado",CV_WINDOW_AUTOSIZE);

			imshow("Mezclado",img0);
			imshow("Imagen en Vivo", imgOriginal);			// show windows
			imshow("Solo Verde", imgThresh);
			
			charCheckForEscKey = cv::waitKey(1);		// delay (in ms) and get key press, if any
		}//  end while
		 //	waitKey(0);
	
	waitKey(0);

}
