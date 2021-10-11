#include <string>
#include <vector>
#include "MapCreate.h"

#include <fstream>

#include <iostream>



CMapCreate::CMapCreate(int sizeX, int sizeY)
{
	sizeV = sizeX;
	sizeH = sizeY;
}

CMapCreate::~CMapCreate()
{
	delete[] mapData;
}

void CMapCreate::MapCretaeBySeed(size_t seed)
{
}

bool CMapCreate::WorldMapMake(std::string fileName)
{
	//std::string fileName;
	std::ifstream inFile;
	inFile.open(fileName.c_str(),std::ios::binary);

	inFile.seekg(0, std::ios::end);
	std::vector<char> inData(inFile.tellg());
	inFile.read((char*)&inData[0], inData.size());
	inFile.seekg(std::ios::beg);
	inFile.read((char*)&inData[0], inData.size());
	inFile.close();

	//if ((sizeV + 1) * (sizeH + 1) > inData.size())return false;
	mapData = new char[inData.size()];
	dataLen = inData.size();
	for (int i = 0; i < inData.size(); i++) {
		mapData[i] = inData[i];
	}
	return true;
}

void CMapCreate::WorldMapMake(int x, int y, int size, int t1, int t2, int t3, int t4, unsigned char map[][18])
{
	//if (size < 1)return;

	////頂点の高さを決める
	//int mapPlus = ((t1 + t2 + t3 + t4) >> 2) + GetRand(size);
	//if (mapPlus >= 255) mapPlus = 255;
	//map[x][y] = mapPlus;

	////四角形の２点同士の中点の高さを決定
	//int s1 = ((t1 + t2) >> 1);
	//int s2 = ((t1 + t3) >> 1);
	//int s3 = ((t2 + t4) >> 1);
	//int s4 = ((t3 + t4) >> 1);

	////四つの地点の座標を決める
	//map[x + size][y] = s3;
	//map[x - size][y] = s2;
	//map[x][y + size] = s4;
	//map[x][y - size] = s1;
	//
	////分割サイズを半分にする
	//size >>= 1;

	////四つに分割
	//WorldMapMake(x - size, y - size, size, t1, s1, s2, map[x][y], map);
	//WorldMapMake(x + size, y - size, size, s1, t2, map[x][y], s3, map);
	//WorldMapMake(x - size, y + size, size, s2, map[x][y], t3, s4, map);
	//WorldMapMake(x + size, y + size, size, map[x][y], s3, s4, t4, map);

}

void CMapCreate::MapSimple(unsigned char map[][18])
{
	WorldMapMake(8, 8, 8, map[0][0], map[16][0], map[0][16], map[16][16], map);
}

void CMapCreate::WorldMake(unsigned char map[][18], int SEED, unsigned char WorldMap[][250])
{
	//for (int i = 0; i < sizeV / 16; i++) {
	//	for (int j = 0; j < sizeH / 16; j++) {
	//		//四角形の四点の高さを決定
	//		srand(SEED + i + j * 10000 + (i&j) * 100);
	//		map[0][0] = GetRand(255);
	//		srand(SEEN + i + 1 + j * 10000 + ((i + 1) & j) * 100);
	//		map[16][0] = GetRand(255);
	//		srand(SEEN + i + (j + 1) * 10000 + (i&(j + 1)) * 100);
	//		map[0][16] = GetRand(255);
	//		srand(SEED + i + 1 + (j + 1) * 10000 + ((i + 1) & (j + 1)) * 100);
	//		map[16][16] = GetRand(255);

	//		MapSimple(map);

	//		for (int i2 = 0; i2 < 16; i2++) {
	//			for (int j2 = 0; j2 < 16; j2++) {
	//				WorldMap[i * 16 + i2][j * 16 + j2] = map[i2][j2];
	//			}
	//		}
	//	}
	//}

	//for (int i = 0; i < (250 / 16) * 16; i++) {
	//	for (int j = 0; j < (250 / 16) * 16; j++) {
	//		//ワールドマップの線画
	//		if (WorldMap[i][j] <= 110 && WorldMap[i][j] > 50) {
	//			DrawPixel(i, j, GetColor(WorldMap[i][j], 165, WorldMap[i][j]));
	//		}
	//		else if (worldMap[i][j] <= 50) {
	//			DrawPixel(i, j, GetColor(WorldMap[i][j], 135, WorldMap[i][j]);)
	//		}
	//		else {
	//			DrawPixel(i, j, GetColor((255 - WorldMap[i][j]) / 3, (255 - WorldMap[i][j]) / 3, 240));
	//		}

	//	}
	//}
}

char CMapCreate::GetData(int num)
{
	int id = num % dataLen;
	return mapData[id];
}
