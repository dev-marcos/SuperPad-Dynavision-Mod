# SuperPad Dynavision Mod

Este repositório contém o código e as instruções para modificar o controle Super Pad do console Dynavision da Dynacom. A modificação utiliza uma placa ESP32 C3 Supermini para ler o estado dos botões e controlar um LED. O projeto é desenvolvido na IDE do Arduino e é configurado para ser compilado e carregado usando o Platform.io.

## Características
- Leitura dos estados dos botões UP, DOWN, LEFT, RIGHT, SELECT, START, A, B, X e Y.
- Exibição dos botões pressionados via serial.
- Controle de um LED indicador.
- Configuração dos botões como entradas com pull-up.
- Compatível com a placa ESP32 C3 Supermini.
- Suporte a Bluetooth utilizando a biblioteca `ESP32-BLE-Gamepad`.

## Requisitos
- Placa ESP32 C3 Supermini
- IDE do Arduino com suporte ao ESP32 ou Platform.io
- Conexões conforme o mapeamento de pinos fornecido

## Mapeamento dos Pinos
- UP: GPIO 2
- DOWN: GPIO 0
- LEFT: GPIO 3
- RIGHT: GPIO 1
- SELECT: GPIO 6
- START: GPIO 4
- A: GPIO 9
- B: GPIO 10
- X: GPIO 8
- Y: GPIO 7
- LED: GPIO 5

## Instalação
1. Clone o repositório: `git clone https://github.com/seuusuario/SuperPad_Dynavision_Mod.git`
2. Abra o projeto no Platform.io.
3. Compile e carregue o código na sua placa ESP32 C3 Supermini.

## Licença
Este projeto está licenciado sob a licença MIT - veja o arquivo [LICENSE](LICENSE) para detalhes.
