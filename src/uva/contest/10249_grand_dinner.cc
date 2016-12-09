#include <iostream>

#include <algorithm>
#include <vector>

#define MAX_TEAMS 71
#define MAX_TABLES 51

using namespace std;

struct Team {
  int id;
  int size;
};

int main() {
  int nTeams, nTables;
  vector<int> tables(MAX_TABLES), teamSizes(MAX_TEAMS);
  vector<Team> teams(MAX_TEAMS);
  int distribution[MAX_TEAMS][MAX_TABLES];

  while (true) {
    cin >> nTeams >> nTables;
    if (nTeams == 0 || nTables == 0) break;

    teams.resize(nTeams);
    teamSizes.resize(nTeams);
    for (int i = 0; i < nTeams; i++) {
      cin >> teamSizes[i];
      teams[i] = {i, teamSizes[i]};
    }

    tables.resize(nTables);
    for (auto& table : tables) cin >> table;

    // Distribute teams in a greedy biggest-first order, so that we have more
    // free tables by the time we try to assign tables to the bigger teams.
    sort(
      teams.begin(), teams.end(),
      [](Team a, Team b){return a.size > b.size;} // decreasing
    );
    
    bool success = true;
    for (auto team : teams) {
      int dist = 0;
      for (int i = 0; i < nTables && dist < team.size; i++) {
        if (tables[i] > 0) {
          distribution[team.id][dist++] = i + 1;
          tables[i]--;
        }
      }
      if (dist < team.size) {
        success = false;
      }
    }

    cout << (success ? '1' : '0') << endl;
    if (success) {
      for (int i = 0; i < nTeams; i++) {
        for (int j = 0; j < teamSizes[i]; j++) {
          cout << distribution[i][j] << ' ';
        }
        cout << endl;
      }
    }
  }
  return 0;
}