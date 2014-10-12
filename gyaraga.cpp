
// アプリ雛形
// ※ファイル分割対応版
//

#include "lib/appEnv.hpp"
#include <math.h>
#include <iostream>
using namespace std;
// ↑他の.cppファイルでは、必要なヘッダファイルを
//   適時インクルードする事


// アプリのウインドウサイズ
enum Window {
  WIDTH  = 750,
  HEIGHT = 1100
};



bool touch(float xpos, float ypos, float enemy_xpos, float enemy_ypos,
	float enemy_xsize, float enemy_ysize)
{
	//自キャラサイズ
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
	//敵キャラ
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




//敵キャラテスト用動作
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
	//Ａキーフラグを立てる（test用）
	if (app_env.isPushKey('A'))
	start_flag[1] = 1;

	//フラグがたったら。
	if (start_flag[1] == 1)
	{
		cout << (movecount / 20) << endl;
		
		
		
		//　3/４回転してスタート
		if ((movecount / 20)  <= 4.5)
		movecount++;
		//出発
		
		////直線
		////回転
		//if (movecount >= 60 * 3)
		//	angle = 3.14 * movecount / 80 + 90;
		////直線
		//if (movecount >= 60 * 5.4)
		//	angle = 2;
		//移動処理
		angle = (movecount / 20) - 90;
		ypos -= sin(angle) * 6;
		xpos += cos(angle) * 6;
	}

	//移動制限
	if (ypos < -HEIGHT / 2)
	{
		//初期位置に戻らせる
		ypos = HEIGHT / 2;
		xpos = 0;
		movecount = 0;
		start_flag[1] = 0;
	}
	//初期位置に戻す
	if (ypos > 300 && start_flag[1] == 0)
		ypos -= 3;


}


//playerテスト用動作処理
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



//方向弾 X座標の移動量とＹ座標の移動量を取得
void Enemy::GetMove(int i,float angle)
{
		shot_xmove[i] = shot_v * cos(angle);// 移動量の大きさ　×　cos　= X座標の移動量
		shot_ymove[i] = shot_v * sin(angle);// 移動量の大きさ　×　sin　= Y座標の移動量
}


//弾を撃つ処理
void Enemy::Shot(AppEnv& app_env,float posx,float posy)
{
	
	//弾を出す判定処理/カウントが０で、Zキーで弾を走らせる
	if (shot_count == 0 && app_env.isPushKey('Z'))
	{
		int enemy_size = 30;
		int size       = 30;
		//どの弾が出てないかを検索   
		for (int i = 0; i < shots_amount; i++)
		{
			if (shot_flag[i] == 0)
			{
				//まだ弾が初期化だされていなかった弾の初期化を行う
				shot_flag[i] = 1;
				shot_xpos[i] = xpos;
				shot_ypos[i] = ypos - enemy_size;
				shot_count = 10;  //カウントの最大値を設定。弾のラグ
				
				//atanで角度をプレイヤーとの角度を取得
				float angle = atan2(posy - ypos,posx - xpos);
				
				GetMove(i,angle/*Rot,5, shot_xmove[i], shot_ymove[i]*/);

				break;
			}
		}
	}
	//弾をの移動処理
	for (int i = 0; i < shots_amount; i++)
	{
		if (shot_flag[i] == 1)
		{
			shot_xpos[i] = shot_xpos[i] + shot_xmove[i];
			shot_ypos[i] = shot_ypos[i] + shot_ymove[i];
		}
		
		//弾の範囲が有効かチェック
		if (shot_ypos[i] < -HEIGHT / 2)
		{
			shot_flag[i] = 0;
		}
			
		
	}
	//カウントを減らす
	if (shot_count > 0)
	{
		shot_count--;
	}

	
}


//弾の描画処理
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
// メインプログラム
// 
int main() {
  // アプリウインドウの準備
  AppEnv app_env(Window::WIDTH, Window::HEIGHT,
                 false, false);



  //プレイヤー
  float xpos = 0;
  float ypos = -WIDTH/2;
  float size = 30;
  float v    = 3;
  float shot_v = 5;
  //敵キャラ
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
    // アプリウインドウが閉じられたらプログラムを終了
    if (!app_env.isOpen()) return 0;
    
    // 描画準備
    app_env.setupDraw();
	
	//test 敵キャラ
	drawFillBox(enemy.xpos, enemy.ypos, enemy.xsize, enemy.ysize, Color(1, 1, 1));
	//弾発射処理
	enemy.Shot(app_env, xpos,ypos);
	//描画処理
	enemy.Draw();
	//test敵キャラ
	enemy.EnemyMove(app_env);
	enemy.Move1(app_env);
	//テスト用敵キャラ動作
	player_move(xpos, v);
	//test プレイヤー
	drawFillBox(xpos,ypos,size,size,Color(1,1,1));
	
    //
    // 描画処理はここでおこなう
    // 
    
    
    // 画面更新
    app_env.update();
  }
  
  // アプリ終了
}