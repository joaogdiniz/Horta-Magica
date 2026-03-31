## 💡 Sobre o Projeto (Horta Mágica)

A **Horta Mágica** é um projeto de Internet das Coisas (IoT) focado em impacto social e educação. Ele foi desenvolvido para instigar crianças do ensino fundamental a se interessarem por robótica e tecnologia através de uma atividade interativa, lúdica e sustentável.

O sistema transforma uma planta comum em um "objeto inteligente" capaz de se comunicar com os alunos. Através de sensores que monitoram o ambiente em tempo real, a planta avalia as condições ao seu redor e expressa suas necessidades diretamente em um display LCD. De forma divertida, ela é capaz de:
* 💧 **Pedir água** quando detecta que o solo está ficando seco;
* 🥵 **Reclamar do ambiente** quando o ar está muito árido, abafado ou frio demais;
* 😁 **Agradecer e demonstrar felicidade** quando é regada ou quando o sensor detecta chuva.

**Objetivo Pedagógico**
Muito além do desenvolvimento de hardware e software, a Horta Mágica atua como uma ferramenta prática para educar as crianças sobre a importância do cuidado com o meio ambiente. O projeto introduz conceitos de tecnologia, eletrônica e programação de uma maneira amigável, provando que a robótica pode estar presente no nosso cotidiano de forma acolhedora.

---

## 🛠️ Lista de Materiais e Componentes

Para a montagem e pleno funcionamento do projeto, foram utilizados os seguintes hardwares, materiais estruturais e dependências de software:

### ⚡ Hardware e Eletrônica
* **1x Microcontrolador Arduino Uno (R3):** O "cérebro" do projeto, responsável pelo processamento dos dados e lógica de intermitência.
* **1x Sensor de Umidade do Solo:** Higrômetro do tipo resistivo (leitura analógica da condutividade do solo).
* **1x Sensor de Chuva:** Com módulo comparador (chip principal LM393).
* **1x Sensor de Temperatura e Umidade DHT11:** Termistor NTC + HR202 para monitoramento do microclima do ambiente.
* **1x Display LCD 16x2:** Utilizado para a interface lúdica (operando em conexão paralela de 4 bits).
* **1x Potenciômetro:** Atuando como divisor de tensão resistivo para o ajuste de contraste do display LCD.
* **1x Protoboard:** Para a prototipagem e distribuição das conexões.
* **1x Fonte de Alimentação:** Para o Arduino (recomendado de 7V a 12V).
* **Cabos Jumper:** Conjunto misto contendo cabos Macho-Macho, Macho-Fêmea e Fêmea-Fêmea.

### 🏗️ Estrutura e Prototipagem
* **Recipiente Transparente:** Utilizado para criar um case de proteção impermeável para o Arduino, protoboard e fonte.
* **Palitos de Picolé:** Utilizados para construir um pequeno "toldo". Esta estrutura serve para instalar o sensor de chuva no topo e abrigar o sensor DHT11 na parte inferior (protegendo-o da luz direta do sol e precipitação, mas mantendo-o exposto ao ar).
* **Planta em Vaso Pequeno:** Para validação e testes práticos de variação de umidade e comportamento do sistema.

### 💻 Software e Dependências
* **IDE do Arduino** (Desenvolvimento em C++)
* **Biblioteca `LiquidCrystal.h`:** Para o controle e envio de caracteres ao Display LCD.
* **Biblioteca `DHT.h`:** Para a correta interpretação do protocolo de comunicação e leitura dos dados do sensor DHT11.
