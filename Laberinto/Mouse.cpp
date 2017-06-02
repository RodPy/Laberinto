#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

bool ldown = false, lup = false;

Mat img;
Point corner1, corner2;
Rect box;




static void mouse_callback(int event, int x, int y, int, void *)
{
	
	// Al hacer clic sobre la imagen guardamos las coordenadas
	if (event == EVENT_LBUTTONDOWN)
	{
		ldown = true;
		corner1.x = x;
		corner1.y = y;
			
	}

	// Cuando soltamos el botón izquierdo del ratón guardamos la segunda coordenada 
	if (event == EVENT_LBUTTONUP)
	{
		lup = true;
		corner2.x = x;
		corner2.y = y;
	}

	// Actualizar la región seleccionada
	if (ldown == true && lup == false)
	{
		Mat local_img = img.clone();
		rectangle(local_img, corner1, Point(x, y), Scalar(0, 255, 0));
		imshow("Recortar Imagen", local_img);
	}

	// Definir la region seleccionada y mostrarla en una ventana
	if (ldown == true && lup == true)
	{
		box.width = abs(corner1.x - corner2.x);
		box.height = abs(corner1.y - corner2.y);
		box.x = min(corner1.x, corner2.x);
		box.y = min(corner1.y, corner2.y);

		// Recortar la imagen segun el rectangulo de selección y mostrarla
/*		Mat crop(img, box);
		imshow("Imagen Recortada", crop);
*/		ldown = false;
		lup = false;
		
	}
}

int main()
{
	img = imread("laberinto.jpg");
	imshow("Recortar Imagen", img);
	
	
	setMouseCallback("Recortar Imagen", mouse_callback);

	while (char(waitKey(30)) != 27) {}

	return 0;
}