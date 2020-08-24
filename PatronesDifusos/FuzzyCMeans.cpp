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

	centroides = crear_arreglo_random(num_clusters, num_dimensiones);
	grado_pertenencia = crear_arreglo_random(num_datos, num_clusters);

	double s;
	int r, rval;
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
	*mientras la el error sea mayor que el permitido o  el numero de iteraciones actual es menor que el limite,
	El algoritmo seguira recalculando la posicion de los centroides.
	*/
	double** FuzzyCMeans::fcm()
	{
		double max_dif;
		int iteracion = 0;

		do {
			calcular_vectores_centroides();
			max_dif = actualizar_pertenencias();
			iteracion++;
		} while (max_dif > epsilon || iteracion <= iteraciones);

		return centroides;
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
		int i, j, k;
		double numerator, denominator;
		double** t = crear_arreglo_random(num_datos, num_clusters);

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

	/*
	Metodo para crear un arreglo de manera dinamica.
	*/
	double** FuzzyCMeans::crear_arreglo_random(int fila, int columna) //Metodo para crear arreglos dinamincos
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

