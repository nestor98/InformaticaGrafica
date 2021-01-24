#!/bin/bash

# Para usarse tiene que estar con el main de p2-hdr, opcion estadisticas...
# Saca el error de cada imagen en $1 (ojo con el nombre) a "estadisticas-v3.csv"

listaSamples="1 2 3 5 7 10 30 50 80 100 200 300 500 800 1001 1500"

for samples in $listaSamples; do
    ./main $samples "$1/convergencia-$samples.ppm" "$1/convergencia-1000.ppm" >> estadisticas-puntual-1000.csv
done
