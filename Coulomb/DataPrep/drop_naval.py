import pandas as pd

path_rel = "./"

file_in_h_m = ['xh1.txt', 'xh5.txt', 'xh10.txt']
file_in_e_m = ['xe1.txt', 'xe5.txt', 'xe10.txt']
file_out_h_m = ['xh_1.txt', 'xh_5.txt', 'xh_10.txt']
file_out_e_m = ['xe_1.txt', 'xe_5.txt', 'xe_10.txt']

# Leer el archivo ignorando espacios múltiples y convirtiendo NaN
for i in range(len(file_in_h_m)):
    file_in_h = file_in_h_m[i]
    file_in_e = file_in_e_m[i]
    file_out_h = path_rel + file_out_h_m[i]
    file_out_e = path_rel + file_out_e_m[i]
    
    dfh = pd.read_csv(file_in_h, sep='\s+', header=None, na_values=['NaN'])
    dfe = pd.read_csv(file_in_e, sep='\s+', header=None, na_values=['NaN'])

    # Filtrar filas donde haya al menos un valor no NaN a partir de la 4ta columna
    filtered_dfh = dfh[dfh.iloc[:, 3:].notna().any(axis=1)]
    filtered_dfe = dfe[dfe.iloc[:, 3:].notna().any(axis=1)]

    # Reemplazar NaN restantes por 0 en todas las columnas
    filtered_dfh = filtered_dfh.fillna(0)
    filtered_dfe = filtered_dfe.fillna(0)

    # Eliminar filas donde todos los valores a partir de la columna 3 sean 0
    filtered_dfh = filtered_dfh[~(filtered_dfh.iloc[:, 3:] == 0).all(axis=1)]
    filtered_dfe = filtered_dfe[~(filtered_dfe.iloc[:, 3:] == 0).all(axis=1)]

    # Imprimir la cantidad de líneas finales
    print(f'Cantidad de líneas en {file_out_h}:', len(filtered_dfh))
    print(f'Cantidad de líneas en {file_out_e}:', len(filtered_dfe))

    # Imprimir el número de columnas de cada archivo
    print(f'Número de columnas en {file_in_h}:', dfh.shape[1])
    print(f'Número de columnas en {file_in_e}:', dfe.shape[1])

    # Guardar resultado manteniendo NaN y formato numérico
    filtered_dfh.to_csv(file_out_h, sep='\t', header=False, index=False, na_rep='NaN', float_format='%.15f')  # Mantener precisión decimal
    filtered_dfe.to_csv(file_out_e, sep='\t', header=False, index=False, na_rep='NaN', float_format='%.15f')  # Mantener precisión decimal