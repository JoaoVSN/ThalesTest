#include <exception>
#include <iostream>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <ctime>

using namespace std;
const int TMem = 200;//Memoria de alocação para as váriaveis e pponteiros auxiliares
const int Segundos = 2;//Contador de tempo

DIR *dirA, *dirB;//Abre os diretorios

struct dirent *ent;//Cria um objeto com as informações do diretório

void funcHelp(char* word){
    if(strcmp(word,"-h") == 0){
        cout << "--------Intructions--------\n";
        cout << "This is a system made for avaliation\n";
        cout << "For correct work of system type on terminal the command lines below\n";
        cout << "Help: ./ThalesTest -h\n";
        cout << "For work: ./ThalesTest <A=path  of directory source> <B=path of directory destiny> <C=piece of the file name>\n";
        cout << "---------'The progam will make a copie searching the piece of the string written at the C variable.'\n";
        cout << "---------'The progam will copy all the files with the group of letters em your name and would copy'\n";
        cout << "---------'of files from directory 'A' to directory 'B'.\n";
    }else{
        cout << "Type -h for help!\n";
    }
}

void funcTransfer(char *pathA, char* pathB, char* arq){//Função Responsável pela transferência
	char arqcomp[TMem];
	char arqcopy[TMem];
	
	//Copia endereço do arquivo em A
	strcpy(arqcomp,pathA);
	strcat(arqcomp,arq);
	cout << arqcomp << endl;
	
	
	//Copia endereço do arquivo em B
	strcpy(arqcopy,pathB);
	strcat(arqcopy,arq);
	cout << arqcopy << endl;
	
	ifstream src(arqcomp, ios::binary);//Abre o arquivo em A
	ofstream dest(arqcopy, ios::binary);//Escreve o arquivo em B
	dest << src.rdbuf();//Transferencia de dados binarios
    cout << arq << " copied to " << pathB << endl;
    src.close();//fecha o arquivo A
    dest.close();//fecha o arquivo B
}

int funcAlocDirect(char *pathA, char* pathB, string word){
	bool checked = false;
    int count = 0;
    string copiado, arq;
    bool estouro = false;
    
    clock_t begin;
    clock_t end;
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
				if((arq.find(word) != string::npos) && (arq.compare(copiado) != 0) && !checked)
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
            	copiado = arq;
        		funcTransfer(pathA, pathB, ent->d_name);//Funcão que copia os arquivos requisitados
        	}else{
            	cout << "End of directory!!!\n";
        	}
        }
    }else{
        return -1;
    }

    closedir(dirA);
    closedir(dirB);
    return 0;
}

int main(int argc, char** argv)
{	
	char pathA[TMem], pathB[TMem];//variaveis que vão receber o endereço dos diretórios
	
    if (argc == 2){//help option
        funcHelp(argv[1]);
        
    }else if(argc == 4){//execution function
    	strcpy(pathA,argv[1]);
    	strcpy(pathB,argv[2]);
    	
        if(funcAlocDirect(pathA,pathB, argv[3]) == 0)
            cout << "Copy made sucessfully!!!\n";
        else
            cout << "The path is nonexistent!!!\n";
        
    }else{
        cout << "Wrong number of arguments!!!\n";
    }

    delete ent;

    return 0;
}
