#include<iostream>
#include<list>
#include<vector>
#include<climits>
// You are not allowed to include additional libraries

#define INF INT_MAX

using namespace std;

class ShortestP2P {
  private:
      unsigned int num_node, num_edge;
      vector<int> dis;
  public:
      ShortestP2P() {
          num_node = 0;
          num_edge = 0;
      }

      /* Read the graph from stdin
       * The input has the following format:
       *
       * Firstline: 1 unsigned int, the total number of verticies, X.
       * Secondline: 1 unsigned int, the total number of edges between vertices, Y.
       * A set of Y lines, each line contains tuple of 2 unsigned int and 1 int (not unsigned int!), in the format of: A(point 1, unsigned) B(point 2, unsigned) dist(distance, int)
       *
       * Example:
       * 4
       * 4
       * 0 1 5
       * 1 2 -1
       * 2 3 4
       * 3 1 6
       * 
       * 
       * 
       *
       * Vertices that are not connected have a infinitly large distance. You may use INF (previously defined at the top of this cope snippet) for infinitly large distance.
       * 
       * Special: when the graph is not proper, where there exist any pair of vertices whose minimum distance does not exist, terminate immediately by printing:
       * cout << "Invalid graph. Exiting." << endl;
       *
       * Note: vertex pairs that are not connected, which have infinitely large distances are not considered cases where "minimum distances do not exist".
       */
      void readGraph(){
          cin >> num_node >> num_edge;
          dis = vector<int>(num_node * num_node, INF);
          unsigned int cnt = 0;
          while (cnt < num_edge) {
              int num1, num2, num3;
              cin >> num1 >> num2 >> num3;
              dis[num1 * num_node + num2] = num3;
              cnt++;
          }
          for (unsigned int i = 0; i < num_node; i++) {
              for (unsigned int j = 0; j < num_node; j++) {
                  for (unsigned int k = 0; k < num_node; k++) {
                      if ((dis[j * num_node + i] != INF) && (dis[i * num_node + k] != INF)) {
                          dis[j * num_node + k] = min(dis[j * num_node + k], dis[j * num_node + i] + dis[i * num_node + k]);
                          if ((j == k) && (dis[j * num_node + k] < 0)) {
                              cout << "Invalid graph. Exiting." << endl;
                              exit(0);
                          }
                      }
                  }
              }
          }
      }

      /* Input: 2 vertices A and B
       * Output: distance between them.
       * cout << dist << endl;
       *
       * When the A and B are not connected print INF:
       * cout << "INF" << endl;
       */
      void distance(unsigned int A, unsigned int B) {
          if (A == B) cout << 0 << endl;
          else if (dis[A * num_node + B] != INF) cout << dis[A * num_node + B] << endl;
          else cout << "INF" << endl;
      }

  private:
    // internal data and functions.

      

};
