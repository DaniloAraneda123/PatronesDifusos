#include <math.h>
#include "FuzzyCMeans.h"

using namespace std;


/*
¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡NOTA IMPORTANTE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
El disenio por contranto aun no es documentando en el codigo.
*/

//Construcctor
	FuzzyCMeans::FuzzyCMeans(int nDatos, int nClusters, int nDimensiones, int it, double error, double gradoFuzzy, double** datos)
	{
		num_datos = nDatos;
		num_clusters = nClusters;
		num_dimensiones = nDimensiones;
		iteraciones = it;
		epsilon = error;
		borrosidad = gradoFuzzy;
		data_puntos = datos;


		//Crear Matriz Centroides
		centroides = new double* [num_clusters];
		for (int i = 0; i < num_clusters; i++)
		{
			centroides[i] = new double[num_dimensiones];

			for (int j = 0; j < num_dimensiones; j++)
			{
				centroides[i][j] = 0;
			}
		}

		//Crear Matriz grados de pertencia 
		double s;
		int r, rval;
		grado_pertenencia = new double* [num_datos];

		for (int i = 0; i < num_datos; i++) 
		{
			s = 0.0;
			r = 100;
			grado_pertenencia[i] = new double[num_clusters];
			for (int j = 0; j < num_clusters; j++) 
			{
				rval = rand() % (r + 1);
				r -= rval;
				grado_pertenencia[i][j] = rval / 100.0;
				s += grado_pertenencia[i][j];
			}
			grado_pertenencia[i][0] = 1.0 - s;
		}
	}


	/*
	*mientras la el error sea mayor que el permitido o  el numero de iteraciones actual es menor que el limite,
	El algoritmo seguira recalculando la posicion de los centroides.
	*/
	double** FuzzyCMeans::fcm()
	{
		double max_dif=0;
		int iteracion = 0;

		cout << "num_data_points: " << num_datos << "    num_clusters:  " << num_clusters << "   num_dimensions:  " << num_dimensiones << endl;
		cout << "borrosidad: " << borrosidad << "   epsilon:  " << epsilon << endl;

		do {



			calcular_vectores_centroides();
			max_dif = actualizar_pertenencias();
			iteracion++;


			cout << "Terminada Iteracion: "<< iteracion <<"  , error: " << max_dif << " es mayor que "<< epsilon <<endl;
			for (int i = 0; i < num_clusters; i++)
			{
				cout << "Centroide " << i << ":  ";
				for (int j = 0; j < num_dimensiones; j++)
				{

					cout << centroides[i][j] << " | ";
				}
				cout << endl;
			}
			cout << endl; cout << endl; cout << "////////////////////////////"; cout << endl;

		} while (max_dif > epsilon && iteracion < iteraciones);

		return centroides;
	}

	bool FuzzyCMeans::guardarCentroides(string ruta)
	{
		return false;
	}

	/*COMPLETAR*/
	/*bool FuzzyCMeans::guardarCentroides(string ruta)
	{
		FILE* f;
		f = fopen("points.dat", "w");

	}*/

	void FuzzyCMeans::cargar_centroides(double** matriz)
	{
		centroides = matriz;
		actualizar_pertenencias();
	}

	double** FuzzyCMeans::getCentroides()
	{
		return centroides;
	}

	double** FuzzyCMeans::getPertenencia()
	{
		return grado_pertenencia;
	}


	/*
	A partir de la distancia de la dintacia de los puntos a los centroides viejos, se calcula los nuevo centroides.
	*/
	void FuzzyCMeans::calcular_vectores_centroides()
	{
		double numerator, denominator;
		double** t;
		t = new double* [num_datos];

		for (int i = 0; i < num_datos; i++)
		{
			t[i] = new double [num_clusters];
			for (int j = 0; j < num_clusters; j++)
			{
				t[i][j] = pow(grado_pertenencia[i][j], borrosidad);
			}
		}

		for (int j = 0; j < num_clusters; j++)
		{
			for (int k = 0; k < num_dimensiones; k++)
			{
				numerator = 0.0;
				denominator = 0.0;
				for (int i = 0; i < num_datos; i++)
				{
					numerator += t[i][j] * data_puntos[i][k];
					denominator += t[i][j];
				}
				centroides[j][k] = numerator / denominator;
			}
		}
	}

	// Calcula la norma del vecto entre el punto i y el centroide j.
	double FuzzyCMeans::get_norm(int i, int j)
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
	Se calcula la pertenencia de una tupla i a un centride j
	*/
	double FuzzyCMeans::get_new_pertenecia(int i, int j)
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
	double FuzzyCMeans::actualizar_pertenencias()
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

