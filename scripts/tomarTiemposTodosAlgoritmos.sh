algoritmos=( "python3 ../algoritmos/IV.4\ Parallel\ Block/src/python/IV.4-Parallel-Block.py"
	     "python3 ../algoritmos/IV.5\ Enhanced\ Parallel\ Block/src/python/IV.5-Enhanced-Parallel-Block.py"
	     "python3 ../algoritmos/IV.3\ Sequential\ block/src/python/IV.3-Sequential-block.py"
	     "gcc -o  ../algoritmos/IV.4\ Parallel\ Block/src/c/Parallel-Block ../algoritmos/IV.4\ Parallel\ Block/src/c/IV.4-Parallel-Block.c && ./../algoritmos/IV.4\ Parallel\ Block/src/c/Parallel-Block ../CasosPrueba/matriz_128x128"
	     "gcc -o ../algoritmos/IV.3\ Sequential\ block/src/c/Sequential-Block ../algoritmos/IV.3\ Sequential\ block/src/c/IV.3-Sequential-Block.c && ./../algoritmos/IV.3\ Sequential\ block/src/c/Sequential-Block"
	     "gcc -fopenmp -o ../algoritmos/IV.5\ Enhanced\ Parallel\ Block/src/c/Enhanced-Parallel-Block ../algoritmos/IV.5\ Enhanced\ Parallel\ Block/src/c/IV.5-Enhanced-Parallel-Block.c && ./../algoritmos/IV.5\ Enhanced\ Parallel\ Block/src/c/Enhanced-Parallel-Block"
	     
)

for algoritmo in "${algoritmos[@]}"
do

	for size in {0..7}
	do
	
	#	./tomarTiemposAlgoritmos.sh "gcc -o  ../algoritmos/IV.4\ Parallel\ Block/src/c/Parallel-Block ../algoritmos/IV.4\ Parallel\ Block/src/c/IV.4-Parallel-Block.c && ./../algoritmos/IV.4\ Parallel\ Block/src/c/Parallel-Block ../CasosPrueba/matriz_128x128"

	./tomarTiemposAlgoritmos.sh "$algoritmo ../CasosPrueba/matriz_$(( 2**$size ))x$(( 2**$size ))"

	done
done
