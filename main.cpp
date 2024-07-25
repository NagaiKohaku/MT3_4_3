#include <Novice.h>
#include <Novice.h>
#include <imgui.h>
#include "math/MakeMatrix.h"
#include "math/MatrixMath.h"
#include "math/Vector3Math.h"
#include "math/Draw.h"
#include "algorithm"

#include "cmath"

const char kWindowTitle[] = "LE2B_17_ナガイ_コハク_MT3_4_3 円錐振り子運動";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//振り子の構造体
	struct Pendulum {
		Vector3 anchor;            //アンカーポイント
		float length;              //紐の長さ
		float angle;               //現在の角度
		float angularVelocity;     //角速度
		float angularAcceleration; //角加速度
	};

	//円錐振り子の構造体
	struct ConicalPendulum {
		Vector3 anchor;        //アンカーポイント
		float length;          //紐の長さ
		float halfApexAngle;   //円錐の頂点の半分
		float angle;           //現在の角度
		float angularVelocity; //角速度
	};

	//ウィンドウサイズ
	float kWindowWidth = 1280.0f;
	float kWindowHeight = 720.0f;

	//カメラ:平行移動
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };

	//カメラ:回転
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	//カメラ:距離
	Vector3 baseOffset{ 0.0f,0.0f,-6.0f };

	//円錐振り子
	ConicalPendulum conicalPendulum;
	conicalPendulum.anchor = { 0.0f,1.0f,0.0f };
	conicalPendulum.length = 0.8f;
	conicalPendulum.halfApexAngle = 0.7f;
	conicalPendulum.angle = 0.0f;
	conicalPendulum.angularVelocity = 0.0f;

	//球の座標
	Vector3 point{ 0.0f,0.0f,0.0f };

	//スタートフラグ
	bool isStart = false;

	//描画クラス
	Draw draw;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		//1フレームの時間
		float deltaTime = 1.0f / 60.0f;

		if (isStart) {

			//角速度の計算
			conicalPendulum.angularVelocity = std::sqrtf(9.8f / (conicalPendulum.length * std::cosf(conicalPendulum.halfApexAngle)));

			//角度の計算
			conicalPendulum.angle += conicalPendulum.angularVelocity * deltaTime;
		}

		//頂点からの半径の計算
		float radius = std::sinf(conicalPendulum.halfApexAngle) * conicalPendulum.length;

		//頂点からの高さの計算
		float height = std::cosf(conicalPendulum.halfApexAngle) * conicalPendulum.length;

		//ボールの座標の計算
		point.x = conicalPendulum.anchor.x + std::cosf(conicalPendulum.angle) * radius;
		point.y = conicalPendulum.anchor.y - height;
		point.z = conicalPendulum.anchor.z - std::sinf(conicalPendulum.angle) * radius;

		if (ImGui::Button("Move Start")) {
			isStart = true;
		}

		ImGui::Text("");

		ImGui::Text("WASD : CameraRotate");
		ImGui::Text("UpArrow : ZoomIn");
		ImGui::Text("DownArrow : ZoomDown");

		ImGui::Text("");

		if (ImGui::TreeNode("Camera")) {
			ImGui::DragFloat3("Translate", &cameraTranslate.x, 0.01f);
			ImGui::DragFloat3("Rotate", &cameraRotate.x, 0.01f);
			ImGui::DragFloat3("offset", &baseOffset.x, 0.01f);
			ImGui::TreePop();
		}

		ImGui::Text("");

		draw.CamaraRotate(baseOffset, cameraRotate, cameraTranslate, keys);

		draw.Pipeline(cameraTranslate, cameraRotate, kWindowWidth, kWindowHeight);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		draw.DrawGrid();

		draw.DrawLine(conicalPendulum.anchor, point - conicalPendulum.anchor, WHITE);

		draw.DrawSphere(Sphere(point, 0.08f), WHITE);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
