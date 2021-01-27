#!/bin/bash

# Para usarse tiene que estar con el main de p2-hdr, opcion estadisticas...
# Saca el error de cada imagen en $1 (ojo con el nombre) a "estadisticas-v3.csv"

listaSamples="1 5 10 30 50 80 100 300 500 750"

for samples in $listaSamples; do
    ./main $samples "$1/convergencia-$samples.ppm" "$1/convergencia-2000.ppm" >> estadisticas-v3.csv
done
