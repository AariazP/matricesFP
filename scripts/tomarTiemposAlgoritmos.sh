#!/bin/bash

# Verificar si se pasó el comando como argumento
if [ "$#" -ne 1 ]; then
    echo "Uso: $0 '<comando>'"
    exit 1
fi

# Comando a ejecutar (se pasa como parámetro)
comando="$1"
nombre_algoritmo=`echo $comando | grep -oP "(?<=algoritmos/).+?(?=/src)" | head -n 1`
nombre_algoritmo=${nombre_algoritmo//\\/}
tamanio_matriz=`echo $comando | grep -oP "(\d+)x\1"`
herramienta_ejecucion=`echo $comando | grep -oP "(gcc|python3)"`
echo $herramienta_ejecucion
declare -A lenguajes
lenguajes[gcc]="c"
lenguajes[python3]="python"
entorno=${lenguajes[$herramienta_ejecucion]}
mkdir -p "../Resultados/${nombre_algoritmo}/$entorno/resultadomultiplicacion"
mkdir -p "../Resultados/${nombre_algoritmo}/$entorno/tiempo"
archivo_salida_resultado="../Resultados/${nombre_algoritmo}/$entorno/resultadomultiplicacion/resultado_${nombre_algoritmo}_${tamanio_matriz}.txt"
archivo_salida_tiempo="../Resultados/${nombre_algoritmo}/$entorno/tiempo/tiempo_${nombre_algoritmo}_${tamanio_matriz}.txt"
# Medir el tiempo de ejecución del comando
tiempo=$( { time eval "$comando" > "$archivo_salida_resultado"; } 2>&1 | grep real | awk '{print $2}' | grep -oP "(?<=m).+(?=s)" )

tiempo=${tiempo/,/.}


echo "Tiempo de ejecución: $tiempo segundos" > "$archivo_salida_tiempo"
