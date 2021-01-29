#!/bin/bash

# Para usarse tiene que estar con el main de p2-hdr, opcion estadisticas...
# Saca el error de cada imagen en $1 (ojo con el nombre) a "estadisticas-v3.csv"

# listaSamples="1 2 3 5 7 10 30 50 80 100 200 300 500 800 1001 1500"
listaSamples="1 2 3 4 5 6 7 8 9 10 20 30 40 50 60 70 80 90 100"

for samples in $listaSamples; do
    echo "$1/iteracion_${samples}escenaprogMasFot.ppm"
    ./main $samples "$1/iteracion_${samples}escenaprogMasFot.ppm" "$1/iteracion_10escenaprogMasFot.ppm" >> estadisticas-progressive-3.csv
done
