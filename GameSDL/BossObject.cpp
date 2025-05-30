
#include <iostream>
#include "BossObject.h"


BossObject::BossObject()
{
    frame_ = 0;
    x_val_ = 0.0;
    y_val_ = 0.0;
    x_pos_ = 0;
    y_pos_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    think_time_ = 0;
    map_x_ = 0;
    map_y_ = 0;
    on_ground_ = 0;
}

BossObject::~BossObject()
{

}

bool BossObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);

    if (ret == true)
    {
        width_frame_ = rect_.w / FRAME_NUM_8;
        height_frame_ = rect_.h;
    }

    return ret;
}

void BossObject::set_clips()
{
    //Clip the sprites
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i < FRAME_NUM_8; i++)
        {
            frame_clip_[i].x = width_frame_ * i;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}


void BossObject::Show(SDL_Renderer* des)
{
    if (think_time_ == 0)
    {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;
        frame_++;
        if (frame_ >= FRAME_NUM_8)
        {
            frame_ = 0;
        }

        SDL_Rect* currentClip = &frame_clip_[frame_];
        SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
        if (currentClip != NULL)
        {
            renderQuad.w = currentClip->w;
            renderQuad.h = currentClip->h;
        }

        SDL_RenderCopy(des, p_object_, currentClip, &renderQuad);
    }
}
void BossObject::ImpMoveType( const float& x, SDL_Renderer* des)
{
        if (on_ground_ == true)
        {
            if (x_pos_ >= x)
            {
                input_type_.left_ = 1;
                input_type_.right_ = 0;
               
                    LoadImg("img//threat_level.png", des);                
            }
            else if (x_pos_ < x)
            {
                input_type_.right_ = 1;
                input_type_.left_ = 0;
                LoadImg("img//threat_level_right.png", des);
            }
        }
}
void BossObject::DoPlayer(Map& g_map)
{
    if (think_time_ == 0)
    {
        x_val_ = 0;
        y_val_ += GRAVITY_SPEED;

        if (y_val_ >= MAX_FALL_SPEED)
        {
            y_val_ = MAX_FALL_SPEED;
        }

        if (input_type_.left_ == 1)
        {
            x_val_ -= BOSS_SPEED;
        }

        else if (input_type_.right_ == 1)
        {
            x_val_ += BOSS_SPEED;
        }

        if (input_type_.jump_ == 1)
        {
            if (on_ground_ == 1)
            {
                y_val_ = -PLAYER_HIGHT_VAL;
            }  
            input_type_.jump_ = 0;
        }

        CheckToMap(g_map);
    }

    if (think_time_ > 0)
    {
        think_time_--;

        if (think_time_ == 0)
        {
            InitPlayer();
        }
    }
}

void BossObject::InitPlayer()
{
    x_val_ = 0;
    y_val_ = 0;

    if (x_pos_ > 256)
    {
        x_pos_ -= 256;
    }
    else
        x_pos_ = 0;
    y_pos_ = 0;
    think_time_ = 0;
    input_type_.left_ = 1;
}

void BossObject::CenterEntityOnMap(Map& g_map)
{
    g_map.start_x_ = x_pos_ - (SCREEN_WIDTH / 2);

    if (g_map.start_x_ < 0)
    {
        g_map.start_x_ = 0;
    }

    else if (g_map.start_x_ + SCREEN_WIDTH >= g_map.max_x_)
    {
        g_map.start_x_ = g_map.max_x_ - SCREEN_WIDTH;
    }

    g_map.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);

    if (g_map.start_y_ < 0)
    {
        g_map.start_y_ = 0;
    }

    else if (g_map.start_y_ + SCREEN_HEIGHT >= g_map.max_y_)
    {
        g_map.start_y_ = g_map.max_y_ - SCREEN_HEIGHT;
    }
}
void BossObject::RemoveBullet(const int& idx)
{
    int size = bullet_list_.size();
    if (size > 0 && idx < size && !bullet_list_.empty())
    {
        BulletObject* p_bullet = bullet_list_.at(idx);
        bullet_list_.erase(bullet_list_.begin() + idx);
        if (p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}
void BossObject::InitBullet(BulletObject* p_bullet, SDL_Renderer* screen)
{
    if (p_bullet != NULL)
    {
        bool ret = p_bullet->LoadImg("img//boss bullet.png", screen);
        if (ret)
        {
            p_bullet->set_is_move(true);
            p_bullet->SetRect(rect_.x, rect_.y);
            bullet_list_.push_back(p_bullet);
        }
    }
}
void BossObject::CheckToMap(Map& g_map)
{
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    on_ground_ = 0;
    int height_min = height_frame_;
    x1 = (x_pos_ + x_val_) / TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

    y1 = (y_pos_) / TILE_SIZE;
    y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

    // Check x1, x2 with full width of map
    // Check y1, y2 with full heigth of map
    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (x_val_ > 0) // when object is moving by right  ===>
        {
            // Check current position of map. It is not blank_tile.
            int val1 = g_map.tile[y1][x2];
            int val2 = g_map.tile[y2][x2];
            if (val1 == BRAVE || val2 == BRAVE)
            {
                g_map.tile[y1][x2] = 0;
                g_map.tile[y2][x2] = 0;
            }
            if ((g_map.tile[y1][x2] != BLANK_TILE) || (g_map.tile[y2][x2] != BLANK_TILE))
            {
                // Fixed post of object at current post of map.
                // => Cannot moving when press button
                x_pos_ = x2 * TILE_SIZE;
                x_pos_ -= width_frame_ + 1;
                x_val_ = 0; // cannot moving
            }
        }
        else if (x_val_ < 0) // When moving by left    <====
        {
            int val1 = g_map.tile[y1][x1];
            int val2 = g_map.tile[y2][x1];
            if (val1 == BRAVE || val2 == BRAVE)
            {
                g_map.tile[y1][x1] = 0;
                g_map.tile[y2][x1] = 0;
            }
            if ((g_map.tile[y1][x1] != BLANK_TILE) || (g_map.tile[y2][x1] != BLANK_TILE))
            {
                x_pos_ = (x1 + 1) * TILE_SIZE;
                x_val_ = 0;
            }
        }
    }


    // Check vertical
    int width_min = width_frame_;// SDLCommonFunc::GetMin(width_frame_, TILE_SIZE);

    x1 = (x_pos_) / TILE_SIZE;
    x2 = (x_pos_ + width_min) / TILE_SIZE;

    y1 = (y_pos_ + y_val_) / TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (y_val_ > 0)
        {
            //Similar for vertical
            if ((g_map.tile[y2][x1] != BLANK_TILE) || (g_map.tile[y2][x2] != BLANK_TILE))
            {
                y_pos_ = y2 * TILE_SIZE;
                y_pos_ -= height_frame_;

                y_val_ = 0;

                on_ground_ = 1;
            }
        }
        else if (y_val_ < 0)
        {
            if ((g_map.tile[y1][x1] != BLANK_TILE) || (g_map.tile[y1][x2] != BLANK_TILE))
            {
                y_pos_ = (y1 + 1) * TILE_SIZE;

                y_val_ = 0;
            }
        }
    }

    //If there is not collision with map tile. 
    x_pos_ += x_val_;
    y_pos_ += y_val_;

    if (x_pos_ < 0)
    {
        x_pos_ = 0;
    }
    else if (x_pos_ + width_frame_ >= g_map.max_x_)
    {
        x_pos_ = g_map.max_x_ - width_frame_ - 1;
    }

    if (y_pos_ > g_map.max_y_)
    {
        think_time_ = 60;
    }
}
void BossObject::MakeBullet(SDL_Renderer* des, const int& x_limit, const int& y_limit, const float& x2, const float& y2, Map& map_data,Mix_Chunk* soundevent[3])
{
    if (x2 <= x_pos_)
    {
        LoadImg("img//threat_level.png", des);
    }
    else
    {
        LoadImg("img//threat_level_right.png", des);
    }
    for (int i = 0; i < bullet_list_.size(); i++)
    {
        BulletObject* p_bullet = bullet_list_.at(i);
        if (p_bullet != NULL)
        {

            if (p_bullet->get_is_move() == true)
            {

                p_bullet->HandleMoveBoss(x_limit, y_limit, x_pos_, y_pos_, x2, y2, map_data);
                p_bullet->Render(des);
            }
            else
            {
                Mix_PlayChannel(CHANNEL_BOSS1, soundevent[1], 0);
                p_bullet->set_is_move(true);
                p_bullet->SetRect(rect_.x, rect_.y);
            }

        }
    }
}
SDL_Rect BossObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;
    return rect;
}