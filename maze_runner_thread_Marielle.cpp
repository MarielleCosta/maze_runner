#include <iostream>
#include <fstream>
#include <stack>
#include <thread>
#include <chrono>

using namespace std;

// Matriz de char representando o labirinto
char** maze;

// Número de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Variável global de saída encontrada
bool exit_found = false;
int count_threads = 0;
int count_exit = 0;
int count_input = 0;

// Representação de uma posição
struct pos_t {
    int i;
    int j;
};

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
    count_threads = 0;
    count_exit = 0;
    count_input = 0;
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            	printf("%c", maze[i][j]);
                if(maze[i][j] == 'o'){
                    count_threads++;
                }
                else if(maze[i][j] == 's'){
                    count_exit++;
                }
                else if(maze[i][j] == 'e'){
                    count_input++;
                }
        }
        printf("\n");
    }
    
    this_thread::sleep_for(chrono::milliseconds(100));
}

// Função responsável pela navegação
void walk(pos_t pos) {
    pos_t newpos;
    stack<pos_t> valid_positions;
    valid_positions.push(pos);

    while (!valid_positions.empty()) {        
        valid_positions.pop();
        maze[pos.i][pos.j] = 'o';

        this_thread::sleep_for(chrono::milliseconds(100));

        maze[pos.i][pos.j] = '.';

		// frente
        if (pos.j + 1 < num_cols) {
            if (maze[pos.i][pos.j + 1] == 's') {
                maze[pos.i][pos.j] = 'o';
                exit_found = true;
            } else if (maze[pos.i][pos.j + 1] == 'x') {
                newpos.i = pos.i;
				newpos.j = pos.j + 1;
                valid_positions.push(newpos);
            }
        }

        // trás
        if (pos.j - 1 >= 0) {
            if (maze[pos.i][pos.j - 1] == 's') {
                maze[pos.i][pos.j] = 'o';
                exit_found = true;
            } else if (maze[pos.i][pos.j - 1] == 'x') {
                newpos.i = pos.i;
				newpos.j = pos.j - 1;
                valid_positions.push(newpos);
            }
        }

        // baixo
        if (pos.i + 1 < num_rows) {
            if (maze[pos.i + 1][pos.j] == 's') {
                maze[pos.i][pos.j] = 'o';
                exit_found = true;
            } else if (maze[pos.i + 1][pos.j] == 'x') {
                newpos.j = pos.j;
				newpos.i = pos.i + 1;
                valid_positions.push(newpos);
            }
        }

        // cima
        if (pos.i - 1 >= 0) {
            if (maze[pos.i - 1][pos.j] == 's') {
                maze[pos.i][pos.j] = 'o';
                exit_found = true;
            } else if (maze[pos.i - 1][pos.j] == 'x') {
                newpos.j = pos.j;
				newpos.i = pos.i - 1;
                valid_positions.push(newpos);
            }
        }
        
        while (valid_positions.size() > 0) {
            newpos = valid_positions.top();
            valid_positions.pop();
            std::thread tway(walk, newpos);
            tway.detach();
        }
    }
}

int main(int argc, char* argv[]) {
    // Carrega o labirinto com o nome do arquivo recebido como argumento
    pos_t initial_pos = load_maze("../data/maze4.txt");

    // Cria thread detached passando a função de navegação
    thread tmain(walk, initial_pos);
    tmain.detach();

    while (!exit_found) {
        print_maze();
        system("clear");
        if ((count_threads == 0) && (count_exit == 0) && (count_input == 0)){
            break;
        }
    }

    print_maze();
    if(exit_found){
		printf("Saída encontrada!\n");
	}
	else if((!exit_found) || ((count_threads == 0) && (count_exit == 0))){
		printf("Saída não encontrada!\n");
	}

    return 0;
}
