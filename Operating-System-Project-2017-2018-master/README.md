# ProjectoSO2017-2018

## 1 Objetivos
O presente trabalho tem como objetivo a concepção e a implementação de um simulador de uma Montanha-Russa . Pretende-se assim que os alunos pratiquem os conceitos aprendidos nas
aulas teóricas e práticas na concepção e desenvolvimento de um sistema simples, empregando os mecanismos de concorrência, sincronização e comunicação existentes na linguagem C.

## 2 Descrição
Uma forma de divertimento das pessoas é a realização de viagens em Montanhas-Russas. Junto à  Montanha-Russa  deverá  existir  um  local  de  venda  de  bilhetes  e,  junto  à  entrada,  alguém verificará que apenas se dirigem aos carros um número máximo de pessoas (capacidade do(s) carro(s)). Por vezes as pessoas cansam-se de esperar e/ou têm receio de fazer a viagem e podem desistir.
Após cada viagem todos devem sair dos carros e, na zona dos carros, já devem estar os próximos passageiros.
A simulação a implementar pretende avaliar as condições de funcionamento de um equipamento deste género em termos de quantas pessoas pode servir, quais os tempos de espera
médios, quantas desistem, o número de carros ótimo, entre outros.
As opções para a simulação são muitas e variadas e ficam à consideração de cada grupo, constituindo  elemento  de  avaliação.   Por  exemplo:  Qual  o  tamanho  máximo  da  fila  para  o guiché?  Existe alguma prioridade (carros da frente e carros traseiros)?  As pessoas chegam todas de uma vez ou vão chegando?  O tempo de viagem é sempre o mesmo?  O tempo de saída/entrada nos carros é relevante?

## 3 Arquitetura
O sistema a desenvolver deverá conter duas aplicações, a primeira (Simulador) que efetuará toda a simulação, e a segunda (Monitor) que receberá todas as mensagens enviadas pela primeira e fará todo o seu tratamento.
O **Simulador** deverá ser lançado tendo por parâmetro o ficheiro de configuração da simulação. Os dados que deverão estar presentes, no mínimo, para o início da simulação são os seguintes: tempo médio (1) de chegada dos utilizadores e tempos diversos, dimensão dos recursos, probabilidade de desistência nas filas, início da simulação e tempo de simulação(2). A Figura 1 apresenta a arquitetura de ficheiros para o projeto. Caso necessário podem ser indicados outros ficheiros (um ficheiro por cada tipo de relatório, apenas um ficheiro de configuração, e outros).
O **Simulador** deverá no seu arranque ligar-se ao **Monitor**. Durante a simulação devem ser apresentados alguns dados sobre o estado da simulação. Por exemplo:

>...
>Chegou um u t i l i z a d o r . Numero 234.
>O utilizador 123 comprou um bilhete.
>O utilizador 136 entrou no carro.
>O utilizador 129 desistiu .
>...

O **Simulador** deverá ter métodos para gerar aleatoriamente a chegada dos utilizadores (cada utilizador será um thread) aos recursos, que permitam a correta coordenação e sincronização dos utilizadores (por via de semáforos) e que enviem mensagens para o **Monitor** (comunicação via *sockets* – Unix ou Internet).
O Monitor aceita uma ligação de um Simulador e guarda em ficheiros todos os registos enviados por este. O tratamento dos dados da simulação acontece ao nível do **Monitor** (cálculo da média do tempo à espera, por exemplo). Durante o funcionamento do Monitor este deve apresentar dados sobre o seu estado de execução. Por exemplo:

>Estado actual => Simulacao a decorrer !
>Utilizadores : 203
>Desistencias : 18
>Nº de Viagens : 11
>Tempo medio de espera : 13 min.

1) Os tempos médios expressam o ritmo aproximado de um dado acontecimento e deve ser implementado um método aleatório para calcular os diversos tempos para diversas fases do projeto.
2) A temporização da simulação deve ser definida de forma a poder ser testada em poucos minutos. Aconselha-se a utilização do time stamp do unix e resumir os tempos da simulação a segundos.
Exemplo dos comandos para lançamento das aplicações:

$ monitor monitor.conf
$ simulador simulacao.conf

## 4 Estrutura de Dados
As duas aplicações terão um conjunto de estruturas de dados para guardar diversas informações. Para os ficheiros de configuração fica a seguinte sugestão:

>PARAMETRO1: dado1
>PARAMETRO2: dado2
>...

onde ’PARAMETRO’ significa a designação do parâmetro e ’dado’ é o seu valor.
Os dados sobre os acontecimentos da simulação poderão ter o seguinte formato:

>IdUtilizador hora Acontecimento
>...

onde o ’Acontecimento’(3) pode ser, por exemplo, um dos seguintes: criação do utilizador,
chegada ao recursos, passagem em filas, entrada e saída dos recursos e desistência.
