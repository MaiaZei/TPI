#ifndef TRANSPORTEURBANO_AUXILIARES_H
#define TRANSPORTEURBANO_AUXILIARES_H

#include "definiciones.h"
#include "string"

double obtenerLatitud(gps posicion);
double obtenerLongitud(gps posicion);
gps obtenerPosicion(tuple<tiempo, gps> medicion);
tiempo obtenerTiempo(tuple<tiempo, gps> medicion);
double distEnKM(gps posicion1, gps posicion2);
gps desviarPunto(gps p, double desvioMtsLatitud, double desvioMtsLongitud);
gps puntoGps(double latitud, double longitud);
tuple<tiempo, gps> medicion(tiempo t, gps g);


int obtenerMaximo (viaje v);
int obtenerMinimo (viaje v);
void ordenarViaje(viaje & v);
double distanciaViaje(viaje v);
double velocidad(tuple<tiempo, gps> p1,tuple<tiempo, gps> p2 );
bool superoVelocidad(viaje v);
bool viajeEnFranjaHoraria(viaje & v, tiempo t0, tiempo tf);
bool esCeldaDeCordenada(gps p, celda c);
celda posEnCelda(gps p, grilla g);
vector<nombre> celdasViaje(viaje v, grilla & g);
double distanciaEntreCeldas(nombre n1, nombre n2);
double altoCelda(gps esq1, gps esq2, int n);
double anchoCelda(gps esq1, gps esq2, int m);
bool cubierto(viaje v, distancia u, gps p);
void resConNoCubiertos(viaje & v, recorrido r, distancia u, vector<gps> & res);

bool esError(int p, viaje v, vector<tiempo> errores);
void losDosMasCercanos(viaje v, int & p1, int & p2, int error, vector<tiempo> errores);
double velocidadBis(tuple<tiempo, gps> p1,tuple<tiempo, gps> p2 );
void cuadrante(tuple<tiempo, gps> p1,tuple<tiempo, gps> p2, int & j,int & k);
gps nuevaPos(tuple<tiempo, gps> p1,tuple<tiempo, gps> p2,tuple<tiempo, gps> error );
int posErrorEnViaje(viaje v, tiempo e);
void corrijo(viaje & v, int p1, int p2, int error);

void guardarGrillaEnArchivo(grilla g, string nombreArchivo);
void guardarRecorridosEnArchivo(vector<recorrido> recorridos, string nombreArchivo);

#endif //TRANSPORTEURBANO_AUXILIARES_H
