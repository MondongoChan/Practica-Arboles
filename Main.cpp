#include <iostream>
#include <fstream>
using namespace std;


//Arbol binario para las páginas del libro
struct numPagina{
    int numPag;
    numPagina *izq=NULL;
    numPagina *der=NULL;

    numPagina(int numPag){
        this->numPag=numPag;
    }
};

//Arbol binario para los términos secundarios o subtérminos
struct subTermino{
    string palabra;
    subTermino *izq=NULL;
    subTermino *der=NULL;
    numPagina *paginas=NULL; //relacionamos las páginas con el subtérmino

    subTermino(string palabra){
        this->palabra=palabra;
    }
};

//Arbol binario para los términos principales
struct termino{
    string palabra;
    termino *izq=NULL;
    termino *der=NULL;
    numPagina *paginas=NULL; //relacionamos las páginas con el término
    subTermino *subTerm=NULL; //relacionamos los subterminos con el término

    termino(string palabra){
        this->palabra=palabra;
    }
};

//Función para insertar una página en el árbol
void insertarPagina(numPagina *&arbp, int numPag){
    if(arbp==NULL){
        arbp=new numPagina(numPag);
    }else{
        if(numPag<arbp->numPag){
            insertarPagina(arbp->izq, numPag);
        }else{
            insertarPagina(arbp->der, numPag);
        }
    }
}

//Función para insertar un subtérmino en el árbol
void insertarSubTermino(subTermino *&arbst, string palabra){
    if(arbst==NULL){
        arbst=new subTermino(palabra);
    }else{
        if(palabra<arbst->palabra){
            insertarSubTermino(arbst->izq, palabra);
        }else{
            insertarSubTermino(arbst->der, palabra);
        }
    }
}

//Función para insertar un término en el árbol
void insertarTermino(termino *&arbt, string palabra){
    if(arbt==NULL){
        arbt=new termino(palabra);
    }else{
        if(palabra<arbt->palabra){
            insertarTermino(arbt->izq, palabra);
        }else{
            insertarTermino(arbt->der, palabra);
        }
    }
}

//Organizar el árbol de páginas INORDEN
void organizarPagina(numPagina *arbp){
    if(arbp!=NULL){
        organizarPagina(arbp->izq);
        cout<<arbp->numPag<<" ";
        organizarPagina(arbp->der);
    }
}

//Organizar el árbol de subtérminos INORDEN
void organizarSubTerminos(subTermino *&arbst){
    if(arbst!=NULL){
        organizarSubTerminos(arbst->izq);
        cout<<"   "<<arbst->palabra<<"->";
        organizarPagina(arbst->paginas);
        cout<<endl;
        organizarSubTerminos(arbst->der);
    }
}

//Organizar el árbol de términos INORDEN
void organizarTerminos(termino *&arbt){
    if(arbt!=NULL){
        organizarTerminos(arbt->izq);
        cout<<endl;
        cout<<arbt->palabra;

        if(arbt->paginas!=NULL){
            cout<<" ";
            organizarPagina(arbt->paginas);
        }

        if(arbt->subTerm!=NULL){
            cout<<endl;
            organizarSubTerminos(arbt->subTerm);
        }
        organizarTerminos(arbt->der);
    }
}

//Posición de la página en el libro
numPagina* buscarPagina(numPagina *&arbp, int numPag){
    if(arbp==NULL){
        return NULL;
    }else{
        if(numPag==arbp->numPag){
            return arbp;
        }else if(numPag<arbp->numPag){
            return buscarPagina(arbp->izq, numPag);
        }else{
            return buscarPagina(arbp->der, numPag);
        }
    }
}

//Posición del subtérmino en el árbol
subTermino* buscarSubTermino(subTermino *&arbst, string palabra){
    if(arbst==NULL){
        return NULL;
    }else{
        if(palabra==arbst->palabra){
            return arbst;
        }else if(palabra<arbst->palabra){
            return buscarSubTermino(arbst->izq, palabra);
        }else{
            return buscarSubTermino(arbst->der, palabra);
        }
    }
}

//Posición del término en el árbol
termino* buscarTermino(termino *&arbt, string palabra){
    if(arbt==NULL){
        return NULL;
    }else{
        if(palabra==arbt->palabra){
            return arbt;
        }else if(palabra<arbt->palabra){
            return buscarTermino(arbt->izq, palabra);
        }else{
            return buscarTermino(arbt->der, palabra);
        }
    }
}

/* Busca el numero que indica la cantidad de veces que una palabra aparece en el libro
Se pone el límite entre 0 y 9, ya que puede aparecer entre este rango nada más (Según código)
*/
int buscarNum(string n){
    for(int i=0; i<n.length(); i++){
        if(n[i]>='0' && n[i]<='9'){
            return i;
            break;
        }
    }
    return -1;
}

// Extracción de las páginas en las que aparece el término o subtérmino
void extraerPaginas(numPagina *&arbp,int n, string pag){
    int numP=stoi(pag.substr(0,1));
    string cadenaRestante=pag.substr(1);
    int a=0;

    //Linkeamos la página al arbol de paginas
    for(int i=0; i<numP; i++){
        int pag=stoi(cadenaRestante.substr(a,n));
        insertarPagina(arbp, pag);
        a+=n;
    }
}

void cargarArchivo(){
    termino *raizTermino=NULL;
    string tactual;
    ifstream archivo;

    archivo.open("Libro.txt",ios::in);
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo";
        exit(1);
    }

    cout<<"Libro Datos y Algoritmos I\n"<<endl;
    int num=2;

    while(!archivo.eof()){
        string linea;
        getline(archivo, linea);
        
        //Buscar los términos principales, aquellos que comienzan por m
        if(linea[0]=='m'){
            int pos=buscarNum(linea);
            if(pos!=-1){
                string tempt=linea.substr(1,pos-1);
                insertarTermino(raizTermino, tempt);
                tactual=tempt;
                string pag=linea.substr(pos);
                extraerPaginas(buscarTermino(raizTermino, tactual)->paginas, num, pag);
            }
            else{
                string tempt=linea.substr(1,linea.length()-1);
                insertarTermino(raizTermino, tempt);
                tactual=tempt;
            }
        //Buscar los subtérminos, aquellos que comiencen por n
        }else if(linea[0]=='n'){
            int pos=buscarNum(linea);
            if(pos!=-1){
                string tempt=linea.substr(1,pos-1);
                insertarSubTermino(buscarTermino(raizTermino, tactual)->subTerm, tempt);
                string pag=linea.substr(pos);
                extraerPaginas(buscarSubTermino(buscarTermino(raizTermino, tactual)->subTerm, tempt)->paginas, num, pag);
            }
            else{
                string tempt=linea.substr(1,linea.length()-1);
                insertarSubTermino(buscarTermino(raizTermino, tactual)->subTerm, tempt);
            }  
        }
    }
    archivo.close();
    organizarTerminos(raizTermino);
}

int main(){
    cargarArchivo();
    return 0;
}
