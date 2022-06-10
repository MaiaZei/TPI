#include "ejercicios.h"
#include "auxiliares.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>

using namespace std;

/******++++**************************** EJERCICIO tiempoTotal ***********+++***********************/

tiempo tiempoTotal(viaje v) {
    tiempo t;
    double max = obtenerTiempo(v[obtenerMaximo(v)]);
    double min = obtenerTiempo(v[obtenerMinimo(v)]);
    t= max -min;
    return t;
}

/************++*********************** EJERCICIO distanciaTotal ************++*********************/

distancia distanciaTotal(viaje v) {
    ordenarViaje(v);
    return distanciaViaje(v);
}

/*****************************+***** EJERCICIO excesoDeVelocidad **********************************/

bool excesoDeVelocidad(viaje v) {
    ordenarViaje(v);
    return superoVelocidad(v);
}
/***************************************** EJERCICIO flota ***************************************/

int flota(vector<viaje> f, tiempo t0, tiempo tf) {
    int resp=0;
    for (int i = 0; i < f.size(); ++i) {
        if(viajeEnFranjaHoraria(f[i], t0, tf)){
            resp+=1;
        }
    }
    return resp;
}

/************************************ EJERCICIO recorridoCubierto *******************************/

vector<gps> recorridoNoCubierto(viaje  v, recorrido  r, distancia u) {
    vector<gps> res;
    resConNoCubiertos(v,r,u,res);
    return res;}

  /************************************** EJERCICIO construirGrilla *******************************/


grilla construirGrilla(gps esq1, gps esq2, int n, int m) {
    grilla resp = {};
    for (int i = 0; i <n ; ++i) {
        for (int j = 0; j <m ; ++j) {
            double latitud0 = obtenerLatitud(esq1) - altoCelda(esq1, esq2, n)*i;
            double longitud0 = obtenerLongitud(esq1) + anchoCelda(esq1, esq2, m)*j;
            double latitud1 = latitud0 - altoCelda(esq1, esq2, n);
            double longitud1 = longitud0 + anchoCelda(esq1, esq2, m);
            celda c = make_tuple(puntoGps(latitud0, longitud0), puntoGps(latitud1, longitud1), make_tuple(i+1, j+1));
            resp.push_back(c);
        }
    }
    return resp;}

/************************************* EJERCICIO cantidadDeSaltos ******************************/

int cantidadDeSaltos(grilla g, viaje v) {
    int resp=0;
    ordenarViaje(v);
    vector<nombre> listaNombres = celdasViaje(v,g);
    for (int i = 1; i < listaNombres.size() ; ++i) {
        if(distanciaEntreCeldas(listaNombres[i-1], listaNombres[i]) > 1){
            resp++;
        }
    }
    return resp;
}


/************************************* EJERCICIO corregirViaje ******************************/

void corregirViaje(viaje& v, vector<tiempo> errores){
    for (int i = 0; i <errores.size() ; ++i) {
        int pos =posErrorEnViaje(v, errores[i]);
        int p1=0;
        int p2=0;
        losDosMasCercanos(v, p1, p2, pos, errores);
        corrijo(v,p1,p2,pos);
    }

}
