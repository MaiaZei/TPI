#ifndef TRANSPORTEURBANO_SOLUCION_H
#define TRANSPORTEURBANO_SOLUCION_H

#include "definiciones.h"

tiempo tiempoTotal(viaje v);
distancia distanciaTotal(viaje v);
bool excesoDeVelocidad(viaje v);
int flota(vector<viaje> v, tiempo t0, tiempo tf);
vector<gps> recorridoNoCubierto(viaje v, recorrido r, distancia u);
grilla construirGrilla(gps esq1, gps esq2, int n, int m);
int cantidadDeSaltos(grilla g, viaje v);
void corregirViaje(viaje& v, vector<tiempo> errores);


#endif //TRANSPORTEURBANO_SOLUCION_H
