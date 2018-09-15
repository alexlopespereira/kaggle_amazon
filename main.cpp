#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include "data.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace cv;
using namespace std;

Mat sobel(Mat src_gray)
{
	  /// Generate grad_x and grad_y
	  Mat grad_x, grad_y;
	  Mat abs_grad_x, abs_grad_y;
	  int ddepth = CV_64F;
	  int scale = 1;
	  int delta = 0;
	  Mat grad;
	  cv::Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta);
	  convertScaleAbs( grad_x, abs_grad_x );
	  cv::Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta);
	  convertScaleAbs( grad_y, abs_grad_y );
	  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

	  return grad;
}

Mat sobel3c(Mat src, bool saturar)
{
	Mat bgr[3];
	Mat sb, sr, sg;
	split(src,bgr);
	sb = sobel(bgr[0]); //Aplicar o sobel em cada canal (banda)
	sg = sobel(bgr[1]);
	sr = sobel(bgr[2]);

	if(saturar)
	{
		sb*=255; // Essas 3 linhas aqui fazem aparecer os quadrados coloridos. É uma operação de saturação.
		sg*=255; //
		sr*=255; //
	}

	Mat color;
    std::vector<cv::Mat> array_to_merge;
    array_to_merge.push_back(sb);
    array_to_merge.push_back(sg);
    array_to_merge.push_back(sr);
    cv::merge(array_to_merge, color); //Faz um stack (composicao colorida) das bandas
	return color;
}

Mat sobel4c(Mat src, bool saturar)
{
	Mat bgr[4];
	Mat sb, sr, sg;
	split(src,bgr);
	sb = sobel(bgr[0]); //Aplicar o sobel em cada canal (banda)
	sg = sobel(bgr[1]);
	sr = sobel(bgr[2]);

	if(saturar)
	{
		sb*=255; // Essas 3 linhas aqui fazem aparecer os quadrados coloridos. É uma operação de saturação.
		sg*=255; //
		sr*=255; //
	}

	Mat color;
    std::vector<cv::Mat> array_to_merge;
    array_to_merge.push_back(sb);
    array_to_merge.push_back(sg);
    array_to_merge.push_back(sr);
    cv::merge(array_to_merge, color); //Faz um stack (composicao colorida) das bandas
	return color;
}

int main( )
{

	int nfiles = sizeof(data);
	for (int i=0; i< 77; i++)
	{
		Mat src1, srctif;
		string tiffile = string("./planet_tif/")+string(data_tif[i]);
		string jpgfile = string("./planet_jpg/")+string(data[i]);
		src1 = imread(jpgfile, CV_LOAD_IMAGE_UNCHANGED);
		srctif = imread(tiffile, CV_LOAD_IMAGE_UNCHANGED);

		imshow( "jpg", src1 );
		imshow( "tif", srctif );

		Mat grad_saturado = sobel3c(src1, true);
		Mat gradtif_saturado = sobel4c(srctif, true);

		Mat grad = sobel3c(src1, false);
		Mat gradtif = sobel4c(srctif, false);

		imshow("sobel", grad);
		imshow("sobel_tif", gradtif);

		imshow("sobel_saturado", grad_saturado);
		imshow("sobel_tif_saturado", gradtif_saturado);

		string sobel_saturado = "./output/sobel_saturado_" + string(data[i]);
		imwrite(sobel_saturado, grad_saturado);

		waitKey();
		src1.release();
		srctif.release();
		grad_saturado.release();
		gradtif_saturado.release();
		grad.release();
		gradtif.release();
	}
    return 0;
}
