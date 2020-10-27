
/*
	Uma tropa consiste de um inimigo, este que, sempre quando na tela, está incluso em uma frota, que controla o seu movimento em grupo
*/

// coloca a tag "ativado" de todas tropas para falso, para poder preenchê-las posteriormente
void inicializarFrotas(){
	for(int i=0;i<MaxFrotasPorTela;i++)
		frotas[i].ativado = false;
}

// coloca todas tropas em estado inativo para facilitar o uso futuro
void inicializarTropas(){
	for(int i=0;i<MaxTropasPorTela;i++){
		tropas[i].alistado = false;
		estadoInicialTropa(i);
	}		
}

// inicializa o estado da tropa. Estado se refere a esta tropa estar seguindo sua frota, avançando o jogador, ou retornando a sua frota
void inicializarEstado(int index){
	if(tropas[index].estado == -1){ // Tropa inativa
		tropas[index].posReal.coordx = 0;
		tropas[index].posReal.coordy = 0;
		tropas[index].posReal.largura = 0;
		tropas[index].posReal.altura = 0;
		tropas[index].deslocamentoIndividual = 0;
		tropas[index].angulo = 0;
	}
	if(tropas[index].estado == 0){ // Tropa em sua formação de frota
		tropas[index].posReal.coordx = tropas[index].posVirtual.coordx;
		tropas[index].posReal.coordy = tropas[index].posVirtual.coordy;
		tropas[index].posReal.largura = tropas[index].posVirtual.largura;
		tropas[index].posReal.altura = tropas[index].posVirtual.altura;
		tropas[index].deslocamentoIndividual = 0;
	}
	
	if(tropas[index].estado == 1){ // Tropa avançando o jogador
		tropas[index].posReal.coordx = tropas[index].posVirtual.coordx;
		tropas[index].posReal.coordy = tropas[index].posVirtual.coordy;
		tropas[index].posReal.largura = tropas[index].posVirtual.largura;
		tropas[index].posReal.altura = tropas[index].posVirtual.altura;

		tropas[index].angulo = (tropas[index].posReal.coordy-jogador.local.coordy)/(tropas[index].posReal.coordx-jogador.local.coordx);
		if((tropas[index].variedade == 1)||(tropas[index].variedade == 2)||(tropas[index].variedade == 3))
			tropas[index].deslocamentoIndividual = 10;
		else if((tropas[index].variedade == 4)||(tropas[index].variedade == 5)||(tropas[index].variedade == 6))
			tropas[index].deslocamentoIndividual = 10;
		else if((tropas[index].variedade == 7)||(tropas[index].variedade == 8)||(tropas[index].variedade == 9))
			tropas[index].deslocamentoIndividual = 10;
		else
			tropas[index].deslocamentoIndividual = 0;
	}
	if(tropas[index].estado == 2){ // Tropa retornando a sua frota por trás da tela
		tropas[index].posReal.coordx = tropas[index].posVirtual.coordx;
		tropas[index].posReal.coordy = 450+tropas[index].posVirtual.altura;
		tropas[index].posReal.largura = tropas[index].posVirtual.largura;
		tropas[index].posReal.altura = tropas[index].posVirtual.altura;
		tropas[index].deslocamentoIndividual = 20;
		tropas[index].angulo = 0;
	}
	if(tropas[index].estado == 3){ // Tropa retornando a sua frota pela esquerda da tela
		tropas[index].posReal.coordy = tropas[index].posVirtual.coordx;
		tropas[index].posReal.coordx = -900-tropas[index].posVirtual.largura;
		tropas[index].posReal.largura = tropas[index].posVirtual.largura;
		tropas[index].posReal.altura = tropas[index].posVirtual.altura;
		tropas[index].deslocamentoIndividual = 20;
		tropas[index].angulo = 0;
	}
	if(tropas[index].estado == 4){ // Tropa retornando a sua frota pela direita da tela
		tropas[index].posReal.coordy = tropas[index].posVirtual.coordx;
		tropas[index].posReal.coordx = 900+tropas[index].posVirtual.largura;
		tropas[index].posReal.largura = tropas[index].posVirtual.largura;
		tropas[index].posReal.altura = tropas[index].posVirtual.altura;
		tropas[index].deslocamentoIndividual = 20;
		tropas[index].angulo = 0;
	}
}

// modifica o estado da tropa (isto se refere a mecânica de avanço da tropa, seja seu ataque ou seu retorno a formação)
void modificarEstado(int valor, int index){
	tropas[index].estado = valor;
	inicializarEstado(index);
}

// coloca todos os indices da tropa em inicial, com exceção de seu alistamento, que será determinado somente a relacao a frota que alistou a tropa
void estadoInicialTropa(int index){
	tropas[index].ativado = false;
	tropas[index].variedade = 0;
	tropas[index].larguraTextura = 80;
	tropas[index].alturaTextura = 80;
	tropas[index].posVirtual.coordx = 0;
	tropas[index].posVirtual.coordy = -360;
	tropas[index].posVirtual.largura = 40;
	tropas[index].posVirtual.altura = 80;
	tropas[index].id = 37;
	
	espescificarTropas(index, 0);
	modificarEstado(-1, index);
	
	tropas[index].timer1 = 0;
	tropas[index].timer2 = 0;
}

// especifica as dimensões da tropa com base de sua função
void espescificarTropas(int index, int func){
	if(func==0) // Tropa inativa
		tropas[index].vida = 0;
	else if((func==1)||(func==2)||(func==3))
		tropas[index].vida = 1;
	else if((func==4)||(func==5)||(func==6))
		tropas[index].vida = 2;
	else if((func==7)||(func==8)||(func==9))
		tropas[index].vida = 3;

}

// determina os valores iniciais de uma tropa atacante, incluindo sua função
void alistarTropas(int index, double x, double y, int variedade, int frota){
	tropas[index].ativado = true;
	tropas[index].alistado = true;
	tropas[index].variedade = variedade;
	tropas[index].posVirtual.coordx = x;
	tropas[index].posVirtual.coordy = y;
	
	tropas[index].frota = frota;
	espescificarTropas(index, variedade);
	modificarEstado(0, index);
}

// preencher uma posiçao de frota, confere se é possivel criar esta frota. Se possível, alista todos inimigos necessarios e inicializa estes
void criarFrota(double x, double y, float largura,float altura,float fileira, float coluna, double deslocamentoHorizontal, double deslocamentoVertical, int frequenciaDeslocamento, int posMax, int posMin, int variedade,int spawnOffSet, int frequenciaDeTiro, int frequenciaDeAvanco){
	int i,j;
	for(i=0;i<MaxFrotasPorTela;i++)
		if(i<MaxFrotasPorTela)
			if(!(frotas[i].ativado))
				break;
	
	for(j=0;j<MaxTropasPorTela;j++){
		if((!(tropas[j].ativado))&&(!(tropas[j].alistado))){
			bool tropasLivres=true;
			for(int k=0;k<(fileira*coluna);k++){
				if((k+j)<(MaxTropasPorTela)){
					if(((tropas[k+j].ativado))||((tropas[k+j].alistado))){
						j+=(k-1);
						tropasLivres=false;
						break;
					}
				}
			}
			if(tropasLivres){
				frotas[i].firstTropa = j;
				break;
			}
		}
	}
	
	if((i<MaxFrotasPorTela)&&((j+(fileira*coluna))<MaxTropasPorTela)){
		frotas[i].ativado = true;
		frotas[i].borda.coordx = x;
		frotas[i].borda.coordy = y;
		frotas[i].borda.largura = largura;
		frotas[i].borda.altura = altura;
		frotas[i].deslocamentoHorizontal = deslocamentoHorizontal;
		frotas[i].deslocamentoVertical = deslocamentoVertical;
		frotas[i].tempoDeslocamento = frequenciaDeslocamento;
		frotas[i].sizeofFrota = fileira*coluna;
		frotas[i].tropasVivas = fileira*coluna;
		frotas[i].firstTropa = j;
		frotas[i].posLimiteMin = posMin;
		frotas[i].posLimiteMax = posMax;
		
		double xTropa, yTropa;
		for(int c=0;c<coluna;c++){
			for(int f=0;f<fileira;f++){
				if(fileira==1)
					xTropa = (float)(x) - largura/2;
				else
					xTropa = (largura*((float)(f)/(fileira-1)) + (float)(x) - largura/2);

				if(coluna==1)
					yTropa = (float)(y) - altura/2;
				else
					yTropa = (altura*((float)(c)/(coluna-1)) + (float)(y) - altura/2);
				
				alistarTropas(c*fileira+f+j,xTropa,yTropa,variedade,i);
				
				tropas[(int) (c*fileira+f+j)].estado = 2;
				tropas[(int) (c*fileira+f+j)].posReal.coordy = spawnOffSet;
				tropas[(int) (c*fileira+f+j)].deslocamentoIndividual = 10;
				tropas[(int) (c*fileira+f+j)].frequenciaDeTiro = frequenciaDeTiro;
				tropas[(int) (c*fileira+f+j)].frequenciaDeAvanco = frequenciaDeAvanco;
			}
		}
	}
}

// Invocam frotas em uma organização predeterminada
void armadas(int varTropas, int avanco, int velocidade){
	// x/y/larg/alt/fil/col/deslocH/deslocV/vel/limMAX/limMIN/var/offSet
	int armada = varTropas;
	if(armada>=15)
		armada = rand()%14+1;

	if(armada==1){
		criarFrota(0,100,300,200,4,3,1.5,avanco,velocidade,560,-560,1,400,16,24);	
	}

	if(armada==2){
		criarFrota(0,100,450,230,6,4,2,avanco, velocidade,560,-560,4,400,16,32);
	}
	if(armada==3){
		criarFrota(0,100,200,0,3,1,2,avanco, velocidade,360,-360,1,400,24,32);
		criarFrota(0,200,400,0,5,1,2,avanco, velocidade,460,-460,4,400,16,24);
		criarFrota(0,300,600,0,7,1,2,avanco, velocidade,560,-560,7,400,8,16);
	}
	if(armada==4){
		criarFrota(0,100,720,200,7,2,1.5,avanco, velocidade,560,-560,2,500,0,48);
		criarFrota(0,200,720,200,7,2,-1.5,avanco, velocidade,560,-560,3,500,0,48);
	}
	if(armada==5){
		criarFrota(-100,200,100,200,2,3,1,avanco, velocidade,360,-560,8,500,48,48);
		criarFrota(100,100,100,200,2,3,1,avanco, velocidade,560,-360,8,400,48,48);
	}
	if(armada==6){
		criarFrota(100,0,100,100,2,2,2,avanco, velocidade,460,-260,6,400,32,16);
		criarFrota(-100,0,100,100,2,2,2,avanco, velocidade,260,-460,5,400,32,16);

		criarFrota(150,250,200,200,3,3,2,avanco, velocidade,560,-260,6,400,32,16);
		criarFrota(-150,250,200,200,3,3,2,avanco, velocidade,260,-560,5,400,32,16);
	}
	if(armada==7){
		criarFrota(-280,100,100,400,2,5,1,avanco, velocidade,-40,-560,5,500,16,48);
		criarFrota(280,100,100,400,2,5,-1,avanco, velocidade,560,40,6,500,16,48);
	}
	if(armada==8){
		criarFrota(-280,150,200,300,3,4,2,avanco, velocidade,-40,-560,5,500,64,64);

		criarFrota(280,0,200,0,3,1,2,avanco, velocidade,560,40,3,500,0,24);
		criarFrota(280,100,200,0,3,1,-2,avanco, velocidade,560,40,3,500,0,24);
		criarFrota(280,200,200,0,3,1,2,avanco, velocidade,560,40,3,500,0,24);
		criarFrota(280,300,200,0,3,1,-2,avanco, velocidade,560,40,3,500,0,24);
	}
	if(armada==9){
		criarFrota(0,150,500,300,6,4,2,avanco, velocidade,560,-560,1,500,32,64);
		criarFrota(0,-100,400,0,5,1,2,avanco,velocidade,510,-510,7,400,64,0);
	}
	if(armada==10){
		criarFrota(0,0,200,0,3,1,2,avanco,velocidade,360,-360,1,400,0,0);
		criarFrota(0,80,400,0,5,1,2,avanco,velocidade,460,-460,1,480,48,0);
		criarFrota(0,160,600,0,7,1,2,avanco,velocidade,560,-560,1,560,96,0);
		criarFrota(0,240,400,0,5,1,2,avanco,velocidade,460,-460,1,640,48,0);
		criarFrota(0,320,200,0,3,1,2,avanco,velocidade,360,-360,1,720,0,0);
	}
	if(armada==11){
		criarFrota(-100,0,100,0,2,1,3,avanco,velocidade,360,-560,5,400,64,128);
		criarFrota(0,80,100,0,2,1,3,avanco,velocidade,460,-460,5,480,64,128);
		criarFrota(100,160,100,0,2,1,3,avanco,velocidade,560,-360,5,560,64,128);
		criarFrota(0,240,100,0,2,1,3,avanco,velocidade,460,-460,5,640,64,128);
		criarFrota(-100,320,100,0,2,1,3,avanco,velocidade,360,-560,5,720,64,128);
	}
	if(armada==12){
		criarFrota(0,0,80,0,2,1,3.9,avanco,velocidade,560,-560,9,400,32,64);
		criarFrota(0,100,160,0,3,1,3.6,avanco,velocidade,560,-560,9,400,32,64);
		criarFrota(0,200,240,0,4,1,3.3,avanco,velocidade,560,-560,9,400,32,64);
		criarFrota(0,300,320,0,5,1,3.0,avanco,velocidade,560,-560,9,400,32,64);
	}
	if(armada==13){
		criarFrota(0,0,400,0,5,1,3,avanco,velocidade,560,-560,6,400,16,24);
		criarFrota(0,80,300,0,4,1,2,avanco,velocidade,390,-390,6,480,16,24);
		criarFrota(0,160,200,0,3,1,1,avanco,velocidade,220,-220,6,560,16,24);
		criarFrota(0,240,300,0,4,1,2,avanco,velocidade,390,-390,6,640,16,24);
		criarFrota(0,320,400,0,5,1,3,avanco,velocidade,560,-560,6,720,16,24);
	}
	if(armada==14){
		criarFrota(0,300,0,0,1,1,2,avanco,velocidade,560,-560,3,700,128,128);

		criarFrota(0,0,640,0,8,1,1,avanco,velocidade,560,-560,7,400,0,8);
		criarFrota(0,100,640,0,8,1,-1,avanco,velocidade,560,-560,7,500,0,8);
		criarFrota(0,200,640,0,8,1,1,avanco,velocidade,560,-560,7,600,0,8);
	}
}

// Retorna 'true' se uma frota for derrotada, assim como modificar os valores das tropas alistadas a esta frota para que estas não estejam mais alistadas
bool seFrotaFoiDerrotada(int index){
	bool frotaDerrotada = false;
	if(frotas[index].tropasVivas == 0){
		frotaDerrotada = true;
	}
	
	if(frotaDerrotada){
		frotas[index].ativado = false;
		for(int j=frotas[index].firstTropa;j<(frotas[index].firstTropa+frotas[index].sizeofFrota);j++){
			tropas[j].alistado = false;
			tropas[j].frota = -1;
		}
		bool semFrotas = true;
		for(int k=0; k<MaxFrotasPorTela; k++)
			if(frotas[k].ativado){
				semFrotas = false;
				break;
			}
		if((semFrotas)&&(nextFrotaTimer==0)){
			nextFrotaTimer ++;
			if(faseAtual<='P')
				faseAtual++;
			if(faseAtual>fasesDesbloqueadas)
				fasesDesbloqueadas=faseAtual;
		}
	}
	return frotaDerrotada;
}


// desloca as tropas
void deslocarTropas(int frotaIndex){
	// responsavel pelo avanco frontal das frotas de inimigos
	if((frotas[frotaIndex].borda.coordx+frotas[frotaIndex].borda.largura/2>=frotas[frotaIndex].posLimiteMax)||(frotas[frotaIndex].borda.coordx-frotas[frotaIndex].borda.largura/2<=frotas[frotaIndex].posLimiteMin)){ // Identifica se a frota alcançou o seu limite máximo/mínimo
		frotas[frotaIndex].deslocamentoHorizontal*=(-1);
		frotas[frotaIndex].borda.coordy -= frotas[frotaIndex].deslocamentoVertical;
		for(int j=frotas[frotaIndex].firstTropa;j<(frotas[frotaIndex].firstTropa+frotas[frotaIndex].sizeofFrota);j++){
			if(tropas[j].ativado){
				tropas[j].posVirtual.coordy -=  frotas[frotaIndex].deslocamentoVertical;
				if((tropas[j].posVirtual.coordy<=jogador.local.coordy+jogador.local.largura/2)&&(tropas[j].estado==0))
					deathTimer=1;
			}
		}
	}
	
	// responsável pelo deslocamento lateral da frota de inimigos
	frotas[frotaIndex].borda.coordx += frotas[frotaIndex].deslocamentoHorizontal;
	for(int j=frotas[frotaIndex].firstTropa;j<(frotas[frotaIndex].firstTropa+frotas[frotaIndex].sizeofFrota);j++){
	
		if(tropas[j].ativado){
			tropas[j].posVirtual.coordx+=frotas[frotaIndex].deslocamentoHorizontal;

			if(tropas[j].estado == 0){
				tropas[j].posReal.coordx = tropas[j].posVirtual.coordx;
				tropas[j].posReal.coordy = tropas[j].posVirtual.coordy;
			}
			
			if(tropas[j].estado == 1){
				float xAux = (tropas[j].deslocamentoIndividual)/(sqrt(1+pow(tropas[j].angulo, 2)));
				float yAux = xAux * tropas[j].angulo;
				
				if(tropas[j].angulo<=0){
					tropas[j].posReal.coordx += xAux;
					tropas[j].posReal.coordy += yAux;
				}
				else{
					tropas[j].posReal.coordx -= xAux;
					tropas[j].posReal.coordy -= yAux;
				}
				
				if(tropas[j].posReal.coordy<=-320-tropas[j].posReal.altura){
					if((tropas[j].variedade == 1)||(tropas[j].variedade == 4)||(tropas[j].variedade == 7))
						modificarEstado(2, j);
					else if((tropas[j].variedade == 2)||(tropas[j].variedade == 5)||(tropas[j].variedade == 8))
						modificarEstado(3, j);
					else if((tropas[j].variedade == 3)||(tropas[j].variedade == 6)||(tropas[j].variedade == 9))
						modificarEstado(4, j);
				}
				
			}
			
			if(tropas[j].estado == 2){
				tropas[j].posReal.coordx = tropas[j].posVirtual.coordx;
				if((tropas[j].posReal.coordy-tropas[j].posVirtual.coordy)<=tropas[j].deslocamentoIndividual)
					modificarEstado(0, j);
				else
					tropas[j].posReal.coordy -= tropas[j].deslocamentoIndividual;
			}

			else if(tropas[j].estado == 3){
				tropas[j].posReal.coordy = tropas[j].posVirtual.coordy;
				if((tropas[j].posVirtual.coordx-tropas[j].posReal.coordx)<=tropas[j].deslocamentoIndividual)
					modificarEstado(0, j);
				else
					tropas[j].posReal.coordx += tropas[j].deslocamentoIndividual;
			}

			else if(tropas[j].estado == 4){
				tropas[j].posReal.coordy = tropas[j].posVirtual.coordy;
				if((tropas[j].posReal.coordx-tropas[j].posVirtual.coordx)<=tropas[j].deslocamentoIndividual)
					modificarEstado(0, j);
				else
					tropas[j].posReal.coordx -= tropas[j].deslocamentoIndividual;
			}
				
		}
	}
}

// Este evento aleatória controla o spawn de power-ups
void randomDeathEvent(int index){
	if(rand()%128<=4)
		atirarTiro(tropas[index].posReal.coordx, tropas[index].posReal.coordy, (rand()%4)+3, M_PI-atan(((float) (rand()%129-64))/128));
}

// Este evento aleatório se refere aos ataques das tropas
void randomEvent(int indexFrota){
	int index = (rand()%frotas[indexFrota].sizeofFrota)+frotas[indexFrota].firstTropa;
	if(tropas[index].estado == 0){
		int aux = rand()%256;
		if(aux<tropas[index].frequenciaDeTiro)
			tropas[index].timer2++;
		else if(aux<tropas[index].frequenciaDeAvanco)
			modificarEstado(1, index);
	}
	
}

// Desloca levemente o inimigo quando este é machucado
void shakeEnemy(int index){
	if(menu.enemyShakeUponDeath){
		if(tropas[index].timer1==48)
			tropas[index].posVirtual.coordx += 1;
		else if(tropas[index].timer1==40)
			tropas[index].posVirtual.coordx -= 1;
		else if(tropas[index].timer1==36)
			tropas[index].posVirtual.coordx -= 1;
		else if(tropas[index].timer1==28)
			tropas[index].posVirtual.coordx += 1;
		else if(tropas[index].timer1==24)
			tropas[index].posVirtual.coordx += 1;
		else if(tropas[index].timer1==18)
			tropas[index].posVirtual.coordx -= 1;
		else if(tropas[index].timer1==12)
			tropas[index].posVirtual.coordx -= 1;
		else if(tropas[index].timer1==6)
			tropas[index].posVirtual.coordx += 1;
	}
}
