#ifndef BULLET_OBJECT_H_
#define BULLET_OBJECT_H_
#define SPEED_BULLET 12
#define SPEED_BULLETTRAP 15
#define SPEED_BOSS 15
#define HEIGHT_THREAT 50

#include "BaseObject.h"
#include "CommonFunc.h"


class BulletObject : public BaseObject
{

public:
    BulletObject();
    ~BulletObject();

    enum BulletDir
    {
        DIR_RIGHT = 20,
        DIR_LEFT = 21,
        DIR_UP = 22,
        DIR_UP_LEFT = 23,
        DIR_UP_RIGHT = 24,
        DIR_DOWN_LEFT = 25,
        DIR_DOWN_RIGHT = 26,
        DIR_DOWN = 27,
    };

    enum BulletType
    {
        SPHERE_BULLET = 50,
        LASER_BULLET = 51,
     
    };

    void set_x_val(const int& xVal) { x_val_ = xVal; }
    void set_y_val(const int& yVal) { y_val_ = yVal; }
    int get_x_val() const { return x_val_; }
    int get_y_val() const { return y_val_; }
    float get_rect_y() { return rect_.y; }

    void set_is_move(const bool& isMove) { is_move_ = isMove; }
    bool get_is_move() const { return is_move_; }

    void set_bullet_dir(const unsigned int& bulletDir) { bullet_dir_ = bulletDir; }
    unsigned int get_bullet_dir() const { return bullet_dir_; }
    void set_flagbullet(bool x) { flag_bullet = x; }
    void set_bullet_type(const unsigned int& bulletType) { bullet_type_ = bulletType; }
    unsigned int get_bullet_type() const { return bullet_type_; }

    void HandleMove(const int& x_border, const int& y_border,Map& map_data);
    bool LoadImgBullet(SDL_Renderer* des);
    void HandleMoveBoss(const int& x_border, const int& y_border, const float& x1,
                        const float& y1, const float& x2, const float& y2, Map& Map_data);
    void HandleMoveThreat(const int& x_border, const int& y_border, const float& x1, 
                          const float& y1,const float& x2,const float& y2,Map& Map_data );
    void HandleMoveTrap(const int& x_border, const int& y_border, const float& x1,
                        const float& y1, const float& x2, const float& y2, Map& map_data);
    void set_speedbullet_x_y(const float& x_, const float& y_) { x_bullet = x_; y_bullet = y_; }
    void CheckToMapMain(Map& map_data);
    void CheckToMapThreat(Map& map_data);
    unsigned int get_bullet_type() { return bullet_type_; }
private:
    int x_val_;
    int y_val_;
    bool is_move_;
    unsigned int bullet_dir_;
    unsigned int bullet_type_;
    bool flag_bullet;
    float x_bullet;
    float y_bullet;
};
#endif
