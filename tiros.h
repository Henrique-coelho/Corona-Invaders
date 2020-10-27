/*
	O código referente a tiros refere aos tiros inimigo, tiros do jogador, e powerups
*/

// Especifica a dimensão do tiro de acordo com sua variedade
void especificarTiro(int index, int func){
	if (func == 0){ // Tiro inativo
		tiros[index].local.altura = 0;
		tiros[index].local.largura = 0;
		tiros[index].ehInimigo = false;
		tiros[index].ehEspecial = false;
		tiros[index].velocidadeTiro = 0;
	}
	else if(func == 1){ // Tiro do Jogador
		tiros[index].id = 45;
		tiros[index].local.altura = 32;
		tiros[index].local.largura = 32;
		tiros[index].ehInimigo = false;
		tiros[index].ehEspecial = false;
		tiros[index].velocidadeTiro = 8;
	}
	else if(func == 2){// Tiro do inimigo
		tiros[index].id = 44;
		tiros[index].local.altura = 32;
		tiros[index].local.largura = 32;
		tiros[index].ehInimigo = true;
		tiros[index].ehEspecial = false;
		tiros[index].velocidadeTiro = 4;
	}
	else if(func == 3){ // PowerUp : Invulnerabilidade
		tiros[index].id = 43;
		tiros[index].local.altura = 32;
		tiros[index].local.largura = 32;
		tiros[index].ehEspecial = true;
		tiros[index].ehInimigo = true;
		tiros[index].velocidadeTiro = (((float) (rand()%3))+8)/10;
	}
	else if(func == 4){ // PowerUp : Tiro Triplo
		tiros[index].id = 43;
		tiros[index].local.altura = 32;
		tiros[index].local.largura = 32;
		tiros[index].ehEspecial = true;
		tiros[index].ehInimigo = true;
		tiros[index].velocidadeTiro = (((float) (rand()%3))+8)/10;
	}
	else if(func == 5){ // PowerUp : Velocidade de Tiro Aumentada
		tiros[index].id = 43;
		tiros[index].local.altura = 32;
		tiros[index].local.largura = 32;
		tiros[index].ehEspecial = true;
		tiros[index].ehInimigo = true;
		tiros[index].velocidadeTiro = (((float) (rand()%3))+8)/10;
	}
	else if(func == 6){ // PowerUp : +1 de vida
		tiros[index].id = 43;
		tiros[index].local.altura = 32;
		tiros[index].local.largura = 32;
		tiros[index].ehEspecial = true;
		tiros[index].ehInimigo = true;
		tiros[index].velocidadeTiro = (((float) (rand()%3))+8)/10;
	}
}

// coloca o projétil do index dado em estado inicial
void estadoInicialTiro(int index){
	tiros[index].local.coordx = 0;
	tiros[index].local.coordy = 0;
	tiros[index].ativado = false;
	tiros[index].ehEspecial = false;
	tiros[index].variedade = 0;
	tiros[index].angulo = 0;
	espescificarTiro(index,0);
}

// inicializa todos os projéteis do jogador para uso futuro
void InicializarTiros(){
	for(int i=0;i<MaxTirosPorTela;i++){
		estadoInicialTiro(i);
	}
}

// inicializa o tiro própriamente
void atirarTiro(int x,int y,int func, float angulo){
	for(int i=0;i<MaxTirosPorTela;i++){
		// caso este tiro ainda não foi atirado, ele será atirado agora
		if(!(tiros[i].ativado)){
			tiros[i].local.coordx = x;
			tiros[i].local.coordy = y;
			tiros[i].ativado = true;
			tiros[i].variedade = func;
			tiros[i].angulo = angulo;
			espescificarTiro(i,func);
			break;
		}
	}
}

// O jogador atira um tiro, utilizando sua determinada arma equipada
void usarArma(int arma){
	float x = jogador.local.coordx;
	float y = jogador.local.coordy + (jogador.local.largura/2);
	if(arma == 0){
		atirarTiro(x, y, 1, 0);
	}
	else if(arma == 1){
		atirarTiro(x, y, 1, 0);
		atirarTiro(x, y, 1, (M_PI/6));
		atirarTiro(x, y, 1, -(M_PI/6));
	}
	else if(arma == 2){
		atirarTiro(x, y, 2, -(5*M_PI/6));
		atirarTiro(x, y, 2, -(5*M_PI/12));
		atirarTiro(x, y, 2, 0);
		atirarTiro(x, y, 2, (5*M_PI/12));
		atirarTiro(x, y, 2, (5*M_PI/6));
	}
}

// identifica colisão entre um objeto e um vertice
bool seVerticeColide(double x1, double y1, int larg1, int alt1, double x2, double y2){
	return (((x2)>=(x1-(larg1/2)))&&((x2)<=(x1+(larg1/2))))&&(((y2)>=(y1-(alt1/2)))&&((y2)<=(y1+(alt1/2))));
}

//identifica colisão entre um objeto 2 com um objeto 1. Funciona corretamente se a proporção do objeto 2 for menor que o objeto 1;
bool ahColisao(double x1, double y1, int larg1, int alt1, double x2, double y2, int larg2, int alt2){
	bool seVertice1Colide = (((x2+(larg2/2))>=(x1-(larg1/2)))&&((x2+(larg2/2))<=(x1+(larg1/2))))&&(((y2+(alt2/2))>=(y1-(alt1/2)))&&((y2+(alt2/2))<=(y1+(alt1/2))));
	bool seVertice2Colide = (((x2+(larg2/2))>=(x1-(larg1/2)))&&((x2+(larg2/2))<=(x1+(larg1/2))))&&(((y2-(alt2/2))>=(y1-(alt1/2)))&&((y2-(alt2/2))<=(y1+(alt1/2))));
	bool seVertice3Colide = (((x2-(larg2/2))>=(x1-(larg1/2)))&&((x2-(larg2/2))<=(x1+(larg1/2))))&&(((y2+(alt2/2))>=(y1-(alt1/2)))&&((y2+(alt2/2))<=(y1+(alt1/2))));
	bool seVertice4Colide = (((x2-(larg2/2))>=(x1-(larg1/2)))&&((x2-(larg2/2))<=(x1+(larg1/2))))&&(((y2-(alt2/2))>=(y1-(alt1/2)))&&((y2-(alt2/2))<=(y1+(alt1/2))));
	
	return ((seVertice1Colide)||(seVertice2Colide)||(seVertice3Colide)||(seVertice4Colide));
}

// Responsável por identificar a colisão entre o jogador e o tiro, modificando o estado do tiro, a vida do jogador, e o power-up do jogador
void detectarColisaoEntreTirosEJogador(int index){
	if((tiros[index].ativado)&&(tiros[index].ehInimigo)){
		if(tiros[index].local.coordy<-250){
			if(tiros[index].ativado){
				if(tiros[index].ehEspecial){ // A tag especial se refere se o tiro é um powerUp ou não. Este if remete a se ele é um powerUp
					if(ahColisao(jogador.local.coordx,jogador.local.coordy,jogador.local.largura,jogador.local.altura,tiros[index].local.coordx,tiros[index].local.coordy, tiros[index].local.largura, tiros[index].local.altura)){
						if(tiros[index].variedade != 6){
							jogador.timerPowerUp = 500; // Ativa o Timer dos Power Ups
						}
						if(tiros[index].variedade == 3){ // Invulnerabilidade
							jogador.invulneravel = true;
						}
						else if(tiros[index].variedade == 4){ // Tiro triplo
							menu.armaEquipada.variedade = 1;
						}
						else if(tiros[index].variedade == 5){ // Frequência do tiro dobrada
							menu.armaEquipada.frequenciaTiro /= 2;
						}
						else if(tiros[index].variedade == 6)
							if(jogador.vida<8)
								jogador.vida++;	
						estadoInicialTiro(index);
					}
				}
				else if(seVerticeColide(jogador.local.coordx,jogador.local.coordy,jogador.local.largura,jogador.local.altura,tiros[index].local.coordx,tiros[index].local.coordy-tiros[index].local.altura/2)){
					if(jogador.vida>0){
						if((jogador.timerDano==0)&&(!jogador.invulneravel)){
							jogador.vida--;
							jogador.timerDano = 100;
						}
						estadoInicialTiro(index);
					}
				}
			}
		}
	}
}

// Responsável pela colisão entre tiros e inimigos
void detectarColisaoEntreTirosEInimigos(int frotaIndex,int tirosIndex){
	if(!tiros[tirosIndex].ehEspecial){
		if((tiros[tirosIndex].ativado)&&(!(tiros[tirosIndex].ehInimigo))){
			for(int k=frotas[frotaIndex].firstTropa; k<(frotas[frotaIndex].firstTropa+frotas[frotaIndex].sizeofFrota); k++){
				if(tropas[k].ativado){								
					// teste para identificar se há colisão entre os tiros e as tropas(trata ambos tiros e tropas como retangulos)
					float xTropa = tropas[k].posReal.coordx;
					float yTropa = tropas[k].posReal.coordy;
					int largTropa = tropas[k].posReal.largura;
					int altTropa = tropas[k].posReal.altura;
							
					if(seVerticeColide(xTropa,yTropa,largTropa,altTropa,tiros[tirosIndex].local.coordx,tiros[tirosIndex].local.coordy+tiros[tirosIndex].local.altura/2)){
						if(tiros[tirosIndex].ativado){
							if(tropas[k].vida>0){
								estadoInicialTiro(tirosIndex);
								if(tropas[k].timer1==0)
									tropas[k].vida--;
								tropas[k].timer1 = 100;
							}
						}
					}
				}
			}
		}
	}
}

// Responsável pela colisão do jogador e a tropa
void detectarColisaoEntreTropasEJogador(int indexTropa){
	if(tropas[indexTropa].ativado){
		if(tropas[indexTropa].estado == 1){
			if(jogador.timerDano==0){
				if(tropas[indexTropa].posReal.coordy<-255){
					float xJogador = jogador.local.coordx;
					float yJogador = jogador.local.coordy;
					float largJogador = jogador.local.largura;
					float altJogador = jogador.local.altura;
					
					float xTropa = tropas[indexTropa].posReal.coordx;
					float yTropa = tropas[indexTropa].posReal.coordy;
					int largTropa = tropas[indexTropa].posReal.largura;
					int altTropa = tropas[indexTropa].posReal.altura;
					
					if(!jogador.invulneravel)	
						if(ahColisao(xJogador,yJogador,largJogador,altJogador,xTropa,yTropa,largTropa,altTropa)){
							jogador.vida--;
							jogador.timerDano = 50;
						}
				}		
			}
		}
	}
}

// Responsável pelo tiro dos inimigos
void inimigoAtirar(int index){
	if((tropas[index].variedade == 1)||(tropas[index].variedade == 2)||(tropas[index].variedade == 3)){
		double angulo = M_PI - atan((tropas[index].posReal.coordx-jogador.local.coordx)/(tropas[index].posReal.coordy-jogador.local.coordy));
		atirarTiro(tropas[index].posReal.coordx,tropas[index].posReal.coordy-tropas[index].posReal.altura/2,2,angulo);
	}
	if((tropas[index].variedade == 7)||(tropas[index].variedade == 8)||(tropas[index].variedade == 9)){
		atirarTiro(tropas[index].posReal.coordx,tropas[index].posReal.coordy-tropas[index].posReal.altura/2,2,M_PI);
	}
	if((tropas[index].variedade == 4)||(tropas[index].variedade == 5)||(tropas[index].variedade == 6)){
		double aux;
		if(tropas[index].posReal.coordx>=0)
			aux = ((frotas[tropas[index].frota].posLimiteMax)+tropas[index].posReal.coordx)/2;
		else 
			aux = -((frotas[tropas[index].frota].posLimiteMin)+tropas[index].posReal.coordx)/2;
		double angulo = M_PI - atan((tropas[index].posReal.coordx-aux)/(tropas[index].posReal.coordy-jogador.local.coordy));
		
		atirarTiro(tropas[index].posReal.coordx,tropas[index].posReal.coordy-tropas[index].posReal.altura/2,2,angulo);
	}
}
