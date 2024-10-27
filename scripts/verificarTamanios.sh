#!/bin/bash

# Verificar que se haya pasado un archivo como argumento
if [ $# -ne 1 ]; then
	echo "Uso: $0 nombre_del_archivo"
	exit 1
fi

archivo="$1"

# Leer el archivo y procesar las matrices
{
	while IFS= read -r linea || [[ -n "$linea" ]]; do
		# Comprobar si la línea está vacía
		if [[ -z "$linea" ]]; then
			# Si hay una línea vacía, procesar la matriz anterior
			if [[ -n "$filas" ]]; then
				# Imprimir el tamaño de la matriz
				echo "Tamaño de la matriz: ${num_filas}x${num_columnas}"
				filas=""
				num_filas=0
				num_columnas=0
			fi
			continue
		fi

		# Contar filas
		if [[ -z "$filas" ]]; then
			filas="$linea"
			num_columnas=$(echo "$linea" | wc -w) # Contar columnas
		fi

		num_filas=$((num_filas + 1))
	done

	# Procesar la última matriz si no termina con línea vacía
	if [[ -n "$filas" ]]; then
		echo "Tamaño de la matriz: ${num_filas}x${num_columnas}"
	fi
} <"$archivo"
