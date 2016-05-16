#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>

using namespace std;

typedef vector< vector<int> > GraphType;

string file01 = "tests/newfile01.txt";
string file02 = "tests/newfile02.txt";
string file03 = "tests/newfile03.txt";
string file04 = "tests/newfile04.txt";
string file05 = "tests/newfile05.txt";

//заполнение матрицы смежности чтением из файла
GraphType readGraphFromFile(string input)
{
    ifstream inputFile(input);
    int graphSize = 0; //количество вершин в графе
    inputFile>>graphSize; //чтение количество вершин
    GraphType graph(graphSize, vector<int>(graphSize, 0));

    //чтение матрицы смежности из файла
    for (int i = 0; i < graphSize; i++)
        {
            for (int j = 0; j < graphSize; j++)
                inputFile>>graph[i][j];
        }

    return graph;
}

//поиск кратчайших путей до каждой вершины графа методом поиска в ширину
void shortestPathSearch(GraphType& graph, int startingVertex)
{
    int graphSize = graph.size();
    bool graphIsConnected = true;

    queue<int> que; //очередь готовящихся к посещению вершин
    vector<bool> visited(graphSize, false); //посещенные вершины
    vector<int> length(graphSize); //длины кратчайших путей всех вершин
    vector<int> parentVertex(graphSize); //родительская вершина для данной в результирующем дереве

    que.push(startingVertex);
    visited[startingVertex-1] = true;
    length[startingVertex-1] = 0;
    parentVertex[startingVertex-1] = -1;

    while (!que.empty())
        {
            int vertex = que.front();
            que.pop();
            for (int i = 0; i < graphSize; i++)
                {
                    if (graph[vertex-1][i] == 1 && !visited[i])
                        {
                            que.push(i+1); //добавляем в очередь вершины, связанные с текущей
                            visited[i] = true;
                            length[i] = length[vertex-1] + 1;
                            parentVertex[i] = vertex;
                        }
                }
        }

    //список смежности дерева кратчайших путей
    GraphType adjacencyList(graphSize, vector<int> ());

    //восстановление кратчайшего пути для каждой вершины
    for (int i = 0; i < graphSize; i++)
        {
            if (visited[i])
                {
                    vector<int> shortestPath;
                    int parent = parentVertex[i];
                    if (parent == -1)
                        shortestPath.push_back(0);
                    else
                        shortestPath.push_back(i+1);

                    while (parent != -1)
                        {
                            shortestPath.push_back(parent);
                            parent = parentVertex[parent-1];
                        }

                    //заполнение списка смежности с использованием кратчайшего пути
                    for (int m = 0; m < shortestPath.size()-1; m++)
                        {
                            bool containsStraight = false;
                            bool containsReverse = false;
                            //проверка на вхождение вершины в строку списка смежности
                            if (shortestPath[m] > 0)
                                {
                                    for (int n : adjacencyList[shortestPath[m]-1])
                                        if (n == shortestPath[m+1])
                                            containsStraight = true;
                                    for (auto l : adjacencyList[shortestPath[m+1]-1])
                                        if (l == shortestPath[m])
                                            containsReverse = true;
                                }

                            //добавление вершины m+1 как смежной вершине m
                            if (!containsStraight)
                                adjacencyList[shortestPath[m]-1].push_back(shortestPath[m+1]);
                            //добавление вершины m как смежной вершине m+1
                            if (!containsReverse)
                                adjacencyList[shortestPath[m+1]-1].push_back(shortestPath[m]);
                        }
                }
            else
                graphIsConnected = false;
            if (!graphIsConnected)
            {
                cout<<"Graph is disconnected! Shortest path tree can not be build properly."<<endl;
                return;
            }
        }

    //сортировка строк списка смежности
    for (vector<int>& v : adjacencyList)
        sort(v.begin(), v.end());

    //вывод списка смежности дерева кратчайших путей на экран
    cout<<"------------------------------------------------------------"<<endl;
    cout<<"Adjacency list of the shortest-path tree, rooted at vertex "<<startingVertex<<":"<<endl;
    cout<<"------------------------------------------------------------"<<endl;
    for (int k = 0; k < adjacencyList.size(); k++)
        {
            cout<<k+1<<") ";
            for (auto f : adjacencyList[k])
                if (f > 0)
                    cout<<f<<" ";
            cout<<endl;
        }
    cout<<"------------------------------------------------------------"<<endl;
}

//Функция корректного считывания с клавиатуры числового значения
void readInteger(int& number)
{
    cin >> number;
    while (!cin.good())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Введено некорректное значение. Повторите ввод: ";
            cin >> number;
        }
}

int main()
{
    string inputFiles[5] = {file01, file02, file03, file04, file05};

    GraphType graph;

    for (int i = 0; i < 5; i++)
        {
            graph = readGraphFromFile(inputFiles[i]);
            system("clear");
            cout<<"Graph №"<<i+1<<" has been successfully loaded."<<endl;

            //вывод на экран матрицы смежности графа
            //-----------------------------------------------------------
            for (int i = 0; i < graph.size()*2 -1; i++)
                cout<<"-";
            cout<<endl;
            for (auto it1 = graph.begin(); it1 < graph.end(); it1++)
            {
                for (auto it2 = it1->begin(); it2 < it1->end(); it2++)
                    cout<<*it2<<" ";
                cout<<endl;
            }
            for (int i = 0; i < graph.size()*2 -1; i++)
                cout<<"-";
            cout<<endl;
            //-----------------------------------------------------------

            while (true)
                {
                    //выбор корневой вершины искомого дерева
                    int rootVertex;
                    do
                        {
                            cout<<"Enter the root vertex for shortest-path tree, from 1 to  "<<graph.size()<<": ";
                            readInteger(rootVertex);
                            if (rootVertex < 1 || rootVertex > graph.size())
                                cout<<"From 1 to "<<graph.size()<<", please"<<endl;
                        }
                    while (rootVertex < 1 || rootVertex > graph.size());

                    shortestPathSearch(graph, rootVertex); //поиск дерева кратчайших путей

                    char choice;
                    cout<<"Do you want to continue working with graph №"<<i+1<<"? y/n"<<endl;
                    cin>>choice;
                    if (choice == 'n' || choice == 'n')
                        break;
                }
        }
    cout<<"All graph's were processed..."<<endl;
    return 0;
}
