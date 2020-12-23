#include<stdio.h>
#include<graphics.h>//easyxͼ�ο��ļ�
#include<conio.h>
#include<time.h>
#include<mmsystem.h>//��ý���豸�ӿ�
#pragma comment(lib,"winmm.lib")
DWORD t1, t2;//unsigned int
IMAGE bk;//���汳��ͼƬ
IMAGE player[2];
IMAGE img_bull[2];//�����ӵ�ͼƬ
IMAGE img_enemy[2][2];//����л�ͼƬ
IMAGE destory[2];
IMAGE gameover;
IMAGE sbk;//��ʼʱ����ͼ
enum My
{
	WIDTH = 591,//���ڿ����߶�
	HEIGHT = 864,
	bull_num = 5,
	ENEMY_num = 10,
	BIG,
	SMALL
};
struct plane//�ɻ��ṹ
{
	int x;
	int y;
	bool live;//�Ƿ���
	int width;
	int height;
	int hp;
	int type;//�л�����
}role, bull[bull_num], enemy[ENEMY_num];
void loadImg();//����ͼƬ
void enemyHP(int i);//�л��������
void GameInit();//��Ϸ��ʼ��
void gameDraw();//��ͼ
void createBullet();//�ӵ�����λ��
void BulletMove();//�ӵ��ƶ�
bool Timer(int ms, int id);
void PlayerMove(int speed);//��ɫ�ƶ�����ȡ��������
void createEnemy();	//�����л�
void enemyMove(int speed);//�л��ƶ�
void playplane();//�л���Ѫ�������ж�

int main()
{
	initgraph(WIDTH, HEIGHT);
	GameInit();
	putimage(0, 0, &sbk);
	getch();
	//˫�����ͼ,�����ڴ����滭�ã��ٳʵ�	
	BeginBatchDraw();
	while (1)
	{
		gameDraw();
		FlushBatchDraw();//��֮ǰ�Ļ�ͼ���
		PlayerMove(1);
		BulletMove();
		if (Timer(500, 0) && (role.live))
		{
			createEnemy();
		}
		if (Timer(10, 2)) {
			enemyMove(6);
		}
		playplane();
	}
	EndBatchDraw();
	return 0;
}

void loadImg()
{
	//����ͼƬ 591*864
	loadimage(&bk, "./images/background.jpg");
	loadimage(&gameover, "./images/gameover.jpg");
	loadimage(&player[0], "./images/planeNormal_1.jpg");//�������ͼƬ
	loadimage(&player[1], "./images/planeNormal_2.jpg");
	loadimage(&img_bull[0], "./images/bullet1.jpg");//�������ӵ�ͼƬ
	loadimage(&img_bull[1], "./images/bullet2.jpg");
	loadimage(&img_enemy[0][0], "./images/enemy_1.jpg");//������л�ͼƬ1
	loadimage(&img_enemy[0][1], "./images/enemy_2.jpg");
	loadimage(&img_enemy[1][0], "./images/enemyPlane1.jpg");//������л�ͼƬ2
	loadimage(&img_enemy[1][1], "./images/enemyPlane2.jpg");
	loadimage(&destory[0], "./images/planeBoom_1.jpg");//����׹��ͼƬ
	loadimage(&destory[1], "./images/planeBoom_2.jpg");
	loadimage(&sbk, "./images/first.jpg");

}
void enemyHP(int i) {
	int flag = rand() % 10;
	if (flag >= 0 && flag <= 2)//0��9
	{
		enemy[i].type = BIG;
		enemy[i].hp = 3;
		enemy[i].width = 52;
		enemy[i].height = 39;

	}
	else {
		enemy[i].type = SMALL;
		enemy[i].hp = 1;
		enemy[i].width = 104;
		enemy[i].height = 148;
	}
}
void GameInit()
{
	mciSendString("open ./images/game_music_1.mp3 alias BGM", NULL, 0, NULL);
	mciSendString("play BGM repeat", NULL, 0, NULL);
	loadImg();
	role.x = WIDTH / 2;
	role.y = HEIGHT - 120;
	role.live = true;
	//��ʼ���ӵ�
	for (int i = 0; i < bull_num; i++)
	{
		bull[i].x = 0;
		bull[i].y = 0;
		bull[i].live = false;
	}
	//��ʼ���л�
	for (int i = 0; i < ENEMY_num; i++) {
		enemy[i].live = false;
		enemyHP(i);
	}

}
void gameDraw()
{
	//�ѱ���ͼƬ����������
	putimage(0, 0, &bk);
	if (role.live) {
		putimage(role.x, role.y, &player[0], NOTSRCERASE);
		putimage(role.x, role.y, &player[1], SRCINVERT);

	}
	else {

		putimage(role.x, role.y, &destory[0], NOTSRCERASE);
		putimage(role.x, role.y, &destory[1], SRCINVERT);
		putimage(0, 0, &gameover);
		mciSendString("close BGM", NULL, 0, NULL);


	}

	for (int i = 0; i < bull_num; i++) {
		if (bull[i].live) {
			putimage(bull[i].x, bull[i].y, &img_bull[0], NOTSRCERASE);
			putimage(bull[i].x, bull[i].y, &img_bull[1], SRCINVERT);
		}
	}//�����ӵ�
	for (int i = 0; i < ENEMY_num; i++) {
		if (enemy[i].live) {
			if (enemy[i].type == BIG) {
				putimage(enemy[i].x, enemy[i].y, &img_enemy[1][0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &img_enemy[1][1], SRCINVERT);
			}
			else {
				putimage(enemy[i].x, enemy[i].y, &img_enemy[0][0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &img_enemy[0][1], SRCINVERT);
			}

		}
	}
}
void createBullet()
{
	for (int i = 0; i < bull_num; i++) {

		if (!bull[i].live)
		{
			bull[i].x = role.x + 45;
			bull[i].y = role.y;
			bull[i].live = true;
			break;
		}

	}

}
void BulletMove() {
	for (int i = 0; i < bull_num; i++)
	{
		if (bull[i].live) {
			bull[i].y -= 1;
			if (bull[i].y < 0) {
				bull[i].live = false;
			}
		}
	}
}
bool Timer(int ms, int id)
{
	static DWORD t[10];
	if (clock() - t[id] > ms)
	{
		t[id] = clock();
		return true;
	}
	return false;
}
void PlayerMove(int speed)//��ɫ�ƶ�����ȡ��������
{
	if (role.live) {
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))
		{
			if (role.y > 0) {
				role.y -= speed;
			}
		}
		if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))
		{
			if (role.y + 120 < HEIGHT) {
				role.y += speed;
			}
		}
		if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))
		{
			if (role.x + 55 > 0) {
				role.x -= speed;
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))
		{
			if (role.x + 52 < WIDTH) {
				role.x += speed;
			}
		}
		static DWORD t1 = 0, t2 = 0;
		if (GetAsyncKeyState(VK_SPACE) && t2 - t1 > 200) {
			createBullet();
			mciSendString("close GUN", NULL, 0, NULL);
			mciSendString("open ./images/f_gun.mp3 alias GUN", NULL, 0, NULL);
			mciSendString("play GUN ", NULL, 0, NULL);
			t1 = t2;

		}
		t2 = GetTickCount();
	}
}
void createEnemy() {
	for (int i = 0; i < ENEMY_num; i++)
	{
		if (!enemy[i].live) {
			enemy[i].live = true;
			enemy[i].x = rand() % (WIDTH - 60);
			enemy[i].y = 0;
			enemyHP(i);
			break;

		}
	}


}
void enemyMove(int speed) {
	for (int i = 0; i < ENEMY_num; i++)
	{
		if (enemy[i].live) {
			enemy[i].y += speed;
			if (role.x > enemy[i].x && role.x<enemy[i].x + enemy[i].width/2 && role.y>enemy[i].y && role.y < enemy[i].y + enemy[i].height/2) {
				role.live = false;
				mciSendString("close ./images/explode.mp3 alias BOOM", NULL, 0, NULL);
				mciSendString("open ./images/explode.mp3 alias BOOM", NULL, 0, NULL);
				mciSendString("play BOOM ", NULL, 0, NULL);
				mciSendString("close ./images/game_music.mp3 alias BGM", NULL, 0, NULL);
				mciSendString("open ./images/ս��(defeat).mp3 alias defeat", NULL, 0, NULL);
				mciSendString("play defeat", NULL, 0, NULL);

			}
		}
		if (enemy[i].y > HEIGHT) {
			enemy[i].live = false;
		}
	}



}
void playplane()
{
	for (int i = 0; i < ENEMY_num; i++)
	{
		if (!enemy[i].live)
			continue;
		for (int k = 0; k < bull_num; k++)
		{
			if (!bull[k].live)
				continue;
			if (bull[k].x > enemy[i].x && bull[k].x<enemy[i].x + enemy[i].width && bull[k].y>enemy[i].y && bull[k].y < enemy[i].y + enemy[i].height - 20)
			{
				bull[i].live = false;
				enemy[i].hp--;
			}

		}
		if (enemy[i].hp <= 0)
		{
			enemy[i].live = false;
			mciSendString("close GOT ", NULL, 0, NULL);
			mciSendString("open ./images/�л��е�.mp3 alias GOT", NULL, 0, NULL);
			mciSendString("play GOT ", NULL, 0, NULL);
		}
	}
}