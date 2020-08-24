#pragma warning (disable : 4996)

#include <stdio.h>
#include <iostream>
#include "FuzzyCMeans.h"

using namespace std;

int main()
{
	int num_data_points, num_clusters, num_dimensions;
	double borrosidad, epsilon;
	FILE* f;
	f = fopen("points.dat", "r");
	if (NULL == f)
	{
		cout << "error";
	}
	else
	{
		fscanf(f, "%d %d %d", &num_data_points, &num_clusters, &num_dimensions);
		fscanf(f, "%lf %lf", &borrosidad, &epsilon);
		double** data_point = new double* [num_data_points];
		for (int i = 0; i < num_data_points; i++)
		{
			data_point[i] = new double[num_dimensions];

			for (int j = 0; j < num_dimensions; j++)
			{
				fscanf(f, "%lf", &data_point[i][j]);
				//cout << data_point[i][j]<< "  --  ";
			}
			cout << endl;
		}

		FuzzyCMeans instancia(num_data_points, num_clusters, num_dimensions, 50, epsilon, 3.0, data_point);
		double** pertenecia, ** patrones;

		pertenecia = instancia.getPertenencia();
		patrones = instancia.fcm();

		for (int i = 0; i < num_clusters; i++)
		{
			cout << "Centroide " << i << ":  ";
			for (int j = 0; j < num_dimensions; j++)
			{
				cout << patrones[i][j] << " - ";
			}
			cout << endl;
		}

		cout << "Pertencias: ";
		for (int i = 0; i < num_clusters; i++)
		{
			cout << pertenecia[0][i] << " - ";
		}

	}
}