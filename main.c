#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"
#include "list.h"
#include "treemap.h"
#include "heap.h"
//--------------------
typedef struct{
    char nombre[50];
    int edad;
    List *estilo_musica;
    char tipo_carrete[50];           //estructura del perfil de la persona
    char comuna[50];
    char casa_depa[50];
    int precio_max;
    List *alcohol;
}perfil;

typedef struct{
    List *estilos_musica;
    int edad_inf;
    int edad_sup;
    int dinero;
    char comuna[50];            //estrucutra del carrete
    char tipo_carrete[50];
    List *alcohol;
    char casa_depa[50];
    int prioridad;
    double distancia;
}carrete;
//------------------------
void base_datos(List*);
void importar_archivos(HashMap*,char [][50]);                 //prototipo de funciones
int lower_than_double(void*,void*);
perfil *crear_perfil();
void crear_carrete(List*);
void encontrar_carrete(HashMap*,List*,perfil*,char [][50]);
void mostrar_carrete_cercano(Heap*);
//-----------------------
int main(){
    //----------------------------------------------------
    perfil *perfil_creado=(perfil*)malloc(sizeof(perfil));
    int opcion;
    List *lista_carretes=createList();
    char arreglo_comunas[312][50];
    HashMap *mapa=createMap(400);          //se definen variables como el arreglo de comunas, lista de carretes, y hashmap
    //-----------------------------------
    base_datos(lista_carretes);
    importar_archivos(mapa,arreglo_comunas);
    //-------------------------------------
    printf("Bienvenidos a...\n");
    printf("En busca del carrete\n");
    printf("-------------------------------------------------------\n");
    printf("que desea hacer?\n");
    printf("1.buscar carrete / 2.crear carrete: ");
    fflush(stdin);
    scanf("%d",&opcion);
    //--------------------------------------
    while (1){
        if (opcion==1){
            printf("\n");
            printf("vamos a proceder a crear su perfil..\n");
            printf("--------------------------------------\n");                           //se crea el perfil
            perfil_creado=crear_perfil();
            encontrar_carrete(mapa,lista_carretes,perfil_creado,arreglo_comunas);
            break;                                                                                       //se itera hasta que "opcion se igual a 1"
        }
        if (opcion==2){
            crear_carrete(lista_carretes);
            printf("---------------------------------------\n");                   //se crea un carrete
            printf("desea crear o buscar otro carrete?\n");
            printf("1.buscar 2.crear: ");
            fflush(stdin);
            scanf("%d",&opcion);
        }
    }
}
//--------------------------------------
void importar_archivos(HashMap *map,char arreglo_comunas[][50]){
    char linea[5000],*comuna,*distanciachar;
    double *distancia,distancia_double;
    int *posicion;
    //-----------------------
    FILE *fp;
    fp=fopen("distancias.csv","r");                    //se abre el csv
    fgets(linea,5000,fp);
    //---------------------------
    int i=0;
    while (fgets(linea,5000,fp)!=NULL){        //se itera hasta que llegue a nulo
        //------------------------------------
        comuna=(char*)malloc(50*sizeof(char));
        comuna=get_csv_field(linea,0);
        strcpy(arreglo_comunas[i],comuna);           //se mete la comuna [i] a el arreglo
        //--------------------------------------
        TreeMap *tree=createTreeMap(lower_than_double);          //se crea el treemap para la comuna
        //--------------------------------------------------
        for (int j=1;j<312;j++){
            //----------------------------------------
            distancia=(double*)malloc(sizeof(double));
            posicion=(int*)malloc(sizeof(int));
            //--------------------------------------
            distanciachar=get_csv_field(linea,j);
            distancia_double=atof(distanciachar);            //se leen las lineas y se transforman a sus respectivos tipos
            *distancia=distancia_double;
            //--------------------------------------
            *posicion=j-1;                            //posición en donde se encontrará la comuna
            //--------------------------
            if (distancia_double!=0)                     //si la distancia es igual a 0 no se toma, ej: valparaiso con valparaiso
                insertTreeMap(tree,distancia,posicion);
        }
        insertMap(map,comuna,tree);    //se ingresa el treemap de la comuna en el hashmap
        i++;
    }
}
//---------------------------------------------
void base_datos(List *list_carrete){
    char linea[1024],*comuna,*tipo_carrete,*casa_depa,*dinerochar,*edad_infchar,*edad_supchar,*musica,*alcohol;
    int edad_inf_int,edad_sup_int,dinero_int,i;
    //----------------------
    FILE *fp;
    fp=fopen("base_datos.csv","r");          //se abre el csv
    //------------------------------
    while (fgets(linea,1024,fp)!=NULL){
        //------------------------------------
        List *tipo_musica=createList();
        List *alcoholes=createList();
        //---------------------------------------
        comuna=get_csv_field(linea,0);
        casa_depa=get_csv_field(linea,1);                      //se leen las lineas
        //-------------------------------------
        dinerochar=get_csv_field(linea,2);
        dinero_int=atoi(dinerochar);
        //-----------------------------------
        tipo_carrete=get_csv_field(linea,3);
        //-----------------------------------
        edad_infchar=get_csv_field(linea,4);
        edad_inf_int=atoi(edad_infchar);
        edad_supchar=get_csv_field(linea,5);
        edad_sup_int=atoi(edad_supchar);
        //-------------------------------------
        i=6;
        while (1){
            musica=(char*)malloc(50*sizeof(char));                   //para meter los estilos de música en lista
            musica=get_csv_field(linea,i);
            if (strcmp(musica,".")==0)                              //si es un punto entonces se deja de leer los estilos
                break;
            pushBack(tipo_musica,musica);                    //se ingresa en la lista
            i++;
        }
        //-----------------------------------
        i++;
        while (1){
            alcohol=(char*)malloc(50*sizeof(char));
            alcohol=get_csv_field(linea,i);                          //lo mismo pero para alcoholes
            if (strcmp(alcohol,".")==0)
                break;
            pushBack(alcoholes,alcohol);
            i++;
        }
        //-----------------------------------------
        carrete *carretes=(carrete*)malloc(sizeof(carrete));
        strcpy(carretes->comuna,comuna);
        strcpy(carretes->casa_depa,casa_depa);
        strcpy(carretes->tipo_carrete,tipo_carrete);              //se ingresan los datos en la estrucura "carretes"
        carretes->edad_inf=edad_inf_int;
        carretes->edad_sup=edad_sup_int;
        carretes->dinero=dinero_int;
        carretes->alcohol=alcoholes;
        carretes->estilos_musica=tipo_musica;
        pushBack(list_carrete,carretes);               //se ingresa la estructura en la lista de carretes
    }
}
//-----------------------------------------
perfil *crear_perfil(){
    int edad,precio,opcionalcohol,i;
    char comuna[50],nombre[50],tipo_carrete[50],casa_depa[50],linealcohol[1024],lineamusica[1024],*alcohol,*musica;
    //-----------------------------------------------------------------------
    List *listalcohol=createList(),*listamusica=createList();
    //---------------------------------------------------------------------------
    printf("ingrese su nombre: ");
    fflush(stdin);
    scanf("%50[^\n]s",nombre);
    printf("ingrese su comuna: ");
    fflush(stdin);
    scanf("%50[^\n]s",comuna);
    printf("ingrese su edad: ");
    fflush(stdin);
    scanf("%d",&edad);
    printf("ingrese precio maximo dispuesto a pagar: ");              //se preguntan las variables para ser ingresada a la estructura de perfil
    fflush(stdin);
    scanf("%d",&precio);
    printf("ingrese tipo de carrete(masivo o no masivo): ");
    fflush(stdin);
    scanf("%50[^\n]s",tipo_carrete);
    printf("ingrese casa o departamento: ");
    fflush(stdin);
    scanf("%50[^\n]s",casa_depa);
    //-------------------------------------------------------
    printf("ingrese estilo/s de musica (separados por coma): ");
    fflush(stdin);                                                  //para separar luego los estilos
    fgets(lineamusica,1024,stdin);
    //-------------------------------------------------
    printf("consume alcohol? 1.si 2.no: ");
    fflush(stdin);
    scanf("%d",&opcionalcohol);
    if (opcionalcohol==1){
        printf("ingrese bebidas alcoholicas separados por coma: ");
        fflush(stdin);
        fgets(linealcohol,1024,stdin);
        i=0;
        while (1){
            alcohol=(char*)malloc(50*sizeof(char));
            alcohol=get_csv_field(linealcohol,i);               //se lee el fgets usando el get_csv_field y se mete en la lista de alcoholes
            if (alcohol==NULL)
                break;
            pushBack(listalcohol,alcohol);
            i++;
        }
    }
    else
        pushBack(listalcohol,"no");                //si no bebe, se ingresará un "no" en la lista
    //-----------------------------------------------
    i=0;
    while (1){
        musica=(char*)malloc(50*sizeof(char));
        musica=get_csv_field(lineamusica,i);             //se ingresa los estilos de música en la lista de musica
        if (musica==NULL)
            break;
        pushBack(listamusica,musica);
        i++;
    }
    //---------------------------------------------------
    perfil *perfil_creado=(perfil*)malloc(sizeof(perfil));
    strcpy(perfil_creado->nombre,nombre);
    strcpy(perfil_creado->comuna,comuna);
    perfil_creado->edad=edad;
    perfil_creado->precio_max=precio;
    strcpy(perfil_creado->tipo_carrete,tipo_carrete);            //se ingresan las variables en la estructura de perfil y se retorna
    strcpy(perfil_creado->casa_depa,casa_depa);
    perfil_creado->estilo_musica=listamusica;
    perfil_creado->alcohol=listalcohol;
    return perfil_creado;
}
//-------------------------------------------
void crear_carrete(List *listacarretes){
    char comuna[50],casa_depa[50],tipo_carrete[50],lineaedad[1024],lineamusica[1024],linealcohol[1024],*edad_infchar,*edad_supchar,*tipo_musica,*alcohol;
    int precio,edad_inf_int,edad_sup_int,i;
    //-----------------------------------------------------------
    List *lista_musica=createList(),*lista_alcohol=createList();
    printf("ingrese comuna donde se hara el carrete: ");
    fflush(stdin);
    scanf("%50[^\n]s",comuna);
    printf("ingrese tipo de vivienda (casa o departamento): ");
    fflush(stdin);                                                     //se ingresan las variables
    scanf("%50[^\n]s",casa_depa);
    printf("ingrese precio estimado por persona: ");
    fflush(stdin);
    scanf("%d",&precio);
    printf("ingrese tipo de carrete (masivo/no masivo): ");
    fflush(stdin);
    scanf("%50[^\n]s",tipo_carrete);
    //-------------------------------------------------
    printf("ingrese el rango de edad separado por coma: ");
    fflush(stdin);
    fgets(lineaedad,1024,stdin);
    edad_infchar=get_csv_field(lineaedad,0);                      //se separa la edad inferior y la superior del fgets
    edad_inf_int=atoi(edad_infchar);
    edad_supchar=get_csv_field(lineaedad,1);
    edad_sup_int=atoi(edad_supchar);
    //-------------------------------------------------------
    printf("ingrese tipo/s de musica separado por coma: ");
    fflush(stdin);
    fgets(lineamusica,1024,stdin);
    //-------------------------------------------------------
    printf("ingrese tipo de bebida alcoholica separado por coma (si no habra escriba 'no'): ");
    fflush(stdin);
    fgets(linealcohol,1024,stdin);
    i=0;
    if (strcmp(linealcohol,"no")!=0){
        while (1){
            alcohol=(char*)malloc(50*sizeof(char));                       //se ingresa las bebidas alcohólicas en la lista
            alcohol=get_csv_field(linealcohol,i);
            if (alcohol==NULL)
                break;
            pushBack(lista_alcohol,alcohol);
            i++;
        }
    }
    else
        pushBack(lista_alcohol,"no");
    //-----------------------------------------------------
    i=0;
    while (1){
        tipo_musica=(char*)malloc(50*sizeof(char));
        tipo_musica=get_csv_field(lineamusica,i);
        if (tipo_musica==NULL)                           //lo mismo para los estilos de música
            break;
        pushBack(lista_musica,tipo_musica);
        i++;
    }
    //------------------------------------------------------
    carrete *carretes=(carrete*)malloc(sizeof(carrete));
    strcpy(carretes->comuna,comuna);
    strcpy(carretes->casa_depa,casa_depa);
    strcpy(carretes->tipo_carrete,tipo_carrete);        //se ingresan los datos en la estructura "carretes"
    carretes->dinero=precio;
    carretes->edad_inf=edad_inf_int;
    carretes->edad_sup=edad_sup_int;
    carretes->estilos_musica=lista_musica;
    carretes->alcohol=lista_alcohol;
    pushBack(listacarretes,carretes);          //se ingresa la estructura en la lista de carretes
}
//------------------------------------------------------
void encontrar_carrete(HashMap *mapa,List *lista_carrete,perfil *perfil_creado,char arreglocomunas[][50]){
    int cont;
    char comuna[50],*alcoholes,*alcoholes_carrete,*estilo_musica,*estilo_musica_c;
    int *pos,posicion;
    double *dist,distancia;
    //--------------------------------------------------------------
    printf("\nhola %s!\n",perfil_creado->nombre);         //se saluda a la persona que está buscando el carrete
    //-------------------------------------------------------------------
    Heap *prioridad_carretes=createHeap();
    Pair *pairhash=searchMap(mapa,perfil_creado->comuna);            //se busca en el hashmap la comuna de la persona para usar el treemap de dicha comuna
    TreeMap *tree=pairhash->value;
    Pairt *partree=firstTreeMap(tree);
    carrete *carretes=(carrete*)malloc(sizeof(carrete));
    //----------------------------------------------------------------
    for (int i=0;i<10;i++){
        //---------------------------------------------------------
        dist=(double*)malloc(sizeof(double));
        pos=(int*)malloc(sizeof(int));
        pos=(int*)partree->value;
        dist=(double*)partree->key;                       //se definen las variables para utilizar antes del while
        distancia=*dist;
        posicion=*pos;
        //-----------------------------------------------------------
        strcpy(comuna,arreglocomunas[posicion]);
        carretes=firstList(lista_carrete);                    //se indica la comuna con la posición del treemap
        cont=0;
        //---------------------------------------------------------------------
        while (carretes!=NULL){
            if (strcmp(comuna,carretes->comuna)==0){
                //-----------------------------------------------------------------------------------
                if (perfil_creado->edad>carretes->edad_inf && perfil_creado->edad<carretes->edad_sup)
                    cont++;
                if (perfil_creado->precio_max<=carretes->dinero)
                    cont++;
                if (strcmp(perfil_creado->tipo_carrete,carretes->tipo_carrete)==0)               //se irá aumentando el contador según las coincidencias
                    cont++;
                if (strcmp(perfil_creado->casa_depa,carretes->casa_depa)==0)
                    cont++;
                //---------------------------------------------------------------------------------
                alcoholes=firstList(perfil_creado->alcohol);
                if (alcoholes!="no"){
                    while (alcoholes!=NULL){
                        alcoholes_carrete=firstList(carretes->alcohol);
                        if (alcoholes_carrete=="no")
                            break;
                        while (alcoholes_carrete!=NULL){                                 //se recorre la lista de alcohol para comparar
                            if (strcmp(alcoholes,alcoholes_carrete)==0){
                                cont++;
                                break;
                            }
                            alcoholes_carrete=nextList(carretes->alcohol);
                        }
                        alcoholes=nextList(perfil_creado->alcohol);
                    }
                }
                //--------------------------------------------------------------------------
                estilo_musica=firstList(perfil_creado->estilo_musica);
                while (estilo_musica!=NULL){
                    estilo_musica_c=firstList(carretes->estilos_musica);
                    while (estilo_musica_c!=NULL){                            //lo mismo para la de estilos de música
                        if (strcmp(estilo_musica,estilo_musica_c)==0){
                            cont++;
                            break;
                        }
                        estilo_musica_c=nextList(carretes->estilos_musica);
                    }
                    estilo_musica=nextList(perfil_creado->estilo_musica);
                }
                //---------------------------------------------------------------------------
                carretes->prioridad=cont;
                carretes->distancia=distancia;                               //se ingresa la prioridad y la distancia con dicha comuna
                heap_push(prioridad_carretes,carretes,carretes->prioridad);           //se ingresa en el montículo binario (cola con prioridad)
            }
            carretes=nextList(lista_carrete);     //se sigue con el siguiente carrete de la lista
        }
        partree=nextTreeMap(tree);   //se continua con el siguiente nodo del treemap
    }
    mostrar_carrete_cercano(prioridad_carretes);     //se llama a la función para mostrar el carrete encontrado
}
void mostrar_carrete_cercano(Heap *prioridad_carretes){
    char *alcohol,*musica;
    int opcion;
    carrete *carretes=(carrete*)malloc(sizeof(carrete));
    carretes=(carrete*)heap_top(prioridad_carretes);
    //--------------------- ------------------------------------------------
    if (carretes==NULL)                //si el montículo se encuentra vacío se indicará dicho mensaje
        printf("lo sentimos, no se encontraron carretes cercanos de acuerdo a su perfil\n");
    //-------------------------------------------------------------------------------------------
    else{
        while (1){
            printf("con %d coincidencias...\n",carretes->prioridad);
            printf("la comuna es: %s y se encuentra a %f km\n",carretes->comuna,carretes->distancia);
            printf("estos son los datos...\n");
            printf("\n");
            printf("----------------------------------------------------------\n");
            printf("rango de edad: %d a %d\n",carretes->edad_inf,carretes->edad_sup);
            printf("presupuesto estimado por persona: %d\n",carretes->dinero);              //se imprimen los datos del carrete
            printf("tipo de carrete: %s\n",carretes->tipo_carrete);
            printf("bebida/s alcoholica/s: ");
            //------------------------------------------------------------------
            alcohol=firstList(carretes->alcohol);
            while (alcohol!=NULL){
                printf("%s",alcohol);
                alcohol=nextList(carretes->alcohol);               //se imprime la lista de alcoholes y las comas hasta que sea nulo
                if (alcohol!=NULL)
                    printf(", ");
            }
            //-----------------------------------------------------------------
            printf("\n");
            printf("estilo/s de musica: ");
            musica=firstList(carretes->estilos_musica);
            while (musica!=NULL){                              //lo mismo para los estilos de música
                printf("%s",musica);
                musica=nextList(carretes->estilos_musica);
                if (musica!=NULL)
                    printf(", ");
            }
            //--------------------------------------------------------------------
            printf("\n----------------------------------------------------\n");
            printf("\n");
            printf("desea seguir buscando?: 1.si 2.no\n");
            printf("su opcion: ");
            scanf("%d",&opcion);
            if (opcion!=1){
                printf("perfecto!, disfrute el carrete.");       //si no desea buscar el siguiente carrete mas cercano se rompe el ciclo
                break;
            }
            //---------------------------------------------------------
            heap_pop(prioridad_carretes);
            carretes=(carrete*)heap_top(prioridad_carretes);                        //si la opción no fue 1 se sigue buscando
            //------------------------------------------------------------------------
            if (carretes==NULL){            //si es nulo se indica que no quedan mas carretes para mostrar
                printf("no hay mas carretes por mostrar\n");
                break;
            }
            printf("******************************************\n");
            printf("mostrando siguiente carrete..\n");
            printf("*******************************************\n");
        }
    }
}
int lower_than_double(void* key1, void* key2){
    double k1 = *((double*) (key1));            //funcion para comparar las distancias en el treemap
    double k2 = *((double*) (key2));
    if (k2>k1)
        return 1;
    return 0;
}
