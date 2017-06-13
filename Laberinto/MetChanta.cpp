#include <opencv2\opencv.hpp>
#include<iostream>
#include <string>
#include <vector>
using namespace std;
using namespace cv;

int main() {

	vector<Mat>Cuadriculacion;

	Mat img0 = imread("laberinto.jpg");
	Mat img;
	Rect Rectangulos = Rect(37,46,137,146);
	Mat CroppedImage = img0(Rectangulos);
	//cout << CroppedImage;
	Cuadriculacion.push_back(CroppedImage);
	imshow("Corte", CroppedImage);
	CroppedImage.copyTo(img);
	int cb = 0, cn = 0;


	cout << "Blancos : " << img(0,0) << endl;

	waitKey(0);
}
	//Grayscale matrix
		cv::Mat grayscaleMat(img.size(), CV_8U);

		//Convert BGR to Gray
		cv::cvtColor(img, grayscaleMat, CV_BGR2GRAY);

		//Binary image
		cv::Mat binaryMat(grayscaleMat.size(), grayscaleMat.type());

		//Apply thresholding
		cv::threshold(grayscaleMat, binaryMat, 100, 255, cv::THRESH_BINARY);

		grayscaleMat.copyTo(img);