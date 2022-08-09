## Interruptor paralelo web
Só quero apagar a luz do quarto sem ter que levantar da cama.


### Sobre
A ideia é acessar uma página web que fará requisições para o "Realtime Database" da google. A placa Wemos estará lendo os dados do RTD e fará o controle do relé a partir dos dados coletados no banco e escreverá os dados coletados pelo leitor de tensão.

 - Wemos D1 R2, que será o cérebro da aplicação;
 - Um módulo relé (5v), que atuará como um interruptor paralelo;
 - Um módulo leitor de tensão, para retornar o estado atual da lâmpada, uma vez que pode-se acender ou apagar a mesma através de um interruptor que não tem ligação com o arduino.