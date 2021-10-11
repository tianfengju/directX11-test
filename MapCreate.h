#pragma once


class CMapCreate
{
public:
	CMapCreate(int sizeX, int sizeY);
	~CMapCreate();

	void MapCretaeBySeed(size_t seed = 0);

	bool WorldMapMake(std::string fileName);

	void WorldMapMake(int x, int y, int size, int t1, int t2, int t3, int t4, unsigned char map[][18]);
	void MapSimple(unsigned char map[][18]);
	void WorldMake(unsigned char map[][18], int SEED, unsigned char WorldMap[][250]);

	char GetData(int num);
private:
	int sizeV;
	int sizeH;
	//std::vector<char> inData;
	char* mapData;
	int dataLen;
};

