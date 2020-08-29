#pragma once

#include <string>
#include "Dataset.h"
#include "Utils.h"
#include <stdlib.h> 
#include "TreeNodo.h"

class FuzzyID3
{

private:
	double nivelVerdad;
	double nivelSignificacia;

public:

    FuzzyID3(double truthLevel, double significantLevel)
    {
        nivelVerdad = truthLevel;
        nivelSignificacia = significantLevel;
    }

    //Metodo principal por el cual se empieza a construir el arbol
    TreeNode construirArbol(Dataset dataset, string nombreClase)  
    {
        vector<string> atributos = dataset.getNombreAtributos(nombreClase),listavacia(0);
        return growTree(dataset, listavacia, atributos, nombreClase);
    }

private:

    TreeNode growTree(Dataset dataset, vector<string> padre, vector<string> atributos, string nombreClase)
    {
        //Si no hay atributos crea un nodo hoja basico
        if (atributos.size() == 0) {
            return TreeNode(1, "UnKnown");
        }

        //Si es la raiz
        if (padre.size() == 0)
        {
            double AmbiguedadMinima = 1000;
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
            TreeNode root(2, bestAtributo);

            //Obtengo los terminos linguisticos
            vector<string> terminos = dataset.getTerminosLinguisticos(bestAtributo);
            vector<string> terminosClase = dataset.getTerminosLinguisticos(nombreClase);

            //Para cada termino del mejor atributo se obtiene el grado de clasificacion de verdad recpecto a cada termino de la clase
            for (string &term : terminos)
            {
                bool canBelongeToClass = false;

                for (string &classTerm : terminosClase)
                {

                    double dot = degreeOfClassificationTruth(dataset, vector<string>({ bestAtributo, term, nombreClase, classTerm }));

                    //Si el grado de verdad de la casificacion es mayor que el nivel de verdad minimo, se crean los hijos del del termino y se asignan
                    if (dot > nivelVerdad)
                    {
                        TreeNode c(0, term);
                        TreeNode c2(1, classTerm);
                        c2.setValue(dot);
                        c.addChild(c2);
                        root.addChild(c);
                        canBelongeToClass = true;

                        break;//Salimos del for
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
                    newArgs[1] = term;

                    TreeNode node(0, term);
                    root.addChild(node);

                    TreeNode c = growTree(dataset, newArgs, newAtributos, nombreClase);

                    //Se uno como hijo el nuevo sub arbol generado
                    node.addChild(c);
                }
            }

            return root;
        }
        else//Hay atributos Padres
        {
            segundoCuerpo(dataset,padre,atributos,nombreClase);
        }
    }

    TreeNode segundoCuerpo(Dataset dataset, vector<string> padre, vector<string> atributos, string nombreClase)
    {
        string bestAttr = "";
        if (atributos.size() > 1) 
        {
            vector<string> args1(padre.size() + 1);
            System.arraycopy(padre, 0, args1, 0, padre.size());
            args1[padre.size()] = nombreClase;

            double ambg = getAmbiguedad(dataset, nombreClase, padre);

            for (string &atr : atributos)
            {
                vector<string>  args2(padre.size() + 1);
                System.arraycopy(args1, 0, args2, 1, args1.size());
                args2[0] = atr;
                double ambg2 = getAmbiguedad(dataset, nombreClase, atr, args);

                if (ambg2 <= ambg) 
                {
                    bestAttr = atr;
                    ambg = ambg2;
                }

            }

            if (""==bestAttr) 
            {
                args1 = vector<string>(args.length + 2);
                System.arraycopy(args, 0, args1, 0, args.length);
                args1[args.size()] = nombreClase;
                vector<string> classTerms = dataset.getAttribute(nombreClase).getLinguisticTerms();
                string bestClass = "";
                double bestDOT = -1;

                for (string &classTerm : classTerms) 
                {
                    args1[args.length + 1] = classTerm;
                    double dot = degreeOfClassificationTruth(dataset, args1);
                    if (dot > bestDOT) 
                    {
                        bestDOT = dot;
                        bestClass = classTerm;
                    }
                }
                TreeNode c2(1, bestClass);
                c2.setValue(bestDOT);
                return c2;
            }
            else 
            {
                TreeNode root(2, bestAttr);
                vector <string> terms = dataset.getAttribute(bestAttr).getLinguisticTerms();
                vector <string> classTerms = dataset.getAttribute(nombreClase).getLinguisticTerms();
                vector <string> args2(args.size() + 4);
                System.arraycopy(args, 0, args2, 2, args.length);
                args2[0] = bestAttr;
                args2[args2.size() - 2] = nombreClase;

                for (string &term : terms) 
                {
                    bool canBelongeToClass = false;
                    args2[1] = term;

                    for (string &classTerm : classTerms) 
                    {
                        args2[args2.size() - 1] = classTerm;
                        double dot = degreeOfClassificationTruth(dataset, args2);

                        if (dot > nivelVerdad) 
                        {
                            TreeNode c (0, term);
                            TreeNode c2 (1, classTerm);
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
                            if (atributos[j]!=bestAttr)
                            {
                                newAttrs[k++] = atributos[j];
                            }
                        }
                        //get the new args
                        vector <string> newArgs(args.length + 2);
                        System.arraycopy(args, 0, newArgs, 2, args.length);
                        newArgs[0] = bestAttr;
                        newArgs[1] = term;

                        TreeNode node(0, term);
                        root.addChild(node);
                        TreeNode c = growTree(dataset, newArgs, newAttrs, nombreClase);

                        if (c != null) 
                        {
                            node.addChild(c);
                        }

                    }

                }
                return root;
            }
        }
        else 
{
            bestAttr = attrs[0];
            TreeNode root(NodeType.ATTRIBUTE, bestAttr);
            vector <string> terms = dataset.getAttribute(bestAttr).getLinguisticTerms();
            vector <string> classTerms = dataset.getAttribute(nombreClase).getLinguisticTerms();
            vector <string> args2(args.size() + 4);
            System.arraycopy(args, 0, args2, 2, args.length);
            args2[0] = bestAttr;
            args2[args2.length - 2] = className;

            terms.stream().map((term) -> {
                args2[1] = term;
                return term;
            }).forEach((term) -> {
                double maxTruth = Double.MIN_VALUE;
                String bestClass = "";
                for (string classTerm : classTerms) 
                {
                    args2[args2.length - 1] = classTerm;
                    double dot = degreeOfClassificationTruth(dataset, args2);
                    if (dot > maxTruth) 
                    {
                        maxTruth = dot;
                        bestClass = classTerm;
                    }
                }

                TreeNode node(NodeType.VALUE, term);
                root.addChild(node);
                TreeNode c(NodeType.LEAF, bestClass);

                c.setValue(maxTruth);
                node.addChild(c);
            });
            return root;
        }
    }

    //Recibe pares de atribustos-termino
    double degreeOfClassificationTruth(Dataset dataset, vector<string> atributo_termino)
    {
        vector<double> a = dataset.getValoresCol(atributo_termino[0], atributo_termino[1]), b;

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
        vector<double> b = dataset.getValoresCol(atributo_termino[t - 2], atributo_termino[t - 1]);

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

};

