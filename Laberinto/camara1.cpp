/*


#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<conio.h>           // may have to modify this line if not using Windows
using namespace cv;
using namespace std;

Mat frame0;

static void onMouse(int event, int x,int y, int,void*)
{
	static int count(0);
	if (event == EVENT_MOUSEMOVE)
	{
		stringstream ss;
		ss << count;
		string countStr = ss.str();
		string imageName = "image_" + countStr;
		string FullPath = imageName + ".jpg";

		imwrite(FullPath, frame0);
		imshow("imagen", frame0);
		cout << "Imagen Guardada" << endl;
		++count;
	}
}

int main() {

	string fileName = "2.avi";
	VideoCapture capture(0);

	Mat frame;
	int teclado;

	cvNamedWindow("Web Cam", CV_WINDOW_AUTOSIZE);
	setMouseCallback("Web Cam", onMouse, 0);


	while (true)
	{
		capture >> frame;

		if (!frame.data)
		{
			cerr << "Web Cam no accesible"<< endl;
			break;
		}
	
		imshow("webCam", frame);

		if (waitKey(30) == 27)
		{
			cout << "Precionar ESC para salir" << endl;
			break;
		}
	}
	system("pause");
	return(0);
}



*/





#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <sstream>

cv::VideoCapture TheVideoCapturer;
cv::Mat bgrMap;

int main(int argc, char *argv[]) {
	char key = 0;

	int numSnapshot = 0;
	std::string snapshotFilename = "0";

	std::cout << "Press 's' to take snapshots" << std::endl;
	std::cout << "Press 'Esc' to exit" << std::endl;

	TheVideoCapturer.open(0);

	if (!TheVideoCapturer.isOpened()) {
		std::cerr << "Could not open video" << std::endl;
		return -1;
	}

	while (key != 27 && TheVideoCapturer.grab()) {
		TheVideoCapturer.retrieve(bgrMap);

		cv::imshow("BGR image", bgrMap);

		if (key == 115) {
			cv::imwrite(snapshotFilename + ".png", bgrMap);
			numSnapshot++;
			snapshotFilename = static_cast<std::ostringstream*>(&(std::ostringstream() << numSnapshot))->str();
		}

		key = cv::waitKey(20);
	}
}

