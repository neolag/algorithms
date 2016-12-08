// UVA Problem 452:
// https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=393

#include <iostream>

#include <algorithm>
#include <vector>

using std::cin;
using std::cout;
using std::max;

/**
 * Classe auxiliar para representação de uma tarefa. Guarda
 * uma lista de tarefas dependentes de si além do número de
 * dependências que a própria tarefa tem, de forma que todas
 * as tarefas dependentes de uma outra possam ser iteradas
 * rapidamente, além do número de dependências não satisfeitas
 * ser checado também imediatamente.
 */
struct Task {
    int weight = 0;
    int criticalPath = 0;
    int dependencyCount = 0;
    std::vector<Task*> dependents;
};

int main() {
  int T;
  Task tasks[30];
  std::vector<Task*> completable;

  cin >> std::noskipws >> T;
  while (T --> 0) {
    // Inicializa todas as estruturas utilizadas
    for (Task& t : tasks) {
      t.weight = t.criticalPath = t.dependencyCount = 0;
      t.dependents.clear();
    }
    completable.clear();
    cin >> std::ws; // Pula todo o espaço em branco antes dos dados

    // Lê as tarefas usando o formato especificado pelo problema. Já
    // inicializa os valores para que o algoritmo de busca pelo caminho
    // crítico funcione. Adiciona todos os dependentes ao vetor e conta
    // o número de dependências para cada tarefa.
    char c;
    while (cin >> c && c >= 'A' && c <= 'Z') {
      Task& curr = tasks[c - 'A'];
      cin >> std::ws >> curr.weight >> c;
      if (c == ' ') {
        while (cin >> c && c >= 'A' && c <= 'Z') {
          curr.dependencyCount++;
          tasks[c - 'A'].dependents.push_back(&curr);
        }
      }
      // Caso tarefa não posssua dependências, já pode ser completada
      // imediatamente, adiciona à lista de tarefas completáveis.
      if (curr.dependencyCount == 0) {
        completable.push_back(&curr);
      }
    }

    // Algoritmo de ordenação topológica com cálculo do (tamanho do) caminho
    // crítico propriamente dito. Para cada tarefa completável (presente no
    // vetor), itera sobre todas as tarefas dependentes, decrementando o
    // contador de dependências de cada uma. As tarefas que terminam sem
    // mais dependências são então adicionadas ao vetor de tarefas completáveis
    // para que possam ser processadas em seguida. Note que não checamos se
    // se trata realmente de um DAG visto que não são apresentados casos desse
    // tipo. Para isso, teríamos que contar também o número de tarefas completas
    // e retornar algum erro caso todas não tenham sido.
    int globalCriticalPath = 0;
    while (!completable.empty()) {
      Task& curr = *completable.back();
      completable.pop_back(); // completable guarda apenas um ponteiro, podemos dar o pop já agora

      // Campo criticalPath guarda o caminho até antes da tarefa em questão.
      // O tamanho do caminho total, incluindo a tarefa atual, deve ser somado
      // então com o peso da mesma.
      int totalPathWeight = curr.criticalPath + curr.weight;
      for (Task* dependent : curr.dependents) {
        // Guarda maior caminho até chegar na tarefa dependente, que quando a
        // mesma for completada/processada, representará o caminho crítico até ela.
        dependent->criticalPath = max(dependent->criticalPath, totalPathWeight);
        if (--dependent->dependencyCount == 0) {
          completable.push_back(dependent);
        }
      }
      globalCriticalPath = max(globalCriticalPath, totalPathWeight);
    }

    cout << globalCriticalPath << std::endl;
    if (T > 0) cout << std::endl; // PE com linha extra ao final
  }
}
