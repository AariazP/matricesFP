for x in {0..7}
do
	tamanio=$((2**$x))
	nombre_archivo="../CasosPrueba/matriz_${tamanio}x${tamanio}"
	if [ ! -e $nombre_archivo ]
	then
		touch "$nombre_archivo"
	else
		echo -n "" > "$nombre_archivo"
	fi


	for (( n=0; $n < 2; n++ ))
	do
		for (( i=0; $i < $tamanio; i++ )) 
		do
			fila=""
			for (( j=0; $j < $tamanio; j++ )) 
			do
				number=$(od -An -N3 -i /dev/urandom | tr -d ' ')
				fila="$fila $(( $number % 900000 + 100000))"
			done
			echo $fila >> $nombre_archivo 
		done

		echo "" >> $nombre_archivo
	done
done
