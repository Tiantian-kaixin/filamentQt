//
// Created by billzbwang on 2020/9/29.
//

#ifndef TNT_KAPUAVATARMANAGER_H
#define TNT_KAPUAVATARMANAGER_H

const int model_pool_count = 1000;

typedef struct {
    char* face_name;
    char* face_json_path;
    char* role_path;
    char* dress_eyes_path;
    char* dress_eyebrow_path;
    bool dress_eyelash_flag;
    char* dress_eyelash_path;
    bool dress_eyelid_flag;
    char* dress_eyelid_path;
    char* dress_hair_path;
    bool dress_eyeglasses_flag;
    char* dress_eyeglasses_path;
    bool dress_lipstick_flag;
    char* dress_lipstick_path;
    bool dress_tatoo_flag;
    char* dress_tatoo_path;
} CMAvatarModelItem;

int CMAvatarModelListFromDir(const char *model_dir, CMAvatarModelItem model_items[model_pool_count]);

#endif //TNT_KAPUAVATARMANAGER_H
