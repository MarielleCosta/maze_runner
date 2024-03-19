#include <stdio.h>
#include <malloc.h>
#include <stack>
#include <cstdlib>
#include <thread>

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;
/* Inserir elemento: 

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 */
// Retornar o numero de elementos: 
//    valid_positions.size();
// 
// Retornar o elemento no topo: 
//  valid_positions.top(); 
// 
// Remover o primeiro elemento do vetor: 
//    valid_positions.pop();


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
	// Abre o arquivo para leitura (fopen)
	FILE *arq;
	arq = fopen(file_name, "r");
	if(arq == NULL)
	{
		printf("Erro na leitura do arquivo.\n");
	}
	
	// Le o numero de linhas e colunas (fscanf) 
	// e salva em num_rows e num_cols
	fscanf(arq, "%d %d", &num_rows, &num_cols);

	// Aloca a matriz maze (malloc)
	maze = (char **)malloc(sizeof(char *)*num_rows);
	if(maze == NULL)
	{
		printf("Memoria insuficiente.\n");
  	}
	// Aloca cada linha da matriz
	for (int i = 0; i < num_rows; ++i){
		maze[i] = (char *)malloc(sizeof(char)*num_cols);
		if(maze[i] == NULL)
		{
			printf("Memoria insuficiente.\n");
		}
	}
		
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			// Se o valor for 'e' salvar o valor em initial_pos
			fscanf(arq, " %c", &maze[i][j]);
			if (maze[i][j] == 'e'){
				initial_pos.i = i;
				initial_pos.j = j;
			}
		}
	}

	fclose(arq);

	return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
	
	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
		// Marcar a posição atual com o símbolo '.'
		maze[pos.i][pos.j] = 'o';
		// Limpa a tela
		system("clear");
		// Imprime o labirinto
		print_maze();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
		/* Dado a posição atual, verifica quais sao as próximas posições válidas
			Checar se as posições abaixo são validas (i>0, i<num_rows, j>0, j <num_cols)
		 	e se são posições ainda não visitadas (ou seja, caracter 'x') e inserir
		 	cada uma delas no vetor valid_positions
		 		- pos.i, pos.j+1 (frente)
		 		- pos.i, pos.j-1 (tras)
		 		- pos.i+1, pos.j (baixo)
		 		- pos.i-1, pos.j (cima)
		 	Caso alguma das posiçÕes validas seja igual a 's', retornar verdadeiro
	 	*/
	 	
		//frente
	 	if (pos.j+1<num_cols){
			if((maze[pos.i][pos.j+1] == 'x') && (maze[pos.i][pos.j+1] != '.')){
				pos_t newpos;
				newpos.i = pos.i;
				newpos.j = pos.j +1;
				valid_positions.push(newpos);
			}
			else if(maze[pos.i][pos.j+1] == 's'){
				return true;
			}
		}
		//tras
	 	if (pos.j-1>=0){
			if((maze[pos.i][pos.j-1] == 'x') && (maze[pos.i][pos.j-1] != '.')){
				pos_t newpos;
				newpos.i = pos.i;
				newpos.j = pos.j -1;
				valid_positions.push(newpos);
			}
			else if(maze[pos.i][pos.j-1] == 's'){
				return true;
			}
		}
		//baixo
	 	if (pos.i+1<num_rows){
			if((maze[pos.i+1][pos.j] == 'x') && (maze[pos.i+1][pos.j] != '.')){
				pos_t newpos;
				newpos.j = pos.j;
				newpos.i = pos.i +1;
				valid_positions.push(newpos);
			}
			else if(maze[pos.i+1][pos.j] == 's'){
				return true;
			}
		}
		//cima
	 	if (pos.i-1>=0){
			if((maze[pos.i-1][pos.j] == 'x') && (maze[pos.i-1][pos.j] != '.')){
				pos_t newpos;
				newpos.j = pos.j;
				newpos.i = pos.i -1;
				valid_positions.push(newpos);
				
			}
			else if(maze[pos.i-1][pos.j] == 's'){
				return true;
			}
		}
	
		// Verifica se a pilha de posições nao esta vazia 
		//Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
		// Caso contrario, retornar falso
		if (!valid_positions.empty()) {
			if(valid_positions.size() >= 1){	
				pos_t next_position = valid_positions.top();
				while(maze[next_position.i][next_position.j] == '.'){
					if(valid_positions.size()> 1){
						valid_positions.pop();
						next_position = valid_positions.top();
					}
					else{
						return false;
					}
				}
				maze[pos.i][pos.j] = '.';
				if(valid_positions.size() >= 1){
					valid_positions.pop();
					walk(next_position);
				}
				else{
					return false;
				}
			}
			else{
				return false;
			}
		}
	return false;
}

int main(int argc, char* argv[]) {
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze("../data/maze5.txt");
	// chamar a função de navegação
	
	bool exit_found = walk(initial_pos);
	
	// Tratar o retorno (imprimir mensagem)
	system("clear");
	print_maze();
	if(exit_found){
		printf("Saída encontrada!\n");
	}
	else{
		printf("Saída não encontrada!\n");
	}
	return 0;
}
