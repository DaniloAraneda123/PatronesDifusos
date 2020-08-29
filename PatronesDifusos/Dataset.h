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
        for(int i = 0 ; atributos.size() ; i++)
        {
            if (atributos[i]!= nombreClase && nombreClase!=actual)
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
/*
    void addRow(Row row)
    {
        row.setIndex(this->rows.size());
        //row.setDataset(*this);
        this->rows.push_back(row);
    }

    void  addAttribute(Attribute attribute)
    {
        attribute.setIndex(this->attribues.size());
        //attribute.setDataset(*this);
        this->attribues.push_back(attribute);
    }*/

    /*
    int Dataset::getRowsCount()
    {
        return this->rows.size();
    }

    int Dataset::getAttributesCount()
    {
        return this->attribues.size();
    }

    Attribute Dataset::getAttribute(int idx)
    {
        return this->attribues[idx];
    }

    Attribute Dataset::getAttribute(string attrName)
    {
        for (int i = 0; i < attribues.size(); i++)
        {
            if (attribues[i].getName() == attrName)
            {
                return attribues[i];
            }
        }

    }


    /*
    Object Dataset::getCrispValue(int rowIdx, int attrIdx)
    {
        return this->rows[rowIdx].crispValues[attrIdx];
    }

    Object Dataset::getCrispValue(int rowIdx, string attrName) {
        int i = 0;
        for (; i < attribues.size(); i++) {
            if (attribues[i].getName()==attrName)
            {
                return this->getCrispValue(rowIdx, i);
            }
        }
    }*/

    /*double Dataset::getFuzzyValue(int rowIdx, int attrIdx, int termIdx)
    {
        return this->rows[rowIdx].getFuzzyValue(attrIdx, termIdx);
    }

    void Dataset::setFuzzyValue(int rowIdx, int idx, int termIdx, double val)
    {
        this->rows[rowIdx].setFuzzyValue(idx, termIdx, val);
    }

    double Dataset::getFuzzyValue(int rowIdx, int attrIdx, string termName)
    {
        return this->rows[rowIdx].getFuzzyValue(attrIdx, termName);
    }

    void Dataset::setFuzzyValue(int rowIdx, int idx, string termName, double val)
    {
        this->rows[rowIdx].setFuzzyValue(idx, termName, val);
    }

    double Dataset::getFuzzyValue(int rowIdx, string attrName, int termIdx)
    {
        return this->rows[rowIdx].getFuzzyValue(attrName, termIdx);
    }

    void Dataset::setFuzzyValue(int rowIdx, string attrName, int termIdx, double val)
    {
        this->rows[rowIdx].setFuzzyValue(attrName, termIdx, val);
    }

    double Dataset::getFuzzyValue(int rowIdx, string attrName, string termName)
    {
        return this->rows[rowIdx].getFuzzyValue(attrName, termName);
    }

    void Dataset::setFuzzyValue(int rowIdx, string attrName, string termName, double val)
    {
        this->rows[rowIdx].setFuzzyValue(attrName, termName, val);
    }

    vector<double> Dataset::getFuzzyValues(int rowIdx, int attrIdx)
    {
        return this->attribues[attrIdx].getFuzzyValues(rowIdx);
    }

    vector<double> Dataset::getFuzzyValues(int rowIdx, string attrName)
    {
        int i = 0;
        for (; i < this->attribues.size(); i++) {
            if (this->attribues[i].getName()==attrName) {
                return this->attribues[i].getFuzzyValues(rowIdx);
            }
        }
    }

    void Dataset::setFuzzyValues(int rowIdx, int attrIdx, double** values)
    {
        this->rows[rowIdx].setFuzzyValues(attrIdx, values);
    }

    void Dataset::setFuzzyValues(int rowIdx, string attrName, double* values)
    {
        this->rows[rowIdx].setFuzzyValues(attrName, values);
    }

    void Dataset::setFuzzyValues(int rowIdx, double** values)
    {
        this->rows[rowIdx].setFuzzyValues(values);
    }*/

