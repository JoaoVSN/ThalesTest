# ThalesTest

# Instruções

Sistema feito como teste avaliativo para empresa Thales Group.
O sistema consiste em copiar um arquivo independente de sua extensão através dos seguintes comandos via terminal.

```sh
$./ThalesTest -h //Exibe o menu de ajuda do sistema
$./ThalesTest <Diretorio A> <Diretorio B> <palavra-chave> 
    #Procura os arquivos no diretório a que tenho a palavra-chave como substring copia os arquivos obdecendo a ordem decrescente data de manipulção, do mais novo por mais antigo e fazendo sua cópia a cada 60 segundos.

$./Thales <opc> <Diretorio A> <Diretorio B> <palavra-chave>
	#Procura os arquivos no diretório a que tenho a palavra-chave como substring copia os arquivos obdecendo as opções digitados no parametro opções<opc> que são os seguinte:
	#-c 		:Faz a trasferência de A para B sem a contagem de tempo.
	#-d		:Faz a trasferência de A para B sem a ordenção por data.
	#-dc		:Faz a trasferência de A para B sem a contagem de tempo e a ordenção por data.

```

Para compilar o código no Ubuntu digite:
```sh
$g++ main.cpp -o ThalesTest
```
# Intructions

System made as an evaluation test for company Thales Group.
The system consists of copying a file independent of its extension through the following terminal commands.

```sh
$./ThalesTest -h //Displays the system help menu
$./ThalesTest  <Directory A> <Directory B> <Keyword>
	#Searches the files in the directory where I have the keyword as substring copies the 	files obduting the descending order date of manipulation, from the oldest to the oldest and making your copy every 60 seconds.

$./Thales <opt> <Directory A> <Directory B> <keyword>
	#Searches the files in the directory where I have the keyword as substring copies the files by obferencing the options typed in the <opc> options switch which are as follows:
	#-c:		Transfers A to B without counting time.
	#-d:		Transfers from A to B without sorting by date.
	#-dc:	Transforms from A to B without counting time and sorting by date.
```

To compile the code in Ubuntu type:
```sh
$g++ main.cpp -o ThalesTest
```
