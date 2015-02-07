/******************************************************************************

Author:Steven Cooper
Project: Computer Vision for Interactive Applications
Version:1.0 Date:14/02/14 Details: Creation of HMM class
Version:1.1 Date:15/02/14 Details: Completion of writing functions
Version:1.2 Date:17/02/14 Details: Creation of Matlab functions
Version:1.3 Date:18/02/14 Details: Kmeans algorithm fixed
Version:1.4 Date:19/02/14 Details: HMM training function fixed
Version:1.5 Date:19/02/14 Details: HMM class working
Version:1.6 Date:21/02/14 Details: Creation of Write to file function
Version:1.7 Date:04/03/14 Details: Rework of HMM Constructor

******************************************************************************/

#include "tinyxml.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <ctime>
#include "Matrix3D.h"



using namespace std;
using namespace Matrix;

#ifndef _HMM_

#define _HMM_


class HMM
{

public:
	HMM();
	HMM(int, int, int, int, string);
	~HMM();

	void Train_HMM();
	void add_Data(vector<int>, vector<int>, vector<int>);
protected:
	Matrix2D cdiv(Matrix2D, Matrix1D);
	Matrix1D csum(Matrix2D);
	Matrix2D rdiv(Matrix2D, Matrix1D);
	Matrix1D rsum(Matrix2D);
	Matrix2D prior_transition_matrix(int, int);
	Matrix3D get_xyz_data(string,string);
	Matrix2D get_point_centroids(Matrix3D, int, int);
	Matrix2D get_point_clusters(Matrix3D, Matrix2D, int);
	Matrix2D kmeans(Matrix2D, int);

	double pr_hmm(Matrix1D, Matrix2D, Matrix2D, Matrix1D);
	void train_hmm(Matrix2D, Matrix2D, Matrix1D, int, int, double);

	double viterbi(Matrix1D, Matrix2D, Matrix2D, Matrix1D);

	void write_data_to_XML(double, Matrix2D, string);
private:
	Matrix2D eye(int);
	double minimum(Matrix1D);
	Matrix1D minimum(Matrix2D);
	double maximum(Matrix1D);
	Matrix1D maximum(Matrix2D);
	Matrix2D random(int, int);
	Matrix1D random(int);

	Matrix1D matrixMultiplication(Matrix1D, Matrix2D);
	Matrix2D matrixMultiplication(Matrix1D, Matrix1D);

	Matrix2D tranverse(Matrix2D);

	int find(double, Matrix1D);

	Matrix2D E;
	Matrix2D P;
	Matrix1D Pi;

	int D;
	int M;
	int N;
	int LR;
	string gesture;
};

HMM::HMM()
{

}

HMM::HMM(int Dim, int m, int n, int lr, string gest)
{
	D = Dim;
	M = m;
	N = n;
	LR = lr;
	gesture = gest;
}

HMM::~HMM()
{

}

void HMM::Train_HMM()
{
	Matrix3D training = get_xyz_data("newdata/train/delete", gesture);

	//Initialize

	double gestureRecThreshold = 0.;

	Matrix2D centroids = get_point_centroids(training, N, D);
	Matrix2D ATrainBinned = get_point_clusters(training, centroids, D);

	//Training

	Matrix2D pP = prior_transition_matrix(M, LR);

	int cyc = 50;
	Matrix1D bins = Matrix1D(N);
	for(int i = 0; i < N; i++) bins.data.at(i) = i+1;
	train_hmm(ATrainBinned, pP, bins, M, cyc,0.00001);

	//Testing

	double sumLik = 0.;
	double minLik = numeric_limits<double>::infinity();
	for(size_t j = 0; j < ATrainBinned.data.size(); j++)
	{
		double lik = viterbi(ATrainBinned.data.at(j), P, tranverse(E), Pi);
		if(lik < minLik) minLik = lik;
		sumLik += lik;
	}

	gestureRecThreshold = sumLik/ATrainBinned.data.size();

	write_data_to_XML(gestureRecThreshold, centroids, gesture);
}

void HMM::add_Data(vector<int> X, vector<int> Y, vector<int> Z)
{
	string filepath = "newdata/train/" + gesture;
	Matrix3D data = get_xyz_data("newdata/train", gesture);

	string xfilepath = filepath + "_x.csv";
	string yfilepath = filepath + "_y.csv";
	string zfilepath = filepath + "_z.csv";
	ofstream xoFile(xfilepath, ofstream::out, ofstream::trunc);
	ofstream yoFile(yfilepath, ofstream::out, ofstream::trunc);
	ofstream zoFile(zfilepath, ofstream::out, ofstream::trunc);

	if(data.data.size() > 0)
	{
		for(size_t i = 0; i < data.data.size(); i++)
		{
			Matrix1D temp = Matrix1D();
			temp.data.push_back(X.at(i));
			temp.data.push_back(Y.at(i));
			temp.data.push_back(Z.at(i));
			data.data.at(i).data.push_back(temp);
		}
	}
	else
	{
		for(size_t i = 0; i < X.size(); i++)
		{
			Matrix2D tempA = Matrix2D();
			Matrix1D tempB = Matrix1D();

			tempB.data.push_back(X.at(i));
			tempB.data.push_back(Y.at(i));
			tempB.data.push_back(Z.at(i));

			tempA.data.push_back(tempB);

			data.data.push_back(tempA);
		}
	}

	for(size_t i = 0; i < data.data.size(); i++)
	{
		for(size_t j = 0; j < data.data.at(0).data.size(); j++)
		{
			xoFile << data.data.at(i).data.at(j).data.at(0);
			if(j != (data.data.at(0).data.size()-1)) xoFile << ",";

			yoFile << data.data.at(i).data.at(j).data.at(1);
			if(j != (data.data.at(0).data.size()-1)) yoFile << ",";

			zoFile << data.data.at(i).data.at(j).data.at(2);
			if(j != (data.data.at(0).data.size()-1)) zoFile << ",";
		}

		xoFile << endl;
		yoFile << endl;
		zoFile << endl;
	}

	xoFile.close();
	yoFile.close();
	zoFile.close();

}

Matrix2D HMM::cdiv(Matrix2D X, Matrix1D Y)
{
	if(X.data.at(0).data.size() != Y.data.size()) 
	{
		throw("Error in Cdiv");
		//return 0;
	}

	Matrix2D Z = Matrix2D(((int)X.data.size()), ((int)X.data.at(0).data.size()));

	for(size_t i = 0; i < X.data.size(); i++) Z.data.at(i) = X.data.at(i) / Y;

	return Z;
}

Matrix1D HMM::csum(Matrix2D X)
{
	int N = ((int)X.data.size());
	Matrix1D Z = Matrix1D(((int)X.data.size()));
	if(N > 1)
	{
		Z = X.sum();
	}
	else Z = X.data.at(0);

	return Z;
}

Matrix2D HMM::rdiv(Matrix2D X, Matrix1D Y)
{
	int N = ((int)X.data.size());
	int M = ((int)X.data.at(0).data.size());
	int K = ((int)Y.data.size());

	if(N != K)
	{
		throw("Error in rDiv");
		//return 0;
	}

	Matrix2D Z = Matrix2D(N,M);

	if(M < N)
	{
		for(int m = 0; m < M; m++)
		{
			for(int i = 0; i < N; i++) Z.data.at(i).data.at(m) = X.data.at(i).data.at(m) / Y.data.at(i);
		}
	}
	else
	{
		for(int n = 0; n < N; n++) Z.data.at(n) = X.data.at(n) / Y.data.at(n);
	}

	return Z;

}

Matrix1D HMM::rsum(Matrix2D X)
{
	int N = ((int)X.data.size());
	int M = ((int)X.data.at(0).data.size());

	Matrix1D Z = Matrix1D(N);

	if(M == 1) Z = X.data.at(0);
	else if(M < (2*N))
		for(int m = 0; m < M; m++) 
			for(int n = 0; n< N; n++) Z.data.at(n) += X.data.at(n).data.at(m);
	else
		for(int n = 0; n < N; n++) Z.data.at(n) = X.data.at(n).sum();

	return Z;
}

Matrix2D HMM::prior_transition_matrix(int K, int LR)
{

	Matrix2D P = eye(K)*((double)1/(double)LR);

	for(int i = 0; i < K-(LR-1); i++)
	{
		for(int j = 1; j < LR; j++)
		{
			P.data.at(i).data.at(i+j) = ((double)1/(double)LR);
		}
	}

	for(int i = K-(LR-2)-1; i < K; i++)
	{
		for(int j = 0; j < K-i; j++) 
		{
			P.data.at(i).data.at(i+j) = ((double)1/(double)(K-i));
		}
	}

	return P;
}

Matrix3D HMM::get_xyz_data(string path, string name)
{
	Matrix3D data = Matrix3D();

	string filepath = path.append("/").append(name);
	string xfilepath = filepath + "_x.csv";
	string yfilepath = filepath + "_y.csv";
	string zfilepath = filepath + "_z.csv";
	ifstream xFile(xfilepath);
	ifstream yFile(yfilepath);
	ifstream zFile(zfilepath);

	while(xFile && yFile && zFile)
	{
		string sx,sy,sz;
		if(!getline(xFile,sx)) break;
		if(!getline(yFile,sy)) break;
		if(!getline(zFile,sz)) break;
		istringstream ssx(sx);
		istringstream ssy(sy);
		istringstream ssz(sz);
		Matrix2D temp2D = Matrix2D();
		while(ssx && ssy && ssz)
		{
			string dx, dy, dz;
			double x,y,z;
			Matrix1D temp = Matrix1D();
			if(!getline(ssx,dx,',')) break;
			if(!getline(ssy,dy,',')) break;
			if(!getline(ssz,dz,',')) break;
			istringstream ix(dx),iy(dy),iz(dz);
			ix >> x;
			iy >> y;
			iz >> z;
			temp.data.push_back(x);
			temp.data.push_back(y);
			temp.data.push_back(z);

			temp2D.data.push_back(temp);
		}
		data.data.push_back(temp2D);
	}

	return data;
}

Matrix2D HMM::get_point_centroids(Matrix3D data, int K, int D)
{
	Matrix2D mean = Matrix2D(((int)data.data.size()), D);
	for(size_t n = 0; n < data.data.size(); n++)
	{
		for(size_t i = 0; i < data.data.at(0).data.size(); i++)
			for(int j = 0; j < D; j++) mean.data.at(n).data.at(j) += data.data.at(n).data.at(i).data.at(j);

		mean.data.at(n) /= ((double)data.data.at(0).data.size());
	}

	Matrix2D centroids = kmeans(mean, K);

	return centroids;
}

Matrix2D HMM::get_point_clusters(Matrix3D data, Matrix2D centroids, int D)
{
	Matrix2D XClustered = Matrix2D(((int)data.data.at(0).data.size()),((int)data.data.size()));
	int K = ((int)centroids.data.size());

	for(size_t n = 0; n < data.data.size(); n++)
	{
		for(size_t i = 0; i < data.data.at(0).data.size(); i++)
		{
			Matrix1D temp = Matrix1D(K);
			for(int j = 0; j < K; j++) 
			{

				if(D == 3)
				{
					double x = (centroids.data.at(j).data.at(0) - data.data.at(n).data.at(i).data.at(0)) * (centroids.data.at(j).data.at(0) - data.data.at(n).data.at(i).data.at(0));
					double y = (centroids.data.at(j).data.at(1) - data.data.at(n).data.at(i).data.at(1)) * (centroids.data.at(j).data.at(1) - data.data.at(n).data.at(i).data.at(1));
					double z = (centroids.data.at(j).data.at(2) - data.data.at(n).data.at(i).data.at(2)) * (centroids.data.at(j).data.at(2) - data.data.at(n).data.at(i).data.at(2));
					temp.data.at(j) = sqrt(x + y + z);
				}
			}
			double I = minimum(temp);
			XClustered.data.at(i).data.at(n) = I;
		}
	}

	return XClustered;
}

Matrix2D HMM::kmeans(Matrix2D data, int nbClusters)
{
	int data_dim = ((int)data.data.at(0).data.size());
	//int nbData = ((int)data.data.size());

	Matrix1D data_min = minimum(data);
	Matrix1D data_max = maximum(data);
	Matrix1D data_diff = data_max - data_min;


	Matrix2D centroid = random(nbClusters, data_dim);

	for(size_t i = 0; i < centroid.data.size(); i++)
	{
		centroid.data.at(i) *= data_diff;
		centroid.data.at(i) += data_min;
	}

	double pos_diff = 1.;

	while(pos_diff > 0.0)
	{
		Matrix1D assignment = Matrix1D(((int)data.data.size()));

		for(size_t d = 0; d < data.data.size(); d++)
		{
			Matrix1D min_diff = data.data.at(d) - centroid.data.at(0);
			min_diff *= min_diff;
			int curAssignment = 1;

			for(int c = 1; c < nbClusters; c++)
			{
				Matrix1D diff2c = data.data.at(d) - centroid.data.at(c);
				diff2c *= diff2c;
				if(min_diff.sum() >= diff2c.sum())
				{
					curAssignment = c+1;
					min_diff = diff2c;
				}
			}

			assignment.data.at(d) = curAssignment;
		}

		Matrix2D oldPositions = centroid;

		centroid = Matrix2D(nbClusters, data_dim);
		Matrix1D pointsInCluster = Matrix1D(nbClusters);

		for(size_t d = 0; d < assignment.data.size(); d++)
		{
			centroid.data.at(((int)assignment.data.at(d))-1) += data.data.at(d);
			pointsInCluster.data.at(((int)assignment.data.at(d))-1) += 1;
		}

		for(int c = 0; c < nbClusters; c++)
		{
			if(pointsInCluster.data.at(c) != 0) centroid.data.at(c) /= pointsInCluster.data.at(c);
			else centroid.data.at(c) = (random(data_dim) * data_diff) + data_min;
		}

		pos_diff = ((centroid - oldPositions) * (centroid - oldPositions)).sum().sum();
		if(!(pos_diff > 0.0))
		{
			double d;
			d = 2;
		}
	}

	return centroid;
}

double HMM::pr_hmm(Matrix1D o, Matrix2D a, Matrix2D b, Matrix1D pi)
{
	int n = ((int)a.data.at(0).data.size());
	int T = ((int)o.data.size());
	Matrix2D m = Matrix2D(T, n);

	for(int i = 0; i < n; i++) m.data.at(0).data.at(i) = b.data.at(i).data.at(((int)o.data.at(0))-1) * pi.data.at(i);

	for(int t = 0; t < T-1; t++)
	{
		for(int j = 0; j < n; j++)
		{
			double z = 0.;
			for(int i = 0; i < n; i++) z += a.data.at(i).data.at(j) * m.data.at(t).data.at(i);

			m.data.at(t+1).data.at(j) = z * b.data.at(j).data.at(((int)o.data.at(t+1))-1);
		}
	}

	double p = 0.;
	for(int i = 0; i < n; i++) p += m.data.at(T-1).data.at(i);

	p = log(p);

	return p;
}

double HMM::viterbi(Matrix1D o, Matrix2D a, Matrix2D b, Matrix1D pi)
{
	int K = (int)a.data.size();
	int T = (int)o.data.size();
	Matrix2D T1 = Matrix2D(K,T);
	Matrix2D T2 = Matrix2D(K,T);


	for(int i = 0; i<K; ++i)
	{
		T1.data.at(i).data.at(0) = pi.data.at(i);
		T2.data.at(i).data.at(0) = 0;
	}


	for(int i = 1; i <T; ++i)
	{
		double max, temp;
		int argmax;

		for(int j = 0; j <K; ++j)
		{
			max = -1;
			argmax = -1;
			for(int k = 0; k<K; ++k)
			{
				temp = T1.data.at(k).data.at(i-1) * a.data.at(k).data.at(j) * b.data.at(k).data.at((int)o.data.at(i-1)-1);

				if(temp > max)
				{
					max = temp;
					argmax = k;
				}
			}

			T1.data.at(j).data.at(i) = max;
			T2.data.at(j).data.at(i) = argmax;
		}
	}

	Matrix1D Z = Matrix1D(T);
	Matrix1D X = Matrix1D(T);

	double max = -1, temp;
	for(int k = 0; k<K; ++k)
	{
		temp = T1.data.at(k).data.at(T-1);

		if(temp > max)
		{
			max = temp;
			Z.data.at(T-1) = k;
		}
	}

	X.data.at(T-1) = Z.data.at(T-1);

	for(int i = T-1; i > 0; --i)
	{
		Z.data.at(i-1) = T2.data.at((int)Z.data.at(i)).data.at(i);
		X.data.at(i-1) = Z.data.at(i-1);
	}
	double prob = 0;
	for(int i = 0; i < T; ++i) prob += T1.data.at((int)X.data.at(i)).data.at(i);

	prob = log(prob);

	return prob;
}

void HMM::train_hmm(Matrix2D X, Matrix2D pP, Matrix1D bins, int K = 2, int cyc = 100, double tol = 0.0001)
{
	double num_bins = ((double)bins.data.size());

	//double epsi = 1e-10;
	double oldlik = 0., likbase = 0.;

	int N = ((int)X.data.size());

	Matrix1D T = Matrix1D(N);
	for(int n = 0; n < N; n++) T.data.at(n) = ((double)X.data.at(n).data.size());

	double TMAX = maximum(T);

	printf("\n****************************************************************************************\n");
	printf("Training %i sequences of maximum length %i from an alphabet of size %i\n", N, (int)TMAX, (int)num_bins);
	printf("HMM with %i hidden states\n", K);
	printf("******************************************************************************************\n");

	E = ((random(((int)num_bins), K) * 0.1) + 1) / num_bins;
	E = cdiv(E, csum(E));

	Matrix2D B = Matrix2D(((int)TMAX), K);

	Pi = random(K);
	Pi /= Pi.sum();

	P = pP;
	P = rdiv(P, rsum(P));

	Matrix1D LL;
	double lik = 0.;

	for(int cycle = 0; cycle < cyc; cycle++)
	{
		// Forward - Backward

		Matrix1D Gammainit = Matrix1D(K);
		Matrix1D Gammasum = Matrix1D(K);
		Matrix2D Gammaksum = Matrix2D(((int)num_bins), K);
		Matrix1D Scale = Matrix1D(((int)TMAX));
		Matrix2D sxi = Matrix2D(K);

		for(int n = 0; n < N; n++)
		{
			Matrix2D alpha = Matrix2D(((int)T.data.at(n)), K);
			Matrix2D beta = Matrix2D(((int)T.data.at(n)), K);
			Matrix2D gamma = Matrix2D(((int)T.data.at(n)), K);
			Matrix2D gammaksum = Matrix2D(((int)num_bins), K);

			Matrix1D Xcurrent = X.data.at(n);

			for(int i = 0; i < T.data.at(n); i++)
			{
				int m = find(Xcurrent.data.at(i), bins);
				if(m == 0) 
				{
					printf("Error: Symbol not found\n");
					return;
				}
				B.data.at(i) = E.data.at(m-1);
			}

			Matrix1D scale = Matrix1D(((int)T.data.at(n)));
			alpha.data.at(0) = Pi * B.data.at(0);
			scale.data.at(0) = alpha.data.at(0).sum();
			alpha.data.at(0) /= scale.data.at(0);

			for(int i = 1; i < T.data.at(n); i++)
			{
				alpha.data.at(i) = (matrixMultiplication(alpha.data.at(i-1), P) * B.data.at(i));
				scale.data.at(i) = alpha.data.at(i).sum();
				alpha.data.at(i) /= scale.data.at(i);
			}

			beta.data.at(((int)T.data.at(n))-1) = (Matrix1D(K) + 1) / scale.data.at(((int)T.data.at(n))-1);
			for(int i = ((int)T.data.at(n))-2; i >= 0; i--)
			{
				beta.data.at(i) = matrixMultiplication(beta.data.at(i+1) * B.data.at(i+1), tranverse(P)) / scale.data.at(i);
			}

			gamma = (alpha * beta);
			gamma = rdiv(gamma, rsum(gamma));

			Matrix1D gammasum = gamma.sum();

			for(int i = 0; i < T.data.at(n); i++)
			{
				int m = find(Xcurrent.data.at(i), bins);
				gammaksum.data.at(m-1) += gamma.data.at(i);
			}

			for(int i = 0; i < T.data.at(n)-1; i++)
			{
				Matrix2D t = P * matrixMultiplication(alpha.data.at(i), (beta.data.at(i+1) * B.data.at(i+1)));
				sxi += t / t.sum().sum();
			}

			Gammainit += gamma.data.at(0);
			Gammasum += gammasum;
			Gammaksum += gammaksum;

			for(int i = 0; i < T.data.at(n)-1; i++) Scale.data.at(i) += log(scale.data.at(i));

			Scale.data.at(((int)T.data.at(n))-1) += log(scale.data.at(((int)T.data.at(n))-1));
		}

		E = cdiv(Gammaksum, Gammasum);

		P = rdiv(sxi, rsum(sxi));

		Pi = Gammainit / Gammainit.sum();

		oldlik = lik;
		lik = Scale.sum();

		LL.data.push_back(lik);
		printf("\n cycle %i log likelihood = %f", cycle, lik);
		if(cycle <= 2) likbase = lik;
		else if(lik < (oldlik - 1e-6)) printf("vionum binstion");
		else if((lik-likbase) < (1 + tol) * (oldlik - likbase) || lik == numeric_limits<double>::infinity())
		{
			printf("\nend\n");
			return;
		}
	}
}

Matrix2D HMM::eye(int n)
{
	Matrix2D temp = Matrix2D(n);
	for(int i = 0; i < n; i++) temp.data.at(i).data.at(i) = 1;

	return temp;
}

double HMM::minimum(Matrix1D m)
{
	double min = 1;
	for(size_t i = 1; i < m.data.size(); i++)
		if(m.data.at(i) < m.data.at(((int)min)-1)) min = ((double)i+1);

	return min;
}

Matrix1D HMM::minimum(Matrix2D m)
{
	Matrix1D min = Matrix1D(((int)m.data.at(0).data.size()));

	for(size_t i = 0; i < min.data.size(); i++)
	{
		min.data.at(i) = m.data.at(0).data.at(i);
		for(size_t j = 1; j < m.data.size(); j++)
			if(min.data.at(i) > m.data.at(j).data.at(i)) 
			{
				min.data.at(i) = m.data.at(j).data.at(i);
			}
	}

	return min;
}

double HMM::maximum(Matrix1D m)
{
	double max = m.data.at(0);
	for(size_t i = 1; i < m.data.size(); i++)
		if(max < m.data.at(i)) max = m.data.at(i);

	return max;
}

Matrix1D HMM::maximum(Matrix2D m)
{
	Matrix1D max = Matrix1D(((int)m.data.at(0).data.size()));

	for(size_t i = 0; i < max.data.size(); i++)
	{
		max.data.at(i) = m.data.at(i).data.at(0);
		for(size_t j = 1; j < m.data.size(); j++)
			if(max.data.at(i) < m.data.at(j).data.at(i)) max.data.at(i) = m.data.at(j).data.at(i);
	}

	return max;
}

Matrix2D HMM::random(int n, int m)
{
	Matrix2D random = Matrix2D(n,m);
	srand(((unsigned int)time(0)));
	for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
		{		
			random.data.at(i).data.at(j) = ((double)rand() / (double)RAND_MAX);
		}

		return random;
}

Matrix1D HMM::random(int n)
{
	Matrix1D random = Matrix1D(n);
	srand(((unsigned int)time(0)));
	for(int i = 0; i < n; i++)
	{
		random.data.at(i) = ((double)rand() / (double)RAND_MAX);
	}

	return random;
}

int HMM::find(double val, Matrix1D set)
{
	for(size_t i = 0; i < set.data.size(); i++) if(val == set.data.at(i)) return ((int)i+1);

	return 0;
}

Matrix1D HMM::matrixMultiplication(Matrix1D A, Matrix2D B)
{
	Matrix1D C = Matrix1D(((int)A.data.size()));
	for(size_t i = 0; i < B.data.at(0).data.size(); i++)
	{
		for(size_t j = 0; j < A.data.size(); j++)
		{
			C.data.at(i) += (A.data.at(j) * B.data.at(j).data.at(i));
		}
	}

	return C;
}

Matrix2D HMM::matrixMultiplication(Matrix1D A, Matrix1D B)
{
	Matrix2D C = Matrix2D(((int)A.data.size()));

	for(size_t i = 0; i < A.data.size(); i++)
		for(size_t j = 0; j < A.data.size(); j++) C.data.at(i).data.at(j) = A.data.at(i) * B.data.at(j);

	return C;
}

Matrix2D HMM::tranverse(Matrix2D matrix)
{
	Matrix2D tran = Matrix2D(((int)matrix.data.at(0).data.size()), ((int)matrix.data.size()));

	for(size_t i = 0; i < matrix.data.at(0).data.size(); i++)
		for(size_t j = 0; j < matrix.data.size(); j++)
		{
			tran.data.at(i).data.at(j) = matrix.data.at(j).data.at(i);
		}

		return tran;
}

void HMM::write_data_to_XML(double threshold, Matrix2D centroids, string Gesture)
{
	string filename = Gesture + ".xml";
	ostringstream tempvar;

	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0","utf-8", "");
	TiXmlElement *MainElement = new TiXmlElement(Gesture.c_str());

	TiXmlElement *GestureThreshold = new TiXmlElement("Threshold");
	tempvar << threshold;
	TiXmlText *thresholdtext = new TiXmlText(tempvar.str().c_str());
	GestureThreshold->LinkEndChild(thresholdtext);

	TiXmlElement *CentroidElement = new TiXmlElement("Centroids");
	CentroidElement->SetAttribute("wSize", ((int)centroids.data.at(0).data.size()));
	CentroidElement->SetAttribute("hSize", ((int)centroids.data.size()));
	tempvar = ostringstream();
	for(size_t i = 0; i < centroids.data.size(); i++)
	{
		for(size_t j = 0; j < centroids.data.at(0).data.size(); j++)
		{
			tempvar << centroids.data.at(i).data.at(j); 
			if(j != (centroids.data.at(0).data.size()-1) || i != (centroids.data.size()-1)) tempvar << ",";
		}
	}
	TiXmlElement *centroidtext = new TiXmlElement(tempvar.str().c_str());
	CentroidElement->LinkEndChild(centroidtext);
	
	TiXmlElement *TransmissionElement = new TiXmlElement("A");
	TransmissionElement->SetAttribute("wSize", ((int)P.data.at(0).data.size()));
	TransmissionElement->SetAttribute("hSize", ((int)P.data.size()));
	tempvar = ostringstream();
	for(size_t i = 0; i < P.data.size(); i++)
	{
		for(size_t j = 0; j < P.data.at(0).data.size(); j++)
		{
			tempvar << P.data.at(i).data.at(j); 
			if(j != (P.data.at(0).data.size()-1) || i != (P.data.size()-1)) tempvar << ",";
		}
	}
	TiXmlElement *Atext = new TiXmlElement(tempvar.str().c_str());
	TransmissionElement->LinkEndChild(Atext);

	TiXmlElement *EmissionElement = new TiXmlElement("B");
	EmissionElement->SetAttribute("wSize", ((int)E.data.at(0).data.size()));
	EmissionElement->SetAttribute("hSize", ((int)E.data.size()));
	tempvar = ostringstream();
	for(size_t i = 0; i < E.data.size(); i++)
	{
		for(size_t j = 0; j < E.data.at(0).data.size(); j++)
		{
			tempvar << E.data.at(i).data.at(j); 
			if(j != (E.data.at(0).data.size()-1) || i != (E.data.size()-1)) tempvar << ",";
		}
	}
	TiXmlElement *Btext = new TiXmlElement(tempvar.str().c_str());
	EmissionElement->LinkEndChild(Btext);

	TiXmlElement *InitProbElement = new TiXmlElement("Pi");
	InitProbElement->SetAttribute("size", ((int)Pi.data.size()));
	tempvar = ostringstream();
	for(size_t i = 0; i < Pi.data.size(); i++)
	{
			tempvar << Pi.data.at(i); 
			if(i != (Pi.data.size()-1)) tempvar << ",";
	}
	TiXmlElement *Pitext = new TiXmlElement(tempvar.str().c_str());
	InitProbElement->LinkEndChild(Pitext);

	MainElement->LinkEndChild(GestureThreshold);
	MainElement->LinkEndChild(CentroidElement);
	MainElement->LinkEndChild(TransmissionElement);
	MainElement->LinkEndChild(EmissionElement);
	MainElement->LinkEndChild(InitProbElement);

	doc.LinkEndChild(decl);
	doc.LinkEndChild(MainElement);
	doc.SaveFile(filename.c_str());
}

#endif // !_HMM_
