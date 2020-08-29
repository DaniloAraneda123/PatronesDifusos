#pragma once

#include<string>
#include<stdlib.h>
#include<stdio.h>
#include<vector>

using namespace std;

class TreeNode 
{
    

private:
    //tipoNodo:
    //0 -> valor
    //1 -> hoja
    //2 -> atributo

    TreeNode* padre;
    string titulo;
    int tipoNodo;
    double valor;
    vector<TreeNode> hijos;

public:
    TreeNode(int tipo, string nombre)
    {
        titulo = nombre;
        tipoNodo = tipo;
    }

    TreeNode(string nombre,double value,bool tipo)
    {
        titulo = nombre;
        valor = value;
        tipoNodo = tipo;
    }

    vector<TreeNode> getChildren() 
    {
        return hijos;
    }

    TreeNode* getParent() 
    {
        return padre;
    }

    string getTitle() 
    {
        return titulo;
    }

    void setParent(TreeNode parent) 
    {
        padre = &parent;
    }

    void setTitle(string t) 
    {
        titulo = t;
    }

    int getChildrenCount() 
    {
        if (hijos.size()!=0)
        {
            return hijos.size();
        }
        return 0;
    }

    void addChild(TreeNode child) 
    {
        hijos.push_back(child);
        child.padre = this;
    }

    void addChild(string title, int nodeType) 
    {
        addChild(TreeNode(nodeType, title));
    }

    void clearChildren() 
    {
        hijos.clear();
    }

    TreeNode getChild(string nombreHijo) 
    {
        for (TreeNode &node : hijos) 
        {
            if (node.titulo== nombreHijo)
            {
                return node;
            }
        }
    }

    bool isRoot() 
    {
        return (padre == NULL);
    }

    bool isHoja() 
    {
        return (tipoNodo == 1);
    }

    double getValue() 
    {
        return valor;
    }

    void setValue(double v) 
    {
        valor = v;
    }
};