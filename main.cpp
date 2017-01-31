#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int slider_max = 360;
int r_max_val = 90;
int r_min_val = 30;
int l_max_val = 150;
int l_min_val = 90;
int min_len_val = 40;
int y_tresh_val = 60;

int main(int, char**)
{
	//VideoCapture cap(0); // open the default camera
	VideoCapture cap("c:\\Users\\MSI_me\\Downloads\\highway.mp4");
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	bool playVideo = true;
	int max_x;
	int max_y;
	Mat edges, frame, gframe, blframe, sm_frame;
	
	namedWindow("edges", CV_WINDOW_KEEPRATIO);
	namedWindow("original", CV_WINDOW_KEEPRATIO);

	cap >> frame; // get a new frame from camera
	resize(frame, sm_frame, Size(400, 300), 0, 0, 1);
	max_x = sm_frame.cols;
	max_y = sm_frame.rows;
	cvtColor(sm_frame, edges, CV_BGR2GRAY);
	GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
	Canny(edges, edges, 0, 30, 3);

	imshow("edges", edges);
	imshow("edges", sm_frame);
	/// Create Trackbars
	char TrackbarNameRmax[50];
	char TrackbarNameRmin[50];
	char TrackbarNameLmax[50];
	char TrackbarNameLmin[50];
	char TrackbarNameMinLength[50];
	char TrackbarNameYTresh[50];
	sprintf(TrackbarNameRmax, "r_max %d", slider_max);
	createTrackbar(TrackbarNameRmax, "edges", &r_max_val, slider_max);

	sprintf(TrackbarNameRmin, "r_min %d", slider_max);
	createTrackbar(TrackbarNameRmin, "edges", &r_min_val, slider_max);

	sprintf(TrackbarNameLmax, "l_max %d", slider_max);
	createTrackbar(TrackbarNameLmax, "edges", &l_max_val, slider_max);

	sprintf(TrackbarNameLmin, "l_min %d", slider_max);
	createTrackbar(TrackbarNameLmin, "edges", &l_min_val, slider_max);

	sprintf(TrackbarNameMinLength, "min_len %d", 100);
	createTrackbar(TrackbarNameMinLength, "edges", &min_len_val, 100);

	sprintf(TrackbarNameYTresh, "y_tresh %d", 100);
	createTrackbar(TrackbarNameYTresh, "edges", &y_tresh_val, 100);

	for (;;)
	{
		if (playVideo)
		{
			cap >> frame; // get a new frame from camera
			resize(frame, sm_frame, Size(400, 300), 0, 0, 1);
			cvtColor(sm_frame, gframe, CV_BGR2GRAY);
			GaussianBlur(gframe, blframe, Size(7, 7), 1.5, 1.5);
			//Canny(edges, edges, 0, slider_value, 3);
			Canny(blframe, edges, 0, 30, 3);

			vector<Vec4i> lines;
			HoughLinesP(edges, lines, 1, CV_PI / 180, 50, min_len_val, 6);
			for (size_t i = 0; i < lines.size(); i++)
			{
				Vec4i l = lines[i];
				double theta_deg = atan2(l[2] - l[0], l[3] - l[1]) * 180 / CV_PI;
				if ((r_min_val < theta_deg && theta_deg < r_max_val) || (l_min_val < theta_deg && theta_deg < l_max_val))
				{
					if((l[3]>y_tresh_val * max_y / 100)&&(l[1])>y_tresh_val * max_y / 100)
						line(sm_frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2, CV_AA);
				}
			}
			line(sm_frame, Point(0, y_tresh_val * max_y / 100), Point(max_x, y_tresh_val * max_y / 100), Scalar(0, 255, 0), 2, CV_AA);

			imshow("edges", edges);
			imshow("original", sm_frame);
		}
		int kp=waitKey(30);
		if(kp == 'q')
		{ //if user pressed 'q'
			break;
		}
		else if (kp == 'p')
		{ //if user pressed 'p'
			playVideo = !playVideo;
		}
	}
	cap.release();
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}

