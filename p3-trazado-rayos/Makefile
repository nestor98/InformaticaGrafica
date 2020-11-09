### vectores:
VECTOR_DIR=../p1-geometria
VECTOR=${VECTOR_DIR}/Vector3

### imagen:
IMAGEN_DIR=../p2-hdr
IMAGEN=${IMAGEN_DIR}/Imagen

### utils:
UTILS_DIR=utils
UTILS=${UTILS_DIR}/utils

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

### Bounding volume hierarchy:
BVH_DIR=bvh
BVH=${BVH_DIR}/bvh


### Texturas
TEX_DIR=textura
TEX=${TEX_DIR}/textura

### main:
MAIN=main

CPPFLAGS=-MMD -I. -I${VECTOR_DIR} -I${IMAGEN_DIR} -I${TEX_DIR} -I${FIGURAS_DIR} -I${UTILS_DIR} -I${ESFERA_DIR} -I${PLANO_DIR} -I${PRISMA_DIR} -I${CAMARA_DIR} -I${ESCENA_DIR} -I${BVH_DIR} -std=c++11 -pthread -O2

src = $(wildcard *.cpp) \
        $(wildcard ../p1-geometria/Vector3.cpp) \
				$(wildcard ../p2-hdr/Imagen.cpp) \
	$(wildcard textura/*.cpp) \
        $(wildcard camara/*.cpp) \
        $(wildcard escena/*.cpp)\
        $(wildcard bvh/*.cpp)\
        $(wildcard figuras/*.cpp)\
        $(wildcard utils/*.cpp)\
        $(wildcard figuras/esfera/*.cpp)\
        $(wildcard figuras/plano/*.cpp)\
        $(wildcard figuras/prisma/*.cpp)
obj = $(src:.cpp=.o)

LDFLAGS = -std=c++11#-lGL -lglut -lpng -lz -lm

main: $(obj)
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) main
