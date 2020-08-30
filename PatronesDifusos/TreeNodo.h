#pragma once

#include<string>
#include<stdlib.h>
#include<stdio.h>
#include<vector>

using namespace std;

class TreeNode 
{
    

private:

    TreeNode* padre=NULL;
    string titulo="NULL";
    string tipoNodo="";
    double valor=0;
    vector<TreeNode> hijos;

public:
    TreeNode(string tipo, string nombre)
    {
        titulo = nombre;
        tipoNodo = tipo;
    }

    TreeNode(string nodeType, string title, TreeNode *parent)
    {
        titulo = title;
        padre = parent;
        tipoNodo = nodeType;
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

    void setParent(TreeNode* parent) 
    {
        padre = parent;
    }

    void setTitle(string t) 
    {
        titulo = t;
    }

    int getChildrenCount() 
    {
        return hijos.size();
    }

    string getTipoNodo()
    {
        return tipoNodo;
    }

    void addChild(TreeNode child) 
    {
        child.padre = this;
        hijos.push_back(child);
    }

    void addChild(string title, string nodeType) 
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
        return (tipoNodo == "HOJA");
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