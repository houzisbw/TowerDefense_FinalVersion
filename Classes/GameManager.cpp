#include "GameManager.h"


GameManager::GameManager()
{
	
}


GameManager::~GameManager()
{
}

GameManager* GameManager::mgr; //不能少，mgr不属于类，所以要声明

GameManager * GameManager::getInstance()
{
	if (mgr == nullptr)
	{
		mgr = new GameManager();
	}

	return mgr;
}

void GameManager::clear()
{
	enemyVector.clear();
	bulletVector.clear();
	prismTowerVector.clear();
	towerVector.clear();
	cityPoint.clear();
	cityVector.clear();
	soldierVector.clear();
	soldierInRectVector.clear();
	groupVector.clear();
	antiVisibleTowerVector.clear();
	crystalMineVector.clear();
	campTowerVector.clear();
	smallMap = nullptr;
}