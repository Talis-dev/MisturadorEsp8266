# Misturador de Carnes Industrial - Controlador Arduino

Este projeto implementa um controlador para um misturador de carnes industrial, utilizando um Arduino. O sistema oferece quatro programas customizáveis para ciclos de mistura, com controle de segurança e interface via keypad e display LCD.

## Funcionalidades

* **Programas Customizáveis:**
    * Quatro programas ajustáveis para diferentes tempos de mistura (obverso, reverso, pausa) e número de ciclos.
    * Armazenamento dos programas na EEPROM para persistência entre reinicializações.
    * Edição dos programas via interface de usuário, protegida por senha.
    * Seleção do programa ativo.
* **Interface de Usuário:**
    * Display LCD para visualização de informações e menus.
    * Keypad para navegação e entrada de dados.
    * Login com senha para acesso as configurações de programa.
    * Navegação por menus para seleção e edição de programas.
* **Controle de Segurança:**
    * Sensores de segurança integrados (porta superior, botão de emergência, relé de emergência, falha do inversor).
    * Botão de emergência para parada imediata.
    * Liberação de válvula pneumática por relé quando não está operando.
* **Controle do Motor:**
    * Controle de direção do motor (obverso e reverso).
    * Integração com inversor de frequência WEG CFW300 (7.5 CV).
    * Lógica de controle de tempo para os ciclos de mistura.
* **Feedback Sonoro:**
    * Buzzer para feedback de cliques no keypad e outras notificações.

## Hardware

* Arduino
* Display LCD I2C
* Keypad 4x4 I2C
* PCF8574 Módulos de I/O
* Relé
* Inversor de frequência WEG CFW300
* Motor 7,5 cv
* Sensores de segurança

## Estrutura do Código

O código está organizado em múltiplos arquivos para melhor modularidade:

* **`programa.ino`:** Arquivo principal com a lógica de controle, leitura de sensores, interação com o usuário e controle do motor.
* **`programas.h`:** Definição da estrutura de dados dos programas e funções relacionadas ao armazenamento e manipulação dos programas na EEPROM.
* **`PCF8574.h`:** Biblioteca para interação com os módulos de I/O PCF8574.h

## Lógica de Controle da Mistura

* O sistema controla os ciclos de mistura com base nos tempos definidos nos programas (obverso, reverso, pausa) e no número de ciclos.
* A lógica de controle alterna entre os estados de obverso e reverso, com pausas entre eles, repetindo o ciclo conforme definido.
* O tempo total do processo é calculado e exibido no display.
* Caso o botão de start seja acionado durante o funcionamento, o processo para, e caso o botão seja acionado novamente, o processo continua.
* O sistema pneumático de segurança é liberado quando o sistema não esta em operação, e bloqueado quando esta em operação.

## Sistema de Segurança

* O sistema monitora continuamente os sensores de segurança:
    * Porta superior aberta.
    * Botão de emergência pressionado.
    * Relé de emergência ativado.
    * Falha no inversor.
* Qualquer ativação de um sensor de segurança interrompe imediatamente o processo de mistura e exibe uma mensagem de erro no display.
* O sistema pneumático de segurança é atuado quando o sistema esta em segurança.

## Instalação

1.  Clone este repositório.
2.  Instale as bibliotecas necessárias no Arduino IDE:
    * Wire.h
    * EEPROM.h
    * Keypad.h
    * LiquidCrystal_I2C.h
    * PCF8574.h
3.  Conecte o hardware conforme o esquema elétrico do projeto.
4.  Carregue o código para o seu Arduino.

## Uso

1.  Ao iniciar, o sistema exibe a tela inicial.
2.  Use o keypad para navegar pelos menus e selecionar os programas.
3.  Ajuste os parâmetros dos programas conforme necessário, após inserir a senha.
4.  O sistema monitora os sensores de segurança e o botão de emergência para garantir a operação segura.
5.  O processo de mistura é iniciado e controlado automaticamente, com base nos parâmetros do programa selecionado.

## Contribuição

Contribuições são bem-vindas! Se você tiver sugestões de melhorias ou correções de bugs, por favor, abra uma issue ou envie um pull request.

## Licença

Este projeto é distribuído sob a licença [MIT](LICENSE).

## Contato

* Talis

## Agradecimentos

* A todos que contribuíram para este projeto.
