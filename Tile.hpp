#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Point.hpp"

class Tile
{
	// �A�j���[�V�����`��
	enum Mode :unsigned char
	{
		vertical = 0,
		horizontal = 1,
		walk = 0,
		step = 2,
	};

	// �^�C�����
	struct Data
	{
		unsigned char tag;	// �C�ӂ̕t�����A�C�Ƃ��A�j���Ƃ��Ȃ�Ƃ��A�Ȃ�ł����� -1(255)�œ��߂Ƃ��A���̕ӂ̈����͔C����
		unsigned char flag;	// �^�C���̃t���O���	�{���I�ɂ͂Ȃ�ł��������A��̒ʍs���ݒ�Ƃ��J�E���^�[�����Ƃ������������t���O�Ƃ�
	};

	// �Ԃ�l�p�\����
	struct Ret
	{
		int handle;	// �^�C���Z�b�g�摜�n���h��
		Point<int> pos;	// �\���ʒu
		Data data;	// �^�C�����
	};

	// �^�C���Z�b�g���
	struct Common
	{
		std::u8string path;
		Point<int> num;
		std::vector<Data> data;

		virtual Ret get(int id, int t)const = 0;
	};
	// �ʏ�^�C���Z�b�g
	struct Norm :public Common
	{
		Norm(const char8_t* graph, unsigned short tnum);
		Ret get(int id, int t)const override;
	};
	// �A�j���[�V�����^�C���Z�b�g
	struct Anim :public Common
	{
		unsigned char pattern;
		unsigned char type;

		Anim(const char8_t* graph, unsigned short tnum, unsigned char type, int pattern);
		Ret get(int id, int t)const override;
	};

	static inline std::vector<std::unique_ptr<Common>> data;	// �^�C���Z�b�g���X�g

public:
	static Point<int> size;	// �^�C���T�C�Y

	static void load(const char* file);
	static Ret get(int id, int t);
};

