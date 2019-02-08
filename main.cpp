
#include <sys/stat.h>
#include <iostream>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <ctime>

using namespace std;

const int TMem = 200;//Memoria de alocação para as váriaveis e ponteiros auxiliares
const int Segundos = 60;//Contador de tempo
bool habCont = true;
bool habData = true;

DIR *dirA, *dirB;//Abre os diretorios

struct dirent *ent;//Cria um objeto com as informações do diretório
struct stat estado;

//Função de ajuda-------------------------------------------------------------------------------------------------------------------
void funcHelp(char* word){
    if(strcmp(word,"-h") == 0){
        cout << "--------Intructions--------\n\n";
        
		cout << "System made as an evaluation test for company Thales Group." << endl;
		cout <<"The system consists of copying a file independent of its extension through the following" << endl;
		cout <<"terminal commands." << endl;


		cout <<"./ThalesTest -h //Displays the system help menu\n" << endl;
		cout <<"./ThalesTest  <Directory A> <Directory B> <Keyword>\n" << endl;

		cout <<"Searches the files in the directory where I have the keyword as substring copies the" << endl;
		cout <<"files obduting the descending order date of manipulation, from the oldest to the oldest" << endl;
		cout <<"and making your copy every 60 seconds.\n" << endl;

		cout <<"./Thales <opt> <Directory A> <Directory B> <keyword>\n" << endl;

		cout <<"Searches the files in the directory where I have the keyword as substring copies the" << endl;
		cout <<"files by obferencing the options typed in the <opc> options switch which are as follows:" << endl;
		cout <<"-c:		Transfers A to B without counting time." << endl;
		cout <<"-d:		Transfers from A to B without sorting by date." << endl;
		cout <<"-dc:	Transforms from A to B without counting time and sorting by date.\n" << endl;

		cout <<"To compile the code in Ubuntu type:\n" << endl;

		cout <<":g++ main.cpp -o ThalesTest\n" << endl;

    }else{
        cout << "Type -h for help!\n";
    }
}
//Função de Transferência-----------------------------------------------------------------------------------------------------------
void funcTransfer(char *pathA, char* pathB, char* arq){//Função Responsável pela transferência
	char arqcomp[TMem];
	char arqcopy[TMem];
	
	cout << "\n ------Saving------\n";
	//Copia endereço do arquivo em A
	strcpy(arqcomp,pathA);
	strcat(arqcomp,arq);
	int ierr =  stat(arqcomp, &estado);
	cout << arqcomp << "-|-  date of the last modification: "<< ctime(&estado.st_mtime);
	
	
	//Copia endereço do arquivo em B
	strcpy(arqcopy,pathB);
	strcat(arqcopy,arq);
	//cout << arqcopy << endl;
	
	ifstream src(arqcomp, ios::binary);//Abre o arquivo em A
	ofstream dest(arqcopy, ios::binary);//Escreve o arquivo em B
	dest << src.rdbuf();//Transferencia de dados binarios
    cout << arq << " copied to " << pathB << endl;
    src.close();//fecha o arquivo A
    dest.close();//fecha o arquivo B
    cout << "\n------End of Save------\n" << endl;
}

//Função Seleção de Arquivo Simples------------------------------------------------------------------------------------------------
void funcSelecaoSimples(char *pathA, char* pathB, string word){
    string arq; 
    while((ent = readdir(dirA)) != NULL){//Continua Verificando os arquivos enquanto não chegar ao fim do diretório
        arq = ent->d_name;
        
    	//verifica se existe a palavra da pesquisa no nome do arquivo
		if((arq.find(word) != string::npos))
    		funcTransfer(pathA, pathB, ent->d_name);//Funcão que copia os arquivos requisitados
	}
	
	cout << "End of directory!!!\n";
}

//Função Seleção de Arquivo com Contador sem Datade Modificação---------------------------------------------------------------------
void funcSelecContSemData(char *pathA, char* pathB, string word){
	bool checked = false;
    int count = 0;
    string arq;
    bool estouro = false;
    
    clock_t begin;
	clock_t end;
    while(!estouro){//Continua Verificando os arquivos enquanto não chegar ao fim do diretório
    	ent = readdir(dirA);//vai para o próximo arquivo no diretório A
        checked = false;
        arq = ent->d_name;
        count = 0;
        begin = clock();//inicio da contagem do tempo
        end = clock(); //fim da contagem do tempo
        while((end-begin)/CLOCKS_PER_SEC < Segundos){
        	
        	if(count != (end-begin)/CLOCKS_PER_SEC){	
            	//cout << ent->d_name << (end-begin)/CLOCKS_PER_SEC << " sec---\t";
            	cout << "................" << (end-begin)/CLOCKS_PER_SEC << " sec---\n";
            	count = (end-begin)/CLOCKS_PER_SEC;
            }
        					
        	end = clock();
        	
        	//verifica se existe a palavra da pesquisa no nome do arquivo
			if((arq.find(word) != string::npos) && !checked)
				checked = true;	
        	//vai para o proximo arquivo até chegar ao fim do diretório
        	if(!checked){
        		 if((ent = readdir(dirA)) != NULL)
        		 	arq = ent->d_name;
        		 else{ 
        		 	checked =true;
        		 	estouro = true;
        		 }
        	}
        	
        }
        
        if(!estouro){
    		funcTransfer(pathA, pathB, ent->d_name);//Funcão que copia os arquivos requisitados
    	}else{
        	cout << "End of directory!!!\n";
		}
	}
}

//Função Seleção de Arquivo sem Contador com Datade Modificação---------------------------------------------------------------------
void funcSelecDataSemCont(char *pathA, char* pathB, string word){
    string arq;
    bool estouro = false;
    char bigAux[TMem];
    char arqescolhido[40];
    int base = 0;
    
    int horario = -99999;
	ent = readdir(dirA);//vai para o próximo arquivo no diretório A
    arq = ent->d_name;
    while(!estouro){//Continua Verificando os arquivos enquanto não chegar ao fim do diretório
        	
    	//verifica se existe a palavra da pesquisa no nome do arquivo
		if((arq.find(word) != string::npos) && !estouro){
			strcpy(bigAux,pathA);
			strcat(bigAux, ent->d_name);
			
			int ierr =  stat(bigAux, &estado);//pega a informações do arquivo no diretório A
			
			//If var base verifica se o arquivo manipulado  mais recente já foi escolhido
			if(base==0){
				//Se não foi escolhido
				if((horario == -99999)){
					horario = estado.st_mtime;
					strcpy(arqescolhido, ent->d_name);
				//Escolhe um arquivo mais novo que o horario
				}else if(estado.st_mtime > horario){
					horario = estado.st_mtime;
					strcpy(arqescolhido, ent->d_name);
				}
			}else{
				//Se foi escolhido
				if((horario == -99999)&& (estado.st_mtime < base)){
					horario = estado.st_mtime;
					strcpy(arqescolhido, ent->d_name);
				//Escolhe um arquivo mais antigo que o base e mais novo que o horario
				}else if((estado.st_mtime > horario) && (estado.st_mtime < base)){
					horario = estado.st_mtime;
					strcpy(arqescolhido, ent->d_name);
				}
			}
		}
			
    	//vai para o proximo arquivo até chegar ao fim do diretório
    	if(!estouro){
    		 if((ent = readdir(dirA)) != NULL){
    		 	arq = ent->d_name;
    		 }
    		 else{
    		 	base = horario;
    		 	estouro = true;
    		 }
    	}
        
        //caso base seja diferente de zero e horario não saía do -99999 significa que todos arquivos possíveis já foram checados        
        if((base != 0) &&  (horario !=- 99999)){
        //Grava o arquivo atual e renova as váriaveis para uma nova varredura
        	horario = -99999;
        	estouro = false;
        	rewinddir(dirA);
        	cout << "Escolhido: "<< arqescolhido << endl; 
    		funcTransfer(pathA, pathB, arqescolhido);//Funcão que copia os arquivos requisitados
    	}else{
        	cout << "End of directory!!!\n";
    	}
    }
}

//Função Seleção de Arquivo Completa------------------------------------------------------------------------------------------------
void funcSelecComp(char *pathA, char* pathB, string word){
    int count = 0;
    string arq;
    bool estouro = false;
    char bigAux[TMem];
    char arqescolhido[40];
    int base = 0;
    
    clock_t begin;
    clock_t end;
    int horario = -99999;
	ent = readdir(dirA);//vai para o próximo arquivo no diretório A
    arq = ent->d_name;
    while(!estouro){//Continua Verificando os arquivos enquanto não chegar ao fim do diretório
        count = 0;
        begin = clock();//inicio da contagem do tempo
        end = clock(); //fim da contagem do tempo
        while((end-begin)/CLOCKS_PER_SEC < Segundos){
        	
        	if(count != (end-begin)/CLOCKS_PER_SEC){	
            	//cout << ent->d_name << (end-begin)/CLOCKS_PER_SEC << " sec---\t";
            	cout << "................" << (end-begin)/CLOCKS_PER_SEC << " sec---\n";
            	count = (end-begin)/CLOCKS_PER_SEC;
            }
        					
        	end = clock();
        	
        	//verifica se existe a palavra da pesquisa no nome do arquivo
			if((arq.find(word) != string::npos) && !estouro){
				strcpy(bigAux,pathA);
				strcat(bigAux, ent->d_name);
				
				int ierr =  stat(bigAux, &estado);//pega a informações do arquivo no diretório A
				
				//If var base verifica se o arquivo manipulado  mais recente já foi escolhido					
				if(base==0){
					//Se não foi escolhido
					if((horario == -99999)){
						horario = estado.st_mtime;
						strcpy(arqescolhido, ent->d_name);
					//Escolhe um arquivo mais novo que o horario
					}else if(estado.st_mtime > horario){
						horario = estado.st_mtime;
						strcpy(arqescolhido, ent->d_name);
					}
				}else{
					//Se foi escolhido
					if((horario == -99999)&& (estado.st_mtime < base)){
						horario = estado.st_mtime;
						strcpy(arqescolhido, ent->d_name);
					//Escolhe um arquivo mais antigo que o base e mais novo que o horario
					}else if((estado.st_mtime > horario) && (estado.st_mtime < base)){
						horario = estado.st_mtime;
						strcpy(arqescolhido, ent->d_name);
					}
				}
			}
			
        	//vai para o proximo arquivo até chegar ao fim do diretório
        	if(!estouro){
        		 if((ent = readdir(dirA)) != NULL)
        		 	arq = ent->d_name;
        		 else{
        		 	estouro = true;
        		 	base = horario;//aponta o ultimo escolhido como o maior atual
        		 }
        	}
        	
        }
        
        //caso base seja diferente de zero e horario não saía do -99999 significa que todos arquivos possíveis já foram checados        
        if((base != 0) &&  (horario !=- 99999)){
        //Grava o arquivo atual e renova as váriaveis para uma nova varredura
        	horario = -99999;
        	estouro = false;
        	rewinddir(dirA);
        	begin = clock();
        	cout << "Escolhido: "<< arqescolhido << endl; 
    		funcTransfer(pathA, pathB, arqescolhido);//Funcão que copia os arquivos requisitados
    	}else{
        	cout << "End of directory!!!\n";
    	}
    }
}

//Função de Alocação e Verificação de diretório-------------------------------------------------------------------------------------
int funcAlocDirect(char *pathA, char* pathB, string word){
    if((dirA = opendir(pathA))!=NULL && (dirB = opendir(pathB))!=NULL){//Verifica se os diretorios existem
    	
    	//Adiciona / se não houver nos diretorios digitados
    	string aux(pathA);
    	int num = aux.rfind("/");
    	if((aux.size()-1) != num){
        	strcat(pathA,"/");
        }
    	
    	aux = pathB;
    	num = aux.rfind("/");
    	if((aux.size()-1) != num)
        	strcat(pathB,"/");
        	
        
        if(habCont && habData){
        	funcSelecComp(pathA, pathB, word);
        }else if(habCont && !habData){
        	funcSelecContSemData(pathA, pathB, word);
        }else if(!habCont && habData){
        	funcSelecDataSemCont(pathA, pathB, word);
        }else{
        	funcSelecaoSimples(pathA, pathB, word);
        }
        
    }else{
        return -1;
    }

    delete ent;
    closedir(dirA);
    closedir(dirB);
    return 0;
}
//Função Main------------------------------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{	
	char pathA[TMem], pathB[TMem];//variaveis que vão receber o endereço dos diretórios
	
    if (argc == 2){//help option
        funcHelp(argv[1]);
        
    }else if(argc == 4){//execution function
    	strcpy(pathA,argv[1]);
    	strcpy(pathB,argv[2]);
    	
        if(funcAlocDirect(pathA, pathB, argv[3]) == 0)
            cout << "Copy made sucessfully!!!\n";
        else
            cout << "The path is nonexistent!!!\n";
    
    }else if(argc == 5){//execution function
    	strcpy(pathA,argv[2]);
    	strcpy(pathB,argv[3]);
    	
    	if(strcmp(argv[1],"-c")==0){
    		habCont =false;
    	}else if(strcmp(argv[1],"-d")==0){
    		habData =false;
    	}else if(strcmp(argv[1],"-dc")==0){
    		habCont =false;
    		habData =false;
    	}else{
    		cout << "Invalid option!!!" <<endl;
    		return 0;
    	}
    	
        if(funcAlocDirect(pathA, pathB, argv[4]) == 0)
            cout << "Copy made sucessfully!!!\n";
        else
            cout << "The path is nonexistent!!!\n";
        
    }else{
        cout << "Wrong number of arguments!!!\n";
    }
    
    return 0;
}
