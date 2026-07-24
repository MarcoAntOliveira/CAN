import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import collections

# === CONFIGURAÇÕES ===
PORTA = '/dev/ttyUSB0'  # Altere conforme necessário
BAUD = 9600
INTERVALO = 100
TAM_JANELA = 100

# === INICIALIZA SERIAL ===
try:
    ser = serial.Serial(PORTA, BAUD, timeout=1)
    print(f"✅ Conectado em {PORTA}")
except Exception as e:
    print(f"❌ Erro ao abrir {PORTA}: {e}")
    exit(1)

# === DADOS ===
dados_G = collections.deque([0]*TAM_JANELA, maxlen=TAM_JANELA)
dados_A = collections.deque([0]*TAM_JANELA, maxlen=TAM_JANELA)
dados_L = collections.deque([0]*TAM_JANELA, maxlen=TAM_JANELA)
dados_N = collections.deque([0]*TAM_JANELA, maxlen=TAM_JANELA)

# === PLOT ===
plt.style.use('ggplot')
fig, (ax1, ax2, ax3, ax4) = plt.subplots(4, 1, figsize=(10, 6), sharex=True)

linha_G, = ax1.plot(dados_G, color='blue', label='Giroscópio')
ax1.set_ylim(-100,100)
ax1.set_ylabel("Giroscópio")
ax1.legend(loc='upper right')

linha_A, = ax2.plot(dados_A, color='green', label='Vel. Angular')
ax2.set_ylim(-5, 5)
ax2.set_ylabel("Vel. Angular")
ax2.legend(loc='upper right')

linha_L, = ax3.plot(dados_L, color='orange', label='Vel. Longitudinal')
ax3.set_ylim(-5,5)
ax3.set_ylabel("Vel. Longitudinal")
ax3.legend(loc='upper right')

linha_N, = ax4.plot(dados_N, color='red', label='Vel. Normal')
ax4.set_ylim(-5,5)
ax4.set_ylabel("Vel. Normal")
ax4.set_xlabel("Tempo (amostras)")
ax4.legend(loc='upper right')


# === FUNÇÃO DE ATUALIZAÇÃO ===
def atualizar(frame):
    try:
        linha = ser.readline().decode('utf-8').strip()
        if linha:
            print(f"Recebido: {linha}")
            if linha.startswith("G:"):
                valor = float(linha.split(":")[1])
                dados_G.append(valor)
                linha_G.set_ydata(dados_G)
            elif linha.startswith("A:"):
                valor = float(linha.split(":")[1])
                dados_A.append(valor)
                linha_A.set_ydata(dados_A)
            elif linha.startswith("L:"):
                valor = float(linha.split(":")[1])
                dados_L.append(valor)
                linha_L.set_ydata(dados_L)
            elif linha.startswith("N:"):
                valor = float(linha.split(":")[1])
                dados_N.append(valor)
                linha_N.set_ydata(dados_N)
    except Exception as e:
        print(f"⚠️ Erro: {e}")
    return linha_G, linha_A, linha_L, linha_N

# === ANIMAÇÃO ===
ani = FuncAnimation(fig, atualizar, interval=INTERVALO, blit=False)
plt.tight_layout()
plt.show()