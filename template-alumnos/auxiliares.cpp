#include <algorithm>
#include "auxiliares.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>

using namespace std;

const double pi = 3.14;
double radioTierra = 6378;

double obtenerLatitud(gps posicion) {
    return get<0>(posicion);
}

double obtenerLongitud(gps posicion) {
    return get<1>(posicion);
}

gps obtenerPosicion(tuple<tiempo, gps> medicion) {
    return get<1>(medicion);
}

tiempo obtenerTiempo(tuple<tiempo, gps> medicion) {
    return get<0>(medicion);
}
double distEnKM(gps posicion1, gps posicion2) {
    double latitud1 = obtenerLatitud(posicion1);
    double latitud2 = obtenerLatitud(posicion2);
    double longitud1 = obtenerLongitud(posicion1);
    double longitud2 = obtenerLongitud(posicion2);

    // obtengo la distancia
    double distanciaLatitud = (latitud2 - latitud1) * pi / 180.0;
    double distanciaLongitud = (longitud2 - longitud1) * pi / 180.0;

    // paso las latitudes a radianes
    latitud1 = (latitud1) * pi / 180.0;
    latitud2 = (latitud2) * pi / 180.0;

    // aplico la formula
    double a = pow(sin(distanciaLatitud / 2), 2) +
               pow(sin(distanciaLongitud / 2), 2) *
               cos(latitud1) * cos(latitud2);

    double c = 2 * asin(sqrt(a));
    return radioTierra * c;
}

gps desviarPunto(gps p, double desvioMtsLatitud, double desvioMtsLongitud){
    double lat = obtenerLatitud(p);
    double lon = obtenerLongitud(p);

    double dx = desvioMtsLatitud / 1000;
    double dy = desvioMtsLongitud / 1000;
    double new_latitude = lat + (dx / radioTierra) * (180 / pi);
    double new_longitude = lon + (dy / radioTierra) * (180 / pi) / cos(lat * pi / 180);
    return puntoGps(new_latitude, new_longitude);

}


gps puntoGps(double latitud, double longitud) {
    return make_tuple(latitud, longitud);
}

tuple<tiempo, gps> medicion(tiempo t, gps g) {
    return make_tuple(t, g);
}


int obtenerMaximo (viaje v){
    int max = obtenerTiempo(v[0]);
    for (int i = 0; i <v.size() ; ++i) {
        if(obtenerTiempo(v[i])> obtenerTiempo(v[max])){
            max = i;
        }
    }
    return max;
}

int obtenerMinimo (viaje v){
    int min = obtenerTiempo(v[0]);
    for (int i = 0; i <v.size() ; ++i) {
        if(obtenerTiempo(v[i])< obtenerTiempo(v[min])){
            min = i;
        }
    }
    return min;
}

void ordenarViaje(viaje & v){
    for(int i = 0; i<v.size(); i++){
        for(int j = i; j<v.size(); j++){
            if(obtenerTiempo(v[j])< obtenerTiempo(v[i])){
                tuple<tiempo, gps> n = v[i];
                v[i] = v[j];
                v[j]= n;
            }
        }
    }
}

double distanciaViaje(viaje v){
    double distancia =0;
    for (int i = 1; i <v.size() ; ++i) {
        distancia += distEnKM(obtenerPosicion(v[i-1]), obtenerPosicion(v[i]));
    }
    return distancia;
}
double velocidad(tuple<tiempo, gps> p1,tuple<tiempo, gps> p2 ){
    double dist =distEnKM(obtenerPosicion(p2), obtenerPosicion(p1));
    double tiempoEnH =abs(obtenerTiempo(p2)- obtenerTiempo(p1))/3600;
    return dist/tiempoEnH;
}

bool superoVelocidad(viaje v){
    bool res = false;
    for (int i = 1; i <v.size() ; ++i) {
        if(velocidad(v[i-1], v[i])>80){
            res=true;
        }
    }
    return res;
}

bool viajeEnFranjaHoraria(viaje & v, tiempo t0, tiempo tf){
    bool res = true;
    ordenarViaje(v);
    if(obtenerTiempo(v[v.size()-1]) <t0 || obtenerTiempo(v[0]) >tf){
        res = false;
    }
    return res;
}

double altoCelda(gps esq1, gps esq2, int n){
    return (obtenerLatitud(esq1)- obtenerLatitud(esq2))/n;
}

double anchoCelda(gps esq1, gps esq2, int m){
    return (obtenerLongitud(esq2)- obtenerLongitud(esq1))/m;
}

bool esCeldaDeCordenada(gps p, celda c){
    bool esta=false;
    if(obtenerLatitud(get<0>(c))>=obtenerLatitud(p) && obtenerLatitud(p)>=obtenerLatitud(get<1>(c)) && obtenerLongitud(get<0>(c))<=obtenerLongitud(p) && obtenerLongitud(p) <= obtenerLongitud(get<1>(c))){
        esta = true;
    }
    return esta;

}

bool cubierto(viaje v, distancia u, gps p){
    int i=0;
    bool estaCubierto= false;
    while(i<v.size() && !estaCubierto){
        if( distEnKM(obtenerPosicion(v[i]),p)<u){
            estaCubierto= true;
        }
        i++;
    }
    return estaCubierto;
}

void resConNoCubiertos(viaje & v, recorrido r, distancia u, vector<gps> & res){
    for (int i = 0; i <r.size() ; ++i) {
        if(!cubierto(v,u,r[i])){
            res.push_back(r[i]);
        }
    }
}

celda posEnCelda(gps p, grilla g){
    celda c;
    bool encontreCelda = false;
    int i =0;
    while(i<g.size() && !encontreCelda){
        if(esCeldaDeCordenada(p, g[i])){
            c= g[i];
            encontreCelda = true;
        }
        i++;
    }
    return c;
}

vector<nombre> celdasViaje(viaje v, grilla & g){
    vector<nombre> listaNombres;
    for (int i = 0; i <v.size() ; ++i) {
        listaNombres.push_back(get<2>(posEnCelda(obtenerPosicion(v[i]),g)));
    }
    return listaNombres;}

double distanciaEntreCeldas(nombre n1, nombre n2){
    return sqrt((pow(get<0>(n1)-get<0>(n2),2) + (pow(get<1>(n1)-get<1>(n2),2))));
}


bool esError(int p, viaje v, vector<tiempo> errores){
    bool es = false;
    for (int i = 0; i <errores.size() ; ++i) {
        if (errores[i] == obtenerTiempo(v[p])){
            es= true;
        }
    }
    return es;
}

void losDosMasCercanos(viaje v, int & p1, int & p2, int error, vector<tiempo> errores){
    int min = INT32_MAX;
    for (int i = 0; i <v.size() ; ++i) {
        if(!esError(i, v,errores) && abs(obtenerTiempo(v[error])- obtenerTiempo(v[i])) < min){
            p1=i;
            min = abs(obtenerTiempo(v[error])- obtenerTiempo(v[p1]));
        }
    }
    int min2 = INT32_MAX;
    for (int j = 0; j <v.size() ; ++j) {
        if(j!=p1 && !esError(j, v,errores)&& abs(obtenerTiempo(v[error])- obtenerTiempo(v[j])) < min2){
            p2=j;
            min2 = abs(obtenerTiempo(v[error])- obtenerTiempo(v[p2]));

        }
    }
}

double velocidadBis(tuple<tiempo, gps> p1,tuple<tiempo, gps> p2 ){
    double difLong = abs(obtenerLongitud(obtenerPosicion(p1)) - obtenerLongitud(obtenerPosicion(p2)));
    double difLat = abs(obtenerLatitud(obtenerPosicion(p1)) - obtenerLatitud(obtenerPosicion(p2)));
    double dist = sqrt(pow(difLong,2) + pow(difLat,2));
    double tiempoEnS =abs(obtenerTiempo(p2)- obtenerTiempo(p1));
    return dist/tiempoEnS;
}

void cuadrante(tuple<tiempo, gps> p1,tuple<tiempo, gps> p2, int & j,int & k){
    double difLong = obtenerLongitud(obtenerPosicion(p1)) - obtenerLongitud(obtenerPosicion(p2));
    double difLat = obtenerLatitud(obtenerPosicion(p1)) - obtenerLatitud(obtenerPosicion(p2));
    //ESTOY EN EL PRIMER CUADRANTE
    if(difLong>0 && difLat>0){j = 1;k = 1;}
    //ESTOY EN EL SEGUNDO CUADRANTE
    else if(difLong<0 && difLat>0){j = 1;k = -1;}
    //ESTOY EN EL TERCER CUADRANTE
    else if(difLong<0 && difLat<0){j = -1; k = -1;}
    //ESTOY EN EL CUARTO CUADRANTE
    else if(difLong>0 && difLat<0){j = -1; k = 1;}
}


gps nuevaPos(tuple<tiempo, gps> p1,tuple<tiempo, gps> p2,tuple<tiempo, gps> error ){

    double distanciaAlError = (velocidadBis(p1,p2))*abs(obtenerTiempo(error)-obtenerTiempo(p2));
    double difLong = abs(obtenerLongitud(obtenerPosicion(p1)) - obtenerLongitud(obtenerPosicion(p2)));
    double difLat = abs(obtenerLatitud(obtenerPosicion(p1)) - obtenerLatitud(obtenerPosicion(p2)));
    double distancia = sqrt(pow(difLong,2) + pow(difLat,2));
    double distLongAlErrorCorregido = difLong/distancia*distanciaAlError;
    double distLatAlErrorCorregido = difLat/distancia*distanciaAlError;
    int j,k;
    cuadrante(p1,p2,j,k);
    double longError =  obtenerLongitud(obtenerPosicion(p2)) +k*distLongAlErrorCorregido;
    double latError = obtenerLatitud(obtenerPosicion(p2)) + j*distLatAlErrorCorregido;
    return make_tuple(latError, longError);
}

int posErrorEnViaje(viaje v, tiempo e){
    bool loEncontre=false;
    int i=0;
    int pos =0;
    while(i<v.size() && !loEncontre){
        if(obtenerTiempo(v[i]) == e){
            loEncontre= true;
            pos=i;
        }
        i++;
    }
    return pos;
}

void corrijo(viaje & v, int p1, int p2, int error){
    v[error]= make_tuple(obtenerTiempo(v[error]), nuevaPos(v[p1], v[p2], v[error]));
}








/*
 void guardarGrillaEnArchivo(grilla g, string nombreArchivo){
    ofstream myfile;
    float esq1_lat, esq2_lat, esq1_lng, esq2_lng;
    int name_0, name_1;

    myfile.open(nombreArchivo);
    myfile << std::fixed << std::setprecision(5);
    for(int i = 0; i < g.size(); i++){
        esq1_lat = get<0>(get<0>(g[i]));
        esq1_lng = get<1>(get<0>(g[i]));

        esq2_lat = get<0>(get<1>(g[i]));
        esq2_lng = get<1>(get<1>(g[i]));

        name_0 = get<0>(get<2>(g[i]));
        name_1 = get<1>(get<2>(g[i]));

        myfile << esq1_lat << "\t"
               << esq1_lng << "\t"
               << esq2_lat << "\t"
               << esq2_lng << "\t"
               << "(" << name_0 << "," << name_1 << ")"
               << endl;

    }
    myfile.close();

}

void guardarRecorridosEnArchivo(vector<recorrido> recorridos, string nombreArchivo){
    ofstream myfile;
    float lat, lng;

    myfile.open(nombreArchivo);
    myfile << std::fixed << std::setprecision(5);
    for(int i = 0; i < recorridos.size(); i++){
        for(int k = 0; k < recorridos[i].size(); k++){
            lat= get<0>(recorridos[i][k]);
            lng= get<1>(recorridos[i][k]);

            myfile << i << "\t"
                   << lat << "\t"
                   << lng << endl;
        }
    }
    myfile.close();

}
 */