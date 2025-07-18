import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import collections

# === CONFIGURAÇÕES ===
PORTA = '/dev/ttyUSB0'
BAUD = 9600
INTERVALO = 50          # ms entre atualizações
TAM_JANELA = 100        # Número de amostras exibidas

# === INICIALIZA SERIAL ===
try:
    ser = serial.Serial(PORTA, BAUD, timeout=1)
    print(f"✅ Conectado em {PORTA}")
except Exception as e:
    print(f"❌ Erro ao abrir {PORTA}: {e}")
    exit(1)

# === DADOS INICIAIS ===
dados1 = collections.deque([0] * TAM_JANELA, maxlen=TAM_JANELA)

# === CONFIGURAÇÃO DO PLOT ===
plt.style.use('ggplot')
fig, ax1 = plt.subplots(figsize=(10, 6), sharex=True)

linha1, = ax1.plot(dados1, color='blue', label='Sensor 1')
ax1.set_ylim(0, 200)
ax1.set_ylabel("Sensor 1")
ax1.legend(loc='upper right')

# === FUNÇÃO DE ATUALIZAÇÃO ===
def atualizar(frame):
    try:
        linha_serial = ser.readline().decode('utf-8').strip()
        if linha_serial:
            print(f"Recebido: {linha_serial}")
            partes = linha_serial.split(',')
            if len(partes) >= 1:
                valor1 = float(partes[0])
                dados1.append(valor1)
                linha1.set_ydata(dados1)
    except Exception as e:
        print("Erro na leitura ou conversão:", e)
    return linha1,

# === ANIMAÇÃO ===
ani = FuncAnimation(fig, atualizar, interval=INTERVALO, blit=True)
plt.tight_layout()
plt.show()
