//#pragma warning (disable : 4996)

#include <stdio.h>
#include <iostream>
#include "FuzzyCMeans.h"
#include "Dataset.h"
#include "FuzzyID3.h"
#include <set>
#include "TreeNodo.h"

using namespace std;


int main()
{
	/*
	double** pertenecia, ** patrones;
	int num_data_points, num_clusters, num_dimensions;
	double borrosidad, epsilon;
	FILE* f;

	f = fopen("iris.dat", "r");
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
			}
		}

		FuzzyCMeans instancia(num_data_points, num_clusters, num_dimensions, 100, epsilon, borrosidad, data_point);


		patrones = instancia.fcm();
		pertenecia = instancia.getPertenencia();



		cout << endl; cout << endl;
		cout << "CENTROIDES "<<endl;
		for (int i = 0; i < num_clusters; i++)
		{
			cout << "Centroide " << i << ":  ";
			for (int j = 0; j < num_dimensions; j++)
			{

				cout << patrones[i][j] << " | ";
			}
			cout << endl;
		}


		cout << endl; cout << endl;
		cout << "Pertenencia: "<<endl;

		for (int i = 0; i < num_data_points; i++)
		{
			for (int j = 0; j < num_clusters; j++)
			{
				cout << pertenecia[i][j] << " | ";
			}
			cout << endl;
		}

		cin.get();

	}*/


	//eate a dataset
	Dataset d;

	d.addAtributo("Outlook"		,vector<string>({ "Sunny", "Cloudy", "Rain"				})	);

	d.addAtributo("Temprature"	,vector<string>({"Hot", "Mild", "Cool"					})	);

	d.addAtributo("Humidity"	,vector<string>({ "Humid", "Normal"						})	);

	d.addAtributo("Wind"		,vector<string>({ "Windy", "Not_Windy"					})	);

	d.addAtributo("Plan"		,vector<string>({ "Volleyball", "Swimming", "W_lifting"	})	);

	d.addFila(vector<double>({ 0.9, 0.1, 0.0, 1.0, 0.0, 0.0, 0.8, 0.2, 0.4, 0.6, 0.0, 0.8, 0.2	}));
	d.addFila(vector<double>({ 0.8, 0.2, 0.0, 0.6, 0.4, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.7, 0.0	}));
	d.addFila(vector<double>({ 0.0, 0.7, 0.3, 0.8, 0.2, 0.0, 0.1, 0.9, 0.2, 0.8, 0.3, 0.6, 0.1	}));
	d.addFila(vector<double>({ 0.2, 0.7, 0.1, 0.3, 0.7, 0.0, 0.2, 0.8, 0.3, 0.7, 0.9, 0.1, 0.0	}));
	d.addFila(vector<double>({ 0.0, 0.1, 0.9, 0.7, 0.3, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 1.0	}));
	d.addFila(vector<double>({ 0.0, 0.7, 0.3, 0.0, 0.3, 0.7, 0.7, 0.3, 0.4, 0.6, 0.2, 0.0, 0.8	}));
	d.addFila(vector<double>({ 0.0, 0.3, 0.7, 0.0, 0.0, 1.0, 0.0, 1.0, 0.1, 0.9, 0.0, 0.0, 1.0	}));
	d.addFila(vector<double>({ 0.0, 1.0, 0.0, 0.0, 0.2, 0.8, 0.2, 0.8, 0.0, 1.0, 0.7, 0.0, 0.3	}));
	d.addFila(vector<double>({ 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.6, 0.4, 0.7, 0.3, 0.2, 0.8, 0.0  }));
	d.addFila(vector<double>({ 0.9, 0.1, 0.0, 0.0, 0.3, 0.7, 0.0, 1.0, 0.9, 0.1, 0.0, 0.3, 0.7	}));
	d.addFila(vector<double>({ 0.7, 0.3, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.2, 0.8, 0.4, 0.7, 0.0	}));
	d.addFila(vector<double>({ 0.2, 0.6, 0.2, 0.0, 1.0, 0.0, 0.3, 0.7, 0.3, 0.7, 0.7, 0.2, 0.1	}));
	d.addFila(vector<double>({ 0.9, 0.1, 0.0, 0.2, 0.8, 0.0, 0.1, 0.9, 1.0, 0.0, 0.0, 0.0, 1.0	}));
	d.addFila(vector<double>({ 0.0, 0.9, 0.1, 0.0, 0.9, 0.1, 0.1, 0.9, 0.7, 0.3, 0.0, 0.0, 1.0	}));
	d.addFila(vector<double>({ 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.8, 0.2, 0.0, 0.0, 1.0	}));
	d.addFila(vector<double>({ 1.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 1.0, 0.8, 0.6, 0.0	}));

	//d.imprimirDataSet();

	FuzzyID3 fdt (0.7, 0.0);

	cout << "G(Outlook) = "<< fdt.getAmbiguedad(d, "Plan", "Outlook") << endl;
	cout << "G(Temprature) = "<< fdt.getAmbiguedad(d, "Plan", "Temprature") << endl;
	cout << "G(Humidity) = "<< fdt.getAmbiguedad(d, "Plan", "Humidity") << endl;
	cout << "G(Wind) = "<<fdt.getAmbiguedad(d, "Plan", "Wind")<<endl << endl << endl << endl;

	TreeNode root = fdt.construirArbol(d, "Plan");

	fdt.printTree(root, "");

	
	cout << root.getChildren().size()<<endl;
	vector<TreeNode> c = root.getChildren();

	for (int i=0; i < c.size() ;i++)
	{
		cout<<c[i].getChildren().size() <<endl;
	}

	cout << fdt.miNodo->getTitle();

	//vector<string> rules = fdt.generarReglas(root);

	//cout << rules.size();
	//for (string &rule : rules) 
	//{
		//cout<<rule<<endl;
	//}
}