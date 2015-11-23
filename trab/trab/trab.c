//#include <stdio.h>
//#include <stdlib.h>
//#include <stdint.h>
//#include <string.h>
//#include <libgen.h>
//
//#define DIR 0x04
//#define FAT_SIZE 1024
//#define CLUSTER_SIZE 4096
//#define DIR_SIZE 128
//
//
//uint32_t fat[FAT_SIZE]; 
//uint8_t block[CLUSTER_SIZE];
//FILE *fatPointer;
//
//typedef struct dir_entry{
//	uint8_t filename[16];
//	uint8_t attributes;
//	uint8_t reserved[7];
//	uint32_t first_block;
//	uint32_t size;
//} dir_entry;
//
//dir_entry dir[128];
//
//
//void initBlock(){
//	int i;
//	
//	printf("\nInicilizando o Boot Block com o valor 0xa5.\n");
//	for(i = 0; i < CLUSTER_SIZE; i++){
//		block[i] = 0xa5;
//	}
//	
//	fatPointer = fopen("fat.part","w+");
//	fseek(fatPointer, 0, SEEK_SET);
//	fwrite(&block, sizeof(block), 1, fatPointer);
//	fclose(fatPointer);	
//}
//
//void initFat(){
//	int i;
//
//	printf("Inicializando a FAT com o valor 0x00.\n");
//	fat[0] = -1;
//	fat[1] = -1;
//	fat[2] = -1;
//
//	for(i = 3; i < FAT_SIZE; i++){
//		fat[i] = 0;
//	}
//
//	fatPointer = fopen("fat.part","r+");
//	fseek(fatPointer, CLUSTER_SIZE, SEEK_SET);
//	fwrite(&fat, sizeof(fat), 1, fatPointer);
//	fclose(fatPointer);
//}
//
//void initDir(){
//	int i;
//	printf("Inicializando a estrutura dir com o valor 0x00.\n\n");
//	for(i = 0; i < DIR_SIZE; i++){
//		dir[i].filename[i] = 0x00;
//		dir[i].reserved[i] = 0x00;
//		dir[i].attributes = 0x00;
//		dir[i].first_block = 0x00;
//		dir[i].size = 0x00;
//	}
//
//	fatPointer = fopen("fat.part","r+");
//	fseek(fatPointer, 2*CLUSTER_SIZE, SEEK_SET);
//	fwrite(&dir, sizeof(dir), 1, fatPointer);
//	fclose(fatPointer);
//}
//
//int isDir(int attr){
//	return (attr&DIR) == DIR;
//}
//
//
//void initDataDir(){
//	int i;
//	//dir_entry cluster[128];
//
//	fatPointer = fopen("fat.part","r+");
//	fseek(fatPointer, 3*CLUSTER_SIZE, SEEK_SET);
//	
//	for(i = 0; i < 1021; i++){
//		fwrite(&dir, sizeof(dir), 1, fatPointer);
//	}
//	
//	fclose(fatPointer);
//	
//}
//
//void init(){
//	initBlock();
//	initFat();
//	initDir();
//	initDataDir();
//}
//
//int load(){
//	fatPointer = fopen("fat.part","r+");
//	
//	if(!fatPointer){
//		printf("Unable to open file!");
//		return 1;
//	}
//		
//	//Posiciona o ponteiro no inicio da fat e carrega ela pra memória
//	fseek(fatPointer, CLUSTER_SIZE, SEEK_SET);
//	fread(&fat, sizeof(fat), 1, fatPointer);	
//
//	//Posiciona o ponteiro no inicio do root dir e carrega ele pra memória
//	fseek(fatPointer, 2*CLUSTER_SIZE, SEEK_SET);
//	fread(&dir, sizeof(dir), 1, fatPointer);	
//	
//	printf("System file has been loaded!\n");
//	printf("Fat and root dir in memory!\n");
//
//	return 0;
//}
//
//void clearDir(){
//	int i, j;
//
//	for(i = 0; i < 127; i++){
//		for(j = 0; j < 15; j++) dir[i].filename[j] = 0x00;
//		dir[i].reserved[i] = 0x00;
//		dir[i].attributes = 0x00;
//		dir[i].first_block = 0x00;
//		dir[i].size = 0x00;
//	}
//}
//
//void clearBlock(){
//	int i, j;
//
//	for(i = 0; i < CLUSTER_SIZE; i++){
//		block[i] = 0x00;
//	}
//}
//
//
//
//int testFilename(char *name){
//	int i;
//	
//	for(i = 0; i < 127; i++){
//		if(strcmp(dir[i].filename, name) == 0) return 1;
//	}
//
//	return 0;
//}
//
//void create(uint8_t name[16]){
//	int i, j, index;	
//	int erro = 0;
//	
//	//Percorrer a fat para encontar um espaço livre e setar como preenchida
//	for(i = 3; i < 1024; i++){
//		if(fat[i] == 0){
//			index = i;
//			fat[i] = -1;
//			break;	
//		}
//	}
//	
//	fseek(fatPointer, CLUSTER_SIZE, SEEK_SET);
//	fwrite(&fat, sizeof(fat), 1, fatPointer);
//	
//	
//	//Carrega o diretório
//	fseek(fatPointer, 2*CLUSTER_SIZE, SEEK_SET);
//	fread(&dir, sizeof(dir), 1, fatPointer);
//
//
//	for(i = 0; i < 127; i++){
//		if(!testFilename(name)){
//			if(strlen(dir[i].filename) == 0){
//				strcpy((char *)dir[i].filename, (char *)name);
//		
//				dir[i].reserved[i] = 0x00;
//				dir[i].attributes = 0x00;
//				dir[i].first_block = index;
//				dir[i].size = CLUSTER_SIZE;
//
//				fseek(fatPointer, 2*CLUSTER_SIZE, SEEK_SET);
//				fwrite(&dir, sizeof(dir), 1, fatPointer);
//				break;
//			}
//		}
//		else{
//			printf("Erro: arquivo '%s' já existe.\n", name);
//			erro = 1;
//			break;		
//		}		
//	}	
//	
//	if(!erro){
//		clearDir();	
//		//fseek(fatPointer, index*CLUSTER_SIZE, SEEK_SET);
//		//fwrite(&dir, sizeof(dir), 1, fatPointer);
//		printf("Criado o arquivo no sistema de arquivos\n");
//		
//	}
//}
//
//
//
//void mkdir(uint8_t name[16]){
//	int i, j, index, indexAux;	
//	int erro = 0;
//	
//	//Percorrer a fat para encontar um espaço livre e setar como preenchida
//	for(i = 3; i < 1024; i++){
//		if(fat[i] == 0){
//			index = i;
//			fat[i] = -1;
//			break;	
//		}
//	}
//	
//	fseek(fatPointer, CLUSTER_SIZE, SEEK_SET);
//	fwrite(&fat, sizeof(fat), 1, fatPointer);
//	
//	
//	//Carrega o diretório
//	fseek(fatPointer, 2*CLUSTER_SIZE, SEEK_SET);
//	fread(&dir, sizeof(dir), 1, fatPointer);
//
//
//	for(i = 0; i < 127; i++){
//		
//		if(!testFilename(name)){
//			if(strlen(dir[i].filename) == 0){
//				//Encontrou um diretorio livre
//			
//				strcpy((char *)dir[i].filename, (char *)name);
//		
//				dir[i].reserved[i] = 0x00;
//				dir[i].attributes = DIR;
//				dir[i].first_block = index;
//				dir[i].size = CLUSTER_SIZE;
//
//				fseek(fatPointer, 2*CLUSTER_SIZE, SEEK_SET);
//				fwrite(&dir, sizeof(dir), 1, fatPointer);
//				indexAux = i;
//				break;
//			}
//		}
//		else{
//			printf("Erro: diretório '%s' já existe.\n", name);
//			erro = 1;
//			break;		
//		}		
//	}	
//	
//	if(!erro){
//		clearDir();	
//		fseek(fatPointer, index*CLUSTER_SIZE, SEEK_SET);
//		fwrite(&dir, sizeof(dir), 1, fatPointer);
//		printf("Criado o diretório no sistema de arquivos\n");
//		
//	}
//}
//
//void rmdir(uint8_t name[16]){
//	int i, j;
//	long size;
//	
//	
//	printf("\nComando RMDIR acionado: \n");
//
//
//	//Recarrega o dir
//	fseek(fatPointer, 2*CLUSTER_SIZE, SEEK_SET);
//	fread(&dir, sizeof(dir), 1, fatPointer);	
//
//
//	for(i = 0; i < 3; i++){
//		if(dir[i].first_block > 0){
//			//Encontrou um diretorio preenchido
//			if(strcmp(dir[i].filename, name) == 0){
//				printf("Encontrou o diretório %s id: %d\n", name, i);
//				if(isDir(dir[i].attributes)){
//					//Atualiza a fat
//					fat[dir[i].first_block] = 0;
//					//Salva a fat no arquivo
//					fseek(fatPointer, CLUSTER_SIZE, SEEK_SET);
//					fwrite(&fat, sizeof(fat), 1, fatPointer);				
//								
//					//Zera estrutura(remove)
//					for(j = 0; j < 15; j++) dir[i].filename[j] = 0x00;
//					dir[i].reserved[i] = 0x00;
//					dir[i].attributes = 0x84; 
//					dir[i].first_block = 0x00;
//					dir[i].size = 0x00;
//			
//					printf("Removendo o diretório %s...\n", name);
//					fseek(fatPointer, 2*CLUSTER_SIZE, SEEK_SET);
//					fwrite(&dir, sizeof(dir), 1, fatPointer);
//					printf("Diretório %s removido.\n", name);
//					break;		
//				}
//				else{
//					printf("Impossível remover. %s não é um diretório. Por favor, use o comando 'rm filename'. \n", name);
//				}	
//			}
//		}
//	}	
//}
//
//void rm(uint8_t name[16]){
//	int i, j;
//	long size;
//	
//	
//	printf("\nComando RM acionado: \n");
//
//
//	//Recarrega o dir
//	fseek(fatPointer, 2*CLUSTER_SIZE, SEEK_SET);
//	fread(&dir, sizeof(dir), 1, fatPointer);	
//
//	for(i = 0; i < 3; i++){
//		if(dir[i].first_block > 0){
//			//Encontrou um diretorio preenchido
//			if(strcmp(dir[i].filename, name) == 0){
//				printf("Encontrou o arquivo %s.\n", name);
//				
//				if(!isDir(dir[i].attributes)){
//					//Atualiza a fat
//					fat[dir[i].first_block] = 0;
//					//Salva a fat no arquivo
//					fseek(fatPointer, CLUSTER_SIZE, SEEK_SET);
//					fwrite(&fat, sizeof(fat), 1, fatPointer);				
//								
//					//Zera estrutura(remove)
//					for(j = 0; j < 15; j++) dir[i].filename[j] = 0x00;
//					dir[i].reserved[i] = 0x00;
//					dir[i].attributes = 0x84; 
//					dir[i].first_block = 0x00;
//					dir[i].size = 0x00;
//			
//					printf("Removendo o diretório %s...\n", name);
//					fseek(fatPointer, 2*CLUSTER_SIZE, SEEK_SET);
//					fwrite(&dir, sizeof(dir), 1, fatPointer);
//					printf("Diretório %s removido.\n", name);
//					break;			
//				}
//				else{
//					printf("Impossível remover. %s  não é um arquivo. Por favor, use o comando 'rmdir dirname'. \n", name);
//				}
//			}
//		}
//	}	
//}
//
//void printRootDir(){
//	int i;	
//	for(i = 0;  i < 128; i++){
//		printf("Filename: %s\n", dir[i].filename);	
//	}
//}
//
//void printFat(){
//	int i;	
//	for(i = 3;  i < 10; i++){
//		printf("Fat value: %d\n", fat[i]);	
//	}
//}
//
//
//void ls(){
//	int i;
//	//Recarrega o dir
//	fseek(fatPointer, 2*CLUSTER_SIZE, SEEK_SET);
//	fread(&dir, sizeof(dir), 1, fatPointer);	
//	
//	for(i = 0; i < 127; i++){
//		if(strlen(dir[i].filename) > 0){
//			if(isDir(dir[i].attributes)) printf("Dir: %s | ", dir[i].filename);	
//			else printf("File: %s | ", dir[i].filename);	
//		}
//	}
//}
//
//
//void cat(uint8_t name[16]){
//	int i;
//	clearBlock();
//
//	//Recarrega o dir
//	fseek(fatPointer, 2*CLUSTER_SIZE, SEEK_SET);
//	fread(&dir, sizeof(dir), 1, fatPointer);	
//	
//	for(i = 0; i < 127; i++){
//		if(testFilename(name)){
//			//if(!isDir(dir[i].attributes)){
//				fseek(fatPointer, dir[i].first_block*CLUSTER_SIZE, SEEK_SET);
//				fread(&block, sizeof(block), 1, fatPointer);
//
//				printf("Conteúdo do arquivo %s: \n", name);
//				printf("%s\n", block);			
//	
//				break;			
//			//}
//		}
//	}
//
//
//
//}
//
//void write(char *data, uint8_t name[16]){
//	int i, j, erro;
//	
//	clearBlock();
//
//	//Recarrega o dir
//	fseek(fatPointer, 2*CLUSTER_SIZE, SEEK_SET);
//	fread(&dir, sizeof(dir), 1, fatPointer);	
//
//	
//	for(i = 0; i < 127; i++){
//		if(testFilename(name)){
//			//Se for arquivo	
//			//if(!isDir(dir[i].attributes)){
//				
//				for(j = 0; j < strlen(data); j++){
//					block[j] = data[j];
//				}				
//				
//				fseek(fatPointer, dir[i].first_block*CLUSTER_SIZE, SEEK_SET);
//				fwrite(&block, sizeof(block), 1, fatPointer);
//				break;
//			//}
//			//else{
//				//printf("Erro. %s não é um arquivo.\n", name);
//				//break;			
//			//	}
//		}
//		else{
//			printf("Arquivo %s não foi encontrado. Use 'create filename' para criar um arquivo!\n", name);
//			break;
//		}
//	}
//
//}
//
//
//
//void help(){
//	printf("\n#################################################################\n");
//	printf("Utilize os comandos abaixo para manipular o sistema de arquivos\n\n");
//	printf("init  - Inicializa o sistema de arquivos. Ele já vem formatado\n");
//	printf("load  - Carrega o sistema de arquivos\n");
//	printf("mkdir /dirname - Cria um diretório com o nome 'dirname'\n");
//	printf("rmdir /dirname - Remove um diretório com o nome 'dirname'\n");
//	printf("create filename - Cria um arquivo com o nome 'filename'\n");
//	printf("rm filename - Remove um arquivo com o nome 'filename'\n");
//	printf("ls - Lista o(s) diretórios/arquivos do diretório atual\n");
//	printf("hexdump - Comando mostra o arquivo binário. Serve para debug\n");
//	printf("quit - Encerra o shell\n");
//	printf("#################################################################\n");
//}
//
//
//
//void getCmd(char *line){
//	int match = 0;
//	char *cmd, *token, *dataFile;
//	token = strtok(line,"   ");
//	cmd = token;
//	
//	if(strcmp(cmd, "write") == 0){
//		dataFile = strtok(NULL, "   ");
//		token = strtok(NULL, "   ");
//	}
//	else token = strtok(NULL, "   ");
//	
//
//	if(strcmp(cmd, "mkdir") == 0){
//		mkdir((char *)token);
//	}	
//	else if(strcmp(cmd, "rmdir") == 0){
//		rmdir(token);
//	}
//	else if(strcmp(cmd, "ls") == 0){
//		ls();	
//	}
//	else if(strcmp(cmd, "load") == 0){
//		load();
//	}
//	else if(strcmp(cmd, "init") == 0){
//		init();	
//	}
//	else if(strcmp(cmd, "create") == 0){
//		create((char *)token);	
//	}
//	else if(strcmp(cmd, "rm") == 0){
//		rm((char *)token);	
//	}
//	else if(strcmp(cmd, "help") == 0){
//		help();
//	}
//	else if(strcmp(cmd, "write") == 0){
//		write(dataFile, token);	
//	}
//	else if(strcmp(cmd, "cat") == 0){
//		cat(token);
//	}
//	else if(strcmp(cmd, "hexdump") == 0){
//		system("clear");
//		system("hexdump -C fat.part");		
//	}
//}
//
//void shell(){
//	char str[1000];
//	do{
//		printf("\n");		
//		printf("bash$: ");
//		scanf("%[^\n]s", str);
//		setbuf(stdin, NULL);
//		
//
//		getCmd(str);
//	}
//    while (strcmp(str,"quit") != 0);
//	
//}
//
//
//
//
//
//
//int main(){
//	printf("\nBash carregado com sucesso.");	
//	printf("\nAntes de qualquer coisa digite 'help' no bash.\n");	
//	shell();
//	
//	/*char *path = "/lorenzo/italo/newfolder";
//	path = strdup(path);
//	char *pathBase = basename(path);
//	char *pathDirectory = dirname(path);
//	printf("%s , %s\n",pathDirectory,pathBase);*/
//	return 0;
//}	
