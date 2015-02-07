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
Version:1.7 Date:26/02/14 Details: Creation of Write to XML function
Version:1.8 Date:27/02/14 Details: Implemented Testing based on Path Data
Version:1.9 Date:29/03/14 Details: Created gesture update function
Version:2.0 Date:02/03/14 Details: Implemented two handed gesture testing

******************************************************************************/

#include "Gestures.h"

using namespace std;
using namespace Matrix;

#ifndef _HMM_

#define _HMM_


class HMM
{

public:
	HMM();
	~HMM();

	void Test_HMM(vector<int>, vector<int>, vector<int>);
	void add_Data(vector<int>, vector<int>, vector<int>);

	Gestures aGesture;
	gesture returnCurrentGesture();
	void updateGestures();
	vector<gesture> gestureList;
protected:


private:
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
	double viterbi(Matrix1D, Matrix2D, Matrix2D, Matrix1D);
	void train_hmm(Matrix2D, Matrix2D, Matrix1D, int, int, double);

	void write_data_to_XML(double, Matrix2D, string);
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

	Matrix3D toMatrix3D(vector<int>, vector<int>, vector<int>);

	int find(double, Matrix1D);

	Matrix2D E;
	Matrix2D P;
	Matrix1D Pi;
};

#endif // !_HMM_
