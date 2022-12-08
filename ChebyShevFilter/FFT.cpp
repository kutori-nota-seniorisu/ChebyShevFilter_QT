#include "FFT.h"

vector<complex<double>> DFT(std::vector<double> input)
{
	int N = input.size();

	vector<complex<double>> output(N, {0, 0});
	for (int f = 0; f < N; ++f)
	{
		double factor = -2 * M_PI * f / N;
		double real = 0, imag = 0;
		for (int t = 0; t < N; ++t)
		{
			real += input[t] * cos(factor * t);
			imag += input[t] * sin(factor * t);
		}
		output[f].real(real);
		output[f].imag(imag);
		//cout << "y[" << f << "]:" << output[f].real() << " + j " << output[f].imag() << endl;
	}
	// 输出幅值
	//for (int f = 0; f < N; ++f)
	//{
	//	double m = output[f].real() * output[f].real() + output[f].imag() * output[f].imag();
	//	m = sqrt(m);
	//	cout << f << ":::" << m << endl;
	//}
	return output;
}
