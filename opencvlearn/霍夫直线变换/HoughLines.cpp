#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, const char *argv[])
{
	Mat src = imread("Lines.png");  //加载原图像
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	namedWindow("input", WINDOW_AUTOSIZE);
	imshow("input", src);

	// 去噪声与二值化
	Mat gray, binary;
	Canny(src, binary, 80, 160, 3, false);
	imshow("binary", binary);
	imwrite("binary.png", binary);

	vector<Vec4i> lines;
	//二值图像，返回直线两个点，极坐标r的步长，角度步长，累加器阈值，最小线段长度，线段间隔
	HoughLinesP(binary, lines, 1, CV_PI / 180, 80, 30, 10);
	Mat result = Mat::zeros(src.size(), src.type());
	for (size_t i = 0; i < lines.size(); i++)
	{
		line(result, Point(lines[i][0], lines[i][1]),
			Point(lines[i][2], lines[i][3]), Scalar(0, 255, 0), 1, 8);
	}
	imshow("contours", result);

	waitKey(0);
	return 0;
}
