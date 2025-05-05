#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}
/*
1.- Implemente la función *createTreeMap* en el archivo treemap.c. Esta función recibe la función de comparación de claves y crea un mapa (TreeMap) inicializando sus variables. 
El siguiente código muestra como inicializar la función de comparación. Reserve memoria, inicialice el resto de variables y retorne el mapa.

    TreeMap * createTreeMap(int (*lt) (void* key1, void* key2)) {


        //map->lower_than = lt;
        return NULL;
    }*/
TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    if (new == NULL) return NULL;
    new->root=NULL;
    new->current=NULL;
    new->lower_than = lower_than;
    return new;
}


/*3.- Implemente la función void insertTreeMap(TreeMap * tree, void* key, void * value). Esta función inserta un nuevo dato
 (key,value) en el árbol y hace que el current apunte al nuevo nodo.
Para insertar un dato, primero debe realizar una búsqueda para encontrar 
donde debería ubicarse. Luego crear el nuevo nodo4
 y enlazarlo. Si la clave del dato ya existe retorne sin hacer nada 
 (recuerde que el mapa no permite claves repetidas).
   
    void insertTreeMap(TreeMap* tree, void* key, void* value){


    }
    
    
*/

void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if(searchTreeMap(tree, key) != NULL) return; 
    TreeNode * parent=NULL;
    TreeNode * node = tree->root;
    while (node != NULL) {
        parent = node;
        if (tree->lower_than(key, node->pair->key)) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    TreeNode * new = createTreeNode(key, value);
    new->parent =parent;
    if (parent == NULL) {
        tree->root = new;
    } else if (tree->lower_than(key, parent->pair->key)) {
        parent->left = new;
    } else {
        parent->right = new;
    }
    tree->current = new;
    

}
/*4.- Implemente la función TreeNode * minimum(TreeNode * x). Esta función retorna el 
**nodo con la mínima clave** ubicado en el subárbol con raiz x.
 Para obtener el nodo tiene que, a partir del nodo x, irse por la rama izquierda hasta llegar al
  final del subárbol. Si x no tiene hijo izquierdo se retorna el mismo nodo.
   
    TreeNode * minimum(TreeNode * x){


        return NULL;
    }*/
TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;
    while (x->left != NULL) {
        x = x->left;
    }
    return x;



}

/*5.- Implemente la función void removeNode(TreeMap * tree, TreeNode* node).
 Esta función elimina el nodo *node* del árbol *tree*. Recuerde que para eliminar un node existen 3 casos:
**Nodo sin hijos:** Se anula el puntero del padre que apuntaba al nodo
**Nodo con un hijo:** El padre del nodo pasa a ser padre de su hijo
**Nodo con dos hijos:** Descienda al hijo derecho y obtenga el menor nodo del subárbol
(con la función minimum). Reemplace los datos (key,value) de *node* con los del nodo "minimum". 
Elimine el nodo minimum (para hacerlo puede usar la misma función *removeNode*).

    void removeNode(TreeMap * tree, TreeNode* node) {}

*/
void removeNode(TreeMap * tree, TreeNode* node) {
    if(node->left==NULL && node->right==NULL)
    {
        if(node->parent->left==node)
        {
            node->parent->left=NULL;
        }
        else
        {
            node->parent->right=NULL;
        }
    }
    else if (node->left == NULL || node->right == NULL) {
    TreeNode* child;
    if (node->left != NULL) 
    {
    child = node->left;
    } 
    else 
    {
    child = node->right;
    }

    
        if (node->parent == NULL) {
            tree->root = child;
        } else if (node->parent->left == node) {
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }
    
        if (child != NULL) child->parent = node->parent;
    }
    else {
        TreeNode* minNode = minimum(node->right);
        node->pair->key = minNode->pair->key;
        node->pair->value = minNode->pair->value;
        removeNode(tree, minNode);
    }
    

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


/*2.- Implemente la función Pair* searchTreeMap(TreeMap* tree, void* key), la cual busca el nodo con clave igual a key y retorna
 el **Pair** asociado al nodo. Si no se encuentra la clave retorna NULL.
Recuerde hacer que el current apunte al nodo encontrado.

    Pair* searchTreeMap(TreeMap* tree, void* key) {


        return NULL;
    }s

*/

Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode* node = tree->root;
    while (node != NULL) {
        if (!tree->lower_than(key, node->pair->key) && !tree->lower_than(node->pair->key, key)) {
            tree->current = node;
            return node->pair;
        } else if (tree->lower_than(key, node->pair->key)) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return NULL;
}



/*7.- La función Pair* upperBound(TreeMap* tree, void* key) 
retorna el **Pair** con clave igual a key. En caso de no encontrarlo retorna el primer 
par asociado a una clave mayor o igual a key. 
Para implementarla puede realizar una búsqueda normal y usar un puntero a nodo auxiliar
 ub_node que vaya guardando el nodo con la menor clave *mayor o igual a key*. Finalmente retorne el par del nodo ub\_node.

    Pair* upperBound(TreeMap* tree, void* key){
        

    }*/

    Pair* upperBound(TreeMap* tree, void* key) {
        
    }
    

/*6.- Implemente las funciones para recorrer la estructura: Pair* firstTreeMap(TreeMap* tree) retorna el primer **Pair** del mapa 
(el menor). Pair* nextTreeMap(TreeMap* tree)  retornar el siguiente **Pair**
 del mapa a partir del puntero TreeNode* current. Recuerde actualizar este puntero.


    Pair * nextTreeMap(TreeMap * tree) {
       

        return NULL;
    }

*/


Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode* node = tree->root;
    while (node->left != NULL) {
        node = node->left;
    }
    tree->current = node; 
    return node->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;

    TreeNode* node = tree->current;
    if (node->right != NULL) {
        node = node->right;
        while (node->left != NULL) {
            node = node->left;
        }
        tree->current = node;  
        return node->pair;
    }
    while (node->parent != NULL && node == node->parent->right) {
        node = node->parent;
    }

    
    tree->current = node->parent;
    return (tree->current != NULL) ? tree->current->pair : NULL;
}