#pragma once

#include <string>
#include "Dataset.h"
#include "Utils.h"
#include <stdlib.h> 
#include <vector> 
#include "TreeNodo.h"

class FuzzyID3
{

public:
    double nivelVerdad;
    double nivelSignificacia;
    TreeNode* miNodo;

    FuzzyID3(double truthLevel, double significantLevel)
    {
        nivelVerdad = truthLevel;
        nivelSignificacia = significantLevel;
        miNodo=&(TreeNode("HOJA", "UnKnown"));
    }

    //Metodo principal por el cual se empieza a construir el arbol
    TreeNode construirArbol(Dataset dataset, string nombreClase)  
    {
        vector<string> atributos = dataset.getNombreAtributos(nombreClase);
        vector<string> listavacia(0);
        return growTree(dataset, listavacia, atributos, nombreClase);
    }

private:

    TreeNode growTree(Dataset dataset, vector<string> padre, vector<string> atributos, string nombreClase)
    {
        //Si no hay atributos crea un nodo hoja basico
        if (atributos.size() == 0) {
            TreeNode hojita("HOJA", "UnKnown");
            return hojita;
        }

        //Si es la raiz
        if (padre.size() == 0)
        {
            double AmbiguedadMinima = 10000;
            string bestAtributo="";

            //Buscamos el atributo con la menor Ambiguedad
            for (string &atr : atributos) 
            {
                double ambResult = getAmbiguedad(dataset, nombreClase, atr);
                if (ambResult < AmbiguedadMinima)
                {
                    AmbiguedadMinima = ambResult;
                    bestAtributo = atr;
                }
            }

            //Creo La Raiz que es de tipo atributo
            TreeNode root= TreeNode("ATRIBUTO", bestAtributo);
            cout << "1:---Se crea un nodo ATRIBUTO " << bestAtributo<<endl;

            //Obtengo los terminos linguisticos
            vector<string> terminos = dataset.getTerminosLinguisticos(bestAtributo);
            vector<string> terminosClase = dataset.getTerminosLinguisticos(nombreClase);

            //Para cada termino del mejor atributo se obtiene el grado de clasificacion de verdad recpecto a cada termino de la clase
            for (int i=0; i<terminos.size();i++)
            {
                bool canBelongeToClass = false;
                for (string &classTerm : terminosClase)
                {

                    double dot = degreeOfClassificationTruth(dataset, vector<string>({ bestAtributo, terminos[i], nombreClase, classTerm }));

                    //Si el grado de verdad de la casificacion es mayor que el nivel de verdad minimo, se crean los hijos del del termino y se asignan
                    if (dot > nivelVerdad)
                    {
                        TreeNode c= TreeNode("VALOR", terminos[i]);
                        cout << "2:---Se crea un nodo VALOR " << terminos[i] <<" con valor de "<< dot <<" y su padre es "<<root.getTitle()<< endl;
                        TreeNode c2= TreeNode("HOJA", classTerm);
                        cout << "3:---Se crea un nodo HOJA " << classTerm << " y su padre es "<< terminos[i] <<endl;
                        c2.setValue(dot);
                        c.addChild(c2);
                        root.addChild(c);
                        canBelongeToClass = true;

                        break;
                    }
                }
                //puede pertenecer al sub arbol
                if (!canBelongeToClass) 
                {
                    //Removemos el best Atributo ,  que fue ocupado ya
                    vector <string> newAtributos(atributos.size() - 1);
                    for (int j = 0, k = 0; j < atributos.size(); j++)
                    {
                        if (!(atributos[j]==bestAtributo))
                        {
                            newAtributos[k++] = atributos[j];
                        }
                    }

                    //Ahora, el best atribbuto el nuevo padre
                    vector <string> newArgs(2);
                    newArgs[0] = bestAtributo;
                    newArgs[1] = terminos[i];

                    TreeNode node= TreeNode("VALOR", terminos[i]);
                    root.addChild(node);

                    cout << "4:---Se crea un nodo VALOR " << node.getTitle() <<" y su padre es " << root.getTitle() << endl;

                    TreeNode c = growTree(dataset, newArgs, newAtributos, nombreClase);
                    //Se uno como hijo el nuevo sub arbol generado
                    node.addChild(c);
                    miNodo = &c;
                    cout << "5:---Se crea un nodo VALOR " << c.getTitle() << " y su padre es " << node.getTitle() << endl;
                }
            }

            return root;
        }
        else//Hay atributos Padres
        {
            string bestAtributo = "";
            if (atributos.size() > 1)
            {
                vector<string> args1(padre.size() + 1);
                for (int i = 0; i < padre.size(); i++)
                    args1.push_back(padre[i]);
                args1.push_back(nombreClase);

                double ambg = getAmbiguedad(dataset, nombreClase, padre);;

                for (string& atr : atributos)
                {
                    vector<string>  args2(padre.size() + 1);
                    args2.assign(args1.begin(), args1.end());
                    args2[0] = atr;
                    double ambg2 = getAmbiguedad(dataset, nombreClase, atr, padre);

                    if (ambg2 <= ambg)
                    {
                        bestAtributo = atr;
                        ambg = ambg2;
                    }

                }

                if ("" == bestAtributo)
                {
                    vector<string> args1;
                    for (int q = 0; q < padre.size(); q++)
                        args1.push_back(padre[q]);
                    args1.push_back(nombreClase);
                    args1.push_back("");
                    vector<string> terminosClase = dataset.getTerminosLinguisticos(nombreClase);
                    string bestClass = "";
                    double bestDOT = -1;

                    for (string& classTerm : terminosClase)
                    {
                        args1[padre.size() + 1] = classTerm;
                        double dot = degreeOfClassificationTruth(dataset, args1);
                        if (dot > bestDOT)
                        {
                            bestDOT = dot;
                            bestClass = classTerm;
                        }
                    }
                    TreeNode c2= TreeNode("HOJA", bestClass);
                    c2.setValue(bestDOT);
                    cout << "6:---Se crea un nodo HOJA " << c2.getTitle() << " y su valor es " << c2.getValue() << endl;
                    return c2;
                }
                else
                {
                    TreeNode root= TreeNode("ATRIBUTO", bestAtributo);
                    cout << "7:---Se crea un nodo ATRIBUTO " << bestAtributo << endl;
                    vector <string> terms = dataset.getTerminosLinguisticos(bestAtributo);
                    vector <string> classTerms = dataset.getTerminosLinguisticos(nombreClase);
                    vector <string> args2(padre.size() + 4);
                    for (int i = 0; i < padre.size(); i++)
                        args2[i + 2] = padre[i];
                    args2[0] = bestAtributo;
                    args2[args2.size() - 2] = nombreClase;


                    for (string& term : terms)
                    {
                        bool canBelongeToClass = false;
                        args2[1] = term;

                        for (string& classTerm : classTerms)
                        {
                            args2[args2.size() - 1] = classTerm;
                            double dot = degreeOfClassificationTruth(dataset, args2);

                            if (dot > nivelVerdad)
                            {
                                TreeNode c= TreeNode("VALOR", term);
                                cout << "8:---Se crea un nodo VALOR " << term << " y su padre es " << root.getTitle() << endl;
                                TreeNode c2= TreeNode("HOJA", classTerm);
                                cout << "9:---Se crea un nodo HOJA " << classTerm << " y su valor es " << dot << " y su padre es " << c.getTitle() << endl;
                                c2.setValue(dot);
                                c.addChild(c2);
                                root.addChild(c);
                                canBelongeToClass = true;
                                break;
                            }
                        }

                        if (!canBelongeToClass)
                        {
                            //Remove the best attribute
                            vector <string> newAttrs(atributos.size() - 1);
                            for (int j = 0, k = 0; j < atributos.size(); j++)
                            {
                                if (atributos[j] != bestAtributo)
                                {
                                    newAttrs[k++] = atributos[j];
                                }
                            }
                            //get the new args
                            vector <string> newArgs(padre.size() + 2);
                            newArgs[0] = bestAtributo;
                            newArgs[1] = term;
                            for (int i = 0; i < padre.size(); i++)
                                newArgs.push_back(padre[i + 2]);

                            TreeNode node= TreeNode("VALOR", term);
                            root.addChild(node);
                            cout << "10:---Se crea un nodo VALOR " << node.getTitle() <<" y su padre es "<<root.getTitle()<< endl;

                            TreeNode c = growTree(dataset, newArgs, newAttrs, nombreClase);

                            if (&c != NULL)//ERROR
                            {
                                node.addChild(c);
                                cout << "11:---Se crea un nodo " <<c.getTipoNodo()+" "+ c.getTitle() << " y su padre es " << node.getTitle() << endl;
                            }

                        }

                    }
                    return root;
                }
            }
            else
            {
                bestAtributo = atributos[0];
                TreeNode root= TreeNode("ATRIBUTO", bestAtributo);
                cout << "12:---Se crea un nodo " << root.getTipoNodo() + " " + root.getTitle() << endl;
                vector <string> terms = dataset.getTerminosLinguisticos(bestAtributo);
                vector <string> terminosClase = dataset.getTerminosLinguisticos(nombreClase);

                vector <string> args2(padre.size() + 4);
                args2[0] = bestAtributo;
                args2[args2.size() - 2] = nombreClase;
                for (int i = 0; i < padre.size(); i++)
                    args2.push_back(padre[i + 2]);


                for (string& term : terms)
                {
                    args2[1] = term;
                    double maxTruth = -1;
                    string bestClass = "";
                    for (string& classTerm : terminosClase)
                    {
                        args2[args2.size() - 1] = classTerm;
                        double dot = degreeOfClassificationTruth(dataset, args2);
                        if (dot > maxTruth)
                        {
                            maxTruth = dot;
                            bestClass = classTerm;
                        }
                    }
                    TreeNode node("VALOR", term);
                    root.addChild(node);
                    TreeNode c("HOJA", bestClass);

                    c.setValue(maxTruth);
                    node.addChild(c);
                    cout << "13:---Se crea un nodo " << node.getTipoNodo() + " " + node.getTitle() << " y su padre es " << root.getTitle() << endl;
                    cout << "14:---Se crea un nodo " << c.getTipoNodo() + " " + c.getTitle() << " y su padre es " << node.getTitle() <<" y su valor es "+ to_string(c.getValue())<< endl;
                }
                return root;
            }
        }
    }

   

    void imprimirArreglo(vector<string>vecl)
    {
        for (int i=0; i<vecl.size() ; i++)
        {
            cout << vecl[i]<<" | ";
        }
        cout << endl;
        //cin.get();
    }

    void imprimirNodo(TreeNode nodo)
    {
        vector<TreeNode> lista=nodo.getChildren();


        cout << nodo.getTitle() << endl;
        cout << endl;
        for (int i = 0; i < lista.size(); i++)
        {
            cout << lista[i].getTitle() << endl;
        }
        //cin.get();
    }

    //Recibe pares de atribustos-termino
    double degreeOfClassificationTruth(Dataset dataset, vector<string> atributo_termino)
    {
        vector<double> a = dataset.getValoresCol(atributo_termino[0], atributo_termino[1]);

        int n = a.size();
        double v = 0;

        //Se queda con el menor valor de cada columnas atributo_termino
        for (int i = 0; i < n; i++)
        {
            for (int j = 2; j < atributo_termino.size() - 2; j += 2)
            {
                v = dataset.getValor(atributo_termino[j], atributo_termino[j + 1], i);
                a[i] = min(v, a[i]);
            }
        }

        int t = atributo_termino.size();
        vector<double> 
            b = dataset.getValoresCol(atributo_termino[t - 2], atributo_termino[t - 1]);

        return Utils::subSetHood(a, b);
    }


public:



    double getAmbiguedad(Dataset dataset, string nombreClase, string nombreAtributo, vector<string >evidence) 
    {
        vector<string> terms = dataset.getTerminosLinguisticos(nombreAtributo);
        vector<string>  classTerms = dataset.getTerminosLinguisticos(nombreClase);
        vector<double> a;

        for (int i = 0; i < evidence.size() - 1; i += 2) 
        {
            if (a.size()== 0) 
            {
                a = dataset.getValoresCol(evidence[i],evidence[i + 1]);
            }
            else 
            {
                vector<double> aPrime = dataset.getValoresCol(evidence[i],evidence[i + 1]);
                for (int j = 0; j < aPrime.size(); j++) 
                {
                    a[j] = min(a[j], aPrime[j]);
                }
            }
        }

        vector<vector<double>> b(terms.size());
        vector<double> weights(terms.size());
        double sum = 0;

        if (a.size()>0 ) 
        {
            for (int i = 0; i < a.size(); i++) 
            {
                if (a[i] < nivelSignificacia)
                {
                    a[i] = 0;
                }
            }

            int i = 0;

            for (string &term : terms) 
            {
                b[i] = dataset.getValoresCol(nombreAtributo,term);
                for (int j = 0; j < a.size(); j++) 
                {
                    b[i][j] = min(a[j], b[i][j]);
                }
                weights[i] = Utils::sum(b[i]);
                sum += weights[i++];
            }

            Utils::normalizeWith(&weights, sum);

            double g = 0;
            for (i = 0; i < weights.size(); i++) 
            {
                vector<double> normalizedPossibility(classTerms.size());
                double max = -1;
                for (int j = 0; j < classTerms.size(); j++) 
                {
                    vector<double> c = dataset.getValoresCol(nombreClase,classTerms[j]);
                    normalizedPossibility[j] = Utils::subSetHood(b[i], c);
                    if (normalizedPossibility[j] > max) 
                    {
                        max = normalizedPossibility[j];
                    }

                }

                Utils::normalizeWith(&normalizedPossibility, max);
                g += weights[i] * Utils::ambiguity(normalizedPossibility);
            }

            return g;
        }
        return -1;
    }

    double getAmbiguedad(Dataset dataset, string className, vector<string> evidence) {

        vector<string> classTerms = dataset.getTerminosLinguisticos(className);

        vector<double> a(0);
        for (int i = 0; i < evidence.size() - 1; i += 2) {
            if (a.size() == 0) 
            {
                a = dataset.getValoresCol(evidence[i],evidence[i + 1]);
            }
            else {
                vector<double> aPrime = dataset.getValoresCol(evidence[i],evidence[i + 1]);
                for (int j = 0; j < aPrime.size(); j++) {
                    a[j] = min(a[j], aPrime[j]);
                }
            }
        }

        if (a.size() != 0) 
        {
            for (int i = 0; i < a.size(); i++) 
            {
                if (a[i] < nivelSignificacia)
                {
                    a[i] = 0;
                }
            }
            vector<double> normalizedPossibility(classTerms.size());
            double max = -1;
            for (int j = 0; j < classTerms.size(); j++) 
            {
                vector<double> b = dataset.getValoresCol(className,classTerms[j]);
                normalizedPossibility[j] = Utils::subSetHood(a, b);
                if (normalizedPossibility[j] > max) 
                {
                    max = normalizedPossibility[j];
                }
            }
            Utils::normalizeWith(&normalizedPossibility, max);

            return Utils::ambiguity(normalizedPossibility);
        }
        return -1;
    }



    //Rentorna la ambiguedad de atributo respecto a una clase
    double getAmbiguedad(Dataset dataset, string nombreClase, string nombreAtributo) 
    {
        //Obtenemos los terminos linguisticos de la clase y del atributo
        vector<string> terminos        = dataset.getTerminosLinguisticos(nombreAtributo);
        vector<string> terminosClass   = dataset.getTerminosLinguisticos(nombreClase);

        vector<double> pesos (terminos.size());
        double sum = 0,sumaCol=0;

        //Matriz para almacenar los valores Fuzzy de los atributos
        vector<vector<double>> a(terminos.size());

        for (int i = 0;i< terminos.size(); i++) 
        {
            a[i] = dataset.getValoresCol(nombreAtributo,terminos[i]);

            for (int j = 0; j < a[i].size(); j++)
            {
                if (a[i][j] < nivelSignificacia)
                {
                    a[i][j] = 0;
                }
            }

            pesos[i] = Utils::sum(a[i]);;
            sum += pesos[i];
        }
        Utils::normalizeWith(&pesos, sum);

        double g = 0;
        for (int i = 0; i < terminos.size(); i++) 
        {
            vector<double> PosibilidadNormalizada(terminosClass.size());
            double max = -1;
            for (int j = 0; j < terminosClass.size(); j++) 
            {
                vector<double> b = dataset.getValoresCol(nombreClase, terminosClass[j]);
                PosibilidadNormalizada[j] = Utils::subSetHood(a[i], b);

                if (PosibilidadNormalizada[j] > max)
                {
                    max = PosibilidadNormalizada[j];
                }
            }
            Utils::normalizeWith(&PosibilidadNormalizada, max);

            g += pesos[i] * Utils::ambiguity(PosibilidadNormalizada);
        }
        return g;
    }

    vector<string> generarReglas(TreeNode node) 
    {
        vector<string> lista;
        string rules = generateRules(node, "",lista);
        return lista;

    }

    string generateRules(TreeNode node, string prefix,vector<string> lista)
    {
        string regla = "";

        if (node.getTitle() == "NULL") {
            return "";
        }
        if (node.isHoja())
        {
            regla = prefix + " THEN " + node.getTitle() + "(" + to_string(node.getValue()) + ")";
            lista.push_back(regla);
            return regla;
        }
        else if (node.isRoot())
        {
            string rules = "";
            vector<TreeNode> childs = node.getChildren();
            for (TreeNode& child : childs)
            {
                rules += generateRules(child.getChildren()[0], "IF " + node.getTitle() + " IS " + child.getTitle(),lista);
            }
            return rules;
        }
        else if (node.getTipoNodo() == "ATRIBUTO")
        {
            string rules = "";
            vector<TreeNode> childs = node.getChildren();
            for (TreeNode& child : childs)
            {
                rules += generateRules(child.getChildren()[0], prefix + " AND " + node.getTitle() + " IS " + child.getTitle(),lista);
                return rules;
            }
            return "";
        }
    }

    void printTree(TreeNode root, string tabs) 
    {
        if (root.getTipoNodo() == "ATRIBUTO") 
        {
            cout<<tabs + "|" + root.getTitle() + "|"<<endl;
        }
        vector<TreeNode> children = root.getChildren();
        for (int i = 0; i < root.getChildrenCount(); i++) 
        {
            TreeNode node = children[i];
            if (node.isHoja()) 
            {
                cout << tabs + "\t\t" + "[" + node.getTitle() + "](" + to_string(node.getValue()) + ")"<<endl;
            }
            else if (node.getTipoNodo() == "VALOR") 
            {
                cout << tabs + "\t" + "<" + node.getTitle() + ">"<<endl;
                printTree(node, "\t" + tabs);
            }
            else 
            {
                printTree(node, "\t" + tabs);
            }


        }
    }

};

