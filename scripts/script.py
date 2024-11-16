import os
import matplotlib.pyplot as plt
import numpy as np

def cargar_tiempos(directorio_resultados):
    tiempos_por_algoritmo = {"python": {}, "c": {}}
    
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
                    
                    # Clasificar en Python o C según el nombre del archivo
                    if "python" in lenguaje.lower():
                        tiempos_por_algoritmo["python"][algoritmo] = (tamaños, tiempos)
                    else:
                        tiempos_por_algoritmo["c"][algoritmo] = (tamaños, tiempos)
    
    return tiempos_por_algoritmo

def graficar_tiempos_barras(tiempos_por_algoritmo, lenguaje_filtro, titulo):
    # Verificar datos del lenguaje
    if lenguaje_filtro not in tiempos_por_algoritmo:
        print(f"No se encontraron datos para el lenguaje '{lenguaje_filtro}'")
        return
    
    tiempos_filtrados = tiempos_por_algoritmo[lenguaje_filtro]
    
    # Obtener todos los tamaños de matrices únicos y algoritmos del lenguaje especificado
    tamaños_unicos = sorted({tamaño for tamaños, _ in tiempos_filtrados.values() for tamaño in tamaños})
    algoritmos = list(tiempos_filtrados.keys())
    
    # Crear un gráfico de barras agrupadas
    x = np.arange(len(tamaños_unicos))  # Posiciones para cada tamaño de matriz
    ancho_barra = 0.05  # Ancho de cada grupo de barras
    
    plt.figure(figsize=(12, 8))
    
    for i, (algoritmo, (tamaños, tiempos)) in enumerate(tiempos_filtrados.items()):
        tiempos_por_tamaño = [tiempos[tamaños.index(t)] if t in tamaños else 0 for t in tamaños_unicos]
        plt.bar(x + i * ancho_barra, tiempos_por_tamaño, width=ancho_barra, label=algoritmo)
    
    plt.xlabel("Tamaño de la matriz (n x n)")
    plt.ylabel("Tiempo de ejecución (s)")
    plt.title(titulo)
    plt.xticks(x + ancho_barra * (len(algoritmos) - 1) / 2, tamaños_unicos)
    plt.yscale("log")
    if len(algoritmos) > 0:
        plt.legend()
    plt.grid(True, which="both", linestyle="--", linewidth=0.5, axis='y')
    plt.show()

def graficar_tiempos_combinados(tiempos_por_algoritmo, titulo):
    # Obtener datos para Python y C
    tiempos_python = tiempos_por_algoritmo["python"]
    tiempos_c = tiempos_por_algoritmo["c"]
    
    if not tiempos_python and not tiempos_c:
        print("No se encontraron datos para Python o C")
        return
    
    # Obtener todos los tamaños de matrices únicos
    tamaños_unicos = sorted(set([tamaño for tamaños, _ in tiempos_python.values() for tamaño in tamaños] +
                                [tamaño for tamaños, _ in tiempos_c.values() for tamaño in tamaños]))
    
    algoritmos_python = list(tiempos_python.keys())
    algoritmos_c = list(tiempos_c.keys())
    
    # Crear un gráfico de barras agrupadas
    x = np.arange(len(tamaños_unicos))  # Posiciones para cada tamaño de matriz
    ancho_barra = 0.04  # Ancho de cada grupo de barras
    
    plt.figure(figsize=(12, 8))
    
    # Graficar tiempos para Python
    for i, (algoritmo, (tamaños, tiempos)) in enumerate(tiempos_python.items()):
        tiempos_por_tamaño = [tiempos[tamaños.index(t)] if t in tamaños else 0 for t in tamaños_unicos]
        plt.bar(x + i * ancho_barra, tiempos_por_tamaño, width=ancho_barra, label=f"{algoritmo} - Python")
    
    # Graficar tiempos para C
    for i, (algoritmo, (tamaños, tiempos)) in enumerate(tiempos_c.items()):
        tiempos_por_tamaño = [tiempos[tamaños.index(t)] if t in tamaños else 0 for t in tamaños_unicos]
        plt.bar(x + (i + len(algoritmos_python)) * ancho_barra, tiempos_por_tamaño, width=ancho_barra, label=f"{algoritmo} - C")
    
    plt.xlabel("Tamaño de la matriz (n x n)")
    plt.ylabel("Tiempo de ejecución (s)")
    plt.title(titulo)
    plt.xticks(x + ancho_barra * (len(algoritmos_python) + len(algoritmos_c) - 1) / 2, tamaños_unicos)
    plt.yscale("log")
    if len(algoritmos_python) + len(algoritmos_c) > 0:
        plt.legend()
    plt.grid(True, which="both", linestyle="--", linewidth=0.5, axis='y')
    plt.show()

# Ruta a la carpeta de resultados
directorio_resultados = "../Resultados"

# Cargar los tiempos de los archivos
tiempos_por_algoritmo = cargar_tiempos(directorio_resultados)

# Graficar los tiempos para Python
graficar_tiempos_barras(tiempos_por_algoritmo, lenguaje_filtro="python", titulo="Tiempos de ejecución - Python")

# Graficar los tiempos para C
graficar_tiempos_barras(tiempos_por_algoritmo, lenguaje_filtro="c", titulo="Tiempos de ejecución - C")

# Graficar tiempos combinados para Python y C
graficar_tiempos_combinados(tiempos_por_algoritmo, titulo="Tiempos de ejecución combinados - Python y C")

