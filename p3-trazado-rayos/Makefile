### vectores:
VECTOR_DIR=../p1-geometria
VECTOR=${VECTOR_DIR}/Vector3

### imagen:
IMAGEN_DIR=../p2-hdr
IMAGEN=${IMAGEN_DIR}/Imagen

### utils:
MATERIAL_DIR=material
MATERIAL=${MATERIAL_DIR}/material

### utils:
UTILS_DIR=utils
UTILS=${UTILS_DIR}/utils

COLOR_DIR=color
COLOR=${COLOR_DIR}/color

### Figuras:
FIGURAS_DIR=figuras
FIGURAS=${FIGURAS_DIR}/figuras
# Subclases:
ESFERA_DIR=${FIGURAS_DIR}/esfera
ESFERA=${ESFERA_DIR}/esfera

PLANO_DIR=${FIGURAS_DIR}/plano
PLANO=${PLANO_DIR}/plano

PRISMA_DIR=${FIGURAS_DIR}/prisma
PRISMA=${PRISMA_DIR}/prisma
### camara:
CAMARA_DIR=camara
CAMARA=${CAMARA_DIR}/camara

### escena:
ESCENA_DIR=escena
ESCENA=${ESCENA_DIR}/escena

LUZ_DIR=${ESCENA_DIR}/luces
LUZ=${LUZ_DIR}/luzpuntual


### renderer:
RENDERER_DIR=renderer
RENDERER=${RENDERER_DIR}/renderer

### Bounding volume hierarchy:
BVH_DIR=bvh
BVH=${BVH_DIR}/bvh

### Texturas
TEX_DIR=textura
TEX=${TEX_DIR}/textura

### PHOTON MAPPING:
PM_DIR=photon-mapping
PM=${PM_DIR}/textura

### escenas prefabricadas:
PREFABS_DIR=prefabricadas

### TFG:

### escenas prefabricadas:
TFG_SDF_DIR=/mnt/d/Universidad/cuarto-2/TFG/TFG-SDF


### main:
MAIN=main

CPPFLAGS=-MMD -Wall -Wno-sign-compare -I. -I${VECTOR_DIR} -I${PREFABS_DIR} -I${IMAGEN_DIR} -I${COLOR_DIR} -I${TEX_DIR} -I${PM_DIR} -I${FIGURAS_DIR} -I${MATERIAL_DIR} -I${UTILS_DIR} -I${ESFERA_DIR} -I${PLANO_DIR} -I${PRISMA_DIR} -I${TFG_SDF_DIR} -I${CAMARA_DIR} -I${ESCENA_DIR} -I${LUZ_DIR} -I${RENDERER_DIR} -I${BVH_DIR} -std=c++17 -pthread -O3

src = $(wildcard *.cpp) \
        $(wildcard ../p1-geometria/Vector3.cpp) \
				$(wildcard ../p2-hdr/Imagen.cpp) \
				$(wildcard textura/*.cpp) \
        $(wildcard camara/*.cpp) \
        $(wildcard escena/*.cpp)\
        $(wildcard escena/luces/*.cpp)\
        $(wildcard renderer/*.cpp)\
        $(wildcard bvh/*.cpp)\
        $(wildcard figuras/*.cpp)\
        $(wildcard color/*.cpp)\
        $(wildcard material/*.cpp)\
        $(wildcard utils/*.cpp)\
        $(wildcard photon-mapping/*.cpp) \
        $(wildcard figuras/esfera/*.cpp)\
				$(wildcard figuras/sdfs/*.cpp)\
        $(wildcard figuras/plano/*.cpp)\
        $(wildcard figuras/prisma/*.cpp)
obj = $(src:.cpp=.o)

# LDFLAGS = -std=c++20#-lGL -lglut -lpng -lz -lm

main: $(obj)
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) main
