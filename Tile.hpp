#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Point.hpp"

class Tile
{
	// アニメーション形式
	enum Mode :unsigned char
	{
		vertical = 0,
		horizontal = 1,
		walk = 0,
		step = 2,
	};

	// タイル情報
	struct Data
	{
		unsigned char tag;	// 任意の付加情報、海とかアニメとかなんとか、なんでもいい -1(255)で透過とか、その辺の扱いは任せる
		unsigned char flag;	// タイルのフラグ情報	本質的にはなんでもいいが、大体通行許可設定とかカウンター属性とか足元半透明フラグとか
	};

	// 返り値用構造体
	struct Ret
	{
		int handle;	// タイルセット画像ハンドル
		Point<int> pos;	// 表示位置
		Data data;	// タイル情報
	};

	// タイルセット情報
	struct Common
	{
		std::u8string path;
		Point<int> num;
		std::vector<Data> data;

		virtual Ret get(int id, int t)const = 0;
	};
	// 通常タイルセット
	struct Norm :public Common
	{
		Norm(const char8_t* graph, unsigned short tnum);
		Ret get(int id, int t)const override;
	};
	// アニメーションタイルセット
	struct Anim :public Common
	{
		unsigned char pattern;
		unsigned char type;

		Anim(const char8_t* graph, unsigned short tnum, unsigned char type, int pattern);
		Ret get(int id, int t)const override;
	};

	static inline std::vector<std::unique_ptr<Common>> data;	// タイルセットリスト

public:
	static Point<int> size;	// タイルサイズ

	static void load(const char* file);
	static Ret get(int id, int t);
};

