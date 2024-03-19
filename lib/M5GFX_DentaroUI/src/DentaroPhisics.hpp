// class rigidBase {
// protected:
//     float x, y; // 座標
//     float width, height; // 幅と高さ

// public:
//     rigidBase(float _x, float _y, float _width, float _height)
//         : x(_x), y(_y), width(_width), height(_height) {}

//     virtual bool isCollision(const rigidBase& other) const {
//         // 当たり判定の基本ロジックをここに実装
//         // 各派生クラスでオーバーライドされる
//         return false;
//     }
// };

// class rigidRect : public rigidBase {
// public:
//     rigidRect(float _x, float _y, float _width, float _height)
//         : rigidBase(_x, _y, _width, _height) {}

//     bool isCollision(const rigidBase& other) const override {
//         // rigidRect特有の当たり判定ロジックをここに実装
//         if (const rigidRect* rect = dynamic_cast<const rigidRect*>(&other)) {
//             // 他の物体が四角形の場合
//             if (x < rect->x + rect->width &&
//                 x + width > rect->x &&
//                 y < rect->y + rect->height &&
//                 y + height > rect->y) {
//                 return true; // 当たり判定がある
//             }
//         }
//         return false; // 当たり判定がない
//     }
// };