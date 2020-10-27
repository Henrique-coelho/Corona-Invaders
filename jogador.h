
// coloca o jogador em sua posição inicial
void inicializarJogador(){
	jogador.ativado = true;
	jogador.invulneravel = false;
	
	jogador.vida = 3;
	jogador.local.coordx = 0;
	jogador.local.coordy = -280;
	jogador.local.largura = 40;
	jogador.local.altura = 40;
	jogador.posLimiteMin = -560;
	jogador.posLimiteMax = 560;
	jogador.pontos = 0;
	
	jogador.timerBala = 0;
	jogador.timerDano = 0;
	jogador.timerPowerUp = 600;

	jogador.id = 41;
}

void desativarJogador(){
	jogador.ativado = false;
}

void identificarMovimentoDoJogador(){
	// desloca o jogador
	if((leftPressed)&&(jogador.local.coordx!=jogador.posLimiteMin)){
		jogador.local.coordx -= 4;
	}
	if((rightPressed)&&(jogador.local.coordx!=jogador.posLimiteMax)){
		jogador.local.coordx += 4;
	}
}


