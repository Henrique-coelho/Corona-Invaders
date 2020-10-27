
void mostrarBorda(bool x){
	menuInterno.mostrarFrota = x;
}

void configuracoesIniciais(int arma){
	
	menu.enemyShakeUponDeath = true;
	if(arma == 1){
		menu.armaEquipada.variedade = 0;
		menu.armaEquipada.frequenciaTiro = 128;
	}

	// As configurações seguintes não serão interagidas com o usuário

	menuInterno.mostrarFrota = false;
}


