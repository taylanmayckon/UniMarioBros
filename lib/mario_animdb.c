#include "mario_animdb.h"

// Mario normal
MarioAnimDB_t InitSmallMarioDB(){
    MarioAnimDB_t db;

    db.idle = (AnimData_t){.isLooping=false, .freezedFrameLeft=4, .freezedFrameRight=7};
    db.walking = (AnimData_t){.isLooping=true, .leftAnimFrames=(FrameRange_t){2, 4}, .rightAnimFrames=(FrameRange_t){7,9}};
    db.jumping = (AnimData_t){.isLooping=false, .freezedFrameLeft=0, .freezedFrameRight=11};
    db.slide = (AnimData_t){.isLooping=false, .freezedFrameLeft=1, .freezedFrameRight=10};
    db.death = (AnimData_t){.isLooping=false, .freezedFrameLeft=5, .freezedFrameRight=5};

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

    db.idle = (AnimData_t){.isLooping=false, .freezedFrameLeft=6, .freezedFrameRight=7};
    db.walking = (AnimData_t){.isLooping=true, .leftAnimFrames=(FrameRange_t){4, 6}, .rightAnimFrames=(FrameRange_t){7,9}};
    db.jumping = (AnimData_t){.isLooping=false, .freezedFrameLeft=2, .freezedFrameRight=11};
    db.slide = (AnimData_t){.isLooping=false, .freezedFrameLeft=3, .freezedFrameRight=10};
    db.crouch = (AnimData_t){.isLooping=false, .freezedFrameLeft=1, .freezedFrameRight=12};
    db.throw = (AnimData_t){.isLooping=false, .freezedFrameLeft=0, .freezedFrameRight=13};

    return db;
}
