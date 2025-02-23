# Simulador de Cronômetro

## Objetivos

- Simular um cronômetro que varia de 1 a 4 minutos.
- Utilizar botões para controlar o funcionamento.
- Exibir a contagem regressiva no display.
- Acionar um LED vermelho ao final da contagem.

## Descrição do Projeto

O projeto implementa um cronômetro simples utilizando um display SSD1306 e botões para controle. O funcionamento ocorre da seguinte maneira:

1. Definição do tempo:

- O display inicia exibindo o valor 0.
- Ao pressionar o botão A, o contador é incrementado em um ciclo de 0 a 4, representando os minutos a serem contados.

2. Início da contagem:

- Ao pressionar o botão B, o cronômetro inicia a contagem regressiva, convertendo o valor selecionado para segundos.
- O tempo restante é atualizado e exibido no display.
  
3. Fim da contagem:

- Quando o cronômetro atinge 0, o LED vermelho é aceso.
- O LED será apagado ao iniciar uma nova contagem.

## Componentes Utilizados

- Microcontrolador RP2040
- Display SSD1306 (I2C)
- Botões de entrada (A e B)
- LED vermelho
- Resistores e fiação

## Controles

- Botão A: Incrementa o contador (0 a 4 minutos).
- Botão B: Inicia a contagem regressiva.
- LED vermelho: Acende quando a contagem chega a zero.

## Protocolo de Comunicação

- I2C: Utilizado para comunicação com o display SSD1306.

## Estrutura do Código

O código está estruturado da seguinte forma:

- Inicialização de hardware: Configura botões, LED e display.
- Manipulação de botões: Interrupção para incrementar contador e iniciar a contagem.
- Loop principal: Executa a contagem regressiva e atualiza o display.
- Atualização do display: Exibe o valor do contador e quando iniciado exibe o tempo restante na tela.

## Como Executar

1. Conecte à placa BitDogLab no computador via cabo USB.
2. Compile o código.
3. Faça o BOOTSEL na placa e clique em run
4. Pressione o Botão A para escolher o valor em minutos do contador e visualize ele no display
5. Pressione o botão B para iniciar o cronometro
6. Veja o cronômetro rodando por meio do display e veja o Led se acender ao finalizar a contagem
