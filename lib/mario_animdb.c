#include "mario_animdb.h"

// Lembra de fazer dps (tanto normal quanto fire mario)
// Mario normal
MarioAnimDB_t InitSmallMarioDB(){
    MarioAnimDB_t db;

    return db;
}

// Super mario
MarioAnimDB_t InitSuperMarioDB(){
    MarioAnimDB_t db;

    db.idle = (AnimData_t){.isLooping=false, .freezedFrameLeft=5, .freezedFrameRight=6};
    db.walking = (AnimData_t){.isLooping=true, .leftAnimFrames=(FrameRange_t){3, 5}, .rightAnimFrames=(FrameRange_t){6,8}};
    db.jumping = (AnimData_t){.isLooping=false, .freezedFrameLeft=1, .freezedFrameRight=10};
    db.slide = (AnimData_t){.isLooping=false, .freezedFrameLeft=2, .freezedFrameRight=9};
    db.crouch = (AnimData_t){.isLooping=false, .freezedFrameLeft=0, .freezedFrameRight=11};

    return db;
}

// Super mario
MarioAnimDB_t InitFireMarioDB(){
    MarioAnimDB_t db;

    return db;
}