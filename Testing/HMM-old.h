/*****************************************************************************

Author:Steven Cooper
Project: Computer Vision for Interactive Applications
Version: 1.0 Date: 30/01/2014 details: Creation of HMM class
Version: 1.1 Date: 31/01/2014 details: Comment all of HMM class

*****************************************************************************/

#ifndef _HMM_H_
#define _HMM_H_

#include <vector>
#include <math.h>
#include "kMeansClustering.h"

using namespace std;

class HMM
{
public:
	HMM();
	HMM(vector<vector<double>> &aa, vector<vector<double>> &bb, vector<int> &obs);
	~HMM();
	void forward_backward();
	void baum_welch();
	vector<vector<double>> A;
	vector<vector<double>> B;

	vector<int> observedData;
protected:
	vector<vector<double>> alpha;
	vector<vector<double>> beta;
	vector<vector<double>> pState;

	double logLikelihood() { return log(lhood) + lrnrm * log(BIGI);}
private:
	int fbdone;
	int nStates;
	int nObservations;
	int nSymbols;

	int lrnrm;
	vector<int> arnrm;
	vector<int> brnrm;

	double BIG;
	double BIGI;
	double lhood;
};

HMM::HMM(vector<vector<double>> &aa, vector<vector<double>> &bb, vector<int>&obs) : 
	A(aa), B(bb), observedData(obs), fbdone(0), nStates(A.size()), nObservations(obs.size()), nSymbols(B.at(0).size()),
	arnrm(vector<int>(nObservations)), brnrm(vector<int>(nObservations)), BIG(exp(20)), BIGI(1/BIG)
{
	//create matrices for alpha, beta and pState
	vector<double> temp = vector<double>(nObservations);
	for(int i = 0; i < nStates; ++i)
	{
		alpha.push_back(temp);
		beta.push_back(temp);
		pState.push_back(temp);
	}

	if(A.at(0).size() != nStates) throw("transition matrix not square");
	if(B.size() != nStates) throw("symbol prob matrix wrong size");

	//if any of the observed data is less than zero or greater than the amount of symbols
	for(int i = 0; i < nStates; i++)
	{
		if(observedData.at(i) < 0 || observedData.at(i) >= nSymbols) throw("bad data in observedData");
	}

	//divide each transitional propability by the sum of it's row
	for(int i = 0; i < nStates; i++)
	{
		double sum = 0.;
		for(int j = 0; j < nStates; j++)
		{
			sum += A.at(i).at(j);
		}

		if(abs(sum - 1.) > 0.01) throw("transition marix not normalised");

		for(int j = 0; j < nStates; j++)
		{
			A.at(i).at(j) /= sum;
		}
	}

	//divide each emission probability by the sum of its row
	for(int i = 0; i < nStates; i++)
	{
		double sum = 0.;
		for(int j = 0; j < nSymbols; j++)
		{
			sum += B.at(i).at(j);
		}
		if(abs(sum-1.) > 0.01) throw("symbol prob matrix not normalised");
		for(int j = 0; j < nSymbols; j++)
		{
			B.at(i).at(j) /= sum;
		}
	}
}

void HMM::forward_backward()
{
	//set first line of alpha is observed probs in B ready for forward pass
	for(int i = 0; i < nStates; i++)
	{
		alpha.at(0).at(i) == B.at(i).at(observedData.at(0));
	}


	arnrm.at(0) = 0;
	//Forward Pass
	for(int t = 1; t < nObservations; t++) // for the number of observations
	{
		double sumA = 0;
		for(int j = 0; j < nStates; j++) // for the number of states
		{
			double sum = 0.;
			// for the number of states add together the forward probabilities
			for(int i = 0; i < nStates; i++) sum += alpha.at(t-1).at(i) * A.at(i).at(j) * B.at(j).at(observedData.at(t));
			alpha.at(t).at(j) = sum;
			sumA += sum;
		}

		//Reormalize the alpha's as necessary to avoid under-flow, keeping track of how many renormalizations for each alpha
		arnrm.at(t) = arnrm.at(t-1);
		if(sumA < BIGI)
		{
			++arnrm.at(t);
			for(int j = 0; j < nStates; j++) alpha.at(t).at(j) *= BIG;
		}
	}

	//set last emission prob to 1 ready for backward pass
	for(int i = 0; i < nStates; i++)
	{
		beta.at(nObservations-1).at(i) = 1.;
	}
	brnrm.at(nObservations-1) = 0;

	//Backward Pass
	// for the number of observations (going backwards)
	for(int t = nObservations-2; t >= 0; t--)
	{
		double sumB = 0.;
		//for the number of states
		for(int i = 0; i < nStates; i++)
		{
			double sum = 0.;
			// for the number of states calculate the backward probabilities
			for(int j = 0; j < nStates; j++) sum += A.at(i).at(j) * B.at(j).at(observedData.at(t+1)) * beta.at(t+1).at(j);
			beta.at(t).at(i) = sum;
			sumB += sum;
		}
		// renormalize the beta's as necessary
		brnrm.at(t) = brnrm.at(t+1);
		if(sumB < BIGI)
		{
			++brnrm.at(t);
			for(int j = 0; j < nStates; j++) beta.at(t).at(j) *= BIG;
		}
	}

	lhood = 0.;	//Overall likelihood is lhood with lnorm renormalizations
	//create likelihood of path using alpha and beta
	for(int i = 0; i < nStates; i++)
	{
		lhood += alpha.at(0).at(i) * beta.at(0).at(i);
	}
		lrnrm = arnrm.at(0) + brnrm.at(0);
		//while the likelihood is less than BIG integer
		while(lhood < BIGI) 
		{
			lhood *= BIG; 
			lrnrm++;
		}
		//for the number of observations get state probs from alpha and beta
		for(int t = 0; t < nObservations;t++)
		{
			double sum = 0.;
			//for the number of states
			for(int i = 0;i < nStates; i++)
			{
				//add the probabilities of the observations 
				sum += (pState.at(t).at(i) = alpha.at(t).at(i) * beta.at(t).at(i));
			}

			//sum = lhood * pow(BIGI, lrnrm - arnrm.at(t) - brnrm.at(t));
			for(int i = 0; i < nStates; i++)
			{
				pState.at(t).at(i) /= sum;
			}
		}
		fbdone = 1;		//prevents missuse of baum welch
}

void HMM::baum_welch()
{
	//create new B matrix
	vector<double> temp = vector<double>(nSymbols);
	vector<vector<double>> newB;
	for(int i = 0; i < nStates; i++) newB.push_back(temp);

	double powtab[10];

	for(int i = 0; i < 10; i++) powtab[i] = pow(BIGI, i-6);	//fill table with powers of BIGI

	//if forward-backward has not been done
	if(fbdone != 1) throw("must do forward_backward first");

	//looping through get denominators and new B
	for(int i = 0; i < nStates; i++)
	{
		double denom = 0.;

		//for the number of symbols set all values in new B to 0.
		for(int j = 0; j < nSymbols; j++)
		{
			newB.at(i).at(j) = 0.;
		}
		for(int j = 0; j < nObservations; j++)
		{
			double term = (alpha.at(j).at(i) * beta.at(j).at(i) / lhood) * powtab[arnrm.at(j) + brnrm.at(j) - lrnrm + 6];
			denom += term;

			newB.at(i).at(observedData.at(j)) += term;
		}

		// inner loop over j to get the elements of A
		for(int j = 0; j < nStates; j++)
		{
			double num = 0.;
			for(int k = 0; k < nObservations-1; k++)
			{
				num += alpha.at(k).at(i) * B.at(j).at(observedData.at(k+1)) * beta.at(k+1).at(j) * powtab[arnrm.at(k) + brnrm.at(k+1) - lrnrm + 6] / lhood;
			}
			A.at(i).at(j) *= (num/denom);
		}
		for(int j = 0; j < nSymbols; j++) newB.at(i).at(j) /= denom;
	}
	B = newB;
	fbdone = 0;	//dont let this be called until forward-backward called
}



#endif // !HMM_H_
