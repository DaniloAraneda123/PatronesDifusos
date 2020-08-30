#pragma once

#include <string>
#include <vector>
#include<iostream>

using namespace std;

class Dataset
{


private:

    vector<string> atributos;

    vector<string> variablesLinguisticas;

vector<vector<double>> datos;

public:

    //Constructor
    Dataset() {}

    //Constructor
    Dataset(vector<string> atributos, vector<string> variablesLinguisticas, vector<vector<double>> datos)
    {
        this->atributos = atributos;
        this->variablesLinguisticas = variablesLinguisticas;
        this->datos = datos;
    }

    //Agregar Atribbuto con sus variables Linguisticas
    void addAtributo(string atributo, vector<string> varLinguisticas)
    {
        for (int i = 0; i < varLinguisticas.size(); i++)
        {
            atributos.push_back(atributo);
            variablesLinguisticas.push_back(varLinguisticas[i]);
        }
    }

    //Agregar Atribbuto con sus variables Linguisticas
    void addFila(vector<double> valoresFuzzy)
    {
        datos.push_back(valoresFuzzy);
    }



    //Retorna una lista con los nombres de los terminos linguisticos de un atributo.
    vector<string>  getTerminosLinguisticos(string atributo)
    {
        vector<string> lista;
        for (int i = 0; i < atributos.size(); i++)
        {
            if (atributos[i] == atributo)
            {
                lista.push_back(variablesLinguisticas[i]);
            }
        }
        return lista;
    }


    //Retorna una lista con los valores de una columna
    vector<double> getValoresCol(string atributo, string termLinguistico)
    {
        vector<double> lista;
        int columna = 0;

        for (int i = 0; i < variablesLinguisticas.size(); i++)
        {
            if (atributos[i] == atributo && variablesLinguisticas[i] == termLinguistico)
            {
                columna = i;
            }
        }

        for (int i = 0; i < datos.size(); i++)
        {
            lista.push_back(datos[i][columna]);
        }

        return lista;

    }

    void imprimirDataSet()
    {
        //Imprimir Atributos
        for (int i = 0; i < atributos.size(); i++)
        {
            cout << atributos[i] << "  |  ";
        }
        cout << endl;

        //Imprimit Variables Linguisticas
        for (int i = 0; i < variablesLinguisticas.size(); i++)
        {
            cout << variablesLinguisticas[i] << "  |  ";
        }
        cout << endl;

        for (int i = 0; i < datos.size(); i++)
        {
            for (int j = 0; j < datos[i].size(); j++)
            {
                cout << datos[i][j] << "  ";
            }
            cout << endl;
        }
    }

    //Retorna una lista con el nombre de todos atributos menos del nombre de la clase 
    vector<string> getNombreAtributos(string nombreClase)
    {
        vector<string> lista;
        string actual="";
        for(int i = 0 ; i<atributos.size() ; i++)
        {
            if (atributos[i]!= nombreClase && atributos[i]!=actual)
            {
                lista.push_back(atributos[i]);
                actual = atributos[i];
            }
        }
        return lista;
    }

    double getValor(string atributo,string termino, int fila)
    {
        int columna = 0;
        for (int i = 0; i < variablesLinguisticas.size(); i++)
        {
            if (atributos[i] == atributo && variablesLinguisticas[i] == termino)
            {
                columna = i;
            }
        }

        return datos[fila][columna];
    }

};


