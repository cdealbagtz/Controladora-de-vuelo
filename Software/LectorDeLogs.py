# -*- coding: utf-8 -*-
"""
Created on Wed May 21 22:25:48 2025

@author: Chris
"""

import numpy as np
import pandas as pd

# === Parámetros del log ===
filename = 'C:/Users/cdealba/Desktop/FD000.bin'
package_size = 128

# === Cargar datos binarios ===
with open(filename, 'rb') as f:
    raw_data = np.frombuffer(f.read(), dtype=np.uint8)

package_count = len(raw_data) // package_size

# === Inicializar arrays ===
def init_array(dtype):
    return np.zeros(package_count, dtype=dtype)

Nmsg = init_array(np.uint32)
Time = init_array(np.uint32)

Ax = init_array(np.float64)
Ay = init_array(np.float64)
Az = init_array(np.float64)
Gx = init_array(np.float64)
Gy = init_array(np.float64)
Gz = init_array(np.float64)
Gfx = init_array(np.float64)
Gfy = init_array(np.float64)
Gfz = init_array(np.float64)
Roll = init_array(np.float64)
Pitch = init_array(np.float64)
Heading = init_array(np.float64)

Alt = init_array(np.uint16)
Latitude = init_array(np.int32)
Longitude = init_array(np.int32)
Arsp = init_array(np.uint16)
Presion = init_array(np.uint32)

SBUS_Flags = init_array(np.uint8)
Flight_Mode = init_array(np.uint8)

Canales = [init_array(np.uint16) for _ in range(16)]
OUTS = [init_array(np.uint16) for _ in range(10)]

Interruptor_1 = init_array(np.uint8)
Interruptor_2 = init_array(np.uint8)

# === Función para extraer datos ===
def extract(data, idx, offset, length, dtype):
    start = idx * package_size + offset
    return np.frombuffer(data[start:start+length], dtype=dtype)[0]

# === Parsear paquetes ===
for n in range(package_count):
    i = n * package_size

    Ax[n]      = extract(raw_data, n, 0, 4, np.float32)
    Ay[n]      = extract(raw_data, n, 4, 4, np.float32)
    Az[n]      = extract(raw_data, n, 8, 4, np.float32)
    Gx[n]      = extract(raw_data, n, 12, 4, np.float32)
    Gy[n]      = extract(raw_data, n, 16, 4, np.float32)
    Gz[n]      = extract(raw_data, n, 20, 4, np.float32)
    Gfx[n]     = extract(raw_data, n, 24, 4, np.float32)
    Gfy[n]     = extract(raw_data, n, 28, 4, np.float32)
    Gfz[n]     = extract(raw_data, n, 32, 4, np.float32)
    Roll[n]    = extract(raw_data, n, 36, 4, np.float32)
    Pitch[n]   = extract(raw_data, n, 40, 4, np.float32)
    Heading[n] = extract(raw_data, n, 44, 4, np.float32)

    Nmsg[n]     = extract(raw_data, n, 48, 4, np.uint32)
    Time[n]     = extract(raw_data, n, 52, 4, np.uint32)

    Latitude[n] = extract(raw_data, n, 56, 4, np.int32)
    Longitude[n]= extract(raw_data, n, 60, 4, np.int32)
    Presion[n]  = extract(raw_data, n, 64, 4, np.uint32)

    Alt[n]      = extract(raw_data, n, 66, 2, np.uint16)
    Arsp[n]     = extract(raw_data, n, 68, 2, np.uint16)

    for i in range(16):
        offset = 70 + i * 2
        Canales[i][n] = extract(raw_data, n, offset, 2, np.uint16)

    for i in range(10):
        offset = 102 + i * 2
        OUTS[i][n] = extract(raw_data, n, offset, 2, np.uint16)

    SBUS_Flags[n]    = extract(raw_data, n, 124, 1, np.uint8)
    Flight_Mode[n]   = extract(raw_data, n, 125, 1, np.uint8)
    Interruptor_1[n] = extract(raw_data, n, 126, 1, np.uint8)
    Interruptor_2[n] = extract(raw_data, n, 127, 1, np.uint8)

# === Crear DataFrame y guardar CSV ===
data = {
    'Nmsg': Nmsg, 'Time': Time,
    'Ax': Ax, 'Ay': Ay, 'Az': Az,
    'Gx': Gx, 'Gy': Gy, 'Gz': Gz,
    'Gfx': Gfx, 'Gfy': Gfy, 'Gfz': Gfz,
    'Roll': Roll, 'Pitch': Pitch, 'Heading': Heading,
    'Alt': Alt, 'Latitude': Latitude, 'Longitude': Longitude,
    'Arsp': Arsp, 'Presion': Presion,
    'SBUS_Flags': SBUS_Flags, 'Flight_Mode': Flight_Mode,
    'Interruptor_1': Interruptor_1, 'Interruptor_2': Interruptor_2,
}

# Añadir canales y outs
for i in range(16):
    data[f'Canal_{i+1}'] = Canales[i]
for i in range(10):
    data[f'OUT_{i+1}'] = OUTS[i]

df = pd.DataFrame(data)
df.to_csv('LogDeVuelo.csv', index=False)
