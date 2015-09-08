#include "AStarPath.h"

int dire[8][2] = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 } }; //不能和helloworld里的重名

AStarPath::AStarPath()
{
	instance = GameManager::getInstance(); //获取单例
	isReachable = true;
}


AStarPath::~AStarPath()
{
}

inline bool AStarPath::isInMapAndValid(int x, int y)
{
	if (x >= 0 && x < MAP_SIZE && y >= 0 && y < MAP_SIZE && instance->walkMap[x][y] == 0) //0表示空地,可以行走
		return true;
	return false;
}

inline int AStarPath::Heuristic(int thisX, int thisY, int destX, int destY)  //估价函数:当前节点到目的节点的曼哈顿距离
{
	return (abs(destX - thisX) + abs(destY - thisY));
}

void AStarPath::AStar(int srcX, int srcY, int destX, int destY)
{
	//A*算法初始化
	memset(visited, false, sizeof(visited));
	memset(openList, false, sizeof(openList));
	for (int i = 0; i<MAP_SIZE; i++)
	for (int j = 0; j<MAP_SIZE; j++)
	{
		g[i][j] = INF;
		nodeMap[i][j] = NULL;
	}

	//初始化源节点
	ANode  * src = (ANode*)malloc(sizeof(ANode));
	src->x = srcX;
	src->y = srcY;
	src->parent = NULL;
	src->f = INF; //开始要放入开启列表中
	src->g = 0;
	g[srcX][srcY] = 0;
	nodeMap[srcX][srcY] = src;
	openList[srcX][srcY] = true;
	open.push(src);

	ANode * dest; //不能在这里malloc，因为搜寻周围格子的过程就已经malloc了

	while (!open.empty()) //开启列表不为空,若为空则没有找到目的节点
	{
		//将该节点(F值最小的一个)从开启列表中去除

		ANode * temp = open.top();
		open.pop();
		int tempX = temp->x;
		int tempY = temp->y;
		visited[tempX][tempY] = true; //已加入关闭列表
		//temp->isVisited=true;
		openList[tempX][tempY] = false;//开启列表
		//temp->isInOpenList=false;
		//如果找到目的节点则退出算法
		if (temp->x == destX && temp->y == destY)
		{
			

			dest = temp;
			dest->x = destX;
			dest->y = destY;
			isReachable = true;//目的地可达

			//保存最短路径
			ANode * t = dest;
			while (t != NULL)
			{
				roadPoint.push_back(Point(t->x, t->y));
				t = t->parent;
			}

			//释放内存
			for (int i = 0; i<MAP_SIZE; i++)
			for (int j = 0; j<MAP_SIZE; j++)
			{
				if (nodeMap[i][j] != NULL)
				{
					free(nodeMap[i][j]);
					nodeMap[i][j] = NULL;
				}
			}

			return;
		}

		//先将源节点周围的节点(上下左右8个)加入到开启列表中
		for (int i = 0; i<8; i++)
		{
			int tx, ty;
			tx = tempX + dire[i][0];
			ty = tempY + dire[i][1];


			if (isInMapAndValid(tx, ty) && visited[tx][ty] == false) //如果该节点不在关闭列表中而且可以行走
			{
				ANode  * add = (ANode*)malloc(sizeof(ANode));
				add->x = tx; //不能忘记！！！我勒个去
				add->y = ty;
				float tempG;
				add->h = Heuristic(tx, ty, destX, destY);
				if (i == 0 || i == 2 || i == 4 || i == 6)
				{
					tempG = g[tempX][tempY] + 14;//斜边
				}
				else
				{
					tempG = g[tempX][tempY] + 10;//上下左右
				}

				if (openList[tx][ty] == true)//如果该点在开启列表中则不用再加入进开启列表
				{
					if (tempG < g[tx][ty])
					{
						g[tx][ty] = tempG;
						nodeMap[tx][ty]->f = g[tx][ty] + nodeMap[tx][ty]->h;
						nodeMap[tx][ty]->parent = temp;//设定父亲节点

					}

				}
				else
				{
					g[tx][ty] = tempG;
					add->f = g[tx][ty] + add->h;
					openList[tx][ty] = true;
					open.push(add);//加入到开启列表中
					add->parent = temp;
					nodeMap[tx][ty] = add;//加入到nodeMap中去

				}

			}
		}

	}

	
	isReachable = false;//目的地不可达

	
}
