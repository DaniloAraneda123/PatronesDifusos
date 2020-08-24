#pragma once
#include <iostream>

using namespace std;


class FuzzyCMeans
{
private:
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

	FuzzyCMeans(int nDatos, int nClusters, int nDimensiones, int it, double error, double gradoFuzzy, double** datos);

	double** fcm();

	bool guardarCentroides(string ruta);

	void cargar_centroides(double** matriz);

	double** getCentroides();

	double** getPertenencia();

private:

	void calcular_vectores_centroides();

	double get_norm(int i, int j);

	double get_new_pertenecia(int i, int j);

	double actualizar_pertenencias();

	double** crear_arreglo_random(int fila, int columna);
};