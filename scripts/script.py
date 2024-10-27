import os
import matplotlib.pyplot as plt
import numpy as np

def cargar_tiempos(directorio_resultados):
    tiempos_por_algoritmo = {}
    
    for algoritmo in os.listdir(directorio_resultados):
        path_algoritmo = os.path.join(directorio_resultados, algoritmo)
        
        if os.path.isdir(path_algoritmo):
            for lenguaje in os.listdir(path_algoritmo):
                path_lenguaje = os.path.join(path_algoritmo, lenguaje, "tiempo")
                
                if os.path.isdir(path_lenguaje):
                    tiempos = []
                    tamaños = []
                    
                    for archivo_tiempo in sorted(os.listdir(path_lenguaje)):
                        tamaño = archivo_tiempo.split("_")[-1].replace(".txt", "")
                        tamaños.append(int(tamaño.split("x")[0]))  # Asume que el tamaño es cuadrado
                        
                        with open(os.path.join(path_lenguaje, archivo_tiempo), "r") as f:
                            linea_tiempo = f.read().strip()
                            tiempo_str = linea_tiempo.split(":")[-1].split()[0]
                            try:
                                tiempo = float(tiempo_str)
                                tiempos.append(tiempo)
                            except ValueError:
                                print(f"Error de conversión en archivo: {archivo_tiempo} con tiempo: '{tiempo_str}'")
                    
                    # Verificar que los tiempos y tamaños se cargaron correctamente
                    print(f"Algoritmo: {algoritmo}, Lenguaje: {lenguaje}")
                    print("Tamaños:", tamaños)
                    print("Tiempos:", tiempos)
                    
                    # Convertir claves a minúsculas para evitar problemas de mayúsculas/minúsculas
                    clave = f"{algoritmo}_{lenguaje.lower()}"
                    tiempos_por_algoritmo[clave] = (tamaños, tiempos)
    
    return tiempos_por_algoritmo

def graficar_tiempos_barras(tiempos_por_algoritmo, lenguaje_filtro):
    # Filtrar los datos por lenguaje en minúsculas
    lenguaje_filtro = lenguaje_filtro.lower()
    tiempos_filtrados = {alg: (tamaños, tiempos) for alg, (tamaños, tiempos) in tiempos_por_algoritmo.items() if lenguaje_filtro in alg.lower()}
    
    # Verificar datos filtrados
    if not tiempos_filtrados:
        print(f"No se encontraron datos para el lenguaje '{lenguaje_filtro}'")
        return

    # Obtener todos los tamaños de matrices únicos y algoritmos del lenguaje especificado
    tamaños_unicos = sorted({tamaño for tamaños, _ in tiempos_filtrados.values() for tamaño in tamaños})
    algoritmos = list(tiempos_filtrados.keys())
    
    # Crear un gráfico de barras agrupadas
    x = np.arange(len(tamaños_unicos))  # Posiciones para cada tamaño de matriz
    ancho_barra = 0.15  # Ancho de cada grupo de barras
    
    plt.figure(figsize=(12, 8))
    
    for i, (algoritmo, (tamaños, tiempos)) in enumerate(tiempos_filtrados.items()):
        tiempos_por_tamaño = [tiempos[tamaños.index(t)] if t in tamaños else 0 for t in tamaños_unicos]
        plt.bar(x + i * ancho_barra, tiempos_por_tamaño, width=ancho_barra, label=algoritmo)
    
    plt.xlabel("Tamaño de la matriz (n x n)")
    plt.ylabel("Tiempo de ejecución (s)")
    plt.title(f"Tiempos de ejecución de algoritmos de multiplicación de matrices ({lenguaje_filtro.capitalize()})")
    plt.xticks(x + ancho_barra * (len(algoritmos) - 1) / 2, tamaños_unicos)
    plt.yscale("log")
    if len(algoritmos) > 0:  # Asegura que existan elementos en la leyenda
        plt.legend()
    plt.grid(True, which="both", linestyle="--", linewidth=0.5, axis='y')
    plt.show()

# Ruta a la carpeta de resultados
directorio_resultados = "../Resultados"

# Cargar los tiempos de los archivos
tiempos_por_algoritmo = cargar_tiempos(directorio_resultados)

# Graficar los tiempos para Python
graficar_tiempos_barras(tiempos_por_algoritmo, lenguaje_filtro="Python")

# Graficar los tiempos para C
graficar_tiempos_barras(tiempos_por_algoritmo, lenguaje_filtro="C")

