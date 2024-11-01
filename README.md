Trabalho da disciplina de SO, que tem o foco em implementar uma aplicação multithread com pelo menos uma condição de disputa.

No cenario desta aplicação, estamos simulando que 2 jogadores podem solicitar itens pra um estregador (no caso, um "burrinho" semelhante ao do Dota 2), o cenario foi adaptado da seguinte forma: 
  O entregador só pode levar 1 item por vez, e possui um tempo até ir ate o jogador e entregar o item, e retornar a base;
  Quando o entregador está na base, está disponivel para levar o item para o proximo jogador;
  Enquanto está levando um item ou voltando para a base, o entregador fica indisponivel para realizar outras entregas, o processo de solicitar uma entrega só ocorre quando ele está na base.

Foi utilizado Mutex para lidar com a condição de disputa, e o programa após ser executado roda de forma continua, para encerra-lo basta dar o comando CTRL + C no terminal.
