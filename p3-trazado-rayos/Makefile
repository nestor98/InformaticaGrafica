### vectores:
VECTOR_DIR=../p1-geometria
VECTOR=${VECTOR_DIR}/Vector3

### imagen:
IMAGEN_DIR=../p2-hdr
IMAGEN=${IMAGEN_DIR}/Imagen

### Figuras:
FIGURAS_DIR=figuras
FIGURAS=${FIGURAS_DIR}/figuras
# Subclases:
ESFERA_DIR=${FIGURAS_DIR}/esfera
ESFERA=${ESFERA_DIR}/esfera

PLANO_DIR=${FIGURAS_DIR}/plano
PLANO=${PLANO_DIR}/plano

### camara:
CAMARA_DIR=camara
CAMARA=${CAMARA_DIR}/camara

### escena:
ESCENA_DIR=escena
ESCENA=${ESCENA_DIR}/escena

### main:
MAIN=main

CPPFLAGS=-MMD -I. -I${VECTOR_DIR} -I${IMAGEN_DIR} -I${FIGURAS_DIR} -I${ESFERA_DIR} -I${PLANO_DIR} -I${CAMARA_DIR} -I${ESCENA_DIR} -std=c++11 -O2

src = $(wildcard *.cpp) \
        $(wildcard ../p1-geometria/Vector3.cpp) \
				$(wildcard ../p2-hdr/Imagen.cpp) \
        $(wildcard camara/*.cpp) \
        $(wildcard escena/*.cpp)\
        $(wildcard figuras/*.cpp)\
        $(wildcard figuras/esfera/*.cpp)\
        $(wildcard figuras/plano/*.cpp)
obj = $(src:.cpp=.o)
dep = $(obj:.o=.d)  # one dependency file for each source

LDFLAGS = -std=c++11#-lGL -lglut -lpng -lz -lm

main: $(obj)
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

-include $(dep) # include all dep files in the makefile


.PHONY: clean
clean:
	rm -f $(obj) main
