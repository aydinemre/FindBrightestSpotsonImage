// Main.cpp
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/highgui/highgui.hpp"

#include<ctime>
#include<iostream>
#include "EAPoint.h"

using namespace std;
using namespace cv;

int main();
/***
 *
 * This function takes 3 EAPoints object and original images.And fiils EAPoint objects.
 *
 * @param imgOrg : original images all operations doing on this picture.
 * @param first  : this EAPoint holds brightest point information on the image
 * @param second : this EAPoint holds second brightest point information on the image
 * @param third  : this EAPoint holds third brightest point information on the image
 *
 */
void findBrightest(const Mat &imgOrg, EAPoint &first, EAPoint &second, EAPoint &third);
/***
 *
 * This function takes an grayscale image and a point.And this function arranges
 * point center and rearrange this point center.
 *
 *
 **/
void arrangeCenter(const Mat &imgGrayscale, EAPoint &point);
int main() {

	VideoCapture capWebcam(0);            // declare a VideoCapture object and associate to webcam, 0 => use 1st webcam

	if (capWebcam.isOpened() == false) {                                // check if VideoCapture object was associated to webcam successfully
		cout << "error: capWebcam not accessed successfully\n\n";       // if not, print error message to std out
		return(0);                                                      // and exit program
	}

	Mat imgOriginal;        // input image

	EAPoint firstBrightestPoint,
		secondBrightestPoint,
		thirdBrightestPoint;

	time_t start, end;

	int counter = 0;
	double sec;
	double fps;
	char charCheckForEscKey = 0;

	while (charCheckForEscKey != 27 && capWebcam.isOpened()) {            // until the Esc key is pressed or webcam connection is lost
																			// fps counter begin
		if (counter == 0) {
			time(&start);
		}

		bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);            // get next frame

		if (!blnFrameReadSuccessfully || imgOriginal.empty()) {                 // if frame not read successfully
			cout << "error: frame not read from webcam\n";                 // print error message to std out
			break;                                                              // and jump out of while loop
		}

		findBrightest(imgOriginal, firstBrightestPoint, secondBrightestPoint, thirdBrightestPoint);

		circle(imgOriginal,
			Point(firstBrightestPoint.getX(), firstBrightestPoint.getY()),
			firstBrightestPoint.getRadius(),
			Scalar(0, 255, 0),
			2);

		putText(imgOriginal, "1", Point(firstBrightestPoint.getX(), firstBrightestPoint.getY()), FONT_HERSHEY_SIMPLEX,1, Scalar(0, 255, 0));

		circle(imgOriginal,
			Point(secondBrightestPoint.getX(), secondBrightestPoint.getY()),
			secondBrightestPoint.getRadius(),
			Scalar(0, 0, 255),
			2);
		putText(imgOriginal, "2", Point(secondBrightestPoint.getX(), secondBrightestPoint.getY()), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255));


		circle(imgOriginal,
			Point(thirdBrightestPoint.getX(), thirdBrightestPoint.getY()),
			thirdBrightestPoint.getRadius(),
			Scalar(255, 0, 0),
			2);
		putText(imgOriginal, "3", Point(thirdBrightestPoint.getX(), thirdBrightestPoint.getY()), FONT_HERSHEY_SIMPLEX, 1, Scalar(255,0, 0));


		firstBrightestPoint.setValue(0);
		secondBrightestPoint.setValue(0);
		thirdBrightestPoint.setValue(0);

		namedWindow("imgOriginal", CV_WINDOW_NORMAL);       // note: you can use CV_WINDOW_NORMAL which allows resizing the window																// CV_WINDOW_AUTOSIZE is the default
		imshow("imgOriginal", imgOriginal);                 // show windows

		time(&end);
		++counter;
		sec = difftime(end, start);
		fps = counter / sec;
		putText(imgOriginal, String("FPS:"), Point(0,0), FONT_HERSHEY_SIMPLEX, 1, Scalar(255,0, 0));

		cout << "fps : " << fps << endl;
		charCheckForEscKey = waitKey(1);        // delay (in ms) and get key press, if any
	}   // end while

	return(0);
}

void findBrightest(const Mat &imgOrg, EAPoint &first, EAPoint &second, EAPoint &third)
{
	 static Mat imgGrayscale = Mat(imgOrg.size(), CV_8UC1);

	for (int y = 0; y < imgOrg.rows; y++)
	{
		for (int x = 0; x < imgOrg.cols; x++)
		{

			int blue = imgOrg.at<Vec3b>(y, x).val[0];
			int green = imgOrg.at<Vec3b>(y, x).val[1];
			int red = imgOrg.at<Vec3b>(y, x).val[2];

			double gray = (blue*0.0722) + (green*0.7152) + (red*0.2126);
			imgGrayscale.at<uchar>(y, x) = gray;

			double value = first.getValue();
			if (gray > value) {
				first.setValue(gray);
				first.setX(x);
				first.setY(y);
			}
			else if ((value - 40 > gray) && (gray >= (value - 70)))
			{
				second.setValue(gray);
				second.setX(x);
				second.setY(y);
			}
			else if ((value - 70 > gray) && (gray >= (value - 140)))
			{
				third.setValue(gray);
				third.setX(x);
				third.setY(y);
			}
		}
	}

	arrangeCenter(imgGrayscale, first);
	arrangeCenter(imgGrayscale, second);
	arrangeCenter(imgGrayscale, third);

}


void arrangeCenter(const Mat &imgGrayscale, EAPoint &point) {

	int left = 0;
	int right = 0;
	int up = 0;
	int down = 0;

	int xCoor = point.getX();
	int yCoor = point.getY();

	// Check right.
	for (int x = xCoor; x < imgGrayscale.cols; ++x)
	{
		if ((point.getValue() - imgGrayscale.at<uchar>(yCoor, x) < 10) &&
			(point.getValue() - imgGrayscale.at<uchar>(yCoor, x) > 0))
			++right;

		else break;

	}

	// Check left.
	for (int x = xCoor; x > 0; --x)
	{
		if ((point.getValue() - imgGrayscale.at<uchar>(yCoor, x) < 10) &&
			(point.getValue() - imgGrayscale.at<uchar>(yCoor, x) > 0))
			--left;
		else break;

	}

	// Check down.
	for (int y = yCoor; y < imgGrayscale.rows; ++y)
	{
		if ((point.getValue() - imgGrayscale.at<uchar>(y, xCoor) < 10) &&
			(point.getValue() - imgGrayscale.at<uchar>(y, xCoor) > 0))
			++down;
		else break;

	}

	// Check up.
	for (int y = yCoor; y > 0; --y)
	{
		if ((point.getValue() - imgGrayscale.at<uchar>(y, xCoor) < 10) &&
			(point.getValue() - imgGrayscale.at<uchar>(y, xCoor) > 0))
			--up;
		else break;

	}
	point.setY(yCoor + ((up + down) / 2));
	point.setX(xCoor + ((left + right) / 2));

	point.setRadius((abs(left) + right) / 2);
}
