#ifndef _ICEBULLET_H_
#define _ICEBULLET_H_
#include "BaseBullet.h"
#include "GameManager.h"
class IceBullet :
	public BaseBullet
{
public:
	IceBullet();
	~IceBullet();
	virtual bool init();
	CREATE_FUNC(IceBullet);
private:
	GameManager * instance;
};
#endif

