#include <iomanip>   
#include <opencv2/opencv.hpp>  

using namespace std;
using namespace cv;

void main()
{
	FILE *fp1 = NULL;
	if ((fp1 = fopen("D:\\29203\\Desktop\\ex.txt", "r")) == NULL)
	{
		printf("Open file error!");
		return;
	}
	double ex[84][8] = { 0.0 };  //外方位元素
	double f = 0.0705;
	for (int i = 0; i < 84; i++)
	{
		for (int k = 0; k < 8; k++)
		{
			fscanf(fp1, "%lf", &ex[i][k]);
		}
	}

	fclose(fp1);
	/*未知数近似值*/
	
	int j = 0;   //循环次数

	while (j < 84)
	{
		double X = 0, Y = 0, Z = 0;   //未知数,地面点坐标
		double X_gai = 0, Y_gai = 0, Z_gai = 0; //未知数改正数
		double rotate[3][3] = { 0.0 };    //旋转矩阵
		double Xs = 0.0, Ys = 0.0, Zs = 0.0, pitch = 0.0, roll = 0.0, yaw = 0.0;   //线元素，角元素
		double x_ji = 0, y_ji = 0;//定义X计，Y计
		if ((ex[j][0] == 1) || (ex[j][0] == 3) || (ex[j][0] == 4) || (ex[j][0] == 6) || (ex[j][0] == 10) || (ex[j][0] == 12) || (ex[j][0] == 13) || (ex[j][0] == 15))
		{
			int n = 2;  //2张像片
			double(*A)[3] = new double[2 * n][3];     //a系数矩阵
			double *L = new double[2 * n];     //L矩阵
			int t = 0;

			for (int i = 0; i < n; i++)
			{
				double Xs = ex[j + i + 2][1], Ys = ex[j + i + 2][2], Zs = ex[j + i + 2][3];
				pitch = ex[j + i + 2][4], roll = ex[j + i + 2][5], yaw = ex[j + i + 2][6];
				//旋转矩阵 phi = pitch, omiga = roll, kapa = yaw,计算旋转矩阵各元素
				rotate[0][0] = cos(pitch)*cos(yaw) - sin(pitch)*sin(roll)*sin(yaw);
				rotate[0][1] = (-1.0)*cos(pitch)*sin(yaw) - sin(pitch)*sin(roll)*cos(yaw);
				rotate[0][2] = (-1.0)*sin(pitch)*cos(roll);
				rotate[1][0] = cos(roll)*sin(yaw);
				rotate[1][1] = cos(roll)*cos(yaw);
				rotate[1][2] = (-1.0)*sin(roll);
				rotate[2][0] = sin(pitch)*cos(yaw) + cos(pitch)*sin(roll)*sin(yaw);
				rotate[2][1] = (-1.0)*sin(pitch)*sin(yaw) + cos(pitch)*sin(roll)*cos(yaw);
				rotate[2][2] = cos(pitch)*cos(roll);
					
				L[i * 2] = f*(rotate[0][0] * Xs + rotate[1][0] * Ys + rotate[2][0] * Zs) + 
					(ex[j + i][1] / 1000) * (rotate[0][2] * Xs + rotate[1][2] * Ys + rotate[2][2] * Zs);
				L[i * 2 + 1] = f*(rotate[0][1] * Xs + rotate[1][1] * Ys + rotate[2][1] * Zs) +
					(ex[j + i][2]/1000) * (rotate[0][2] * Xs + rotate[1][2] * Ys + rotate[2][2] * Zs);
				/*求a矩阵*/
				A[i * 2][0] = rotate[0][0] * f + rotate[0][2] * ex[j + i][1] / 1000;
				A[i * 2][1] = rotate[1][0] * f + rotate[1][2] * ex[j + i][1] / 1000;
				A[i * 2][2] = rotate[2][0] * f + rotate[2][2] * ex[j + i][1] / 1000;
				A[i * 2 + 1][0] = rotate[0][1] * f + rotate[0][2] * ex[j + i][2] / 1000;
				A[i * 2 + 1][1] = rotate[1][1] * f + rotate[1][2] * ex[j + i][2] / 1000;
				A[i * 2 + 1][2] = rotate[2][1] * f + rotate[2][2] * ex[j + i][2] / 1000;
			}
			Mat A_(2 * n, 3, CV_64FC1);   
			Mat L_(2 * n, 1, CV_64FC1);
			Mat V(3, 1, CV_64FC1);

			for (int i = 0; i < 2 * n; i++)
			{
				A_.at<double>(i, 0) = A[i][0];
			}
			for (int i = 0; i < 2 * n; i++)
			{
				A_.at<double>(i, 1) = A[i][1];
			}
			for (int i = 0; i < 2 * n; i++)
			{
				A_.at<double>(i, 2) = A[i][2];
			}
			for (int i = 0; i < 2 * n; i++)
			{
				L_.at<double>(i, 0) = L[i];
			}

			V = ((A_.t()*A_).inv())*A_.t()*L_;  

			X = V.at<double>(0, 0);
			Y = V.at<double>(1, 0);
			Z = V.at<double>(2, 0);
				
			cout << fixed << setprecision(0) << "地面点：" << ex[j][0] << endl;
			cout << fixed << setprecision(3) << X << "  " << Y << "  " << Z << endl;
		}

		else if ((ex[j][0] == 2) || (ex[j][0] == 5) || (ex[j][0] == 11) || (ex[j][0] == 14))
		{
			int n = 3;  //3张像片
			double(*A)[3] = new double[2 * n][3];     //a系数矩阵
			double *L = new double[2 * n];     //L矩阵
			int t = 0;

			for (int i = 0; i < n; i++)
			{
				double Xs = ex[j + i + 3][1], Ys = ex[j + i + 3][2], Zs = ex[j + i + 3][3];
				pitch = ex[j + i + 3][4], roll = ex[j + i + 3][5], yaw = ex[j + i + 3][6];
				//旋转矩阵 phi = pitch, omiga = roll, kapa = yaw,计算旋转矩阵各元素
				rotate[0][0] = cos(pitch)*cos(yaw) - sin(pitch)*sin(roll)*sin(yaw);
				rotate[0][1] = (-1.0)*cos(pitch)*sin(yaw) - sin(pitch)*sin(roll)*cos(yaw);
				rotate[0][2] = (-1.0)*sin(pitch)*cos(roll);
				rotate[1][0] = cos(roll)*sin(yaw);
				rotate[1][1] = cos(roll)*cos(yaw);
				rotate[1][2] = (-1.0)*sin(roll);
				rotate[2][0] = sin(pitch)*cos(yaw) + cos(pitch)*sin(roll)*sin(yaw);
				rotate[2][1] = (-1.0)*sin(pitch)*sin(yaw) + cos(pitch)*sin(roll)*cos(yaw);
				rotate[2][2] = cos(pitch)*cos(roll);

				/*计算L矩阵,将未知数近似值带入共线条件方程得到x计*/
				L[i * 2] = f*(rotate[0][0] * Xs + rotate[1][0] * Ys + rotate[2][0] * Zs) +
					(ex[j + i][1] / 1000) * (rotate[0][2] * Xs + rotate[1][2] * Ys + rotate[2][2] * Zs);
				L[i * 2 + 1] = f*(rotate[0][1] * Xs + rotate[1][1] * Ys + rotate[2][1] * Zs) +
					(ex[j + i][2] / 1000) * (rotate[0][2] * Xs + rotate[1][2] * Ys + rotate[2][2] * Zs);
				/*求a矩阵*/
				A[i * 2][0] = rotate[0][0] * f + rotate[0][2] * ex[j + i][1] / 1000;
				A[i * 2][1] = rotate[1][0] * f + rotate[1][2] * ex[j + i][1] / 1000;
				A[i * 2][2] = rotate[2][0] * f + rotate[2][2] * ex[j + i][1] / 1000;
				A[i * 2 + 1][0] = rotate[0][1] * f + rotate[0][2] * ex[j + i][2] / 1000;
				A[i * 2 + 1][1] = rotate[1][1] * f + rotate[1][2] * ex[j + i][2] / 1000;
				A[i * 2 + 1][2] = rotate[2][1] * f + rotate[2][2] * ex[j + i][2] / 1000;
			}
			Mat A_(2 * n, 3, CV_64FC1);   
			Mat L_(2 * n, 1, CV_64FC1);
			Mat V(3, 1, CV_64FC1);
			
			for (int i = 0; i < 2 * n; i++)
			{
				A_.at<double>(i, 0) = A[i][0];
			}
			for (int i = 0; i < 2 * n; i++)
			{
				A_.at<double>(i, 1) = A[i][1];
			}
			for (int i = 0; i < 2 * n; i++)
			{
				A_.at<double>(i, 2) = A[i][2];
			}
			for (int i = 0; i < 2 * n; i++)
			{
				L_.at<double>(i, 0) = L[i];
			}

			V = ((A_.t()*A_).inv())*A_.t()*L_; 

			X = V.at<double>(0, 0);
			Y = V.at<double>(1, 0);
			Z = V.at<double>(2, 0);
			
			cout << fixed << setprecision(0) << "地面点：" << ex[j][0] << endl;
			cout << fixed << setprecision(3) << X << "  " << Y << "  " << Z << endl;	
		}
		else if ((ex[j][0] == 7) || (ex[j][0] == 9))
		{
			int n = 4;  //4张像片
			double(*A)[3] = new double[2 * n][3];     //a系数矩阵
			double *L = new double[2 * n];     //L矩阵
			int t = 0;

			for (int i = 0; i < n; i++)
			{
				double Xs = ex[j + i + 4][1], Ys = ex[j + i + 4][2], Zs = ex[j + i + 4][3];
				pitch = ex[j + i + 4][4], roll = ex[j + i + 4][5], yaw = ex[j + i + 4][6];
				//旋转矩阵 phi = pitch, omiga = roll, kapa = yaw,计算旋转矩阵各元素
				rotate[0][0] = cos(pitch)*cos(yaw) - sin(pitch)*sin(roll)*sin(yaw);
				rotate[0][1] = (-1.0)*cos(pitch)*sin(yaw) - sin(pitch)*sin(roll)*cos(yaw);
				rotate[0][2] = (-1.0)*sin(pitch)*cos(roll);
				rotate[1][0] = cos(roll)*sin(yaw);
				rotate[1][1] = cos(roll)*cos(yaw);
				rotate[1][2] = (-1.0)*sin(roll);
				rotate[2][0] = sin(pitch)*cos(yaw) + cos(pitch)*sin(roll)*sin(yaw);
				rotate[2][1] = (-1.0)*sin(pitch)*sin(yaw) + cos(pitch)*sin(roll)*cos(yaw);
				rotate[2][2] = cos(pitch)*cos(roll);

				L[i * 2] = f*(rotate[0][0] * Xs + rotate[1][0] * Ys + rotate[2][0] * Zs) +
					(ex[j + i][1] / 1000) * (rotate[0][2] * Xs + rotate[1][2] * Ys + rotate[2][2] * Zs);
				L[i * 2 + 1] = f*(rotate[0][1] * Xs + rotate[1][1] * Ys + rotate[2][1] * Zs) +
					(ex[j + i][2] / 1000) * (rotate[0][2] * Xs + rotate[1][2] * Ys + rotate[2][2] * Zs);
				/*求a矩阵*/
				A[i * 2][0] = rotate[0][0] * f + rotate[0][2] * ex[j + i][1] / 1000;
				A[i * 2][1] = rotate[1][0] * f + rotate[1][2] * ex[j + i][1] / 1000;
				A[i * 2][2] = rotate[2][0] * f + rotate[2][2] * ex[j + i][1] / 1000;
				A[i * 2 + 1][0] = rotate[0][1] * f + rotate[0][2] * ex[j + i][2] / 1000;
				A[i * 2 + 1][1] = rotate[1][1] * f + rotate[1][2] * ex[j + i][2] / 1000;
				A[i * 2 + 1][2] = rotate[2][1] * f + rotate[2][2] * ex[j + i][2] / 1000;
			}
			Mat A_(2 * n, 3, CV_64FC1);  
			Mat L_(2 * n, 1, CV_64FC1);
			Mat V(3, 1, CV_64FC1);
				
			for (int i = 0; i < 2 * n; i++)
			{
				A_.at<double>(i, 0) = A[i][0];
			}
			for (int i = 0; i < 2 * n; i++)
			{
				A_.at<double>(i, 1) = A[i][1];
			}
			for (int i = 0; i < 2 * n; i++)
			{
				A_.at<double>(i, 2) = A[i][2];
			}
			for (int i = 0; i < 2 * n; i++)
			{
				L_.at<double>(i, 0) = L[i];
			}

			V = ((A_.t()*A_).inv())*A_.t()*L_; 

			X = V.at<double>(0, 0);
			Y = V.at<double>(1, 0);
			Z = V.at<double>(2, 0);
				
			cout << fixed << setprecision(0) << "地面点：" << ex[j][0] << endl;
			cout << fixed << setprecision(3) << X << "  " << Y << "  " << Z << endl;
		}
		else if ((ex[j][0] == 8))
		{
			int n = 6;  //2张像片
			double(*A)[3] = new double[2 * n][3];     //a系数矩阵
			double *L = new double[2 * n];     //L矩阵
			int t = 0;

			for (int i = 0; i < n; i++)
			{
				double Xs = ex[j + i + 6][1], Ys = ex[j + i + 6][2], Zs = ex[j + i + 6][3];
				pitch = ex[j + i + 6][4], roll = ex[j + i + 6][5], yaw = ex[j + i + 6][6];
				//旋转矩阵 phi = pitch, omiga = roll, kapa = yaw,计算旋转矩阵各元素
				rotate[0][0] = cos(pitch)*cos(yaw) - sin(pitch)*sin(roll)*sin(yaw);
				rotate[0][1] = (-1.0)*cos(pitch)*sin(yaw) - sin(pitch)*sin(roll)*cos(yaw);
				rotate[0][2] = (-1.0)*sin(pitch)*cos(roll);
				rotate[1][0] = cos(roll)*sin(yaw);
				rotate[1][1] = cos(roll)*cos(yaw);
				rotate[1][2] = (-1.0)*sin(roll);
				rotate[2][0] = sin(pitch)*cos(yaw) + cos(pitch)*sin(roll)*sin(yaw);
				rotate[2][1] = (-1.0)*sin(pitch)*sin(yaw) + cos(pitch)*sin(roll)*cos(yaw);
				rotate[2][2] = cos(pitch)*cos(roll);

				L[i * 2] = f*(rotate[0][0] * Xs + rotate[1][0] * Ys + rotate[2][0] * Zs) +
					(ex[j + i][1] / 1000) * (rotate[0][2] * Xs + rotate[1][2] * Ys + rotate[2][2] * Zs);
				L[i * 2 + 1] = f*(rotate[0][1] * Xs + rotate[1][1] * Ys + rotate[2][1] * Zs) +
					(ex[j + i][2] / 1000) * (rotate[0][2] * Xs + rotate[1][2] * Ys + rotate[2][2] * Zs);
				/*求a矩阵*/
				A[i * 2][0] = rotate[0][0] * f + rotate[0][2] * ex[j + i][1] / 1000;
				A[i * 2][1] = rotate[1][0] * f + rotate[1][2] * ex[j + i][1] / 1000;
				A[i * 2][2] = rotate[2][0] * f + rotate[2][2] * ex[j + i][1] / 1000;
				A[i * 2 + 1][0] = rotate[0][1] * f + rotate[0][2] * ex[j + i][2] / 1000;
				A[i * 2 + 1][1] = rotate[1][1] * f + rotate[1][2] * ex[j + i][2] / 1000;
				A[i * 2 + 1][2] = rotate[2][1] * f + rotate[2][2] * ex[j + i][2] / 1000;
			}
			Mat A_(2 * n, 3, CV_64FC1);   
			Mat L_(2 * n, 1, CV_64FC1);
			Mat V(3, 1, CV_64FC1);
				
			for (int i = 0; i < 2 * n; i++)
			{
				A_.at<double>(i, 0) = A[i][0];
			}
			for (int i = 0; i < 2 * n; i++)
			{
				A_.at<double>(i, 1) = A[i][1];
			}
			for (int i = 0; i < 2 * n; i++)
			{
				A_.at<double>(i, 2) = A[i][2];
			}
			for (int i = 0; i < 2 * n; i++)
			{
				L_.at<double>(i, 0) = L[i];
			}

			V = ((A_.t()*A_).inv())*A_.t()*L_;  

			X = V.at<double>(0, 0);
			Y = V.at<double>(1, 0);
			Z = V.at<double>(2, 0);
			
			cout << fixed << setprecision(0) << "地面点：" << ex[j][0] << endl;
			cout << fixed << setprecision(3) << X << "  " << Y << "  " << Z<<endl;
		}
		j++;
	}
	system("pause");
}
