
// �A�v�����`
// ���t�@�C�������Ή���
//

#include "lib/appEnv.hpp"
#include <math.h>
#include <iostream>
using namespace std;
// ������.cpp�t�@�C���ł́A�K�v�ȃw�b�_�t�@�C����
//   �K���C���N���[�h���鎖


// �A�v���̃E�C���h�E�T�C�Y
enum Window {
  WIDTH  = 750,
  HEIGHT = 1100
};



bool touch(float xpos, float ypos, float enemy_xpos, float enemy_ypos,
	float enemy_xsize, float enemy_ysize)
{
	//���L�����T�C�Y
	float xsize = 30;
	float ysize = 30;
	
	if (xpos + xsize < enemy_xpos && xpos < enemy_xpos + enemy_xsize &&
		ypos + ysize < enemy_ypos && ypos < enemy_ypos + enemy_ysize)
	{
		return true;
	}

}




class Enemy{
	
private:
	float shot_v = 5;
public:
	//�G�L����
	int   shots_amount;
	int   search;
	float xpos;
	float ypos;
	float xsize;
	float ysize;
	float angle;
	float movecount;
	float shot_xpos[2];
	float shot_ypos[2];
	int   shot_flag[2];
	float shot_count;
	float shot_xmove[2];
	float shot_ymove[2];
	float start_flag[2];

	void EnemyMove(AppEnv& app_env);
	void Move1(AppEnv& app_env);
	void GetMove(int i,float angle);
	void Shot(AppEnv& app_env,float xpos,float ypos);
	void Draw();

};




//�G�L�����e�X�g�p����
void Enemy::EnemyMove(AppEnv& app_env)
{
	if (app_env.isPressKey(GLFW_KEY_RIGHT))
	{
		xpos += 5;
	}
	if (app_env.isPressKey(GLFW_KEY_LEFT))
	{
		xpos -= 5;
	}
	
}
void Enemy::Move1(AppEnv& app_env)
{
	//�`�L�[�t���O�𗧂Ă�itest�p�j
	if (app_env.isPushKey('A'))
	start_flag[1] = 1;

	//�t���O����������B
	if (start_flag[1] == 1)
	{
		cout << (movecount / 20) << endl;
		
		
		
		//�@3/�S��]���ăX�^�[�g
		if ((movecount / 20)  <= 4.5)
		movecount++;
		//�o��
		
		////����
		////��]
		//if (movecount >= 60 * 3)
		//	angle = 3.14 * movecount / 80 + 90;
		////����
		//if (movecount >= 60 * 5.4)
		//	angle = 2;
		//�ړ�����
		angle = (movecount / 20) - 90;
		ypos -= sin(angle) * 6;
		xpos += cos(angle) * 6;
	}

	//�ړ�����
	if (ypos < -HEIGHT / 2)
	{
		//�����ʒu�ɖ߂点��
		ypos = HEIGHT / 2;
		xpos = 0;
		movecount = 0;
		start_flag[1] = 0;
	}
	//�����ʒu�ɖ߂�
	if (ypos > 300 && start_flag[1] == 0)
		ypos -= 3;


}


//player�e�X�g�p���쏈��
void player_move(float& xpos,float& v)
{
	xpos += v;

	if (xpos > WIDTH / 2){
		v = -v;
	}
	if (xpos < -WIDTH / 2){
		v = -v;
	}
}



//�����e X���W�̈ړ��ʂƂx���W�̈ړ��ʂ��擾
void Enemy::GetMove(int i,float angle)
{
		shot_xmove[i] = shot_v * cos(angle);// �ړ��ʂ̑傫���@�~�@cos�@= X���W�̈ړ���
		shot_ymove[i] = shot_v * sin(angle);// �ړ��ʂ̑傫���@�~�@sin�@= Y���W�̈ړ���
}


//�e��������
void Enemy::Shot(AppEnv& app_env,float posx,float posy)
{
	
	//�e���o�����菈��/�J�E���g���O�ŁAZ�L�[�Œe�𑖂点��
	if (shot_count == 0 && app_env.isPushKey('Z'))
	{
		int enemy_size = 30;
		int size       = 30;
		//�ǂ̒e���o�ĂȂ���������   
		for (int i = 0; i < shots_amount; i++)
		{
			if (shot_flag[i] == 0)
			{
				//�܂��e��������������Ă��Ȃ������e�̏��������s��
				shot_flag[i] = 1;
				shot_xpos[i] = xpos;
				shot_ypos[i] = ypos - enemy_size;
				shot_count = 10;  //�J�E���g�̍ő�l��ݒ�B�e�̃��O
				
				//atan�Ŋp�x���v���C���[�Ƃ̊p�x���擾
				float angle = atan2(posy - ypos,posx - xpos);
				
				GetMove(i,angle/*Rot,5, shot_xmove[i], shot_ymove[i]*/);

				break;
			}
		}
	}
	//�e���̈ړ�����
	for (int i = 0; i < shots_amount; i++)
	{
		if (shot_flag[i] == 1)
		{
			shot_xpos[i] = shot_xpos[i] + shot_xmove[i];
			shot_ypos[i] = shot_ypos[i] + shot_ymove[i];
		}
		
		//�e�͈̔͂��L�����`�F�b�N
		if (shot_ypos[i] < -HEIGHT / 2)
		{
			shot_flag[i] = 0;
		}
			
		
	}
	//�J�E���g�����炷
	if (shot_count > 0)
	{
		shot_count--;
	}

	
}


//�e�̕`�揈��
void Enemy::Draw(/*int shot_flag[],float shot_xpos[],float shot_ypos[]*/)
{
	for (int i = 0; i < shots_amount; i++)
	{
		if (shot_flag[i] == 1)
		{
			int size = 30;
			drawFillBox(shot_xpos[i], shot_ypos[i], size, size, Color(1, 1, 1));
		}
	}
}



// 
// ���C���v���O����
// 
int main() {
  // �A�v���E�C���h�E�̏���
  AppEnv app_env(Window::WIDTH, Window::HEIGHT,
                 false, false);



  //�v���C���[
  float xpos = 0;
  float ypos = -WIDTH/2;
  float size = 30;
  float v    = 3;
  float shot_v = 5;
  //�G�L����
  Enemy  enemy;
  {
	  enemy.shots_amount = 2;
	  enemy.xpos       = 0;
	  enemy.ypos       = 0;
	  enemy.xsize      = 30;
	  enemy.ysize      = 30;
	  enemy.shot_count = 0;
	  enemy.movecount      = 0;
	  for (int i = 0; i < enemy.shots_amount; i++){
		  enemy.shot_xpos[i]  = 0;
		  enemy.shot_ypos[i]  = 0;
		  enemy.shot_flag[i]  = 0;
		  enemy.shot_xmove[i] = 0;
		  enemy.shot_ymove[i] = 0;
	  }
  }
  
 

  

  while (1) {
    // �A�v���E�C���h�E������ꂽ��v���O�������I��
    if (!app_env.isOpen()) return 0;
    
    // �`�揀��
    app_env.setupDraw();
	
	//test �G�L����
	drawFillBox(enemy.xpos, enemy.ypos, enemy.xsize, enemy.ysize, Color(1, 1, 1));
	//�e���ˏ���
	enemy.Shot(app_env, xpos,ypos);
	//�`�揈��
	enemy.Draw();
	//test�G�L����
	enemy.EnemyMove(app_env);
	enemy.Move1(app_env);
	//�e�X�g�p�G�L��������
	player_move(xpos, v);
	//test �v���C���[
	drawFillBox(xpos,ypos,size,size,Color(1,1,1));
	
    //
    // �`�揈���͂����ł����Ȃ�
    // 
    
    
    // ��ʍX�V
    app_env.update();
  }
  
  // �A�v���I��
}