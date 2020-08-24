#pragma warning (disable : 4996)
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;


class FuzzyCMeans
{
	public:
		int num_datos;
		int num_clusters;
		int num_dimensiones;
		int iteraciones;
		double** grado_pertenencia;
		double epsilon;
		double borrosidad;
		double** data_puntos;
		double** centroides;

	public:

		//Construcctor
		FuzzyCMeans(int nDatos, int nClusters,int nDimensiones,int it,double error, double gradoFuzzy,double** datos ) 
		{
			num_datos = nDatos;
			num_clusters = nClusters;
			num_dimensiones = nDimensiones;
			iteraciones = it;
			epsilon = error;
			borrosidad = gradoFuzzy;
			data_puntos = datos;

			centroides = crear_arreglo_random(num_clusters, num_dimensiones);
			grado_pertenencia= crear_arreglo_random(num_datos, num_clusters);

			double s;
			int r,rval;
			for (int i = 0; i < num_datos; i++) {
				s = 0.0;
				r = 100;
				for (int j = 1; j < num_clusters; j++) {
					rval = rand() % (r + 1);
					r -= rval;
					grado_pertenencia[i][j] = rval / 100.0;
					s += grado_pertenencia[i][j];
				}
				grado_pertenencia[i][0] = 1.0 - s;
			}

		}


		/*
		*mientras la 
		*/
		double** fcm() 
		{
			double max_dif;
			int iteracion=0;

			do {
				calcular_vectores_centroides();
				max_dif = update_degree_of_membership();
				iteracion++;
			} while (max_dif > epsilon || iteracion<=iteraciones );

			return centroides;
		}

		/*COMPLETAR*/
		bool guardarCentroides(string ruta)
		{
			FILE* f;
			f = fopen("points.dat", "w");
		}

		void cargar_centroides(double** matriz)
		{
			centroides = matriz;
			update_degree_of_membership();
		}

		double** getCentroides()
		{
			return centroides;
		}

		double** getPertenencia()
		{
			return grado_pertenencia;
		}

	private:

		void calcular_vectores_centroides() 
		{
			int i, j, k;
			double numerator, denominator;
			double** t= crear_arreglo_random(num_datos, num_clusters);

			for (i = 0; i < num_datos; i++) 
			{

				for (j = 0; j < num_clusters; j++) 
				{
					t[i][j] = pow(grado_pertenencia[i][j], borrosidad);
				}

			}

			for (j = 0; j < num_clusters; j++) 
			{

				for (k = 0; k < num_dimensiones; k++) 
				{
					numerator = 0.0;
					denominator = 0.0;

					for (i = 0; i < num_datos; i++) 
					{
						numerator += t[i][j] * data_puntos[i][k];
						denominator += t[i][j];
					}
					centroides[j][k] = numerator / denominator;

				}

			}
		}

		// Calcula la norma del vecto entre el punto i y el centroide j.
		double get_norm(int i, int j) 
		{
			int k;//dimension
			double sum = 0.0;

			for (k = 0; k < num_dimensiones; k++) 
			{
				sum += pow(data_puntos[i][k] - centroides[j][k], 2);
			}

			return sqrt(sum);
		}

		/*
		
		*/
		double get_new_pertenecia(int i, int j) 
		{
			int k;
			double t, p, sum;
			sum = 0.0;
			p = 2 / (borrosidad - 1);

			for (k = 0; k < num_clusters; k++) 
			{
				t = get_norm(i, j) / get_norm(i, k);
				t = pow(t, p);
				sum += t;
			}

			return 1.0 / sum;
		}

		/*
		Este metodo actualiza la matriz la pertenencia de cada
		punto por cada cluster , ademas retorna la diferencia entre
		la maxima diferencia entre la nueva pertenencia y la pertenencia anterior de un dato
		*/
		double update_degree_of_membership() 
		{
			int i, j;
			double new_Wij;
			double max_dif = 0, dif;
			for (j = 0; j < num_clusters; j++) 
			{

				for (i = 0; i < num_datos; i++) 
				{

					new_Wij = get_new_pertenecia(i, j);
					dif = new_Wij - grado_pertenencia[i][j];

					if (dif > max_dif)
					{
						max_dif = dif;
					}

					grado_pertenencia[i][j] = new_Wij;

				}

			}
			return max_dif;
		}

		/*
		Metodo para crear un arreglo de manera dinamica.
		*/
		double** crear_arreglo_random(int fila, int columna) //Metodo para crear arreglos dinamincos
		{
			int rval;
			double** matriz = new double* [fila];

			for (int i = 0; i < fila; i++)
			{
				matriz[i] = new double[columna];

				for (int j = 0; j < columna; j++)
				{
					rval = rand() % (100 + 1);
					matriz[i][j] = rval / 100.0;
				}
			}

			return matriz;
		}
};

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
		double** patrones = instancia.fcm();


		for (int i = 0; i < num_clusters; i++)
		{
			cout << "Centroide " << i<<":  ";
			for (int j = 0; j < num_dimensions; j++)
			{
				cout << patrones[i][j]<< " - ";
			}
			cout << endl;
		}

		cout << "Pertencias: ";
		for (int i = 0; i < num_clusters; i++)
		{
			cout << instancia.grado_pertenencia[0][i]<< " - ";
		}

	}
}