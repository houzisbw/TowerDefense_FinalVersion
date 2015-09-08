#ifndef _ASTARPATH_H_
#define _ASTARPATH_H_
#include "cocos2d.h"
#include "GameManager.h"
#include <queue>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <memory.h>
#define MAP_SIZE 80
#define INF 10000
//A*算法点结构
struct ANode
{
	int x;
	int y;
	ANode * parent;
	int f, g, h;   //F = G + H,G为从起点开始移动到指定方格的耗费,h为预估耗费 整数比浮点数更高效
};
//优先队列比较结构
struct cmp{
	bool operator ()(ANode * a, ANode *b){
		return a->f > b->f;//最小值优先
	}
};
class AStarPath
{
public:
	AStarPath();
	~AStarPath();
	//A*算法函数
	inline bool isInMapAndValid(int x, int y);
	inline int Heuristic(int thisX, int thisY, int destX, int destY);  //估价函数:当前节点到目的节点的曼哈顿距离
	//A* 最短路算法
	void AStar(int srcX, int srcY, int destX, int destY);
	std::vector<cocos2d::Vec2> roadPoint;//存放最短路的vector
	int roadLength;//路径的方格数目
	std::priority_queue<ANode*, std::vector<ANode*>, cmp> open; //开启列表
	CC_SYNTHESIZE(bool, isReachable, IsReachable);//目的地是否可达
private:
	

	ANode * nodeMap[MAP_SIZE][MAP_SIZE]; //存放节点指针的数组
	bool visited[MAP_SIZE][MAP_SIZE];
	bool openList[MAP_SIZE][MAP_SIZE];
	float g[MAP_SIZE][MAP_SIZE];
	//A*算法结束
	GameManager * instance;
};
#endif

