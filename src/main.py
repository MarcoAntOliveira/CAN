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
dados2 = collections.deque([0] * TAM_JANELA, maxlen=TAM_JANELA)
dados3 = collections.deque([0] * TAM_JANELA, maxlen=TAM_JANELA)
dados4 = collections.deque([0] * TAM_JANELA, maxlen=TAM_JANELA)

# === CONFIGURAÇÃO DO PLOT ===
plt.style.use('ggplot')
fig, (ax1, ax2, ax3, ax4) = plt.subplots(4, 1, figsize=(10, 6), sharex=True)

# Gráfico 1
linha1, = ax1.plot(dados1, color='blue', label='Sensor 1')
ax1.set_ylim(0,200)
ax1.set_ylabel("Sensor 1")
ax1.legend(loc='upper right')

# Gráfico 2
linha2, = ax2.plot(dados2, color='green', label='Sensor 2')
ax2.set_ylim(0, 200)
ax2.set_ylabel("Sensor 2")
ax2.set_xlabel("Tempo (pontos)")
ax2.legend(loc='upper right')

# Gráfico 3
linha3, = ax3.plot(dados3, color='yellow', label='Sensor 3')
ax3.set_ylim(0,200)
ax3.set_ylabel("Sensor 3")
ax3.legend(loc='upper right')

# Gráfico 1
linha4, = ax4.plot(dados4, color='red', label='Sensor 4')
ax4.set_ylim(0,200)
ax4.set_ylabel("Sensor 4")
ax4.legend(loc='upper right')




#
# === FUNÇÃO DE ATUALIZAÇÃO ===
def atualizar(frame):
    try:
        linha_serial = ser.readline().decode('utf-8').strip()
        if linha_serial:
            print(f"Recebido: {linha_serial}")

            # Espera algo como: 512,400
            partes = linha_serial.split(',')
            if len(partes) >= 2:
                valor1 = float(partes[0])
                valor2 = float(partes[1])
                valor3 = float(partes[2])
                valor4 = float(partes[3])
                dados1.append(valor1)
                dados2.append(valor2)
                dados3.append(valor3)
                dados4.append(valor4)
                linha1.set_ydata(dados1)
                linha2.set_ydata(dados2)
                linha3.set_ydata(dados3)
                linha4.set_ydata(dados4)
    except Exception as e:
        print("Erro na leitura ou conversão:", e)
    return linha1, linha2, linha3, linha4

# === ANIMAÇÃO ===
ani = FuncAnimation(fig, atualizar, interval=INTERVALO, blit=True)
plt.tight_layout()
plt.show()
