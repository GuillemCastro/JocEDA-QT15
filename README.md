# JocEDA-QT15 Star-War

En la carpeta Utils está todo lo necesario para compilar el código (incluido el Makefile).

Para jugar contra el dummy hay que seguir unos pasos:

1. Copia AIPlayer.cc en la misma carpeta que el resto de código
2. Dependiendo de tu SO y arquitectura, eliminale la extensión a AIDummy.o.loquesea y dejalo como AIDummy.o
3. Ejecuta `run.sh` alternativamente haz `make all && ./Game AIPlayer AIDummy default.cnf result.res`
4. Abre el Viewer (si no has ejecutado run.sh) y selecciona result.res (que estará en la misma carpeta que el código)

Este player es muy sencillo y solo llegó a la ronda 21.
