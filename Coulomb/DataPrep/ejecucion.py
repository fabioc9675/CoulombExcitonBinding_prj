import subprocess
import pandas as pd
import matplotlib.pyplot as plt

# BANDERA para ejecutar o no los programas
ejecutar_integral = True  # Cambia a True si quieres volver a ejecutar los .exe

# Parámetros y rutas
dirpath = "./"
archivos_e = ['xe_1.txt', 'xe_5.txt', 'xe_10.txt']
archivos_h = ['xh_1.txt', 'xh_5.txt', 'xh_10.txt']
arc_salida = ["res_1.dat", "res_5.dat", "res_10.dat"]
valores_dz = [0.2, 0.5, 1.0]
valores_dr = [2, 2, 2]
valores_in = [2.0, 15.0, 110.0]
cols = [9, 18, 10]
cap = [7701, 12289, 11269]
res = [1, 5, 10]

# Ejecutar y recolectar resultados
if ejecutar_integral:
    for i in range(len(valores_dr)):
        dro = valores_dr[i]
        dz  = valores_dz[i]
        archivo_e = dirpath + archivos_e[i]
        archivo_h = dirpath + archivos_h[i]
        archivo_salida = dirpath + arc_salida[i]
        inicial = valores_in[i]

        args = [
            "./exciton_openmp",
            archivo_e,
            archivo_h,
            archivo_salida,
            str(dro),
            str(dz),
            str(dro),
            str(dz),
            str(res[i]),
            str(inicial),
            str(cols[i]),
            str(cap[i]),
        ]

        print(f"Ejecutando: {' '.join(args)}")
        result = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        print(result.stdout)
        print(result.stderr)
else:
    print("Saltando ejecución de integrales (archivos ya generados).")

# Concatenar archivos de salida
archivo_salida_total = "resultados_1.dat"
with open(archivo_salida_total, "w") as fout:
    for fname in arc_salida:
        ruta_completa = dirpath + fname
        with open(ruta_completa, "r") as fin:
            fout.writelines(fin.readlines())

# Cargar los datos del archivo unido
df = pd.read_csv(archivo_salida_total, delim_whitespace=True, header=None)

# Extraer las dos primeras columnas
x = df[0]
y = df[1]

# Graficar
plt.figure(figsize=(8, 5))
plt.plot(x, y, marker='o', linestyle='-', color='b', label="Datos")
plt.xlabel("Columna 1")
plt.ylabel("Columna 2")
plt.title("Gráfico de las primeras dos columnas")
plt.legend()
plt.grid(True)
plt.show()

# Guardar para usos posteriores
y1 = y
