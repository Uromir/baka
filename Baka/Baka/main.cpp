#include "DOptimization.h"

int main()
{
	Plan mainPlan(25, 2);

	/*ofstream out("fisher.txt");
	for(int i = 0; i < mainPlan.remarkCount; i++)
	{
		out << mainPlan.plan[i].coord[0] << " " << mainPlan.plan[i].coord[1] << " " << mainPlan.plan[i].weight << endl;
	}
	out << endl << endl << endl;
	DOptimization mainOptimizationAlgoritm(&mainPlan, -1, 1, 0.01);
	Plan optimal = mainOptimizationAlgoritm.calcOptimalPlan();

	vector<vector<double>> matr = mainOptimizationAlgoritm.getFisherMatrix();

	for (int i = 0; i < matr.size(); i++)
	{
		for (int j = 0; j < matr[i].size(); j++)
		{
			out << matr[i][j] << "                ";
		}
		out << endl;
	}
	
	out << endl << endl << endl;

	for(int i = 0; i < optimal.remarkCount; i++)
	{
		out << optimal.plan[i].coord[0] << " " << optimal.plan[i].coord[1] << " " << optimal.plan[i].weight << endl;
	}*/
	int n = 6;
	vector<vector<double>> a(n);
	ifstream in("plan.txt");
	for (int i = 0; i < n; i++)
	{
		a[i].resize(n);
		for (int j = 0; j < n; j++)
		{
			in >> a[i][j];
		}
	}

	int i,j,p,k=0,t;
	double kst;
	char yn;
	p = 0;
	for (i=0; i<n-1; i++)
	{
		t=1;
		while(a[i][i]==0)
		{
			for(j=0; j<n; j++)
			{
				a[i][j]=kst;
				a[i][j]=a[i+t][j];
				a[i+t][j]=kst;
			}
			p++;
			t++;
		}
 
		for (k=i+1; k<n; k++)
		{
			kst=a[k][i]/a[i][i];
			for(j=0; j<n; j++)
				a[k][j]-=a[i][j]*kst;
		}
	}
 
	kst=pow(-1,p);
	for(i=0; i<n; i++)
		kst*=a[i][i];

	cout << kst;

	return 0;
}