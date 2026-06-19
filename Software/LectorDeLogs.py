# -*- coding: utf-8 -*-
"""
Conversión y concatenación de logs FDXXX.bin a CSV

@author: Chris
"""

from pathlib import Path
import re
import numpy as np
import pandas as pd
from pathlib import Path


# ============================================================
# Parámetros del log
# ============================================================

folder_path = Path(r"E:\Flight001")   # Cambia esta ruta por la carpeta real
output_csv = folder_path / "LogDeVuelo.csv"

package_size = 128

# Verificar que la carpeta exista antes de continuar
if not folder_path.exists():
    raise FileNotFoundError(
        f"No existe la carpeta especificada:\n{folder_path}\n\n"
        "Verifica que la memoria esté conectada, que la letra de unidad sea correcta "
        "y que la carpeta exista."
    )

if not folder_path.is_dir():
    raise NotADirectoryError(
        f"La ruta especificada existe, pero no es una carpeta:\n{folder_path}"
    )


# ============================================================
# Tipos de dato
# ============================================================
# STM32 normalmente guarda en little-endian.
# Si tus datos fueron escritos desde STM32, esta configuración es la adecuada.

F32 = np.dtype("<f4")   # float32
U32 = np.dtype("<u4")   # uint32
I32 = np.dtype("<i4")   # int32
U16 = np.dtype("<u2")   # uint16
U8  = np.dtype("u1")    # uint8


# ============================================================
# Buscar archivos FDXXX.bin
# ============================================================

def get_fd_files(folder: Path):
    pattern = re.compile(r"^FD(\d{3})\.bin$", re.IGNORECASE)

    files = []

    for file in folder.iterdir():
        match = pattern.match(file.name)
        if match:
            file_number = int(match.group(1))
            files.append((file_number, file))

    files.sort(key=lambda x: x[0])

    return [file for _, file in files]


# ============================================================
# Función para extraer datos
# ============================================================

def extract(raw_bytes, packet_index, offset, dtype):
    start = packet_index * package_size + offset
    return np.frombuffer(raw_bytes, dtype=dtype, count=1, offset=start)[0]


# ============================================================
# Parsear un archivo individual
# ============================================================

def parse_fd_file(filename: Path):
    print(f"Procesando: {filename.name}")

    with open(filename, "rb") as f:
        raw_data = f.read()

    total_bytes = len(raw_data)
    package_count = total_bytes // package_size
    remaining_bytes = total_bytes % package_size

    if remaining_bytes != 0:
        print(
            f"  Advertencia: {filename.name} tiene {remaining_bytes} bytes sobrantes. "
            "Se ignorará el paquete incompleto final."
        )

    if package_count == 0:
        print(f"  Advertencia: {filename.name} no contiene paquetes completos.")
        return pd.DataFrame()

    # === Inicializar arrays ===
    def init_array(dtype):
        return np.zeros(package_count, dtype=dtype)

    Nmsg = init_array(U32)
    Time = init_array(U32)

    Ax = init_array(F32)
    Ay = init_array(F32)
    Az = init_array(F32)
    Gx = init_array(F32)
    Gy = init_array(F32)
    Gz = init_array(F32)
    Gfx = init_array(F32)
    Gfy = init_array(F32)
    Gfz = init_array(F32)
    Roll = init_array(F32)
    Pitch = init_array(F32)
    Heading = init_array(F32)

    Alt = init_array(U16)
    Latitude = init_array(I32)
    Longitude = init_array(I32)
    Arsp = init_array(U16)
    Presion = init_array(U32)

    SBUS_Flags = init_array(U8)
    Flight_Mode = init_array(U8)

    Canales = [init_array(U16) for _ in range(16)]
    OUTS = [init_array(U16) for _ in range(10)]

    Interruptor_1 = init_array(U8)
    Interruptor_2 = init_array(U8)

    # === Parsear paquetes ===
    for n in range(package_count):

        Ax[n]      = extract(raw_data, n, 0, F32)
        Ay[n]      = extract(raw_data, n, 4, F32)
        Az[n]      = extract(raw_data, n, 8, F32)

        Gx[n]      = extract(raw_data, n, 12, F32)
        Gy[n]      = extract(raw_data, n, 16, F32)
        Gz[n]      = extract(raw_data, n, 20, F32)

        Gfx[n]     = extract(raw_data, n, 24, F32)
        Gfy[n]     = extract(raw_data, n, 28, F32)
        Gfz[n]     = extract(raw_data, n, 32, F32)

        Roll[n]    = extract(raw_data, n, 36, F32)
        Pitch[n]   = extract(raw_data, n, 40, F32)
        Heading[n] = extract(raw_data, n, 44, F32)

        Nmsg[n]    = extract(raw_data, n, 48, U32)
        Time[n]    = extract(raw_data, n, 52, U32)

        Latitude[n]  = extract(raw_data, n, 56, I32)
        Longitude[n] = extract(raw_data, n, 60, I32)
        Presion[n]   = extract(raw_data, n, 64, U32)

        Alt[n]      = extract(raw_data, n, 68, U16)
        Arsp[n]     = extract(raw_data, n, 70, U16)

        for ch in range(16):
            offset = 72 + ch * 2
            Canales[ch][n] = extract(raw_data, n, offset, U16)

        for out in range(10):
            offset = 104 + out * 2
            OUTS[out][n] = extract(raw_data, n, offset, U16)

        SBUS_Flags[n]    = extract(raw_data, n, 124, U8)
        Flight_Mode[n]   = extract(raw_data, n, 125, U8)
        Interruptor_1[n] = extract(raw_data, n, 126, U8)
        Interruptor_2[n] = extract(raw_data, n, 127, U8)

    # === Crear DataFrame del archivo ===
    data = {
        "Archivo": filename.name,
        "Paquete_en_archivo": np.arange(package_count, dtype=np.uint32),

        "Nmsg": Nmsg,
        "Time": Time,

        "Ax": Ax,
        "Ay": Ay,
        "Az": Az,

        "Gx": Gx,
        "Gy": Gy,
        "Gz": Gz,

        "Gfx": Gfx,
        "Gfy": Gfy,
        "Gfz": Gfz,

        "Roll": Roll,
        "Pitch": Pitch,
        "Heading": Heading,

        "Alt": Alt,
        "Latitude": Latitude,
        "Longitude": Longitude,

        "Arsp": Arsp,
        "Presion": Presion,

        "SBUS_Flags": SBUS_Flags,
        "Flight_Mode": Flight_Mode,

        "Interruptor_1": Interruptor_1,
        "Interruptor_2": Interruptor_2,
    }

    for i in range(16):
        data[f"Canal_{i + 1}"] = Canales[i]

    for i in range(10):
        data[f"OUT_{i + 1}"] = OUTS[i]

    df = pd.DataFrame(data)

    return df


# ============================================================
# Script principal
# ============================================================

fd_files = get_fd_files(folder_path)

if not fd_files:
    raise FileNotFoundError(
        f"No se encontraron archivos con formato FDXXX.bin en: {folder_path}"
    )

print("Archivos encontrados:")
for file in fd_files:
    print(f"  {file.name}")

dataframes = []

for file in fd_files:
    df_file = parse_fd_file(file)

    if not df_file.empty:
        dataframes.append(df_file)

if not dataframes:
    raise RuntimeError("No se pudo extraer información de ningún archivo.")

df = pd.concat(dataframes, ignore_index=True)

# Crear un índice global de paquete después de concatenar
df.insert(0, "Paquete_global", np.arange(len(df), dtype=np.uint32))

df.to_csv(output_csv, index=False)

print("")
print(f"Conversión finalizada.")
print(f"Paquetes totales: {len(df)}")
print(f"Archivo generado: {output_csv}")