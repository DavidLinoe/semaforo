import cv2
import numpy as np
import urllib.request

url = 'http://192.168.148.127/cam-hi.jpg'

whT = 320
confThreshold = 0.5
nmsThreshold = 0.3
arquivoClasses = 'coco.names'
nomesClasses = []
with open(arquivoClasses, 'rt') as f:
    nomesClasses = f.read().rstrip('\n').split('\n')

# Classes de interesse
classesAlvo = ['person', 'car', 'motorbike', 'truck']

configModelo = 'yolov3.cfg'
pesosModelo = 'yolov3.weights'
net = cv2.dnn.readNetFromDarknet(configModelo, pesosModelo)
net.setPreferableBackend(cv2.dnn.DNN_BACKEND_OPENCV)
net.setPreferableTarget(cv2.dnn.DNN_TARGET_CPU)

def encontrarObjetos(saidas, imagem):
    hT, wT, cT = imagem.shape
    caixas = []
    idsClasses = []
    confiancas = []
    contagemObjetos = 0

    for saida in saidas:
        for det in saida:
            pontuacoes = det[5:]
            idClasse = np.argmax(pontuacoes)
            confianca = pontuacoes[idClasse]
            if confianca > confThreshold and nomesClasses[idClasse] in classesAlvo:
                w, h = int(det[2] * wT), int(det[3] * hT)
                x, y = int((det[0] * wT) - w / 2), int((det[1] * hT) - h / 2)
                caixas.append([x, y, w, h])
                idsClasses.append(idClasse)
                confiancas.append(float(confianca))

    indices = cv2.dnn.NMSBoxes(caixas, confiancas, confThreshold, nmsThreshold)
    if len(indices) > 0:
        for i in indices.flatten():
            caixa = caixas[i]
            x, y, w, h = caixa[0], caixa[1], caixa[2], caixa[3]
            rotulo = nomesClasses[idsClasses[i]]
            contagemObjetos += 1
            
            cv2.rectangle(imagem, (x, y), (x + w, y + h), (255, 0, 255), 2)
            cv2.putText(imagem, f'{rotulo.upper()} {int(confiancas[i] * 100)}%', (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 255), 2)
    
    cv2.putText(imagem, f'Contagem de Objetos: {contagemObjetos}', (20, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    print(f'Número de objetos detectados: {contagemObjetos}')

# Capturar uma única imagem da URL
img_resp = urllib.request.urlopen(url)
img_np = np.array(bytearray(img_resp.read()), dtype=np.uint8)
imagem = cv2.imdecode(img_np, -1)

if imagem is None:
    print("Falha ao capturar imagem do URL.")
else:
    blob = cv2.dnn.blobFromImage(imagem, 1/255, (whT, whT), [0, 0, 0], 1, crop=False)
    net.setInput(blob)
    nomesCamadas = net.getLayerNames()
    nomesSaidas = [nomesCamadas[i - 1] for i in net.getUnconnectedOutLayers()]
    saidas = net.forward(nomesSaidas)
    encontrarObjetos(saidas, imagem)
    
    cv2.imshow('Imagem', imagem)
    cv2.waitKey(0)
    cv2.destroyAllWindows()